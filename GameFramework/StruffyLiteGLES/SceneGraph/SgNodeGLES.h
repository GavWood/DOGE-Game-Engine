////////////////////////////////////////////////////////////////////////////////
/// SgNodeWinGL.h

#ifndef __SgNodeWinGL_h__
#define __SgNodeWinGL_h__

#include <new>
#include "SgNode.h"
#include "BaFileData.h"
#include "ErrorLog.h"

struct RcRenderable;

// Scene node classes
class SgSkinWin32GL;
class SgMeshWinGL;
class SgMaterialsWinGL;

class SgRigidBodyImpl;
class SgBoneDX11;
class SgCollisionWinGL;
class SgCameraWin32;
class SgBlendShapeImpl;
class SgLightWin32;
struct SgMeshRenderable;

// Class Declaration
class SgNodeWinGL : public SgNode
{
public:

	SgNodeWinGL();

	void						Destroy();
	
	const MtMatrix4&			GetLocalTransform();
	void						SetLocalTransform( const MtMatrix4& Transform );
	const MtMatrix4&			GetWorldTransform();
	MtMatrix3 					WorldRotation() const;

	SgNode*						pFind( const BtChar* pName );
	SgNode*						GetDuplicate();

	void						FixPointers( BtU8 *pFileData, BaArchive *pArchive );
	void						Update();
	void						Render();

	void						Flatten( NodePtr& pSkeleton );
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

	void                        SetRendered( BtBool enabled );
	void                        SetForceRendered( BtBool enabled );
	BtBool                      IsRendered() const;

	BtBool						IsEnvMapped() const;
	BtBool						HasMaterials() const;

	// Instances
    BtU32						GetNumJoints();
    SgNode*						GetJoint(BtU32 jointIndex) const;
	SgRigidBody*				pRigidBody();
	SgBone*						pBone() const;
	SgMesh*						GetMesh() const;
	SgSkin*						pSkin() const;
	SgCollision*				pCollision() const;
	SgCamera*					pCamera() const;
	SgBlendShape*				pBlendShape() const;
	SgLight*					pLight() const;
	SgMaterial*					pMaterial() const;

private:

	friend class SgMeshWinGL;
	friend class SgSkinWin32GL;
	friend class SgCollisionWinGL;

	// Instances in order
	SgMeshWinGL*				m_pMesh;
	SgSkinWin32GL*				m_pSkin;
	SgRigidBodyImpl*			m_pRigidBody;
	SgCollisionWinGL*			m_pCollision;
	SgBoneDX11*				m_pBone;
	SgCameraWin32*				m_pCamera;
	SgBlendShapeImpl*			m_pBlendShape;
	SgLightWin32*				m_pLight;
	SgMaterialsWinGL*			m_pMaterials;

	// File data
	BaSgNodeFileData*			m_pFileData;
};

////////////////////////////////////////////////////////////////////////////////
// localTransform

inline const MtMatrix4& SgNodeWinGL::GetLocalTransform()
{
	return m_pFileData->m_m4Local;
}

////////////////////////////////////////////////////////////////////////////////
// worldTransform

inline const MtMatrix4& SgNodeWinGL::GetWorldTransform()
{
	return m_pFileData->m_m4World;
}

////////////////////////////////////////////////////////////////////////////////
// worldRotation

inline MtMatrix3 SgNodeWinGL::WorldRotation() const
{
	return m_pFileData->m_m4World.GetRotation();
}

////////////////////////////////////////////////////////////////////////////////
// NodeType

inline BtU32 SgNodeWinGL::NodeType() const
{
	return m_pFileData->m_nodeType;
}

////////////////////////////////////////////////////////////////////////////////
// pName

inline const BtChar* SgNodeWinGL::pName() const
{
	return m_pFileData->m_name;
}

////////////////////////////////////////////////////////////////////////////////
// HasChildren

inline BtBool SgNodeWinGL::HasChildren() const
{
	if( m_pFileData->m_pFirstChild != BtNull )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// pParent

inline SgNode* SgNodeWinGL::pParent() const
{
	return m_pFileData->m_pParent;
}

////////////////////////////////////////////////////////////////////////////////
// pFirstChild

inline SgNode* SgNodeWinGL::pFirstChild() const
{
	// Get the first child
	return (SgNodeWinGL*) m_pFileData->m_pFirstChild;
}

////////////////////////////////////////////////////////////////////////////////
// pNextSibling

inline SgNode* SgNodeWinGL::pNextSibling() const
{
	return (SgNodeWinGL*) m_pFileData->m_pNextSibling;
}

////////////////////////////////////////////////////////////////////////////////
// pMesh

inline SgMesh* SgNodeWinGL::GetMesh() const
{
	return (SgMesh*) m_pMesh;
}

////////////////////////////////////////////////////////////////////////////////
// pSkin

inline SgSkin* SgNodeWinGL::pSkin() const
{
	return (SgSkin*) m_pSkin;
}

////////////////////////////////////////////////////////////////////////////////
// pCollision

inline SgCollision* SgNodeWinGL::pCollision() const
{
	return (SgCollision*) m_pCollision;
}

////////////////////////////////////////////////////////////////////////////////
// pCamera

inline SgCamera* SgNodeWinGL::pCamera() const
{
	return (SgCamera*) m_pCamera;
}

////////////////////////////////////////////////////////////////////////////////
// pBlendShape

inline SgBlendShape* SgNodeWinGL::pBlendShape() const
{
	return (SgBlendShape*) m_pBlendShape;
}

////////////////////////////////////////////////////////////////////////////////
// pLight

inline SgLight* SgNodeWinGL::pLight() const
{
	return (SgLight*) m_pLight;
}


////////////////////////////////////////////////////////////////////////////////
// IsEnvMapped

inline BtBool SgNodeWinGL::IsEnvMapped() const
{
	if( m_pFileData->m_nodeType & SgType_EnvMapped )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// HasMaterial

inline BtBool SgNodeWinGL::HasMaterials() const
{
	if( m_pFileData->m_nodeType & SgType_Materials )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// pMaterial

inline SgMaterial* SgNodeWinGL::pMaterial() const
{
	return (SgMaterial*) m_pMaterials;
}

#endif // __SgNodeWinGL_h__
