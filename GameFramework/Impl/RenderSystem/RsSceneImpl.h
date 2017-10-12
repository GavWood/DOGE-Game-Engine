////////////////////////////////////////////////////////////////////////////////
/// RcSceneWinGL.h

#pragma once

#include "BtBase.h"
#include "BaResource.h"
#include "BaFileData.h"
#include "RsPrimitive.h"

class DyCollisionMesh;
class RsVertexBufferImpl;
class RsIndexBufferImpl;

// Class Declaration
class RsSceneImpl : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Scene );

	void							FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	void							CreateOnDevice();

	BaRenderBlockFileData*			pRenderBlock( BtU32 nIndex );
	RsIndexedPrimitive*             pPrimitiveBlock( BtU32 nIndex );
	RsVertexBufferImpl*			pVertexBuffer( BtU32 vertexType );
	RsIndexBufferImpl*				pIndexBuffer();
	DyCollisionMesh*				pCollisionMesh();
	DyCollisionAnalytical*			pCollisionAnalytical();

private:

	BaMaterialBlockFileData* 		m_pMaterialBlocks;
	BaRenderBlockFileData*	 		m_pRenderBlocks;
	RsIndexedPrimitive*             m_pPrimitives;

	BaSceneFileData*				m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// pRenderBlock

inline BaRenderBlockFileData* RsSceneImpl::pRenderBlock( BtU32 nIndex )
{
	BtAssert( nIndex < m_pFileData->m_nRenderBlocks );

	return &m_pRenderBlocks[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// pPrimitiveBlock

inline RsIndexedPrimitive* RsSceneImpl::pPrimitiveBlock( BtU32 nIndex )
{
	BtAssert( nIndex < m_pFileData->m_nPrimitives );

	return &m_pPrimitives[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// pVertexBuffer

inline RsVertexBufferImpl* RsSceneImpl::pVertexBuffer( BtU32 vertexType )
{
	return (RsVertexBufferImpl*)m_pFileData->m_pVertexBuffer[vertexType];
}

////////////////////////////////////////////////////////////////////////////////
// pIndexBuffer

inline RsIndexBufferImpl* RsSceneImpl::pIndexBuffer()
{
	return (RsIndexBufferImpl*)m_pFileData->m_pIndexBuffer;
}

////////////////////////////////////////////////////////////////////////////////
// pCollisionMesh

inline DyCollisionMesh* RsSceneImpl::pCollisionMesh()
{
	return m_pFileData->m_pCollisionMesh;
}

////////////////////////////////////////////////////////////////////////////////
// pCollisionAnalytical

inline DyCollisionAnalytical* RsSceneImpl::pCollisionAnalytical()
{
	return m_pFileData->m_pCollisionAnalytical;
}
