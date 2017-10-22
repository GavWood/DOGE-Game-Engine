////////////////////////////////////////////////////////////////////////////////
// DyDyWorld.h

#pragma once
#include "BtTypes.h"
#include "MtVector3.h"

// Collision
class btCollisionAlgorithm;
class btCompoundCollisionAlgorithm;
class btConvexConcaveCollisionAlgorithm;
class btConvexConvexCAlgorithm;
class btEmptyAlgorithm;
class btSphereBoxCollisionAlgorithm;
class btSphereSphereCollisionAlgorithm;
class btSphereTriangleCollisionAlgorithm;
class btCollisionWorld;
class btCollisionObject;
class btCollisionShape;
class btCompoundShape;
class btConcaveShape;
class btEmptyShape;
class btHeightfieldTerrainShape;
class btStaticPlaneShape;
class btTriangleMeshShape;
class btConvexShape;
class btConvexInternalShape;
class btUniformScalingShape;
class btCapsuleShape;
class btConeShape;
class btMinkowskiSumShape;
class btMultiSphereShape;
class btPolyhedralConvexShape;
class btConvexHullShape;
class btConvexTriangleMeshShape;
class btTetrahedronShape;
class btTriangleShape;
class btSphereShape;
class btBoxShape;
class btCollisionConfiguration;
class btDefaultCollisionConfiguration;
class btBroadphaseInterface;

// Dispatchers
class btDispatcher;
class btCollisionDispatcher;

// Constraints
class btConstraintSolver;
class btSequentialImpulseConstraintSolver;
class btRigidBody;
class btSoftBody;

// World
class btDynamicsWorld;
class DyBody;

struct DyCollision
{
	MtVector3								m_hitPointInWorld;
	MtVector3								m_hitNormalInWorld;
	BtFloat									m_distFraction;
	void								   *m_pUserPointer;
};

// Class Declaration
class DyWorld
{
public:

	void									Create();
	void									Update();
	void									Destroy();

	void									AddRigidBody( btRigidBody* pRigidBody );
	void									RemoveRigidBody( btRigidBody* pRigidBody );
	void									RemoveSoftBody(btSoftBody* pSoftBody);

	const btRigidBody*						RayCast( const MtVector3& from,
													 const MtVector3& to,
													 DyCollision& collision
												   );

	void									RegisterBroadphaseCollision();

	const void*								GetUserPointerFromRayCast( const MtVector3& from,
																	   const MtVector3& to,
																	   DyCollision& collision
																	);

	static BtFloat							MetersPerSecondToKph( BtFloat SpeedInMetersPerSecond );
	static BtFloat							MetersPerSecondToKnots( BtFloat SpeedInMetersPerSecond );
	static BtFloat							KnotsToMetersPerSecond(BtFloat knots );
	static BtFloat							MetersPerSecondToMph( BtFloat SpeedInMetersPerSecond );

	MtVector3								GetGravity() const;
	void									SetGravity( const MtVector3& v3Gravity );

	BtFloat									GetSimulationSpeed() const;
	void									SetSimulationSpeed( BtFloat speed );

	btDynamicsWorld*						GetDynamicsWorld() { return m_pDynamicsWorld; }

	void									AddConstraint(DyBody *pA, DyBody *pB);
	void									AddRigidConstraint(DyBody *pA, DyBody *pB);

	// Accessors
	void									SetSubsteps( BtU32 subSteps );

private:

	friend class DyVehicle;
	friend class DySliderConstraint;
	friend class DyBody;
	friend class DySoftBody;

	btDefaultCollisionConfiguration*		m_pCollisionConfiguration;
	btCollisionDispatcher*					m_pCollisionDispatcher;
	btConstraintSolver*						m_pConstraintSolver;

	// 
	btBroadphaseInterface*					m_pBroadphaseInterface;

	// The dynamics world
	btDynamicsWorld*						m_pDynamicsWorld;

	BtBool									m_paused;

	BtFloat									m_gravity;

	BtFloat									m_tick;

	BtFloat									m_speed;

	BtU32									m_subSteps;
};

inline void DyWorld::SetSimulationSpeed( BtFloat speed )
{
	m_speed = speed;
}

inline BtFloat DyWorld::GetSimulationSpeed() const
{
	return m_speed;
}
