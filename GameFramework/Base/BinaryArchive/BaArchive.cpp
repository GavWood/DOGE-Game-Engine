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
#include "BtCRC.h"
#include "BtString.h"
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

	BtStrCopy(m_filename, archiveName);

	// Load the file
	BtChar filename[256];
	BtStrCopy(filename, 256, ApConfig::GetResourcePath());
	BtStrCat(filename, 256, archiveName);
	BtStrCat(filename, 256, ApConfig::GetExtension());

	FILE *f = fopen(filename, "rb");
    
    //printf( "Filename=%s", filename );
	if (f)
	{
		// Read the archive header
		fread((void*)&m_header, 1, sizeof(BaArchiveHeader), f);

		// Check the archive header
		BtU32 version = m_header.m_nPackerVersion;
		(void)version;

		BtU32 checksum = BtCRC::GenerateHashCode((BtU8*)&m_header, sizeof(BaArchiveHeader) - sizeof(BtU32));

		// Compare the m_header
		BtAssert(checksum == m_header.m_nHeaderCheckSum);

		// Make the memory
		m_pArchiveMemory = (BtU8*)BtMemory::Allocate(m_header.m_nDataSize);

		BtU32 kBytes = m_header.m_nDataSize / 1024;
		BtU32 mBytes = kBytes / 1024;
		(void)mBytes;

		//ErrorLog::Printf("Uncompressing %s from %d\n", filename, archiveHeader.m_nDataSize);

		if (m_pArchiveMemory == BtNull)
		{
			ErrorLog::Fatal_Printf("Can't allocate memory for archive.");
		}

		// Load the compressed file
		BtCompressedFile compressedFile;
		compressedFile.Read(f, m_pArchiveMemory, m_header.m_nDataSize );

		fclose(f);
		m_isLoaded = BtTrue;
	}
	else
	{
		m_isLoaded = BtFalse;
        
#ifdef WIN32
		system("dir \\temp\\test.txt");
#endif
        
        ErrorLog::Fatal_Printf("Could not find archive \"%s\"\r\n", archiveName);
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

void BaArchive::GetInstanceSizes()
{
	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)(m_pArchiveMemory);

	BtU32 totalInstanceSize = 0;

	// Loop through the resources and markup any pointers
	for (BtU32 i = 0; i < m_header.m_nNumResources; i++)
	{
		// Set the instance position
		pResourceHeader->m_instancePosition = totalInstanceSize;

		// Get the instance size
		BtU32 instanceSize = GetInstanceSize(pResourceHeader);

		// Increment the instance sizes
		totalInstanceSize += instanceSize;

		// Increment the resource header
		pResourceHeader++;
	}

	m_pInstanceMemory = (BtU8*)BtMemory::Allocate(totalInstanceSize);
}

////////////////////////////////////////////////////////////////////////////////
// CreateResources

void BaArchive::CreateResources()
{
	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pArchiveMemory );

	// Loop through the resources and markup any pointers
	for( BtU32 i=0; i<m_header.m_nNumResources; i++ )
	{
		// Set the pointer to the instance
		pResourceHeader->m_pResource = (BaResource*)(m_pInstanceMemory + pResourceHeader->m_instancePosition);

		// Create the instance
		CreateResourceInstance( pResourceHeader->m_type, (BtU8*)pResourceHeader->m_pResource );

		// Cache the base type
		BaResource* pResourceInstance = pResourceHeader->m_pResource;

		// Set the resource title
		pResourceInstance->SetTitle( pResourceHeader->m_szTitle );

		BaResourceType type = pResourceHeader->m_pResource->resourceType();
		(void)type;

		// Increment the resource header
		pResourceHeader++;
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
	GetInstanceSizes();
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
	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pArchiveMemory );

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<m_header.m_nNumResources; nResourceIndex++ )
	{		
		// Cache each resource
		BaResource* pResource = pResourceHeader->m_pResource;

		// Locate the file data
		BtU8 *pFileData = ( m_pArchiveMemory + pResourceHeader->m_nResourceOffset );

		BaResourceType resourceType = pResource->resourceType();

		// Check the resource type
		if( resourceType != BaRT_Max )
		{	
			if (nResourceIndex == 21)
			{
				int a = 0;
				a++;
			}

			if (nResourceIndex == 22)
			{
				int a = 0;
				a++;
			}

			BtAssert(pResource->resourceType() == pResourceHeader->m_type);

			switch (pResourceHeader->m_type)
			{
                case BaRT_Sprite:
                {
                    int a = 0;
                    a++;
                }
                break;
                case BaRT_Scene:
                {
                    int a = 0;
                    a++;
                }
                break;
                case BaRT_Flash:
                {
                    int a = 0;
                    a++;
                }
				case BaRT_UserData:
				{
					int a = 0;
					a++;
				}
                case BaRT_Texture:
                {
                    int a = 0;
                    a++;
                }
                    
                default:
                    break;
			}

			// Fix pointers
			pResource->FixPointers( pFileData, this );
		
			// Create on the device
			pResource->CreateOnDevice();
		}
		
		// Move to the next resource
		pResourceHeader++;	
	}
    // BtPrint( "Ended creating resources.\r\n" );
}


////////////////////////////////////////////////////////////////////////////////
// ValidateResources

void BaArchive::ValidateResources()
{
	RsSprite *pSprite;

	// Cache each resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pArchiveMemory );

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<m_header.m_nNumResources; nResourceIndex++ )
	{		
		// Cache each resource
		BaResource* pResource = pResourceHeader->m_pResource;

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
		// Cache each resource header
		BaResourceHeader* pResourceHeader = (BaResourceHeader*)( m_pArchiveMemory );

		// Loop through the resources and markup any pointers
		for( BtU32 i=0; i<m_header.m_nNumResources; i++ )
		{
			// Get the instance address
			BaResource* pResource = pResourceHeader->m_pResource;

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
		BtMemory::Free( m_pArchiveMemory );

		// Free the instance memory
		BtMemory::Free( m_pInstanceMemory );

		// Remove this from the list
		m_archives.Remove( &m_archiveNode );

		// Mark as not loaded
		m_isLoaded = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

BaResource* BaArchive::GetResource( BaResourceType eType, const BtChar* szTitle ) const
{
    if( m_pArchiveMemory == BtNull )
    {
        ErrorLog::Fatal_Printf( "Archive not loaded." );
    }

	// Get the first resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)m_pArchiveMemory;

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<m_header.m_nNumResources; nResourceIndex++ )
	{
		// Is this the string resource?
		if( pResourceHeader->m_type == eType )
		{
			if( BtStrCompareNoCase( pResourceHeader->m_szTitle, szTitle ) == BtTrue )
			{
				return pResourceHeader->m_pResource;
			}
		}

		// Get the next resource header
		pResourceHeader++;
	}

	// Return BtNull though we will never reach here
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

BaResource* BaArchive::GetResource( BtU32 nResourceID ) const
{
	// Don't search for a null resource
	if( nResourceID == 0  )
	{
		return BtNull;
	}

	// Get the first resource header
	BaResourceHeader* pResourceHeader = (BaResourceHeader*)m_pArchiveMemory;

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<m_header.m_nNumResources; nResourceIndex++ )
	{
		// Is this the string resource?
		if( pResourceHeader->m_nResourceID == nResourceID )
		{
			return pResourceHeader->m_pResource;
		}

		// Get the next resource header
		pResourceHeader++;
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
