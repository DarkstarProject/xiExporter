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
    m_Indexes       = NULL;
    m_IndexCount    = NULL;
}

void MapObjectVertexBuffer::InitBuffers(UINT NumIndexes, UINT NumVerteses, CONST void* pIndexData, CONST void* pVertexData)
{
    m_VertexCount = NumVerteses;
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

    m_IndexCount = NumIndexes;
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

void MapObjectVertexBuffer::WriteObj(std::stringstream* buf, D3DXMATRIX matrix, D3DCULL cull)
{
  *buf << "hello\n";
}
