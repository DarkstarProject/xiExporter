#pragma once
#include "MapObjectVertexBuffer.h"
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>
#define MAPOBJECTVERTEXBUFFER_SIZE 32

class MapObject
{

private:
    bool m_Render;
    char m_ID[16];
    MapObjectVertexBuffer* m_ObjectBuffers[MAPOBJECTVERTEXBUFFER_SIZE];
    unsigned int m_ObjectBufferCount;
public:
    MapObject() : m_ObjectBufferCount(NULL){}
    MapObject(char* ID);
    ~MapObject();

	int m_ObjID;

    void Release();
    unsigned int AddVertexBuffer(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData);

    //get
    char* GetID() { return m_ID; }
    unsigned int GetObjectBufferCount() { return m_ObjectBufferCount; }
    MapObjectVertexBuffer* GetObjectVertexBuffer(unsigned int Index) { return m_ObjectBuffers[Index]; }
    
    //set
    void SetRender(bool value) { m_Render = value; }
    std::string* ToObj(unsigned int ID, D3DXMATRIX matrix, D3DCULL cull, uint64_t& totalVerts);
};
