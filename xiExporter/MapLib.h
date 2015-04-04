#pragma once
#ifndef _MAPLIB_HEADER_
#define _MAPLIB_HEADER_

#ifndef CONST
#define CONST const
#endif
typedef unsigned int UINT;

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(MAPLIB_STATIC)
#   ifdef MAPLIB_EXPORTS
#       define MAPLIBEXPORT __declspec(dllexport)
#   else
#       define MAPLIBEXPORT __declspec(dllimport)
#       pragma comment(lib, "FFxiMapLib.lib")
#   endif
#       define MAPLIBPRIVATE
#else
#       define MAPLIBEXPORT
#       define MAPLIBPRIVATE
#endif



namespace FFxiMapLib
{
    class ICallbackClass
    {
    public:
        virtual bool HandleNewObject(char* ID, UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData) = 0;
        virtual bool HandleNewTexture(char* ID, char* pdata) = 0;
        virtual void HandleProgress(const char* pStage, UINT Progress, UINT Total) = 0;
    };

    #pragma pack(push,1)
    typedef struct
    {
      char id[16];
      float fTransX,fTransY,fTransZ;
      float fRotX,fRotY,fRotZ;
      float fScaleX,fScaleY,fScaleZ;
      float fa,fb,fc,fd;
      long  fe,ff,fg,fh,fi,fj,fk,fl;
    } OBJINFO;
    #pragma pack(pop)

    class MAPLIBEXPORT MapLib
    {
    private:
        ICallbackClass* m_CallBack;
        UINT m_MapID;

        char m_ffxiDir[1024];

        char* m_pDatData;
        UINT m_datSize;   

        OBJINFO* m_ObjectMap;
	    UINT m_ObjectMapCount;

    private:
        bool ParseDat();
        bool ParseObject(char* pObject);
        UINT GetFtableID(UINT Table, UINT ID);
    public:
        MapLib();
        virtual ~MapLib();

        bool SetGamePath(const char* Path);
        bool LoadMap(UINT ZoneID);
        bool LoadMap(const char* DatPath);
        void FreeMap();

        OBJINFO* GetObjectMap(UINT Index);
        UINT GetObjectMapCount();

        void AddCallback(ICallbackClass* T);

    };
};
#endif
