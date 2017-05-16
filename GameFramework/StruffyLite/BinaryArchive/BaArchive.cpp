////////////////////////////////////////////////////////////////////////////////
// BaArchive

// Includes
#include <new>
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
#include "ErrorLog.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Statics

BtLinkedList<BaArchiveNode> BaArchive::m_archives;

////////////////////////////////////////////////////////////////////////////////
// Constructor

BaArchive::BaArchive()
{
	m_isLoaded = BtFalse;
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
	BtBool isInitialised = ApConfig::IsInitialised();
	if (isInitialised == BtFalse)
	{
		ErrorLog::Printf("ApConfig is not initialised. This is needed for archive extensions and filenames.");
	}

	BaArchiveHeader archiveHeader;
	
	BtStrCopy(m_filename, archiveName);

	// Load the file
	BtChar filename[256];
	BtStrCopy(filename, 256, ApConfig::GetResourcePath());
	BtStrCat(filename, 256, archiveName);
	BtStrCat(filename, 256, ApConfig::GetExtension());

	FILE *f = fopen(filename, "rb");

	if (f)
	{
		fread((void*)&archiveHeader, 1, sizeof(archiveHeader), f);

		// Make the memory
		m_pMemory = (BtU8*)BtMemory::Allocate(archiveHeader.m_nDataAndInstanceSize);

		if (m_pMemory == BtNull)
		{
			ErrorLog::Fatal_Printf("Can't allocate memory for archive.");
		}

		// Load the compressed file
		LBtCompressedFile compressedFile;
		compressedFile.Read(f, m_pMemory, archiveHeader.m_nDataAndInstanceSize);

		BtU32 kBytes = archiveHeader.m_nDataAndInstanceSize / 1024;
		BtU32 mBytes = kBytes / 1024;
		(void)mBytes;

		fclose(f);
		m_isLoaded = BtTrue;
	}
	else
	{
		m_isLoaded = BtFalse;
		system("dir \\temp\\test.txt");
		BtPrint("Could not find archive %s\r\n", archiveName);
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsLoaded

BtBool BaArchive::IsLoaded()
{
	return m_isLoaded;
}

////////////////////////////////////////////////////////////////////////////////
// CreateResources

void BaArchive::CreateResources()
{
	// Cache the archive header pointer
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader* )m_pMemory;

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
	if( m_isLoaded )
	{
		Unload();
	}

	LoadFile( archiveName );
	CreateResources();
	FixPointers();
	ValidateResources();

	// Add this to our list of archives
	m_archiveNode.m_pArchive = this; 
	m_archives.AddHead( &m_archiveNode );
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void BaArchive::FixPointers()
{
	// Cache the archive header pointer
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader*)m_pMemory;

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

		// Check the resource type
		if( resourceType != BaRT_Max )
		{			
			BtAssert( pResource->resourceType() == pResourceHeader->m_type );

			switch( pResourceHeader->m_type )
			{
			case BaRT_Sprite:
				{
					int a=0;
					a++;
				}
				break;

			case BaRT_Scene:
				{
					int a=0;
					a++;
				}
				break;

			case BaRT_Flash:
				{
					int a=0;
					a++;
				}
			}

			// Fix pointers
			pResource->FixPointers( pFileData, this );
		
			// Create on the device
			pResource->CreateOnDevice();
		}
		
		// Move to the next resource
		pResourceHeader++;	
	}
	
	BtPrint( "Ended creating resources.\r\n" );	
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
	if( m_isLoaded )
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

		// Free the memory
		BtMemory::Free( m_pMemory );

		// Remove this from the list
		m_archives.Remove( &m_archiveNode );

		// Mark as not loaded
		m_isLoaded = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

BtU8* BaArchive::GetResource( BaResourceType eType, const BtChar* szTitle ) const
{
	// Get the archive header
	BaArchiveHeader* pArchiveHeader = (BaArchiveHeader*) m_pMemory;

	// Get the start of the resource headers
	BtU32 nData = ( (BtU32) ( m_pMemory ) + sizeof( BaArchiveHeader ) );

	// Get the first resource header
	BaResourceHeader* pResource = (BaResourceHeader*) nData;

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
	BtU32 nData = ( (BtU32) ( m_pMemory ) + sizeof( BaArchiveHeader ) );

	// Get the first resource header
	BaResourceHeader* pResource = (BaResourceHeader*) nData;

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
