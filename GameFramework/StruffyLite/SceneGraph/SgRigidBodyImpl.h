////////////////////////////////////////////////////////////////////////////////
/// SgRigidBodyImpl.h

#pragma once
#include "BaFileData.h"
#include "SgRigidBody.h"

class SgNodeDX11;

// Class Declaration
class SgRigidBodyImpl : public SgRigidBody
{
public:

	SgRigidBodyImpl();
	~SgRigidBodyImpl();

	MtVector3					CenterOfMass();
	void						SetMass( BtFloat mass );
	BtFloat						GetMass();
	BtBool						IsDynamic();
	BtFloat						Restitution();
	BtFloat						Friction();

private:

	friend class SgNodeDX11;
	friend class SgNodeWinDX;
	friend class SgNodeWinGL;

	void						WorldTransform( const MtMatrix4& m4Transform );
	void						FixPointers( BtU8* pMemory );

	SgNode					   *m_pNode;
	BaSgRigidBodyFileData*		m_pRigidBodyFileData;
};
