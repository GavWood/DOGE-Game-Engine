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
#include "MtVector3.h"
#include "RsColour.h"
#include <vector>

class btCompoundShape;
class btMotionState;
class btDefaultMotionState;
class btRigidBody;
class btSoftBody;
class SgNode;
class DyBody;

// Class Declaration
class DySoftBody
{
public:

	DySoftBody();

	void					CreatePatch(MtVector3 v3A, MtVector3 v3B, BtFloat width, BtU32 segments, BtBool anchorA, BtBool anchorB, BtFloat mass);
	void					CreateRope( MtVector3 v3A, MtVector3 v3B, BtU32 segments, BtBool anchorA, BtBool anchorB, BtFloat mass );
	void					Detach();

	void					Update();
	void					Render( const RsColour &colour );
	void					Destroy();

	void					Move( MtVector3 v3Pos );

	void					AttachFirst( DyBody *pBody );
	void					AttachLast( DyBody *pBody );

	void					SetWorldTransform( const MtMatrix4& m4WorldTransform );
	MtMatrix4				GetWorldTransform() const;
	MtVector3				GetPosition() const;

//protected:

	// Rigid body
	btSoftBody			   *m_pSoftBody;
};
