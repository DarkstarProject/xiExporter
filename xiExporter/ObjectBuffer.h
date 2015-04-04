#pragma once
#include "MapObject.h"

#define MAPOBJECTBUFFER_SIZE 1024

class ObjectBuffer
{
private:
    MapObject* m_MapObjects[MAPOBJECTBUFFER_SIZE];
    unsigned int m_MapObjectCount;

public:
    ObjectBuffer();
    ~ObjectBuffer();
    MapObject* NewMapObject(char* ID);
    void AddMapObject(MapObject* Object);

    unsigned int GetMapObjectCount() { return m_MapObjectCount; }
    MapObject* GetMapObject(unsigned int Index) { return m_MapObjects[Index]; }
    MapObject* GetMapObject(char* ID);
};
