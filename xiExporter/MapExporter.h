#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "ObjectBuffer.h"
#include "MapLib.h"
#include "MapObject.h"
#include <fstream>
#include <string>

struct ObjectMap
{
    FFxiMapLib::OBJINFO     ObjectInfo;
    MapObject*  Object;
    D3DXMATRIX  MatWorld;
    D3DCULL     CullMode;
    ObjectMap() : Object(NULL) {}
};

class MapExporter : public FFxiMapLib::ICallbackClass
{
private:
    FFxiMapLib::MapLib* m_MapLib;
    ObjectBuffer* m_ObjectBuffer;
    ObjectMap* m_ObjectMap;
    unsigned int m_ObjectMapCount;
    bool m_Loaded;
    unsigned int m_ZoneID;

    std::string* ObjectMapToObj(unsigned int ID, ObjectMap* obj);
protected:
    //callbacks
   virtual bool HandleNewObject(char* ID, UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData);
   virtual bool HandleNewTexture(char* ID, char* pdata) { return true; }
   virtual void HandleProgress(const char* pStage, UINT Progress, UINT Total) { }
public:
    MapExporter(char* ffxiPath);
    virtual ~MapExporter();

    bool LoadMap(unsigned int ZoneID);

    bool ExportMap();
    unsigned int GetZoneID();
};
