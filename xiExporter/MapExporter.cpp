#include "MapExporter.h"

MapExporter::MapExporter() :
    m_MapLib(NULL), m_ObjectBuffer(NULL), m_ObjectMap(NULL), m_ObjectMapCount(NULL), m_Loaded(false)
{
    m_MapLib = new FFxiMapLib::MapLib();
    m_ZoneID = 0;
    m_MapLib->AddCallback(this);
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

void MapExporter::SetPath(char* ffxiPath)
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
        for (unsigned int i = 0; i < m_ObjectMapCount; i++)
        {
          MapObjectVertexBuffer::vertexCount = 1;

          char name[100];
          char* objName = "";
          int id = 0;
          if(m_ObjectMap[i].Object){
            objName = m_ObjectMap[i].Object->GetID();
            id = m_ObjectMap[i].Object->m_ObjID;
          }

          sprintf(name, "obj_%d_%d_%.15s.obj", i, id, objName);
          std::ofstream objFile(name);

          if(!objFile.is_open()){
            return false;
          }

            if( m_ObjectMap[i].Object)
            {
              // if(m_ObjectMap[i].Object->
                std::string* out = ObjectMapToObj(i, &m_ObjectMap[i]);
                objFile << out->c_str();
                objFile << "\n";

                delete out;
            }

            int progress = (i / (float)m_ObjectMapCount) * 100;
            printf("Progress: %%%d. Exporting (%d)\n", progress, i);
            objFile.close();
        }


        return true;
    }

    return false;
}

std::string* MapExporter::ObjectMapToObj(unsigned int ID, ObjectMap* obj)
{
    return obj->Object->ToObj(ID, obj->MatWorld, obj->CullMode);
}

unsigned int MapExporter::GetZoneID()
{
    return m_ZoneID;
}
