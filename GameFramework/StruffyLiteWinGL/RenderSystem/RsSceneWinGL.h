////////////////////////////////////////////////////////////////////////////////
/// RcSceneWinGL.h

#ifndef __RcSceneWinGL_h__
#define __RcSceneWinGL_h__

#include "BtBase.h"
#include "BaResource.h"
#include "BaFileData.h"
#include "RsPrimitiveWinGL.h"

class DyCollisionMesh;
class RsVertexBufferWinGL;
class RsIndexBufferWinGL;

// Class Declaration
class RsSceneWinGL : public BaResource
{
public:

	DECLARE_RESOURCETYPE( BaRT_Scene );

	void							FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	void							CreateOnDevice();

	BaRenderBlockFileData*			pRenderBlock( BtU32 nIndex );
	RsIndexedPrimitiveWinGL*		pPrimitiveBlock( BtU32 nIndex );
	RsVertexBufferWinGL*			pVertexBuffer( BtU32 vertexType );
	RsIndexBufferWinGL*				pIndexBuffer();
	DyCollisionMesh*				pCollisionMesh();
	DyCollisionAnalytical*			pCollisionAnalytical();

private:

	BaMaterialBlockFileData* 		m_pMaterialBlocks;
	BaRenderBlockFileData*	 		m_pRenderBlocks;
	RsIndexedPrimitiveWinGL*		m_pPrimitives;
	BaSceneFileData*				m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// pRenderBlock

inline BaRenderBlockFileData* RsSceneWinGL::pRenderBlock( BtU32 nIndex )
{
	BtAssert( nIndex < m_pFileData->m_nRenderBlocks );

	return &m_pRenderBlocks[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// pPrimitiveBlock

inline RsIndexedPrimitiveWinGL* RsSceneWinGL::pPrimitiveBlock( BtU32 nIndex )
{
	BtAssert( nIndex < m_pFileData->m_nPrimitives );

	return &m_pPrimitives[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// pVertexBuffer

inline RsVertexBufferWinGL* RsSceneWinGL::pVertexBuffer( BtU32 vertexType )
{
	return (RsVertexBufferWinGL*)m_pFileData->m_pVertexBuffer[vertexType];
}

////////////////////////////////////////////////////////////////////////////////
// pIndexBuffer

inline RsIndexBufferWinGL* RsSceneWinGL::pIndexBuffer()
{
	return (RsIndexBufferWinGL*)m_pFileData->m_pIndexBuffer;
}

////////////////////////////////////////////////////////////////////////////////
// pCollisionMesh

inline DyCollisionMesh* RsSceneWinGL::pCollisionMesh()
{
	return m_pFileData->m_pCollisionMesh;
}

////////////////////////////////////////////////////////////////////////////////
// pCollisionAnalytical

inline DyCollisionAnalytical* RsSceneWinGL::pCollisionAnalytical()
{
	return m_pFileData->m_pCollisionAnalytical;
}

#endif // __RcSceneWinGL_h__
