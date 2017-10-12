////////////////////////////////////////////////////////////////////////////////
/// RcSceneWinGL.cpp

#include "BtMemory.h"
#include "BaArchive.h"
#include "RsSceneImpl.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void RsSceneImpl::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	// Set the file data
	m_pFileData = (BaSceneFileData*) pFileData;

	// Setup the blocks
	pFileData += sizeof( BaSceneFileData );

	m_pRenderBlocks = (BaRenderBlockFileData*) pFileData;
	pFileData +=( sizeof( BaRenderBlockFileData ) * m_pFileData->m_nRenderBlocks );

	m_pPrimitives = (RsIndexedPrimitive*) pFileData;
	pFileData +=( sizeof( RsIndexedPrimitive ) * m_pFileData->m_nPrimitives );

	for( BtU32 i=0; i<1024; i++ )
	{
		if( m_pFileData->m_nVertexBuffer[i] != BtNull )
		{
			m_pFileData->m_pVertexBuffer[i] = (BaResource*)pArchive->GetResource( m_pFileData->m_nVertexBuffer[i] );
		}
	}

	m_pFileData->m_pIndexBuffer  = (BaResource*) pArchive->GetResource( m_pFileData->m_nIndexBuffer );
	m_pFileData->m_pCollisionMesh = (DyCollisionMesh*) pArchive->GetResource( m_pFileData->m_nCollisionMesh );
	m_pFileData->m_pCollisionAnalytical = (DyCollisionAnalytical*) pArchive->GetResource( m_pFileData->m_nCollisionAnalytical );
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsSceneImpl::CreateOnDevice()
{
}
