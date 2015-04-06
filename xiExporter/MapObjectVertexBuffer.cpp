#include "MapObjectVertexBuffer.h"

int MapObjectVertexBuffer::vertexCount = 1;

MapObjectVertexBuffer::~MapObjectVertexBuffer()
{
}

void MapObjectVertexBuffer::ReleaseBuffer(){
}

void MapObjectVertexBuffer::InitBuffers(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData)
{
    m_VertexCount = NumVerteses;
    m_VertexData = (float*)pVertexData;
    m_IndexData = (short*)pIndexData;
    m_IndexCount = NumIndexes;
}

void MapObjectVertexBuffer::WriteVertexes(std::stringstream* buf, D3DXMATRIX worldMatrix, D3DCULL cull)
{

  // vertices
  //D3DFVF_XYZ 4 4 4|D3DFVF_DIFFUSE 4|D3DFVF_NORMAL 4 4 4|D3DFVF_TEX1 4 4,
  // the array contains floats and will skip by 4 bytes for us
  // so this can be divided by 4
  int vertexLength = 36/4;
  for(unsigned int i=0; i<m_VertexCount*vertexLength; i+=vertexLength){

    float x = m_VertexData[i];
    float y = m_VertexData[i + 1];
    float z = m_VertexData[i + 2];

    D3DXVECTOR4 vector(x,y,z,1.0f);
    D3DXVECTOR4 worldVector(1.0f, 1.0f, 1.0f, 1.0f);
    // transform point to world space
    D3DXVec4Transform(&worldVector, &vector, &worldMatrix);

    *buf << "v " << std::fixed << worldVector.x << " " << std::fixed << worldVector.y << " " << std::fixed << worldVector.z << "\n";
  }

}

void MapObjectVertexBuffer::WriteFaces(std::stringstream* buf, D3DXMATRIX worldMatrix, D3DCULL cull)
{

  // paste faces
  for(unsigned int i=0; i<m_IndexCount; i++){
    unsigned short f1, f2, f3;

    // skip this; moving onto new mesh
    if(i+1 >= m_IndexCount || i+2 >= m_IndexCount){
      continue;
    }

    f1 = m_IndexData[i] + MapObjectVertexBuffer::vertexCount;

    *buf << "f " << f1 << " ";

    if(i%2 == 0){
      f2 = m_IndexData[i+1] + MapObjectVertexBuffer::vertexCount;
      f3 = m_IndexData[i+2] + MapObjectVertexBuffer::vertexCount;

      *buf << f2 << " ";
      *buf << f3 << "\n";

    } else {
      f2 = m_IndexData[i+2] + MapObjectVertexBuffer::vertexCount;
      f3 = m_IndexData[i+1] + MapObjectVertexBuffer::vertexCount;

      *buf << f2 << " ";
      *buf << f3 << "\n";

    }

  }

  MapObjectVertexBuffer::vertexCount += m_VertexCount;
}
