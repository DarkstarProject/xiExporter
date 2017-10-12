#include "MapExporter.h"
#include <regex>
#include <string>
#include <vector>

MapExporter::MapExporter() :
    m_MapLib(NULL), m_ObjectBuffer(NULL), m_ObjectMap(NULL), m_ObjectMapCount(NULL), m_Loaded(false)
{
    m_MapLib = new FFxiMapLib::MapLib();
    m_ZoneID = 0;
    m_MapLib->AddCallback(this);

    std::ifstream infile("./filters.txt");
    std::string line = "";

    if (infile && infile.good())
    {
       while (std::getline(infile, line))
       {
          if (line.empty() || line[0] == '\0' || line[0] == '#')
             continue;

          std::string pushLn = "";
          for (int i = 0; i < line.size(); ++i)
          {
             char c = line[i];
             if (c == '\r' || c == '\n' || c == '\0' || c == '#')
                break;
             pushLn += c;
          }
          if (pushLn.size() >= 5)
             filters.push_back(pushLn);
       }
    }
}

MapExporter::~MapExporter()
{
    m_Loaded = false;

    if( m_ObjectBuffer )
        delete m_ObjectBuffer;
    m_ObjectBuffer = NULL;

    if( m_ObjectMap )
        delete[] m_ObjectMap;
    m_ObjectMap = NULL;
    m_ObjectMapCount = 0;

    if( m_MapLib )
    {
        m_MapLib->FreeMap();
        delete m_MapLib;
        m_MapLib = NULL;
    }
}

void MapExporter::SetPath(const char* ffxiPath)
{
    m_MapLib->SetGamePath(ffxiPath);
}

bool MapExporter::HandleNewObject(char* ID, UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData)
{
    //an object can have more than one sub object 
    //so we have to check that the object hasn't already been added
    MapObject* Object = this->m_ObjectBuffer->GetMapObject(ID);
    if( Object == NULL )
        Object = this->m_ObjectBuffer->NewMapObject(ID);
    
    printf("Adding vertexbuffer to (%s) Indexes (%d)\n", ID, NumIndexes);
    Object->AddVertexBuffer(NumIndexes, NumVerteses, pIndexData, pVertexData);
    return true;
}

bool MapExporter::LoadMap(unsigned int ZoneID)
{
    m_ZoneID = ZoneID;
    //create a buffer to hold the objects
    this->m_ObjectBuffer = new ObjectBuffer();

    //start loading the map
    if( !m_MapLib->LoadMap(ZoneID) )
        return false; //something went wrong parsing the dat

    m_ObjectMapCount = m_MapLib->GetObjectMapCount();
    //create the array to hold the object map
    m_ObjectMap = new ObjectMap[m_ObjectMapCount];

    //copy the raw object info to our array.
    //find the object that goes with it and save a pointer to it.
    //pre transform a matrix for the object.
    for (unsigned int i = 0; i < m_ObjectMapCount; i++)
    {
        FFxiMapLib::OBJINFO* RawObject = m_MapLib->GetObjectMap(i);
        ObjectMap* Map = &m_ObjectMap[i];

        //copy the OBJINFO over
        memcpy(Map, RawObject, sizeof(FFxiMapLib::OBJINFO));

        //find the object for this entry
        Map->Object = m_ObjectBuffer->GetMapObject(RawObject->id);

        //preprocess the objects matrix
        D3DXMATRIX matWorld;
        D3DXMATRIX matWorld2;
        D3DXMATRIX matWorld3;
        D3DXMATRIX matWorld4;
        D3DXMATRIX matWorldR4;
        D3DXMATRIX matWorldR5;
        D3DXMATRIX matWorldR6;
        ZeroMemory(&matWorld,sizeof(D3DXMATRIX));

        D3DXMatrixScaling(&matWorld3, this->m_ObjectMap[i].ObjectInfo.fScaleX, this->m_ObjectMap[i].ObjectInfo.fScaleY, this->m_ObjectMap[i].ObjectInfo.fScaleZ); 
        D3DXMatrixTranslation(&matWorld, this->m_ObjectMap[i].ObjectInfo.fTransX, this->m_ObjectMap[i].ObjectInfo.fTransY,this->m_ObjectMap[i].ObjectInfo.fTransZ);
        D3DXMatrixRotationX(&matWorldR4, this->m_ObjectMap[i].ObjectInfo.fRotX);
        D3DXMatrixRotationY(&matWorldR5, this->m_ObjectMap[i].ObjectInfo.fRotY);
        D3DXMatrixRotationZ(&matWorldR6, this->m_ObjectMap[i].ObjectInfo.fRotZ);
        matWorld2 = matWorldR4 * matWorldR5 * matWorldR6;
        matWorld=((matWorld3*matWorld2))*matWorld;
        Map->MatWorld = matWorld;

        if( this->m_ObjectMap[i].ObjectInfo.fScaleX * this->m_ObjectMap[i].ObjectInfo.fScaleY * this->m_ObjectMap[i].ObjectInfo.fScaleZ < 0.0f )
            Map->CullMode = D3DCULL_CW;
        else
            Map->CullMode =  D3DCULL_CCW;
    }

    m_Loaded = true;
    return true;
}

bool MapExporter::ExportMap()
{
    if( m_Loaded && m_ObjectBuffer && m_ObjectMap )
    {
        printf("Writing Objects\n");
        
        std::string fileName;
        fileName.resize(15);
        sprintf(&fileName[0], "%u.obj", m_ZoneID);
        
        uint64_t totalVerts = 0;
        uint64_t totalIndices = 0;
        std::ofstream objFile(fileName, std::ios::out | std::ios::trunc);
        objFile.close();
        objFile.open(fileName, std::ios::ate | std::ios::app);
        int updateFrequency = m_ObjectMapCount / 50;
        uint64_t startTime = time(NULL);

        printf("Exporting ");
        for (unsigned int i = 0; i < m_ObjectMapCount; i++)
        {
          MapObjectVertexBuffer::vertexCount = 1;

          if (i % updateFrequency == 0)
            printf(".");

          char name[100];
          std::string objName = "";
          int id = 0;
          if(m_ObjectMap[i].Object){
            objName = std::string(&m_ObjectMap[i].Object->GetID()[0], 16);
            id = m_ObjectMap[i].Object->m_ObjID;
            objName = objName.substr(objName.find_first_not_of(' '), objName.find_first_of(' '));
            if (!CanDumpObj(objName))
               continue;
          }

         // sprintf(name, "obj_%d_%d_%.15s.obj", i, id, objName);
         // objFile << "\no " << name << "\n";

          if(!objFile.is_open()){
            return false;
          }

            if( m_ObjectMap[i].Object)
            {
              // if(m_ObjectMap[i].Object->
                std::string* out = ObjectMapToObj(i, &m_ObjectMap[i], totalVerts);
                objFile << out->c_str();
                objFile << "\n";

                delete out;
            }
            //objFile.close();
        }
        objFile.close();
        printf("\nFinished exporting in %u seconds\n", time(NULL) - startTime);

        return true;
    }

    return false;
}

bool MapExporter::CanDumpObj(const std::string& name)
{
   bool push = true;
   std::string reg = "";
   // lambda to parse filters

   auto filterFunc = [&](std::string filter, const std::string& mode) -> bool
   {
      int offset = 0;
      auto inc = filter.substr(0, filter.find_first_of(" ", 0));
      std::string ogFilter = filter;
      bool isMatch = false;

      offset += inc.size();
      filter = filter.substr(offset + 1, filter.size() - 1);
      if (filter.substr(0, 2) == "re")
      {
         std::regex re(reg = filter.substr(3, filter.size() - 1));

         if (std::regex_match(name, re))
         {
            if (inc.substr(0, 3) == "exc" && mode == "exc")
               isMatch = !(push = false);
            else if (inc.substr(0, 3) == "inc" && mode == "inc")
               isMatch = (push = true);
         }
      }
      else if (name == filter)
      {
         if (inc.substr(0, 3) == "exc" && mode == "exc")
            isMatch = !(push = false);
         else if (inc.substr(0, 3) == "inc" && mode == "inc")
            isMatch = (push = true);
      }
      
      return (isMatch && push && mode == "inc");
   };

   for (auto i = 0; i < filters.size(); ++i)
   {
      std::string filterStr = filters[i];
      filterFunc(filterStr, "exc");
   }
   for (auto i = 0; i < filters.size(); ++i)
   {
      std::string filterStr = filters[i];
      filterFunc(filterStr, "inc");
   }
   return push;
}

std::string* MapExporter::ObjectMapToObj(unsigned int ID, ObjectMap* obj, uint64_t& totalVerts)
{
    return obj->Object->ToObj(ID, obj->MatWorld, obj->CullMode, totalVerts);
}

unsigned int MapExporter::GetZoneID()
{
    return m_ZoneID;
}
