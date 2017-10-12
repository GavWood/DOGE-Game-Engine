////////////////////////////////////////////////////////////////////////////////
// BaArchive

// Includes
#include <new>
#include "BtString.h"
#include "BtPrint.h"
#include "BtTypes.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "BaUserData.h"
#include "BaResource.h"
#include "BtMemory.h"
#include "BtCompressedFile.h"
#include "ApConfig.h"

#include "DyCollisionMesh.h"
#include "DyCollisionAnalytical.h"

// impl
#include "RsTextureImpl.h"
#include "RsFontImpl.h"
#include "SdSoundImpl.h"
#include "RsMaterialImpl.h"
#include "RsSpriteImpl.h"
#include "RsShaderImpl.h"
#include "RsSceneImpl.h"
#include "RsVertexBufferImpl.h"
#include "RsIndexBufferImpl.h"
#include "RsFlashImpl.h"
#include "SgNodeImpl.h"
#include "SgAnimatorImpl.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// GetInstanceSize

BtU32 BaArchive::GetInstanceSize(BaResourceHeader *pResourceHeader)
{
    BaResourceType resourceType = pResourceHeader->m_type;
    
    switch (resourceType)
    {
        case BaRT_CollisionMesh:
            return sizeof(DyCollisionMesh);
            break;
            
        case BaRT_CollisionAnalytical:
            return sizeof(DyCollisionAnalytical);
            break;
            
        case BaRT_VertexBuffer:
            return sizeof(RsVertexBufferImpl);
            break;
            
        case BaRT_IndexBuffer:
            return sizeof(RsIndexBufferImpl);
            break;
            
        case BaRT_Material:
            return sizeof(RsMaterialImpl);
            break;
            
        case BaRT_UserData:
            return sizeof(BaUserData);
            break;
            
        case BaRT_Sprite:
            return sizeof(RsSpriteImpl);
            break;
            
        case BaRT_Font:
            return sizeof(RsFontImpl);
            break;
            
        case BaRT_Texture:
            return sizeof(RsTextureImpl);
            break;
            
        case BaRT_Sound:
            return sizeof(SdSoundImpl);
            break;
            
        case BaRT_Scene:
            return sizeof(RsSceneImpl);
            break;
            
        case BaRT_SgNode:
            return SgNodeImpl::GetInstanceSize(pResourceHeader);
            break;
            
        case BaRT_Animator:
            return sizeof(SgAnimatorImpl);
            break;
            
        case BaRT_Shader:
            return sizeof(RsShaderImpl);
            break;
            
        case BaRT_Flash:
            return sizeof(RsFlashImpl);
            break;
            
        default:
            return sizeof(BaResource);
            break;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// CreateResourceInstance

void BaArchive::CreateResourceInstance( BaResourceType resourceType, BtU8 *pMemory )
{
    // Cache the platform
    ApPlatform platform = ApConfig::GetPlatform();
    
    // Validate the platform
    BtAssert( platform != ApPlatform_Max );
    
    switch( resourceType )
    {
        case BaRT_CollisionMesh:
        {
            new(pMemory) DyCollisionMesh;
        }
            break;
            
        case BaRT_CollisionAnalytical:
        {
            new(pMemory) DyCollisionAnalytical;
        }
            break;
            
        case BaRT_VertexBuffer:
            new (pMemory) RsVertexBufferImpl;
            break;
            
        case BaRT_IndexBuffer:
            new (pMemory) RsIndexBufferImpl;
            break;
            
        case BaRT_Material:
            new( pMemory ) RsMaterialImpl;
            break;
            
        case BaRT_UserData:
            new( pMemory ) BaUserData;
            break;
            
        case BaRT_Sprite:
            new( pMemory ) RsSpriteImpl;
            break;
            
        case BaRT_Font:
            new( pMemory ) RsFontImpl;
            break;
            
        case BaRT_Texture:
            new( pMemory ) RsTextureImpl;
            break;
            
        case BaRT_Sound:
            new( pMemory ) SdSoundImpl;
            break;
            
        case BaRT_Scene:
            new (pMemory) RsSceneImpl;
            break;
            
        case BaRT_SgNode:
            new( pMemory ) SgNodeImpl;
            break;
            
        case BaRT_Animator:
            new( pMemory ) SgAnimatorImpl;
            break;
            
        case BaRT_Shader:
            new( pMemory ) RsShaderImpl;
            break;
            
        case BaRT_Flash:
            new( pMemory )RsFlashImpl;
            break;
            
        default:
            new( pMemory ) BaResource;
            break;
    }
}
