////////////////////////////////////////////////////////////////////////////////
/// DyBody.h

#pragma once
#include "BtTypes.h"
#include "BtArray.h"
#include "MtVector3.h"
#include "MtMatrix3.h"
#include "MtMatrix4.h"
#include "MtAABB.h"
#include "RsVertex.h"
#include <vector>

class btCompoundShape;
class btMotionState;
class btDefaultMotionState;
class btRigidBody;
class btCollisionShape;
class SgNode;
class DyBody;

struct DyContact
{
	MtVector3				m_v3Position;
	DyBody				   *m_pBody;
};

// Class Declaration
class DyBody
{
public:

	DyBody();

	void					Create();
	void					Create( BtFloat mass );
	void					CreateBox( BtFloat mass, const MtVector3 &v3Dimension );
	void					CreateSphere( BtFloat mass, const MtVector3 &v3Dimension );
	void					CreateCylinder( BtFloat mass, const MtVector3 &v3Dimension );
	void					Create( SgNode* pNode );
	void					CreateSubShapes( SgNode* pNode );
	void					Create( std::vector<MtVector3> vertex, MtMatrix4 &m4Transform );

	void					Destroy();

	BtFloat					GetMass() const;
	void					SetMass( BtFloat mass, const MtVector3& v3Tensor, const MtVector3& v3Gravity );

	void					ClearForces();

	void					ApplyWorldForce(   const MtVector3& v3WorldForce, MtVector3& v3LocalPosition );
	void					ApplyWorldImpulse( const MtVector3& v3WorldForce, const MtVector3& v3LocalPosition );
	
	void					ApplyLocalForce(   const MtVector3& v3LocalForce, const MtVector3& v3LocalPosition );
	void					ApplyLocalImpulse( const MtVector3& v3LocalForce, MtVector3& v3LocalPosition );
	void					ApplyTorque( const MtVector3& v3Torque );

	MtVector3				GetLocalXInWorld();
	MtVector3				GetLocalYInWorld();
	MtVector3				GetLocalZInWorld();

	MtMatrix4 				GetWorldTransform() const;
	void	 				SetWorldTransform( const MtMatrix4& WorldTransform );
	MtMatrix3				GetRotation() const;
	MtVector3 				GetPosition() const;

	MtMatrix3				GetInverseInertiaTensor() const;
	MtVector3				GetCenterOfMass() const;
	
	BtFloat					GetSpeed() const;
	MtVector3				GetWorldVelocity() const;
	MtVector3				GetWorldVelocity( const MtVector3& v3WorldPosition ) const;
	MtVector3				GetWorldPosition(const MtVector3& v3LocalPosition) const;
	MtVector3				GetLocalVelocity() const;
	MtVector3				GetLocalVelocity( const MtVector3& v3LocalPosition ) const;
	void					SetLocalVelocity( const MtVector3& v3LocalVelocity );
	void					SetWorldVelocity( const MtVector3& v3WorldVelocity );
	MtVector3				GetWorldAngularVelocity() const;
	void					SetWorldAngularVelocity( const MtVector3& v3WorldVelocity );
	MtVector3				GetLocalAngularVelocity() const;
	void					SetLocalAngularVelocity( MtVector3 v3LocalVelocity );

	MtVector3				GetDimension();

	void					SetFriction( BtFloat friction );
	void					SetRestitution( BtFloat bounce );
	void					SetDamping( BtFloat scalar );
	void					SetDamping( BtFloat lindamping, BtFloat angdamping );

	void					SetNoCollision();
	void					SetUserPointer( void* userPointer );

	btRigidBody*			GetRigidBody() { return m_pRigidBody; }

	BtU32					GetNumContacts() const;
	const DyContact*		GetContact( BtU32 iContact ) const;

	BtBool					GetActive() { return m_isActive; }
	void					SetActive( BtBool isActive );

	BtBool					IsRigidBody();

	const MtVector3			GetVecI() const;
	const MtVector3			GetVecJ() const;
	const MtVector3			GetVecK() const;

	void					Stop();
	void					Update();
	void					Render();

protected:

	friend class DySliderConstraint;
	friend class DyVehicle;
	friend class Mugi;
	friend class DySoftBody;

	// Rigid body
	btDefaultMotionState*	m_pMotionState;
	btRigidBody*			m_pRigidBody;
	btCompoundShape*		m_pCompound;
	btCollisionShape  	   *m_pBoxShape;
	BtFloat					m_mass;
	SgNode*					m_pNode;
	BtArray<DyContact, 256> m_contacts;
	BtChar					m_name[256];
	MtVector3				m_v3Dimension;
	BtBool					m_isActive;
};

inline BtU32 DyBody::GetNumContacts() const
{
	return m_contacts.GetNumItems();
}

inline const DyContact* DyBody::GetContact( BtU32 iContact ) const
{
	return &m_contacts[iContact];
}

inline BtFloat DyBody::GetMass() const
{
	return m_mass;
}

inline BtBool DyBody::IsRigidBody()
{
	return m_pRigidBody != BtNull;
}
