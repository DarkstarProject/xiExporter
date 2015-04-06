#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <sstream>

class MapObjectVertexBuffer
{
private:
    unsigned int m_VertexCount;
    CONST float* m_VertexData;
    short* m_IndexData;
    unsigned int m_IndexCount;

public:
    static int vertexCount;
    MapObjectVertexBuffer() : m_VertexCount(NULL), m_VertexData(NULL), m_IndexData(NULL), m_IndexCount(NULL) {}
    ~MapObjectVertexBuffer();
    void InitBuffers(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData);

    void ReleaseBuffer();
    void WriteVertexes(std::stringstream* buf, D3DXMATRIX matrix, D3DCULL cull);
    void WriteFaces(std::stringstream* buf, D3DXMATRIX matrix, D3DCULL cull);

};
