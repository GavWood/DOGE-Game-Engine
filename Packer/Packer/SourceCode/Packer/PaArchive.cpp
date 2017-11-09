////////////////////////////////////////////////////////////////////////////////
// PaArchive.cpp

// Includes
#include <stdio.h>
#include <string.h>
#include "FsFile.h"
#include "PaArchive.h"
#include "PaPacker.h"
#include "PaTopState.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaArchive::PaArchive()
{
	m_assets.clear();
	m_resources.clear();
	m_files.clear();
	
	// Clear the archive stats
	m_archiveHeader.m_nNumResources = 0;

	// Set the starting size
	m_archiveSize = sizeof( BaArchiveHeader );
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

PaArchive::~PaArchive()
{
	BtU32 nSize = (BtU32) m_resources.size();

	for( BtU32 i=0; i<nSize ; i++ )
	{
		delete m_resources[i];
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddResource

PaResource* PaArchive::AddResource( PaResource* pResource )
{
	for( BtU32 i=0; i<m_resources.size(); i++ )
	{
		PaResource* pFoundResource = m_resources[i];

		if( BtStrCompare( pFoundResource->uncompiledFilename(), pResource->uncompiledFilename() ) == BtTrue )
		{
			return pFoundResource;
		}
	}

	// Add to the array of resources
	m_resources.push_back( pResource );

	// Return the pointer
	return pResource;
}

////////////////////////////////////////////////////////////////////////////////
// AddResource

BtU32 PaArchive::AddResource( PaAsset* pAsset,
							  PaMemoryResource& memoryResource,
							  const BtChar* pFilename,
							  const BtChar* pTitle,
							  BaResourceType eType )
{
    char FS = FsFile::GetFileSeparator();
	BtChar Filename[MaxFilePath];

	BtU32 nResourceID = PaPacker::GlobalID();

	// This resource is a binary resource created inside the export class
	if( pFilename == NULL )
	{
		sprintf( Filename, "%04d%s%c%s.tmp", nResourceID,
                                             pAsset->File().m_szPath,
                                             FS,
                                             pAsset->File().m_szTitle );

		pFilename = Filename;
	}
	else
	{
		sprintf( Filename, "%04d%s.tmp", nResourceID, pFilename );

		pFilename = Filename;
	}

	// Add the dependency to the asset
	pAsset->AddDependency( pFilename );

	// Create a resource
	PaResource* pResource = new PaResource( pAsset,
											pFilename,
											pTitle,
											eType );

	PaPacker::AdvanceGlobalID();

	// Set the resource ID
	pResource->GetHeader()->m_nResourceID = nResourceID;

	// Add the resource
	AddResource( pResource );

	// Set the resource size
	pResource->GetHeader()->m_nResourceSize = memoryResource.Tell();

	BtChar* pBinFile = pResource->compiledFilename();

	// Save the resource
	PaMemoryResource resource(PMR_Write);
	resource << *pResource->GetHeader();
	resource.Save( pBinFile );
	memoryResource.Append( pBinFile );

	// Return the resource ID
	return nResourceID;
}

////////////////////////////////////////////////////////////////////////////////
// pTitle

const BtChar* PaArchive::pTitle() const
{
	return m_file.m_szTitle;
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

PaResource* PaArchive::GetResource( BtU32 nIndex )
{
	return m_resources[nIndex];
}
	
////////////////////////////////////////////////////////////////////////////////
// nResources

BtU32 PaArchive::GetNumResources() const
{
	return (BtU32) m_resources.size();
}

////////////////////////////////////////////////////////////////////////////////
// GetAsset

PaAsset* PaArchive::GetAsset( BtU32 nIndex )
{
	return &m_assets[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// nAssets

BtU32 PaArchive::GetNumAssets() const
{
	return (BtU32) m_assets.size();
}

////////////////////////////////////////////////////////////////////////////////
// AddAsset

void PaArchive::AddAsset( PaAsset& asset )
{
	m_assets.push_back( asset );
}

////////////////////////////////////////////////////////////////////////////////
// nFiles

BtU32 PaArchive::nFiles() const
{
	return (BtU32) m_files.size();
}

////////////////////////////////////////////////////////////////////////////////
// AddFile

void PaArchive::AddFile( const PaFileDetails& file )
{
	m_files.push_back( file );
}

////////////////////////////////////////////////////////////////////////////////
// GetFile

const PaFileDetails* PaArchive::GetFile( BtU32 nIndex )
{
	return &m_files[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// GetFile

const PaFileDetails* PaArchive::GetFile( const BtChar* pFilename )
{
    BtU32 numFiles = nFiles();
    
	for( BtU32 i=0; i<numFiles; i++ )
	{
        BtChar *other = m_files[i].m_szFilename;
		if( BtStrCompareNoCase( pFilename, other ) == BtTrue )
		{
			return &m_files[i];
		}
	}
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// isDependency

BtBool PaArchive::isDependency( const BtChar* pFilename )
{
	PaAsset* pAsset = BtNull;

	// Loop through the asset list
	for( BtU32 nAsset=0; nAsset<GetNumAssets(); nAsset++ )
	{
		// Cache each resource header
		pAsset = GetAsset(nAsset);

		BtU32 nDependencies = pAsset->GetNumDependencies();

		for( BtU32 nDependency=0; nDependency<nDependencies; nDependency++ )
		{
			// Cache each dependency
			const PaFilename& dependency = pAsset->GetDependency( nDependency );

			if( BtStrCompare( dependency.GetFilename(), pFilename ) == BtTrue )
			{
				return BtTrue;
			}
		}		
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveOldDependencies

void PaArchive::RemoveOldDependencies()
{
	ErrorLog::Printf( "Checking dependencies still exist in %s.\n", m_archiveHeader.m_szTitle );

	BtU32 nNumResources = GetNumResources();

	for( BtU32 iIndex=0; iIndex<nNumResources; iIndex++ )
	{
        std::vector< PaResource* >::iterator It = m_resources.begin();
        std::vector< PaResource* >::iterator EndIt = m_resources.end();

		for( ; It != EndIt; ++It )
		{
			PaResource* pResource = *It;

			// Cache the uncompiled filename
			const BtChar* pUncompiledFilename = pResource->uncompiledFilename();

			if( isDependency( pUncompiledFilename ) == BtFalse )
			{
				ErrorLog::Printf( " Removed %s.\n", pResource->compiledFilename() );
				m_resources.erase( It );

				// Remove this file
                //FsFile::DeleteFile( pResource->compiledFilename() ); GAV TO DO
				break;
			}
		}
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// WriteResourceSizes

struct ResourceSizes
{
	BtS32					m_size;
	BaResourceHeader*		m_pHeader;
};

int ResourceSizeCompare( const void* elem1, const void* elem2 )
{
	ResourceSizes* pHeaderA = (ResourceSizes*)elem1;
	ResourceSizes* pHeaderB = (ResourceSizes*)elem2;
	return pHeaderB->m_size - pHeaderA->m_size;
}

////////////////////////////////////////////////////////////////////////////////
// WriteResourceSizes

void PaArchive::WriteResourceSizes()
{
	BtChar text[256];

	std::vector<ResourceSizes> resourceSizes;

	BtU32 mainMemoryTotal = 0;
	BtU32 graphicsMemoryTotal = 0;
	BtU32 soundMemoryTotal = 0;

	// Cache the resource count
	BtU32 nArchiveResources = (BtU32) GetNumResources();

	// Validate the archive
	if( nArchiveResources == 0 )
	{
		ErrorLog::Fatal_Printf( "PaPacker::WriteResourceSizes Empty archive %s.\n", m_archiveHeader.m_szTitle );
	}

	FsFile archiveDetails;

	// Set the title
    char FS = FsFile::GetFileSeparator();
    BtChar buildNotes[MaxFilePath];
    sprintf(buildNotes, "%s%cBuildNotes%c", PaTopState::pWorkingPath(), FS, FS );
    
    BtChar filename[MaxFilePath];
	switch( PaTopState::Instance().GetPlatform() )
	{
	case PackerPlatform_WinDX:
		sprintf( filename, "%sWin32%c%s.txt",   buildNotes, FS, m_file.m_szTitle );
		break;
	case PackerPlatform_WinGL:
		sprintf( filename, "%sWin32GL%c%s.txt", buildNotes, FS, m_file.m_szTitle );
		break;
	case PackerPlatform_GLES:
		sprintf( filename, "%siPhone%c%s.txt",  buildNotes, FS, m_file.m_szTitle );
		break;
	case PackerPlatform_OSX:
		sprintf(filename,  "%sOSX%c%s.txt",     buildNotes, FS, m_file.m_szTitle);
		break;
    default:
        ErrorLog::Fatal_Printf("Platform not set.");
        break;
	}

	// Open the output archive file
	archiveDetails.Open( filename, FsMode_WriteAscii );

	// Did the file open?
	if( archiveDetails.IsOpen() == BtFalse )
	{
		ErrorLog::Fatal_Printf( "Cannot open %s\n", filename );
	}

	// Write out resource data
	for( BtU32 i=0; i<m_resources.size(); i++ )
	{
		// Cache each resource
		PaResource* pResource = GetResource(i);

		// Cache the resource header
		BaResourceHeader* pHeader = pResource->GetHeader();

		ResourceSizes resourceSize;
		resourceSize.m_size    = pHeader->m_nResourceSize;
		resourceSize.m_pHeader = pHeader;
		resourceSizes.push_back( resourceSize );
	}

	// Sort the resource sizes
	qsort( &resourceSizes[0], (BtU32)resourceSizes.size(), sizeof(ResourceSizes), ResourceSizeCompare );

	// Write out resource data
	for( BtU32 i=0; i<m_resources.size(); i++ )
	{
		// Cache the resource header
		BaResourceHeader* pHeader = resourceSizes[i].m_pHeader;

		// Open each resource file
		FsFile resourceFile;

		// Setup the text
		BtU32 nSize = pHeader->m_nResourceSize;

		sprintf( text, "%.2f, %s, ", (BtFloat)nSize / 1024.0f, pHeader->m_szTitle );

		switch( pHeader->m_type )
		{
		case BaRT_Texture:
			BtStrCat( text, 256, "Texture" );
			// Total the graphics sizes
			graphicsMemoryTotal += nSize;
			break;
		case BaRT_Sprite:
			BtStrCat( text, 256, "Sprite" );
			break;
		case BaRT_Sound:
			{
				BtChar buff[32];
				sprintf( buff, "Sound (channels=%d)", pHeader->m_resourceFlags );
				BtStrCat( text, 256, buff );
				// Total the sound sizes
				soundMemoryTotal += nSize;
			}
			break;
		case BaRT_Material:
			BtStrCat( text, 256, "Material" );
			break;
		case BaRT_Font:
			BtStrCat( text, 256, "Font" );
			break;
		case BaRT_UserData:
			BtStrCat( text, 256, "Userdata" );
			break;
		default:
			BtStrCat( text, 256, "Not included in size list" );
			break;
		}

		BtStrCat( text, 256, "\n" );

		// Write out the resource data
		archiveDetails.Write( (BtU8*)text, BtStrLength( text ) );

		// Total the sizes
		mainMemoryTotal += nSize;
	}

	// Total the size
	sprintf( text, "\nMain memory total %d KBytes\n", mainMemoryTotal / 1024 );
	archiveDetails.Write( (BtU8*)text, BtStrLength( text ) );

	sprintf( text, "Graphics total %d KBytes\n", graphicsMemoryTotal / 1024 );
	archiveDetails.Write( (BtU8*)text, BtStrLength( text ) );

	sprintf( text, "Sound total %d KBytes\n", soundMemoryTotal / 1024 );
	archiveDetails.Write( (BtU8*)text, BtStrLength( text ) );

	// Close the file
	archiveDetails.Close();
}

////////////////////////////////////////////////////////////////////////////////
// WriteArchive

void PaArchive::WriteArchive()
{
	// Cache the resource count
	BtU32 nArchiveResources = (BtU32) GetNumResources();

	// Validate the archive
	if( nArchiveResources == 0 )
	{
		ErrorLog::Fatal_Printf( "PaPacker::WriteArchive Empty archive %s.\n", m_archiveHeader.m_szTitle );
	}

	FsFile archiveFile;

	// Open the output archive file
	archiveFile.Open( m_szArchiveFilename, FsMode_Write );

	// Did the file open?
	if( archiveFile.IsOpen() == BtFalse )
	{
		ErrorLog::Fatal_Printf( "Cannot open %s\n", m_szArchiveFilename );
	}

	// Display progress
	ErrorLog::Printf( "Packing stage.\n" );

	// Write out the resource headers
	for( BtU32 i=0; i<nArchiveResources; i++ )
	{
		// Cache each resource
		PaResource* pResource = GetResource(i);

		// Write out each resource header
		archiveFile.Write( (BtU8*) pResource->GetHeader(), sizeof(BaResourceHeader) );
	}

	// Write out resource data
	for( BtU32 i=0; i<nArchiveResources; i++ )
	{
		// Cache each resource
		PaResource* pResource = GetResource(i);

		// Cache the resource header
		BaResourceHeader* pHeader = pResource->GetHeader();

		// Open each resource file
		FsFile resourceFile;

		// Open the resource container file
		resourceFile.Open( pResource->compiledFilename(), FsMode_Read );

		// Skip the headers
		resourceFile.Skip( sizeof( BaResourceHeader ) );

		// Cache the size
		BtU32 nSize = pHeader->m_nResourceSize;

		// Create a memory resource to hold this resource
		PaMemoryResource memoryResource( PMR_Read, nSize );

		// Read the resource data
		resourceFile.Read( memoryResource.pCurrent(), nSize );

		// Packing
		//ErrorLog::Printf( " Packing %s %.2f KBytes.\n", pHeader->m_szTitle, (BtFloat) nSize / 1024.0f );

		// Cache the file position
		BtU32 nRelativeFilePositionOfResource = archiveFile.GetPosition();

		// Find out what position in the file has been reached
		BtU32 nBytesShortOfAlignment = pHeader->m_nResourceOffset - nRelativeFilePositionOfResource;

		// Skip these bytes
		archiveFile.Skip( nBytesShortOfAlignment );

		// Write out the resource data
		archiveFile.Write( memoryResource.pCurrent(), nSize );

		// Close the container file
		resourceFile.Close();
	}

	// Close the file
	archiveFile.Close();
}
