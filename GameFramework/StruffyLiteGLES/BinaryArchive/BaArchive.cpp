////////////////////////////////////////////////////////////////////////////////
// BaArchive

// Includes
#include "BtBase.h"
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

// WinGL
//#include "SdSoundWinGL.h"
#include "RsMaterialGLES.h"
#include "RsSpriteGLES.h"
#include "RsTextureGLES.h"
#include "RsFontGLES.h"
#include "RsFlashGLES.h"
#include "RsShaderGLES.h"
#include "RsSceneGLES.h"
#include "SgNodeGLES.h"
#include "SgAnimatorGLES.h"
#include "RsVertexBufferGLES.h"
#include "RsIndexBufferGLES.h"
#include "SdSoundWinGL.h"
#include <stdio.h>
#include <new>

//static
BtLinkedList<BaArchiveNode> BaArchive::m_archives;

////////////////////////////////////////////////////////////////////////////////
// Constructor

BaArchive::BaArchive()
{
}

////////////////////////////////////////////////////////////////////////////////
// AddDuplicate

void BaArchive::AddDuplicate( BaResource* pResource )
{
	// Create the duplicate
	BaDuplicate* pDuplicate = new BaDuplicate( pResource );

	// Set the archive
	pResource->m_pArchive = this;

	// Add it to the list
	m_duplicates.AddHead( pDuplicate );
}

////////////////////////////////////////////////////////////////////////////////
// LoadFile

void BaArchive::LoadFile( const BtChar* archiveName )
{
    BaArchiveHeader archiveHeader;
    
    // Load the file
    BtChar filename[256];
    
    BtStrCopy( filename, 256, ApConfig::GetResourcePath() );
    BtStrCat(  filename, 256, archiveName );
    BtStrCat(  filename, 256, ApConfig::GetExtension() );
    
    printf( "Filename: %s\n", filename );
    
    strcpy( m_filename, filename );
    
    FILE *f = fopen( filename, "rb" );
    
    if( f )
    {
        fread( (void*)&archiveHeader, 1, sizeof( archiveHeader), f );
        
        // Make the memory
        m_pMemory = (BtU8*)BtMemory::Allocate( archiveHeader.m_nDataAndInstanceSize );
        
        BtStrCat( filename, 256, "z" );
        
        // Load the compressed file
        LBtCompressedFile compressedFile;
        compressedFile.Read( f, m_pMemory, archiveHeader.m_nDataAndInstanceSize );
        
        fclose(f);
        
        m_isLoaded = BtTrue;
    }
    else
    {
        ErrorLog::Fatal_Printf( "Could not find archive %s", archiveName );
    }
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
		new (pMemory) RsVertexBufferWinGL;
		break;

	case BaRT_IndexBuffer:
		new (pMemory) RsIndexBufferWinGL;
		break;

	case BaRT_Material:
		new( pMemory ) RsMaterialWinGL;
		break;

	case BaRT_UserData:
		new( pMemory ) BaUserData;
		break;

	case BaRT_Sprite:
		new( pMemory ) RsSpriteWinGL;
		break;

	case BaRT_Font:
		new( pMemory ) RsFontGLES;
		break;

    case BaRT_Flash:
        new( pMemory ) RsFlashWinGL;
        break;
            
	case BaRT_Texture:
		new( pMemory ) RsTextureWinGL;
		break;

	case BaRT_Sound:
		new( pMemory ) SdSoundWinGL;
		break;

	case BaRT_Scene:
		new (pMemory) RsSceneWinGL;
		break;

	case BaRT_SgNode:
		new( pMemory ) SgNodeWinGL;
		break;

	case BaRT_Animator:
		new( pMemory ) SgAnimatorWinGL;
		break;

	case BaRT_Shader:
		new( pMemory ) RsShaderWinGL;
		break;

	default:
		new( pMemory ) BaResource;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// CreateResources

void BaArchive::CreateResources()
{
	// Cache the archive header pointer
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader* ) m_pMemory;

	// Validate the archive header
	BtAssert( pArchiveHeader != BtNull );

	// Cache each resource header
	BaResourceHeader* pResource = (BaResourceHeader*)( m_pMemory + sizeof( BaArchiveHeader ) );

	// Loop through the resources and markup any pointers
	for( BtU32 i=0; i<pArchiveHeader->m_nNumResources; i++ )
	{
		// Calculate the instance pointer
		BtU8* pInstance = (BtU8*) ( m_pMemory + pResource->m_nInstanceOffset );

		// Validate that the instances start after the archive and it's data
		BtAssert( pResource->m_nInstanceOffset < pArchiveHeader->m_nDataAndInstanceSize );

		// Create the instance
		CreateResourceInstance( pResource->m_type, pInstance );

		// Cache the base type
		BaResource* pResourceInstance = (BaResource*) pInstance;

		// Set the resource title
		pResourceInstance->SetTitle( pResource->m_szTitle );

		// Increment the resource header
		pResource++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Load

void BaArchive::Load( const BtChar* archiveName )
{
	LoadFile( archiveName );
	CreateResources();
	FixPointers();
	ValidateResources();
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void BaArchive::FixPointers()
{
	// Cache the archive header pointer
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader* ) m_pMemory;

	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pMemory + sizeof( BaArchiveHeader ) );

	// Cache each resource header
	pResourceHeader = (BaResourceHeader*)( m_pMemory + sizeof( BaArchiveHeader ) );
				
	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{
		// Cache each resource
		BaResource* pResource = (BaResource*)( m_pMemory + pResourceHeader->m_nInstanceOffset );
        
		// Locate the file data
		BtU8 *pFileData = ( m_pMemory + pResourceHeader->m_nResourceOffset );

		BaResourceType resourceType = pResource->resourceType();
        
        BtAssert(resourceType != BaRT_Max )
        
        if( resourceType == BaRT_Texture)
        {
            BtU32 sizeOfInstance = sizeof(RsTextureWinGL);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );

            int a=0;
            a++;
        }
        else if( resourceType == BaRT_Sprite)
        {
            BtU32 sizeOfInstance = sizeof(RsSpriteWinGL);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );
            
            int a=0;
            a++;
        }
        else if( resourceType == BaRT_Material)
        {
            BtU32 sizeOfInstance = sizeof(RsMaterialWinGL);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );
            
            int a=0;
            a++;
        }
        else if( resourceType == BaRT_Scene)
        {
            BtU32 sizeOfInstance = sizeof(RsSceneWinGL);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );
            
            int a=0;
            a++;
        }
        else if( resourceType == BaRT_CollisionAnalytical )
        {
            int a=0;
            a++;
        }
        else if( resourceType == BaRT_SgNode)
        {
            BtU32 sizeOfInstance = sizeof(SgNodeWinGL);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );
            
            int a=0;
            a++;
        }
        else if( resourceType == BaRT_Shader)
        {
            BtU32 sizeOfInstance = sizeof(RsShaderWinGL);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );
            
            int a=0;
            a++;
        }
        else if( resourceType == BaRT_Font)
        {
            BtU32 sizeOfInstance = sizeof(RsFontGLES);
            BtAssert( pResourceHeader->m_nInstanceSize >= sizeOfInstance );
            
            int a=0;
            a++;
        }
        
		// Check the resource type
		if( resourceType != BaRT_Max )
		{			
			BtAssert( pResource->resourceType() == pResourceHeader->m_type );

			// Fix pointers
			pResource->FixPointers( pFileData, this );
		
			// Create on the device
			pResource->CreateOnDevice();
		}
		
		// Move to the next resource
		pResourceHeader++;	
	}
}


////////////////////////////////////////////////////////////////////////////////
// ValidateResources

void BaArchive::ValidateResources()
{
	RsSprite *pSprite;

	// Cache the archive header pointer
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader* ) m_pMemory;

	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pMemory + sizeof( BaArchiveHeader ) );

	// Cache each resource header
	pResourceHeader = (BaResourceHeader*)( m_pMemory + sizeof( BaArchiveHeader ) );

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{		
		// Cache each resource
		BaResource* pResource = (BaResource*)( m_pMemory + pResourceHeader->m_nInstanceOffset );

		BtChar *pTitle = pResource->GetTitle();
		(void)pTitle;

		BaResourceType resourceType = pResource->resourceType();
		(void)resourceType;

		if( resourceType == BaRT_Sprite )
		{
			pSprite = (RsSprite*)pResource;
			int a=0;
			a++;
		}

		// Move to the next resource
		pResourceHeader++;
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Unload

void BaArchive::Unload()
{
	// Cache the archive header pointer
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader* ) m_pMemory;

	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pMemory + sizeof( BaArchiveHeader ) );

	// Loop through the resources and markup any pointers
	for( BtU32 i=0; i<pArchiveHeader->m_nNumResources; i++ )
	{
		// Get the instance address
		BaResource* pResource = (BaResource*)( m_pMemory + pResourceHeader->m_nInstanceOffset );

		if( pResource->resourceType() != BaRT_Max )
		{
			// Remove from the resource list
			pResource->Remove();

			// Remove from the device
			pResource->RemoveFromDevice();

			// Destroy
			pResource->Destroy();	
		}
		
		// Increment the resource header
		pResourceHeader++;		
	}

	BtMemory::Free( m_pMemory );
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

BtU8* BaArchive::GetResource( BaResourceType eType, const BtChar* szTitle ) const
{
	// Get the archive header
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader*) m_pMemory;

	// Get the start of the resource headers
	BtChar *pData = ( (BtChar*) ( m_pMemory ) + sizeof( BaArchiveHeader ) );

	// Get the first resource header
	BaResourceHeader* pResource = (BaResourceHeader*) pData;

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{
		// Get the instance address
		BtU8* pInstanceAddress = (BtU8*) ( m_pMemory + pResource->m_nInstanceOffset );

		// Is this the string resource?
		if( pResource->m_type == eType )
		{
			if( BtStrCompareNoCase( pResource->m_szTitle, szTitle ) == BtTrue )
			{
				return pInstanceAddress;
			}
		}

		// Get the next resource header
		pResource++;
	}

	// Return BtNull though we will never reach here
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

BtU8* BaArchive::GetResource( BtU32 nResourceID ) const
{
	// Don't search for a null resource
	if( nResourceID == 0  )
	{
		return BtNull;
	}
    
    // Get the archive header
    BaArchiveHeader* pArchiveHeader = (BaArchiveHeader*) m_pMemory;
    
    // Get the start of the resource headers
    BtChar *pData = ( (BtChar*) ( m_pMemory ) + sizeof( BaArchiveHeader ) );
    
    // Get the first resource header
    BaResourceHeader* pResource = (BaResourceHeader*) pData;

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{
		// Get the instance address
		BtU8* pInstanceAddress = (BtU8*) ( m_pMemory + pResource->m_nInstanceOffset );

		// Is this the string resource?
		if( pResource->m_nResourceID == nResourceID )
		{
			return pInstanceAddress;
		}

		// Get the next resource header
		pResource++;
	}

	// Return BtNull though we will never reach here
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetFont

RsFont* BaArchive::GetFont( const BtChar* name )
{
	RsFont* pFont = (RsFont*)GetResource( BaRT_Font, name );

	if( pFont == BtNull )
	{
		BtPrint( "Could not find Font %s\r\n", name );
	}
	return pFont;
}

////////////////////////////////////////////////////////////////////////////////
// GetNode

SgNode* BaArchive::GetNode( const BtChar *name )
{
	SgNode* pNode = (SgNode*)GetResource( BaRT_SgNode, name );

	if( pNode == BtNull )
	{
		BtPrint( "Could not find node %s\r\n", name );
	}
	return pNode;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaterial

RsMaterial* BaArchive::GetMaterial( const BtChar *name )
{
	RsMaterial* pMaterial = (RsMaterial*)GetResource( BaRT_Material, name );

	if( pMaterial == BtNull )
	{
		BtPrint( "Could not find Material %s\r\n", name );
	}
	return pMaterial;
}

////////////////////////////////////////////////////////////////////////////////
// GetShader

RsShader* BaArchive::GetShader( const BtChar *name )
{
	RsShader* pShader = (RsShader*)GetResource( BaRT_Shader, name );

	if( pShader == BtNull )
	{
		BtPrint( "Could not find shader %s\r\n", name );
	}
	return pShader;
}

////////////////////////////////////////////////////////////////////////////////
// GetAnimator

SgAnimator* BaArchive::GetAnimator( const BtChar *name )
{
	SgAnimator* pAnimator = (SgAnimator*)GetResource( BaRT_Animator, name );

	if( pAnimator == BtNull )
	{
		BtPrint( "Could not find animator %s\r\n", name );
	}
	return pAnimator;
}

////////////////////////////////////////////////////////////////////////////////
// GetTexture

RsTexture* BaArchive::GetTexture( const BtChar* name )
{
	RsTexture* pTexture = (RsTexture*)GetResource( BaRT_Texture, name );

	if( pTexture == BtNull )
	{
		BtPrint( "Could not find texture %s\r\n", name );
	}
	return pTexture;
}

////////////////////////////////////////////////////////////////////////////////
// GetSprite

RsSprite* BaArchive::GetSprite( const BtChar* name )
{
	RsSprite* pSprite = (RsSprite*)GetResource( BaRT_Sprite, name );

	if( pSprite == BtNull )
	{
		BtPrint( "Could not find sprite %s\r\n", name );
	}
	return pSprite;
}

////////////////////////////////////////////////////////////////////////////////
// GetUserData

BaUserData* BaArchive::GetUserData( const BtChar* name )
{
	BaUserData* pUserData = (BaUserData*)GetResource( BaRT_UserData, name );

	if( pUserData == BtNull )
	{
		BtPrint( "Could not find user data %s\r\n", name );
	}
	return pUserData;
}

////////////////////////////////////////////////////////////////////////////////
// GetSound

SdSound* BaArchive::GetSound( const BtChar* name )
{
	SdSound* pSound = (SdSound*)GetResource( BaRT_Sound, name );

	if( pSound == BtNull )
	{
		BtPrint( "Could not find sound %s\r\n", name );
	}
	return pSound;
}

////////////////////////////////////////////////////////////////////////////////
// GetFlash

RsFlash* BaArchive::GetFlash( const BtChar* name )
{
	RsFlash* pFlash = (RsFlash*)GetResource( BaRT_Flash, name );

	if( pFlash == BtNull )
	{
		BtPrint( "Could not find Flash %s\r\n", name );
	}
	return pFlash;
}
