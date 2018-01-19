////////////////////////////////////////////////////////////////////////////////
/// SgNodeWinGL.h

#pragma once
#include <new>
#include "SgNode.h"
#include "BaFileData.h"
#include "ErrorLog.h"

struct RcRenderable;

// Scene node classes
class SgSkinImpl;
class SgMeshWinGL;
class SgMaterialsWinGL;

class SgRigidBodyImpl;
class SgBoneImpl;
class SgCollisionWinGL;
class SgCameraWin32;
class SgBlendShapeImpl;
class SgLightWin32;
struct SgMeshRenderable;

// Class Declaration
class SgNodeImpl : public SgNode
{
public:

	SgNodeImpl();

	void						Destroy();
	
    static BtU32                GetInstanceSize(BaResourceHeader *pResourceHeader);
    
	const MtMatrix4&			GetLocalTransform();
	void						SetLocalTransform( const MtMatrix4& Transform );
	const MtMatrix4&			GetWorldTransform();
	MtMatrix3 					WorldRotation() const;

	SgNode*						pFind( const BtChar* pName );
	SgNode*						GetDuplicate();

	void						FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	void						Update();
	void						Render();

	void						AddChild( SgNode* pChildInst );
	void						RemoveChild( SgNode* pChildInst );

	// Accessors

	// Hierarchy
	BtU32						NodeType() const;
	const BtChar*				pName() const;
	SgNode*						pParent() const;
	SgNode*						pFirstChild() const;
	SgNode*						pNextSibling() const;
	BtBool						HasChildren() const;

	// Accessors
	void						SetRendered( BtBool enabled );
	void						SetForceRendered( BtBool enabled );
	BtBool						IsForceRendered() const;
	BtBool						IsRendered() const;

	BtBool						IsEnvMapped() const;
	BtBool						HasMaterials() const;

	// Instances
	SgRigidBody*				pRigidBody();
	SgBone*						pBone() const;
	SgMesh*						GetMesh() const;
	SgSkin*						pSkin() const;
	SgCollision*				pCollision() const;
	SgCamera*					pCamera() const;
	SgBlendShape*				pBlendShape() const;
	SgLight*					pLight() const;
	SgMaterial*					pMaterial() const;
	BtU32						GetNumJoints();
	SgNode*						GetJoint(BtU32 jointIndex) const;

private:

	friend class SgMeshWinGL;
	friend class SgSkinImpl;
	friend class SgCollisionWinGL;

	// Instances in order
	SgMeshWinGL*				m_pMesh;
	SgSkinImpl*					m_pSkin;
	SgRigidBodyImpl*			m_pRigidBody;
	SgCollisionWinGL*			m_pCollision;
	SgBoneImpl*					m_pBone;
	SgCameraWin32*				m_pCamera;
	SgBlendShapeImpl*			m_pBlendShape;
	SgLightWin32*				m_pLight;
	SgMaterialsWinGL*			m_pMaterials;

	// File data
	BaSgNodeFileData*			m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// localTransform

inline const MtMatrix4& SgNodeImpl::GetLocalTransform()
{
	return m_pFileData->m_m4Local;
}

////////////////////////////////////////////////////////////////////////////////
// worldTransform

inline const MtMatrix4& SgNodeImpl::GetWorldTransform()
{
	return m_pFileData->m_m4World;
}

////////////////////////////////////////////////////////////////////////////////
// worldRotation

inline MtMatrix3 SgNodeImpl::WorldRotation() const
{
	return m_pFileData->m_m4World.GetRotation();
}

////////////////////////////////////////////////////////////////////////////////
// NodeType

inline BtU32 SgNodeImpl::NodeType() const
{
	return m_pFileData->m_nodeType;
}

////////////////////////////////////////////////////////////////////////////////
// pName

inline const BtChar* SgNodeImpl::pName() const
{
	return m_pFileData->m_name;
}

////////////////////////////////////////////////////////////////////////////////
// HasChildren

inline BtBool SgNodeImpl::HasChildren() const
{
	if( m_pFileData->m_pFirstChild != BtNull )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// pParent

inline SgNode* SgNodeImpl::pParent() const
{
	return m_pFileData->m_pParent;
}

////////////////////////////////////////////////////////////////////////////////
// pFirstChild

inline SgNode* SgNodeImpl::pFirstChild() const
{
	// Get the first child
	return (SgNodeImpl*) m_pFileData->m_pFirstChild;
}

////////////////////////////////////////////////////////////////////////////////
// pNextSibling

inline SgNode* SgNodeImpl::pNextSibling() const
{
	return (SgNodeImpl*) m_pFileData->m_pNextSibling;
}

////////////////////////////////////////////////////////////////////////////////
// pMesh

inline SgMesh* SgNodeImpl::GetMesh() const
{
	return (SgMesh*) m_pMesh;
}

////////////////////////////////////////////////////////////////////////////////
// pSkin

inline SgSkin* SgNodeImpl::pSkin() const
{
	return (SgSkin*) m_pSkin;
}

////////////////////////////////////////////////////////////////////////////////
// pCollision

inline SgCollision* SgNodeImpl::pCollision() const
{
	return (SgCollision*) m_pCollision;
}

////////////////////////////////////////////////////////////////////////////////
// pCamera

inline SgCamera* SgNodeImpl::pCamera() const
{
	return (SgCamera*) m_pCamera;
}

////////////////////////////////////////////////////////////////////////////////
// pBlendShape

inline SgBlendShape* SgNodeImpl::pBlendShape() const
{
	return (SgBlendShape*) m_pBlendShape;
}

////////////////////////////////////////////////////////////////////////////////
// pLight

inline SgLight* SgNodeImpl::pLight() const
{
	return (SgLight*) m_pLight;
}

////////////////////////////////////////////////////////////////////////////////
// IsEnvMapped

inline BtBool SgNodeImpl::IsEnvMapped() const
{
	if( m_pFileData->m_nodeType & SgType_EnvMapped )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// HasMaterial

inline BtBool SgNodeImpl::HasMaterials() const
{
	if( m_pFileData->m_nodeType & SgType_Materials )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// pMaterial

inline SgMaterial* SgNodeImpl::pMaterial() const
{
	return (SgMaterial*) m_pMaterials;
}
