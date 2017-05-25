////////////////////////////////////////////////////////////////////////////////
/// SgNodeGLES.cpp

#include "BtMemory.h"
#include "BtString.h"
#include "SgNodeGLES.h"
#include "RsMaterial.h"
#include "RsMaterialGLES.h"
#include "RsCamera.h"
#include "RsTextureGLES.h"

#include "SgMaterialsGLES.h"
#include "SgCameraImpl.h"
#include "SgLightImpl.h"
#include "SgBoneImpl.h"
#include "SgBlendShapeImpl.h"
#include "SgCollisionGLES.h"
#include "SgRigidBodyImpl.h"
#include "RsImplGLES.h"
#include "RsSceneGLES.h"

#include "SgMeshGLES.h"
#include "SgSkinGLES.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

SgNodeWinGL::SgNodeWinGL()
{
	m_pRigidBody = BtNull;
	m_pBone = BtNull;
	m_pCollision = BtNull;
	m_pMesh = BtNull;
	m_pSkin = BtNull;
	m_pBlendShape = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SgNodeWinGL::Destroy()
{
}

////////////////////////////////////////////////////////////////////////////////
// pFind

SgNode* SgNodeWinGL::pFind( const BtChar* pName )
{
	if( BtStrCompare( m_pFileData->m_name, pName ) == BtTrue )
	{
		return this;
	}

	// Cache the first child
	SgNodeWinGL* pChild = (SgNodeWinGL*) m_pFileData->m_pFirstChild;

	// Loop through the children
	while( pChild != BtNull )
	{
		SgNodeWinGL* pNode = (SgNodeWinGL*)pChild->pFind( pName );

		// Update the child
		if( pNode != BtNull )
		{
			return pNode; 
		}

		// Move to the next child
		pChild = (SgNodeWinGL*) pChild->m_pFileData->m_pNextSibling;
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Duplicate

// virtual
SgNode* SgNodeWinGL::GetDuplicate()
{
	// Allocate the memory
	BtU8* pMemory = BtMemory::Allocate( m_pFileData->m_nInstanceSize + m_pFileData->m_nFileDataSize );
    
	// Create an instance of the new node
	SgNodeWinGL* pSgNode = new (pMemory) SgNodeWinGL;
    
	// Find the new file data
	BtU8* pNewFileData = pMemory + m_pFileData->m_nInstanceSize;
    
	// Set the file data
	pSgNode->m_pFileData = (BaSgNodeFileData*)pNewFileData;
    
	// Set the title
	pSgNode->m_pTitle = m_pTitle;
    
	// Copy the file data
	BtMemory::Copy( pNewFileData, m_pFileData, m_pFileData->m_nFileDataSize );
    
	// Flag as a duplicate
	pSgNode->m_isDuplicate = BtTrue;
    
	// Fix up the pointers
	pSgNode->FixPointers( pNewFileData, m_pArchive );
    
	// Add the duplicate
	m_pArchive->AddDuplicate( pSgNode );
    
	// Cache the first child
	SgNodeWinGL** pChild = (SgNodeWinGL**) &pSgNode->m_pFileData->m_pFirstChild;
    
	// Loop through the children
	while( *pChild != BtNull )
	{
		SgNodeWinGL* pCurrent = *pChild;
        
		// Make the duplicate
		SgNodeWinGL* pDuplicate = (SgNodeWinGL*)pCurrent->GetDuplicate();
        
		// Set the parent
		pDuplicate->m_pFileData->m_pParent = pSgNode;
        
		// Update the child
		*pChild = pDuplicate;
        
		// Move to the next sibling
		pChild = (SgNodeWinGL**) &pDuplicate->m_pFileData->m_pNextSibling;
	}
    
	// Return the node
	return pSgNode;
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgNodeWinGL::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	// Set the file data
	m_pFileData = (BaSgNodeFileData*) pFileData;

	// Set the archive
	m_pArchive = pArchive;

	// Set the relationships to the siblings and children
	m_pFileData->m_pFirstChild  = (SgNodeWinGL*)  pArchive->GetResource( m_pFileData->m_nFirstChild );
	m_pFileData->m_pParent      = (SgNodeWinGL*)  pArchive->GetResource( m_pFileData->m_nParent );
	m_pFileData->m_pNextSibling = (SgNodeWinGL*)  pArchive->GetResource( m_pFileData->m_nNextSibling );
	m_pFileData->m_pScene       = (RsSceneWinGL*) pArchive->GetResource( m_pFileData->m_nScene );

	BtU8* pInstanceMemory = (BtU8*) this;
	BtU8* pMemory = (BtU8*) m_pFileData;

	// Advance past the scene node
	pMemory+=sizeof(BaSgNodeFileData);
	pInstanceMemory+=sizeof(SgNodeWinGL);

	if( NodeType() & SgType_Mesh )
	{
		// Fix up the scene mesh
		m_pMesh = new SgMeshWinGL;
		m_pMesh->FixPointers( pArchive, pMemory );
		m_pMesh->m_pNode = this;
		pMemory+=sizeof(BaSgMeshFileData);
		pInstanceMemory+=sizeof(SgMeshWinGL);
	}

  	if( NodeType() & SgType_Skin )
	{
		// Fix up the scene mesh
		m_pSkin = new SgSkinWin32GL;

		// Fix up the skeleton root
		m_pSkin->FixPointers( pArchive, pMemory );
		m_pSkin->m_pNode = this;
		pMemory+=sizeof(BaSgSkinFileData);
		pInstanceMemory+=sizeof(SgSkinWin32GL);
	}

    if( NodeType() & SgType_RigidBody )
    {
        // Fix up the rigid body
        m_pRigidBody = new SgRigidBodyImpl;
        m_pRigidBody->m_pNode = this;
        m_pRigidBody->FixPointers( pMemory );
        pMemory+=sizeof(BaSgRigidBodyFileData);
        pInstanceMemory+=sizeof(SgRigidBodyImpl);
    }

	if( NodeType() & SgType_Collision )
	{
		m_pCollision = new SgCollisionWinGL;
		m_pCollision->m_pNode = this;
		m_pCollision->FixPointers( pMemory );
		pMemory+=sizeof(BaSgCollisionFileData);
		pInstanceMemory+=sizeof(SgCollisionWinGL);
	}

    if( NodeType() & SgType_Bone )
    {
        m_pBone = new SgBoneDX11;
        m_pBone->m_pNode = this;
        m_pBone->FixPointers( pMemory );
        pMemory+=sizeof(BaSgBoneFileData);
        pInstanceMemory+=sizeof(SgBoneDX11);
    }

	if( NodeType() & SgType_Camera )
	{
		pMemory+=sizeof(BaSgCameraFileData);
	}

	if( NodeType() & SgType_BlendShape )
	{
		m_pBlendShape = new SgBlendShapeImpl;
		m_pBlendShape->m_pNode = this;
		m_pBlendShape->FixPointers( pMemory );
		pMemory+=sizeof(BaSgBlendShapeFileData);
		pInstanceMemory+=sizeof(SgBlendShapeImpl);
	}

	if( NodeType() & SgType_Light )
	{
		pMemory+=sizeof(BaSgLightFileData);
	}

	if( ( NodeType() & SgType_Materials ) )
	{
		m_pMaterials = new SgMaterialsWinGL;
		m_pMaterials->m_pNode = this;
		m_pMaterials->FixPointers( pArchive, pMemory );
	}
}

////////////////////////////////////////////////////////////////////////////////
// pRigidBody

SgRigidBody* SgNodeWinGL::pRigidBody()
{
	return (SgRigidBody*) m_pRigidBody;
}

////////////////////////////////////////////////////////////////////////////////
// pBone

SgBone* SgNodeWinGL::pBone() const
{
	return (SgBone*) m_pBone;
}


////////////////////////////////////////////////////////////////////////////////
// GetNumJoints

BtU32 SgNodeWinGL::GetNumJoints()
{
    SgSkinWin32GL *pSkin32 = (SgSkinWin32GL*)pSkin();
    return pSkin32->m_pFileData->m_numJoints;
}

////////////////////////////////////////////////////////////////////////////////
// GetJoint

SgNode* SgNodeWinGL::GetJoint(BtU32 jointIndex) const
{
    SgSkinWin32GL *pSkin32 = (SgSkinWin32GL*)pSkin();
    return pSkin32->m_pFileData->m_skeleton[jointIndex].m_pJoint;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SgNodeWinGL::Update()
{
	if( m_pFileData->m_pParent == BtNull )
	{
		m_pFileData->m_m4World = m_pFileData->m_m4Local;
	}
	else
	{
		SgNodeWinGL* pParent = (SgNodeWinGL*) m_pFileData->m_pParent;

		m_pFileData->m_m4World = m_pFileData->m_m4Local * pParent->m_pFileData->m_m4World;
	}

	// Cache the first child
	SgNodeWinGL* pChild = (SgNodeWinGL*) m_pFileData->m_pFirstChild;

	// Loop through the children
	while( pChild != BtNull )
	{
		// Update the child
		pChild->Update();

		// Move to the next child
		pChild = (SgNodeWinGL*) pChild->m_pFileData->m_pNextSibling;
	}

    if( m_pFileData->m_nodeType & SgType_Skin )
    {
        for (BtU32 i = 0; i < m_pSkin->m_pFileData->m_numJoints; i++)
        {
            SgNode *pNode = m_pSkin->m_pFileData->m_skeleton[i].m_pJoint;
            
            SgBoneDX11 *pBone = (SgBoneDX11*)pNode->pBone();
            
            pBone->m_boneTransform = pBone->GetInverseBindPose() * pNode->GetWorldTransform();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// SetLocalTransform

void SgNodeWinGL::SetLocalTransform( const MtMatrix4& m4Transform )
{
	m_pFileData->m_m4Local = m4Transform;

	// Cache the parent
	SgNodeWinGL* pParent = (SgNodeWinGL*) m_pFileData->m_pParent;

	if( pParent == BtNull )
	{
		m_pFileData->m_m4World = m4Transform;
	}
	else
	{
		m_pFileData->m_m4World = m4Transform * pParent->m_pFileData->m_m4World;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetRendered

void SgNodeWinGL::SetRendered( BtBool enabled )
{
    if( enabled == BtTrue )
	{
		m_pFileData->m_renderFlags = RsRF_Visible;
	}
	else
	{
		m_pFileData->m_renderFlags = RsRF_Hidden;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetRendered

void SgNodeWinGL::SetForceRendered( BtBool enabled )
{
    if( enabled == BtTrue )
	{
		m_pFileData->m_renderFlags = RsRF_ForceVisible;
	}
	else
	{
		m_pFileData->m_renderFlags = RsRF_Visible;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

BtBool SgNodeWinGL::IsRendered() const
{
    return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SgNodeWinGL::Render()
{
	if( m_pFileData->m_renderFlags != RsRF_Hidden )
	{
		if( NodeType() & SgType_Mesh )
		{
			m_pMesh->Render();
		}

		if( NodeType() & SgType_Skin )
		{
			m_pSkin->Render();
		}
	}

	// Cache the first child
	SgNodeWinGL* pChild = (SgNodeWinGL*) m_pFileData->m_pFirstChild;

	// Loop through the children
	while( pChild != BtNull )
	{
		// Render the child
		pChild->Render();

		// Move to the next child
		pChild = (SgNodeWinGL*) pChild->m_pFileData->m_pNextSibling;
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddChild

void SgNodeWinGL::AddChild( SgNode* pChild )
{
	SgNodeWinGL* pChildInst = (SgNodeWinGL*) pChild;

	// Add the new child as the first
	SgNodeWinGL* pNextChild = (SgNodeWinGL*) m_pFileData->m_pFirstChild;
	m_pFileData->m_pFirstChild = pChildInst;
	pChildInst->m_pFileData->m_pNextSibling = pNextChild;
	pChildInst->m_pFileData->m_pParent = this;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveChild

void SgNodeWinGL::RemoveChild( SgNode* pChildNode )
{
	SgNodeWinGL* pChildToRemove = (SgNodeWinGL*) pChildNode;

	if( m_pFileData->m_pFirstChild == pChildToRemove )
	{
		m_pFileData->m_pFirstChild = pChildToRemove->m_pFileData->m_pNextSibling;
	}
	else
	{
		SgNodeWinGL* pChild = (SgNodeWinGL*) m_pFileData->m_pFirstChild;

		while( pChild != NULL )
		{
			SgNodeWinGL* pNextChild = (SgNodeWinGL*) pChild->m_pFileData->m_pNextSibling;

			if ( pNextChild == pChildToRemove )
			{
				pChild->m_pFileData->m_pNextSibling = pChildToRemove->m_pFileData->m_pNextSibling;
				break;
			}
			pChild = (SgNodeWinGL*) pChild->m_pFileData->m_pNextSibling;
		}
	}

	pChildToRemove->m_pFileData->m_pParent = BtNull;
	pChildToRemove->m_pFileData->m_pNextSibling = BtNull;
}
