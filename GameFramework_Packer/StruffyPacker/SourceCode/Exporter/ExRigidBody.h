////////////////////////////////////////////////////////////////////////////////
// ExRigidBody.h

#ifndef __ExRigidBody_h__
#define __ExRigidBody_h__

struct ExSgNode;

class ExRigidBody
{
public:

	ExRigidBody( ExSgNode* pNode, ExScene *pScene );
	~ExRigidBody();

	void							CopyAttributes();

	// Accessors
	void							Mass( BtFloat mass );
	void							IsDynamic( BtBool isDynamic );
	void							Friction( BtFloat friction );
	void							Restitution( BtFloat restitution );

private:

	ExSgNode*						m_pNode;
	ExScene*						m_pScene;
	BtBool							m_isDynamic;
	BtFloat							m_mass;
	MtMatrix4						m_m4Transform;
	BtFloat							m_friction;
	BtFloat							m_restitution;
};

inline void ExRigidBody::Mass( BtFloat mass )
{
	m_mass = mass;
}

inline void ExRigidBody::Friction( BtFloat friction )
{
	m_friction = friction;
}

inline void ExRigidBody::Restitution( BtFloat restitution )
{
	m_restitution = restitution;
}

inline void ExRigidBody::IsDynamic( BtBool isDynamic )
{
	m_isDynamic = isDynamic;
}

#endif // __ExRigidBody_h__
