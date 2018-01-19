////////////////////////////////////////////////////////////////////////////////
/// SgNodeWinGL.cpp

#include "BtMemory.h"
#include "BtString.h"
#include "SgNodeImpl.h"
#include "RsMaterial.h"
#include "RsCamera.h"
#include "RsTextureImpl.h"
#include "MtMath.h"

#include "SgBoneImpl.h"
#include "SgMaterialsImpl.h"
#include "SgCameraImpl.h"
#include "SgLightImpl.h"
#include "SgBoneImpl.h"
#include "SgBlendShapeImpl.h"
#include "SgCollisionImpl.h"
#include "SgRigidBodyImpl.h"
#include "RsImpl.h"
#include "RsSceneImpl.h"
#include "SgMeshImpl.h"
#include "SgSkinImpl.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

SgNodeImpl::SgNodeImpl()
{
    m_pRigidBody = BtNull;
    m_pBone = BtNull;
    m_pCollision = BtNull;
    m_pMesh = BtNull;
    m_pSkin = BtNull;
    m_pBlendShape = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetInstanceSize

BtU32 SgNodeImpl::GetInstanceSize(BaResourceHeader *pResourceHeader)
{
    BtU32 instance = sizeof(SgNodeImpl);
    
    size_t specificInstanceSize = 0;
    
    specificInstanceSize = MtMax(specificInstanceSize, sizeof(SgMeshWinGL));
    specificInstanceSize = MtMax(specificInstanceSize, sizeof(SgSkinImpl));
    specificInstanceSize = MtMax(specificInstanceSize, sizeof(SgCollisionWinGL));
    specificInstanceSize = MtMax(specificInstanceSize, sizeof(SgBoneImpl));
    //specificInstanceSize = MtMax(specificInstanceSize, 0));// sizeof(SgCameraWin32));
    specificInstanceSize = MtMax(specificInstanceSize, sizeof(SgBlendShapeImpl));
    //specificInstanceSize = MtMax(specificInstanceSize, 0));// sizeof(SgLightWin32));
    specificInstanceSize = MtMax(specificInstanceSize, sizeof(SgMaterialsWinGL));
    return instance + (BtU32)specificInstanceSize;
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void SgNodeImpl::Destroy()
{
}

////////////////////////////////////////////////////////////////////////////////
// pFind

SgNode* SgNodeImpl::pFind( const BtChar* pName )
{
    if( BtStrCompare( m_pFileData->m_name, pName ) == BtTrue )
    {
        return this;
    }
    
    // Cache the first child
    SgNodeImpl* pChild = (SgNodeImpl*) m_pFileData->m_pFirstChild;
    
    // Loop through the children
    while( pChild != BtNull )
    {
        SgNodeImpl* pNode = (SgNodeImpl*)pChild->pFind( pName );
        
        // Update the child
        if( pNode != BtNull )
        {
            return pNode;
        }
        
        // Move to the next child
        pChild = (SgNodeImpl*) pChild->m_pFileData->m_pNextSibling;
    }
    return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetDuplicate

// virtual
SgNode* SgNodeImpl::GetDuplicate()
{
    BtU32 instanceSize = GetInstanceSize(BtNull);
    
    // Allocate the memory
    BtU8* pMemory = BtMemory::Allocate( instanceSize + m_pFileData->m_nFileDataSize );
    
    // Create an instance of the new node
    SgNodeImpl* pSgNode = new (pMemory) SgNodeImpl;
    
    // Find the new file data
    BtU8* pNewFileData = pMemory + instanceSize;
    
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

	// Is it a skin?
	if( NodeType() & SgType_Skin)
	{
		// Duplicate the skin
		SgSkin *pSkin1 = m_pSkin;
		SgSkin *pSkin2 = pSgNode->m_pSkin;
		(void)pSkin1;
		(void)pSkin2;
		pSgNode->m_pSkin->GetDuplicate();
	}
    
    // Cache the first child
    SgNodeImpl** pChild = (SgNodeImpl**) &pSgNode->m_pFileData->m_pFirstChild;
    
    // Loop through the children
    while( *pChild != BtNull )
    {
        SgNodeImpl* pCurrent = *pChild;

		if (pCurrent->NodeType() & SgType_Skin )
		{
			int a = 0;
			a++;
		}

		if (pCurrent->NodeType() & SgType_Bone)
		{
			int a = 0;
			a++;
		}
        
        // Make the duplicate
        SgNodeImpl* pDuplicate = (SgNodeImpl*)pCurrent->GetDuplicate();
        
        // Set the parent
        pDuplicate->m_pFileData->m_pParent = pSgNode;
        
        // Update the child
        *pChild = pDuplicate;
        
        // Move to the next sibling
        pChild = (SgNodeImpl**) &pDuplicate->m_pFileData->m_pNextSibling;
    }
    
    // Return the node
    return pSgNode;
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgNodeImpl::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
    // Set the file data
    m_pFileData = (BaSgNodeFileData*) pFileData;
    
    // Set the archive
    m_pArchive = pArchive;
    
    // Set the relationships to the siblings and children
    m_pFileData->m_pFirstChild  = (SgNodeImpl*)  pArchive->GetResource( m_pFileData->m_nFirstChild );
    m_pFileData->m_pParent      = (SgNodeImpl*)  pArchive->GetResource( m_pFileData->m_nParent );
    m_pFileData->m_pNextSibling = (SgNodeImpl*)  pArchive->GetResource( m_pFileData->m_nNextSibling );
    m_pFileData->m_pScene       = (RsSceneImpl*) pArchive->GetResource( m_pFileData->m_nScene );
    
    BtU8* pInstanceMemory = (BtU8*) this;
    BtU8* pMemory = (BtU8*) m_pFileData;
    
    // Advance past the scene node
    pMemory+=sizeof(BaSgNodeFileData);
    pInstanceMemory+=sizeof(SgNodeImpl);
    
    if( NodeType() & SgType_Mesh )
    {
        // Fix up the scene mesh
        m_pMesh = new( pInstanceMemory ) SgMeshWinGL;
		m_pMesh->m_pNode = this;
		m_pMesh->FixPointers(pArchive, pMemory);
        pMemory+=sizeof(BaSgMeshFileData);
        pInstanceMemory+=sizeof(SgMeshWinGL);
    }
    
    if (NodeType() & SgType_Skin)
    {
        // Fix up the scene mesh
        m_pSkin = new(pInstanceMemory) SgSkinImpl;
        
        // Fix up the skeleton root
		m_pSkin->m_pSkinNode = this;
		m_pSkin->FixPointers(pArchive, pMemory);
        pMemory += sizeof(BaSgSkinFileData);
        pInstanceMemory += sizeof(SgSkinImpl);
    }
    
    if( NodeType() & SgType_RigidBody )
    {
        // Fix up the rigid body
        m_pRigidBody = new( pInstanceMemory ) SgRigidBodyImpl;
        m_pRigidBody->m_pNode = this;
        m_pRigidBody->FixPointers( pMemory );
        pMemory+=sizeof(BaSgRigidBodyFileData);
        pInstanceMemory+=sizeof(SgRigidBodyImpl);
    }
    
    if( NodeType() & SgType_Collision )
    {
        m_pCollision = new( pInstanceMemory ) SgCollisionWinGL;
        m_pCollision->m_pNode = this;
        m_pCollision->FixPointers( pMemory );
        pMemory+=sizeof(BaSgCollisionFileData);
        pInstanceMemory+=sizeof(SgCollisionWinGL);
    }
    
    if( NodeType() & SgType_Bone )
    {
        m_pBone = new( pInstanceMemory ) SgBoneImpl;
        m_pBone->m_pNode = this;
        m_pBone->FixPointers( pMemory );
        pMemory+=sizeof(BaSgBoneFileData);
        pInstanceMemory+=sizeof(SgBoneImpl);
    }
    
    if( NodeType() & SgType_Camera )
    {
        //		m_pCamera = new( pInstanceMemory ) SgCameraWin32;
        //		m_pCamera->m_pNode = this;
        //		m_pCamera->FixPointers( pMemory );
        pMemory+=sizeof(BaSgCameraFileData);
        //		pInstanceMemory+=sizeof(SgCameraWin32);
    }
    
    if( NodeType() & SgType_BlendShape )
    {
        m_pBlendShape = new( pInstanceMemory ) SgBlendShapeImpl;
        m_pBlendShape->m_pNode = this;
        m_pBlendShape->FixPointers( pMemory );
        pMemory+=sizeof(BaSgBlendShapeFileData);
        pInstanceMemory+=sizeof(SgBlendShapeImpl);
    }
    
    if( NodeType() & SgType_Light )
    {
        //		m_pLight = new( pInstanceMemory ) SgLightWin32;
        //		m_pLight->m_pNode = this;
        //		m_pLight->FixPointers( pMemory );
        pMemory+=sizeof(BaSgLightFileData);
        //		pInstanceMemory+=sizeof(SgLightDX11);
    }
    
    if( ( NodeType() & SgType_Materials ) )
    {
        m_pMaterials = new( pInstanceMemory ) SgMaterialsWinGL;
        m_pMaterials->m_pNode = this;
        m_pMaterials->FixPointers( pArchive, pMemory );
    }
}

////////////////////////////////////////////////////////////////////////////////
// SetRendered

void SgNodeImpl::SetRendered( BtBool enabled )
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
// SetForceRendered

void SgNodeImpl::SetForceRendered( BtBool enabled )
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
// IsRendered

BtBool SgNodeImpl::IsRendered() const
{
    return ( m_pFileData->m_renderFlags == RsRF_Visible ) || ( m_pFileData->m_renderFlags == RsRF_ForceVisible );
}

////////////////////////////////////////////////////////////////////////////////
// pRigidBody

SgRigidBody* SgNodeImpl::pRigidBody()
{
    return (SgRigidBody*) m_pRigidBody;
}

////////////////////////////////////////////////////////////////////////////////
// pBone

SgBone* SgNodeImpl::pBone() const
{
    return (SgBone*) m_pBone;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumJoints

BtU32 SgNodeImpl::GetNumJoints()
{
    SgSkinImpl *pSkin32 = (SgSkinImpl*)pSkin();
    return pSkin32->m_pFileData->m_numJoints;
}

////////////////////////////////////////////////////////////////////////////////
// GetJoint

SgNode* SgNodeImpl::GetJoint(BtU32 jointIndex) const
{
	// Cache the first child
	SgNodeImpl* pChild = (SgNodeImpl*)this->pFirstChild();

	// Loop through the children
	while (pChild != BtNull)
	{
		if( pChild->NodeType() & SgType_Bone)
		{
			int a = 0;
			a++;
			return pChild;
		}

		// Move to the next child
		pChild = (SgNodeImpl*)pChild->m_pFileData->m_pNextSibling;
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SgNodeImpl::Update()
{
    if( m_pFileData->m_pParent == BtNull )
    {
        m_pFileData->m_m4World = m_pFileData->m_m4Local;
    }
    else
    {
        SgNodeImpl* pParent = (SgNodeImpl*) m_pFileData->m_pParent;
        
        m_pFileData->m_m4World = m_pFileData->m_m4Local * pParent->m_pFileData->m_m4World;
    }
    
    // Cache the first child
    SgNodeImpl* pChild = (SgNodeImpl*) m_pFileData->m_pFirstChild;
    
    // Loop through the children
    while( pChild != BtNull )
    {
        // Update the child
        pChild->Update();
        
        // Move to the next child
        pChild = (SgNodeImpl*) pChild->m_pFileData->m_pNextSibling;
    }
    
    if( m_pFileData->m_nodeType & SgType_Skin )
    {
        for (BtU32 i = 0; i < m_pSkin->m_pFileData->m_numJoints; i++)
        {
            SgNode *pNode = m_pSkin->m_pFileData->m_skeleton[i].m_pJoint;
            
            SgBoneImpl *pBone = (SgBoneImpl*)pNode->pBone();
            
            pBone->m_boneTransform = pBone->GetInverseBindPose() * pNode->GetWorldTransform();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// SetLocalTransform

void SgNodeImpl::SetLocalTransform( const MtMatrix4& m4Transform )
{
    m_pFileData->m_m4Local = m4Transform;
    
    // Cache the parent
    SgNodeImpl* pParent = (SgNodeImpl*) m_pFileData->m_pParent;
    
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
// Render

void SgNodeImpl::Render()
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
    SgNodeImpl* pChild = (SgNodeImpl*) m_pFileData->m_pFirstChild;
    
    // Loop through the children
    while( pChild != BtNull )
    {
        // Render the child
        pChild->Render();
        
        // Move to the next child
        pChild = (SgNodeImpl*) pChild->m_pFileData->m_pNextSibling;
    }
}

////////////////////////////////////////////////////////////////////////////////
// AddChild

void SgNodeImpl::AddChild( SgNode* pChild )
{
    SgNodeImpl* pChildInst = (SgNodeImpl*) pChild;
    
    // Add the new child as the first
    SgNodeImpl* pNextChild = (SgNodeImpl*) m_pFileData->m_pFirstChild;
    m_pFileData->m_pFirstChild = pChildInst;
    pChildInst->m_pFileData->m_pNextSibling = pNextChild;
    pChildInst->m_pFileData->m_pParent = this;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveChild

void SgNodeImpl::RemoveChild( SgNode* pChildNode )
{
    SgNodeImpl* pChildToRemove = (SgNodeImpl*) pChildNode;
    
    if( m_pFileData->m_pFirstChild == pChildToRemove )
    {
        m_pFileData->m_pFirstChild = pChildToRemove->m_pFileData->m_pNextSibling;
    }
    else
    {
        SgNodeImpl* pChild = (SgNodeImpl*) m_pFileData->m_pFirstChild;
        
        while( pChild != NULL )
        {
            SgNodeImpl* pNextChild = (SgNodeImpl*) pChild->m_pFileData->m_pNextSibling;
            
            if ( pNextChild == pChildToRemove )
            {
                pChild->m_pFileData->m_pNextSibling = pChildToRemove->m_pFileData->m_pNextSibling;
                break;
            }
            pChild = (SgNodeImpl*) pChild->m_pFileData->m_pNextSibling;
        }
    }
    
    pChildToRemove->m_pFileData->m_pParent = BtNull;
    pChildToRemove->m_pFileData->m_pNextSibling = BtNull;
}
