///////////////////////////////////////////////////////////////////////////////
// SgNode.h

#pragma once
#include "BaResource.h"
#include "MtVector3.h"
#include "MtMatrix3.h"
#include "MtMatrix4.h"

////////////////////////////////////////////////////////////////////////////////
// SgType

enum SgType
{
	SgType_Node = 1 << 0,
	SgType_Mesh = 1 << 1,
	SgType_RigidBody = 1 << 2,
	SgType_Skin = 1 << 3,
	SgType_Collision = 1 << 4,
	SgType_Bone = 1 << 5,
	SgType_Camera = 1 << 6,
	SgType_BlendShape = 1 << 7,
	SgType_Light = 1 << 8,
	SgType_Materials = 1 << 9,
	SgType_EnvMapped = 1 << 10,
};

class RsMaterial;
class SgLight;
class SgRigidBody;
class SgBone;
class SgMesh;
class SgSkin;
class SgMaterial;
class SgCollision;
class SgCamera;
class SgBlendShape;

typedef SgBone** NodePtr;

// Class declaration
class SgNode : public BaResource
{
public:
	
	DECLARE_RESOURCETYPE( BaRT_SgNode );

	virtual BtU32				NodeType() const = 0;
	virtual const BtChar*		pName() const = 0;
	virtual BtBool				HasChildren() const = 0;
	virtual SgNode*				pFind( const BtChar* pName ) = 0;
	virtual SgNode*				pParent() const = 0;
	virtual SgNode*				pFirstChild() const = 0;
	virtual SgNode*				pNextSibling() const = 0;
	virtual SgNode*				GetDuplicate() = 0;
	virtual void				AddChild( SgNode* pNode ) = 0;
	virtual void				RemoveChild( SgNode* pNode ) = 0;
	virtual void				Update() = 0;
	virtual void				Render() = 0;

	// Accessors
	virtual BtBool				IsEnvMapped() const = 0;
	virtual const MtMatrix4&	GetLocalTransform() = 0;
	virtual void				SetLocalTransform( const MtMatrix4& Transform ) = 0;
	virtual const MtMatrix4&	GetWorldTransform() = 0;
	virtual MtMatrix3 			WorldRotation() const = 0;
	virtual BtBool				HasMaterials() const = 0;

	virtual void				SetRendered( BtBool enabled ) = 0;
	virtual void				SetForceRendered( BtBool enabled ) = 0;
	virtual BtBool				IsRendered() const = 0;

	//

	// Node functionality				 
	virtual BtU32				GetNumJoints() = 0;
	virtual SgNode*				GetJoint( BtU32 jointIndex ) const = 0;
	virtual SgRigidBody*		pRigidBody() = 0;
	virtual SgBone*				pBone() const = 0;
	virtual SgMesh*				GetMesh() const = 0;
	virtual SgLight*			pLight() const = 0;
	virtual SgSkin*				pSkin() const = 0;
	virtual SgCollision*		pCollision() const = 0;
	virtual SgCamera*			pCamera() const = 0;
	virtual SgBlendShape*		pBlendShape() const = 0;
	virtual SgMaterial*			pMaterial() const = 0;
};
