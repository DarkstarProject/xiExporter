#include "MapObjectVertexBuffer.h"

MapObjectVertexBuffer::~MapObjectVertexBuffer()
{
    ReleaseBuffer();
}

void MapObjectVertexBuffer::ReleaseBuffer()
{
    if( m_Vertexs )
        m_Vertexs->Release();
    if( m_Indexes )
        m_Indexes->Release();
    m_Vertexs       = NULL;
    m_VertexCount   = NULL;
    m_VertexData    = NULL;
    m_IndexData    = NULL;
    m_Indexes       = NULL;
    m_IndexCount    = NULL;
}

void MapObjectVertexBuffer::InitBuffers(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData)
{
    m_VertexCount = NumVerteses;
    m_VertexData = (float*)pVertexData;
    m_IndexData = (short*)pIndexData;
    m_IndexCount = NumIndexes;
    // if( FAILED(m_pD3DDevice->CreateVertexBuffer(NumVerteses * 36,
    //                                  D3DUSAGE_WRITEONLY,
    //                                  D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_NORMAL|D3DFVF_TEX1,
    //                                  D3DPOOL_MANAGED,
    //                                  &m_Vertexs,
    //                                  NULL)) )
        return;
    void* pData;
    if( FAILED(m_Vertexs->Lock(0, 0, &pData, NULL)) )
        return;
    memcpy(pData, pVertexData, NumVerteses * 36);
    if( FAILED(m_Vertexs->Unlock()) )
        return;

    // if( FAILED(m_pD3DDevice->CreateIndexBuffer(NumIndexes * 2,
    //                                 D3DUSAGE_WRITEONLY,
    //                                 D3DFMT_INDEX16,
    //                                 D3DPOOL_MANAGED,
    //                                 &m_Indexes,
    //                                 NULL)) )
        return;
    if( FAILED(m_Indexes->Lock(0, 0, &pData, NULL)) )
        return;
    memcpy(pData, pIndexData, NumIndexes * 2);
    if( FAILED(m_Indexes->Unlock()) )
        return;

}

void MapObjectVertexBuffer::RenderBuffer()
{
    if( !m_Vertexs || m_VertexCount == 0 || !m_Indexes || m_IndexCount == 0 )
        return;

    // HRESULT hr;
    // m_pD3DDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_NORMAL|D3DFVF_TEX1);
    // hr = m_pD3DDevice->SetStreamSource(0, m_Vertexs, 0, 36);
    // if( FAILED(hr) )
    //     return;
    // hr = m_pD3DDevice->SetIndices(m_Indexes);
    // if( FAILED(hr) )
    //     return;
    // hr = m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_IndexCount, 0, m_IndexCount - 2);
    // if( FAILED(hr) )
    //     return;
    //
}

void MapObjectVertexBuffer::WriteObj(std::stringstream* buf, D3DXMATRIX worldMatrix, D3DCULL cull)
{

  // vertices
  //d3dfvf_xyz 4 4 4|d3dfvf_diffuse 4|d3dfvf_normal 4 4 4|d3dfvf_tex1 4 4,
  // the array contains floats and will skip by 4 bytes for us
  // so this can be divided by 4
  int vertexlength = 36/4;
  for(unsigned int i=0; i<m_vertexcount*vertexlength; i+=vertexlength){

    // m_renderer->settransform(d3dts_world, &m_objectmap[i].matworld);
    // m_renderer->setrenderstate(d3drs_cullmode, m_objectmap[i].cullmode);
    float x = m_vertexdata[i];
    float y = m_vertexdata[i + 1];
    float z = m_vertexdata[i + 2];

    d3dxvector4 vector(x,y,z,1.0f);
    d3dxvector4 worldvector(1.0f, 1.0f, 1.0f, 1.0f);
    // transform point to world space
    d3dxvec4transform(&worldvector, &vector, &worldmatrix);

    *buf << "v " << std::fixed << worldvector.x << " " << std::fixed << worldvector.y << " " << std::fixed << worldvector.z << "\n";

  }

  // paste faces
  *buf << "\n";
  printf("index count %d\n", m_IndexCount);
  for(unsigned int i=2; i<m_IndexCount; i++){
    short f1 = m_IndexData[i-2] + 1;
    short f2 = m_IndexData[i-1] + 1;
    short f3 = m_IndexData[i] + 1;

    *buf << "f " << f1 << " " << f2 << " " << f3 << "\n";
  }
}
