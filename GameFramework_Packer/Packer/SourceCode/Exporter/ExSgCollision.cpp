////////////////////////////////////////////////////////////////////////////////
// ExSgCollision

#include "BtCRC.h"
#include "ExSgCollision.h"
#include "ExSgNode.h"
#include "ExSgMesh.h"
#include "ExScene.h"
#include "DyCollisionMesh.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgCollision::ExSgCollision( ExSgNode* pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
}

////////////////////////////////////////////////////////////////////////////////
// Begin

void ExSgCollision::Begin( BtU32 count )
{
	//m_pNode->m_collisionFileData.m_startShape = count;
	m_pNode->m_collisionFileData.m_startShape = m_pScene->CollisionAnalytical().NumShapes();
}

////////////////////////////////////////////////////////////////////////////////
// End

void ExSgCollision::End( BtU32 count )
{
	//m_pNode->m_collisionFileData.m_startShape = count;
	m_pNode->m_collisionFileData.m_endShape = m_pScene->CollisionAnalytical().NumShapes();
	m_pNode->m_collisionFileData.m_shapes = m_pNode->m_collisionFileData.m_endShape - 
											m_pNode->m_collisionFileData.m_startShape;

}
