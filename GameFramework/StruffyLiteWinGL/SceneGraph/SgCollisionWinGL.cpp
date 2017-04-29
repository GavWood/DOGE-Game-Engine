////////////////////////////////////////////////////////////////////////////////
/// SgCollisionWin32.cpp

#include "BtBase.h"
#include "DyCollisionMesh.h"
#include "DyCollisionAnalytical.h"
#include "SgCollisionWinGL.h"
#include "RsSceneWinGL.h"
#include "SgNodeWinGL.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

SgCollisionWinGL::SgCollisionWinGL()
{
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgCollisionWinGL::FixPointers( BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgCollisionFileData*) pMemory;

	// Cache the scene node
	RsSceneWinGL *pSceneNode = (RsSceneWinGL*)m_pNode->m_pFileData->m_pScene;

	BtAssert( pSceneNode != BtNull );

	// Set the collision mesh
	m_pCollisionMesh = pSceneNode->pCollisionMesh();

	// Set the analytical geometry
	m_pCollisionAnalytical = pSceneNode->pCollisionAnalytical();
}

////////////////////////////////////////////////////////////////////////////////
// NumShapes

BtU32 SgCollisionWinGL::NumShapes() const
{
	return m_pFileData->m_endShape - m_pFileData->m_startShape;
}

////////////////////////////////////////////////////////////////////////////////
// pShape

const SgCollisionShape* SgCollisionWinGL::pShape( BtU32 iShape ) const
{
	BtU32 iShapeIndex = m_pFileData->m_startShape + iShape;
	//BtAssert( iShape < m_pFileData->m_pBodies );

	return m_pCollisionAnalytical->pCollisionShape( iShapeIndex );
}

////////////////////////////////////////////////////////////////////////////////
// pVertex

BtFloat* SgCollisionWinGL::pVertex() const
{
	return m_pCollisionMesh->pVertex();
}

////////////////////////////////////////////////////////////////////////////////
// pIndex

BtU32* SgCollisionWinGL::pIndex() const
{
	return m_pCollisionMesh->pIndex();
}