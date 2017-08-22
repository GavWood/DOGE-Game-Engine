////////////////////////////////////////////////////////////////////////////////
// ExStrip.cpp

// Includes
#include "ExSgNode.h"
#include "ExSgMesh.h"
#include "ExRigidBody.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExRigidBody::ExRigidBody( ExSgNode* pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
	m_mass = 0;
	m_isDynamic = BtFalse;
	m_friction = 0;
	m_restitution = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExRigidBody::~ExRigidBody()
{
}

////////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExRigidBody::CopyAttributes()
{
	// Set the inertia tensor
	m_pNode->m_rigidBodyFileData.m_mass = m_mass;

	// Set whether the object is static
	m_pNode->m_rigidBodyFileData.m_isDynamic = m_isDynamic;

	// Set the center of mass
	m_pNode->m_rigidBodyFileData.m_v3CenterOfMass = MtVector3( 0, 0, 0 );	

	// Set the friction
	m_pNode->m_rigidBodyFileData.m_friction = m_friction;

	// Set the restitution
	m_pNode->m_rigidBodyFileData.m_restitution = m_restitution;	
}
