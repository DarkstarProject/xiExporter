#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <sstream>

class MapObjectVertexBuffer
{
private:
    bool m_Render;
    LPDIRECT3DVERTEXBUFFER9 m_Vertexs;
    unsigned int m_VertexCount;
    LPDIRECT3DINDEXBUFFER9 m_Indexes;
    unsigned int m_IndexCount;

public:
    MapObjectVertexBuffer() : m_Vertexs(NULL), m_VertexCount(NULL), m_Indexes(NULL), m_IndexCount(NULL) {}
    ~MapObjectVertexBuffer();
    void InitBuffers(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData);

    void RenderBuffer();
    void WriteObj(std::stringstream* buf, D3DXMATRIX matrix, D3DCULL cull);
    void ReleaseBuffer();

    void SetRender(bool Value) { m_Render = Value; }

};
