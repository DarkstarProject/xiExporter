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

UINT32 MapObjectVertexBuffer::WriteVertexes(std::stringstream* buf, D3DXMATRIX worldMatrix, D3DCULL cull)
{

  // vertices
  //D3DFVF_XYZ 4 4 4|D3DFVF_DIFFUSE 4|D3DFVF_NORMAL 4 4 4|D3DFVF_TEX1 4 4,
  // the array contains floats and will skip by 4 bytes for us
  // so this can be divided by 4
  int vertexLength = 36/4;
  UINT32 ret = 0;
  for(unsigned int i=0; i<m_VertexCount*vertexLength; i+=vertexLength){

    float x = m_VertexData[i];
    float y = m_VertexData[i + 1];
    float z = m_VertexData[i + 2];

    D3DXVECTOR4 vector(x,y,z,1.0f);
    D3DXVECTOR4 worldVector(1.0f, 1.0f, 1.0f, 1.0f);
    // transform point to world space
    D3DXVec4Transform(&worldVector, &vector, &worldMatrix);

    *buf << "v " << std::fixed << worldVector.x * -1.0f << " " << std::fixed << worldVector.y * -1.0f << " " << std::fixed << worldVector.z << "\n";
    ret++;
  }
  return ret;
}

UINT32 MapObjectVertexBuffer::WriteFaces(std::stringstream* buf, D3DXMATRIX worldMatrix, D3DCULL cull, uint64_t& totalVerts)
{
  // paste faces
  uint32_t ret = totalVerts;
  for(unsigned int i=0; i<m_IndexCount; i++){
    unsigned short f1, f2, f3;

    // skip this; moving onto new mesh
    if(i+1 >= m_IndexCount || i+2 >= m_IndexCount){
      continue;
    }

    f1 = m_IndexData[i] + MapObjectVertexBuffer::vertexCount;

    *buf << "f " << f1 + ret << " ";

    if(i%2 == 0){
      f2 = m_IndexData[i+1] + MapObjectVertexBuffer::vertexCount;
      f3 = m_IndexData[i+2] + MapObjectVertexBuffer::vertexCount;

      *buf << f2 + ret << " ";
      *buf << f3 + ret << "\n";

    } else {
      f2 = m_IndexData[i+2] + MapObjectVertexBuffer::vertexCount;
      f3 = m_IndexData[i+1] + MapObjectVertexBuffer::vertexCount;

      *buf << f2 + ret << " ";
      *buf << f3 + ret << "\n";

    }
  }
  MapObjectVertexBuffer::vertexCount += m_VertexCount;
  return m_VertexCount;
}
