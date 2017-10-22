////////////////////////////////////////////////////////////////////////////////
/// DyImpl.h

#pragma once
#include "BtTypes.h"
#include "DyWorld.h"

class btRigidBody;

// Class Declaration
class DyImpl
{
public:

	static void					Create();
	static void					Destroy();
	static void					Update();
	static void					AddRigidBody( btRigidBody* pRigidBody );
	static void					RemoveRigidBody( btRigidBody* pRigidBody );
	static void					RemoveSoftBody(btSoftBody* pSoftBody);

	static DyWorld*				pWorld();

	static BtBool               IsPaused();
	static void                 SetPaused(BtBool isPaused);

private:

	static DyWorld				m_world;
	static BtBool				m_isPaused;
};
