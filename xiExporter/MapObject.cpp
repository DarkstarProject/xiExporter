#include "MapObject.h"
#include <cstdint>

MapObject::MapObject(char* ID) : m_ObjectBufferCount(NULL)
{
  static int objCount = 0;
    memset(m_ID, 0, 16);
    memcpy(m_ID, ID, 16);
    memset(this->m_ObjectBuffers, 0, sizeof(this->m_ObjectBuffers));
    m_ObjID = ++objCount;
}

MapObject::~MapObject()
{
    Release();
}

void MapObject::Release()
{
    for (unsigned int i = 0; i < m_ObjectBufferCount; i++)
    {
        m_ObjectBuffers[i]->ReleaseBuffer();
        delete m_ObjectBuffers[i];
    }
    m_ObjectBufferCount = NULL;
}

std::string* MapObject::ToObj(unsigned int ID, D3DXMATRIX matrix, D3DCULL cull, uint64_t& totalVerts)
{
  // set object name
  std::string tempId(m_ID);
  // remove weird characters
  std::size_t pos = tempId.find_first_of(-51);

  std::stringstream* buf = new std::stringstream();
  uint64_t tempVerts = totalVerts;
  // *buf << "o ";
  // buf->write(m_ID, pos);
  // *buf << ID << "\n";
  //
  // *buf << "g ";
  // buf->write(m_ID, pos);
  // *buf << ID << "\n";
  std::string name(&m_ID[0], 16);
  name = name.substr(name.find_first_not_of(' '), name.find_first_of(' '));
  *buf << "o " << name << "_" << tempVerts << "\n";
  // output objects
  for(int i=0; i<m_ObjectBufferCount; i++)
    m_ObjectBuffers[i]->WriteVertexes(buf, matrix, cull);

  uint64_t tempVertsCount = totalVerts;
  for(int i=0; i<m_ObjectBufferCount; i++){
    *buf << "g " << name << "_" << tempVerts << "_" << i << "\n";
    totalVerts += m_ObjectBuffers[i]->WriteFaces(buf, matrix, cull, tempVertsCount);
  }

  std::string* output = new std::string(buf->str());

  delete buf;

  return output;
}

unsigned int MapObject::AddVertexBuffer(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData)
{
    // assert(m_ObjectBufferCount + 1 < MAPOBJECTVERTEXBUFFER_SIZE);

    m_ObjectBuffers[m_ObjectBufferCount] = new MapObjectVertexBuffer();
    m_ObjectBuffers[m_ObjectBufferCount]->InitBuffers(NumIndexes, NumVerteses, pIndexData, pVertexData);
    m_ObjectBufferCount++;
    return m_ObjectBufferCount - 1;
}
