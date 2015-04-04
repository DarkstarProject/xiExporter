#include "ObjectBuffer.h"

ObjectBuffer::ObjectBuffer() :
    m_MapObjectCount(0)
{
}

ObjectBuffer::~ObjectBuffer()
{
    for (int i = 0; i < m_MapObjectCount; i++)
    {
        m_MapObjects[i]->Release();
        delete m_MapObjects[i];
    }
    m_MapObjectCount = NULL;
}

MapObject* ObjectBuffer::NewMapObject(char* ID)
{
    // assert( m_MapObjectCount + 1 <= MAPOBJECTBUFFER_SIZE );

    MapObject* Object = new MapObject(ID);
    m_MapObjects[m_MapObjectCount] = Object;
    m_MapObjectCount++;
    return Object;
}

void ObjectBuffer::AddMapObject(MapObject* Object)
{
    // assert( m_MapObjectCount + 1 <= MAPOBJECTBUFFER_SIZE );

    m_MapObjects[m_MapObjectCount] = Object;
    m_MapObjectCount++;
}

MapObject* ObjectBuffer::GetMapObject(char* ID)
{
    for (unsigned int i = 0; i < m_MapObjectCount; i++)
    {
        if( memcmp(ID, m_MapObjects[i]->GetID(), 16) == 0 )
            return m_MapObjects[i];
    }
    return NULL;
}
