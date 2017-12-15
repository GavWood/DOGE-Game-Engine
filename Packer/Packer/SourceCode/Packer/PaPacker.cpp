////////////////////////////////////////////////////////////////////////////////
// PaPacker.cpp

// Includes
#include <string.h>
#include <zlib.h>
#include "BaArchive.h"
#include "BtTypes.h"
#include "BtString.h"
#include "BtMemory.h"
#include "BtMemory.h"
#include "BtTime.h"
#include "ExSchedule.h"
#include "ExExporter.h"
#include "BaArchive.h"
#include "BaFileData.h"
#include "ErrorLog.h"
#include "PaPacker.h"
#include "PaResource.h"
#include "PaTopState.h"
#include "FsFile.h"
#include "FsFindFile.h"
#include "BtCRC.h"

// GAV TODO DIRTY STATE
//BaResourceType gDirty[] = { BaRT_Scene, BaRT_VertexBuffer, BaRT_IndexBuffer };
//BaResourceType gDirty[] = { BaRT_Shsader };
//BaResourceType gDirty[] = { BaRT_Texture };
BaResourceType gDirty[] = { BaRT_Max };
//BaResourceType gDirty[] = { BaRT_Scene };
////BaResourceType gDirty[] = { BaRT_Scene, BaRT_CollisionAnalytical, BaRT_CollisionMesh };
//BaResourceType gDirty[] = { BaRT_Sound };
//BaResourceType gDirty[] = { BaRT_Material };

const BtU32 PACKER_VERSION = 100;

////////////////////////////////////////////////////////////////////////////////
// Declarations

BtU32 PaPacker::m_nGlobalID = 1;

PaPacker::PaPacker()
{
	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// GetResourceBuildListName

BtChar *PaPacker::GetResourceBuildListName()
{
    BtChar FS = FsFile::GetFileSeparator();
	switch( PaTopState::Instance().GetPlatform() )
	{
	case PackerPlatform_WinDX:
        sprintf( m_resourceListName, "%s%cBuildFiles%cResourceList.WinDX", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_GLES:
		sprintf( m_resourceListName, "%s%cBuildFiles%cResourceList.iOS", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_WinGL:
		sprintf( m_resourceListName, "%s%cBuildFiles%cResourceList.WinGL", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_OSX:
		sprintf( m_resourceListName, "%s%cBuildFiles%cResourceList.OSX", PaTopState::pWorkingPath(), FS, FS );
		break;
    default:
        ErrorLog::Fatal_Printf("Platform not defined");
        break;
	}
	return m_resourceListName;
}

////////////////////////////////////////////////////////////////////////////////
// GetAssetBuildListName

BtChar *PaPacker::GetAssetBuildListName()
{
    BtChar FS = FsFile::GetFileSeparator();
    static BtChar assetBuildListName[256];
	switch( PaTopState::Instance().GetPlatform() )
	{
	case PackerPlatform_WinDX:
		sprintf( assetBuildListName, "%s%cBuildFiles%cAssetBuildList.WinDX", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_GLES:
		sprintf( assetBuildListName, "%s%cBuildFiles%cAssetBuildList.iOS", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_WinGL:
		sprintf( assetBuildListName, "%s%cBuildFiles%cAssetBuildList.WinGL", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_OSX:
		sprintf( assetBuildListName, "%s%cBuildFiles%cAssetBuildList.OSX", PaTopState::pWorkingPath(), FS, FS );
		break;
    default:
        ErrorLog::Fatal_Printf("Platform not defined");
        break;
	}
	return assetBuildListName;
}

////////////////////////////////////////////////////////////////////////////////
// GetArchiveBuildListName

BtChar *PaPacker::GetArchiveBuildListName()
{
    BtChar FS = FsFile::GetFileSeparator();
    switch( PaTopState::Instance().GetPlatform() )
	{
	case PackerPlatform_WinDX:
		sprintf( m_archiveListName, "%s%cBuildFiles%cArchiveTimes.WinDX", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_GLES:
		sprintf( m_archiveListName, "%s%cBuildFiles%cArchiveTimes.iOS", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_WinGL:
		sprintf( m_archiveListName, "%s%cBuildFiles%cArchiveTimes.WinGL", PaTopState::pWorkingPath(), FS, FS );
		break;
	case PackerPlatform_OSX:
		sprintf( m_archiveListName, "%s%cBuildFiles%cArchiveTimes.OSX", PaTopState::pWorkingPath(), FS, FS );
		break;
    default:
        ErrorLog::Fatal_Printf("Platform not defined");
        break;
	}
	return m_archiveListName;
}

////////////////////////////////////////////////////////////////////////////////
// LoadArchiveBuildTimes

void PaPacker::LoadArchiveBuildTimes()
{
	PaMemoryResource ArchiveTimes( PMR_Read );

	if( ArchiveTimes.Load( GetArchiveBuildListName() ) == BtFalse )
	{
		return;
	}

	BtU32 m_nArchiveBuildTimes;

	// Read in the number of archive build times
	ArchiveTimes >> m_nArchiveBuildTimes;

	// Get each schedule item
	ExScheduleItem* pItem = (ExScheduleItem*) ArchiveTimes.pCurrent();

	for( BtU32 nItems=0; nItems<m_nArchiveBuildTimes; nItems++ )
	{
		// Loop through the archive list
		for( BtU32 nArchive=0; nArchive<m_archives.size(); nArchive++ )
		{
			// Cache each archive
			PaArchive* pArchive = m_archives[nArchive];

			// Set the correct resource ID
			if( BtStrCompare( pArchive->m_file.m_szFilename, pItem->m_szFilename ) == BtTrue )
			{
				// Copy build time here
				break;
			}
		}
		pItem++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// isDependency

BtBool PaPacker::isDependency( BtChar* pFilename )
{
	PaAsset* pAsset = BtNull;

	// Loop through the archive list
	for( BtU32 nArchive=0; nArchive<m_archiveCount; nArchive++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[nArchive];

		// Loop through the asset list
		for( BtU32 nAsset=0; nAsset<pArchive->GetNumAssets(); nAsset++ )
		{
			// Cache each resource header
			pAsset = pArchive->GetAsset(nAsset);

			for( BtU32 nDependency=0; nDependency<pAsset->GetNumDependencies(); nDependency++ )
			{
				// Cache each dependency
				const PaFilename& dependency = pAsset->GetDependency( nDependency );

				if( BtStrCompare( dependency.GetFilename(), pFilename ) == BtTrue )
				{
					return BtTrue;
				}
			}
		}
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetAsset

PaAsset* PaPacker::GetAsset( BtChar* pFilename )
{
	PaAsset* pResource = BtNull;

	// Loop through the file list and create 
	for( BtU32 i=0; i<m_archiveCount; i++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[i];

		// Loop through the file list and create 
		for( BtU32 j=0; j<pArchive->GetNumAssets(); j++ )
		{
			// Cache each resource header
			pResource = pArchive->GetAsset(j);

			if( BtStrCompare( pResource->m_file.m_szFilename, pFilename ) == BtTrue )
			{
				return pResource;
			}
		}
	}
	return pResource;
}

////////////////////////////////////////////////////////////////////////////////
// LoadGlobalID

void PaPacker::LoadGlobalID()
{
    char FS = FsFile::GetFileSeparator();
	BtChar szFilename[256];
	sprintf( szFilename, "%s%cBuildFiles%cglobalID.dat", PaTopState::pWorkingPath(), FS, FS );

	PaMemoryResource globalID( PMR_Read );
	if( globalID.Load( szFilename ) == BtTrue )
	{
		globalID >> m_nGlobalID;
	}
}

////////////////////////////////////////////////////////////////////////////////
// ReadAssetBuildTimes

void PaPacker::ReadAssetBuildTimes()
{
	PaMemoryResource ArchiveTimes( PMR_Read );

	if( ArchiveTimes.Load( GetAssetBuildListName() ) == BtFalse )
	{
		return;
	}

	ExScheduleItem scheduleItem;

	PaAsset* pAsset;

	// Set the number of Resources
	BtU32 nResourceBuildTimes;
	ArchiveTimes >> nResourceBuildTimes;

	for( BtU32 nScheduleItems=0; nScheduleItems<nResourceBuildTimes; nScheduleItems++ )
	{
		// Get each schedule item
		ArchiveTimes >> scheduleItem;

		// Match the schedule item to the asset
		pAsset = GetAsset( scheduleItem.m_szFilename );

		if( pAsset != BtNull )
		{
			pAsset->m_lastExportTime = scheduleItem.m_tLastExport;
			pAsset->m_dependencies.Read( ArchiveTimes );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// FindArchivesAndFiles

void PaPacker::FindArchivesAndFiles()
{
    char FS = FsFile::GetFileSeparator();
    BtU32 nFileList;
    PaFileDetails fileInformation;

	// Construct the path where the assets are held
	BtChar szStartFolder[256];
    
    sprintf( szStartFolder, "%sgamedata", PaTopState::pWorkingPath() );

    FsFindFile fileFind( szStartFolder );

	// Scan the folder
	fileFind.search();

	// Cache the number of files
	nFileList = (BtU32) fileFind.filelist.size();
    
	m_archiveCount = 0;

	// Clear the array of archives
	m_archives.clear();

	PaArchive* pArchive = NULL;

	m_nFiles = 0;
    
    // Display the number of files
    ErrorLog::Printf("Found %d files.\r\n", nFileList);
    
    for( BtU32 i=0; i<nFileList; i++ )
	{
		// Cache each file
		fileInformation = fileFind.filelist[i];

		if( ( fileInformation.m_bFolder == BtTrue ) &&
			  ( BtStrCompare( fileInformation.m_szExtension, "archive" ) == BtTrue )
		  )
		{
			// Create a new archive
			pArchive = new PaArchive;
			
			// Set the file information
			pArchive->m_file = fileInformation;

			// Set the tile
			BtStrCopy( pArchive->m_archiveHeader.m_szTitle, MaxArchiveName, fileInformation.m_szTitle );

			// Setup the archive filename
			switch( PaTopState::Instance().GetPlatform() )
			{
			case PackerPlatform_WinDX:
				sprintf( pArchive->m_szArchiveFilename, "%s%cBuildFiles%c%s.WinDX",
						   PaTopState::pWorkingPath(),
                           FS, FS,
						   fileInformation.m_szTitle );
				break;

			case PackerPlatform_GLES:
				sprintf( pArchive->m_szArchiveFilename, "%s%cBuildFiles%c%s.iPhone",
						   PaTopState::pWorkingPath(),
                           FS, FS,
						   fileInformation.m_szTitle );
				break;

			case PackerPlatform_OSX:
				sprintf( pArchive->m_szArchiveFilename, "%s%cBuildFiles%c%s.OSX",
                            PaTopState::pWorkingPath(),
                            FS, FS,
                            fileInformation.m_szTitle);
				break;

			case PackerPlatform_WinGL:
				sprintf( pArchive->m_szArchiveFilename, "%s%cBuildFiles%c%s.WinGL",
                            PaTopState::pWorkingPath(),
                            FS, FS,
                            fileInformation.m_szTitle );
				break;
            
            default:
                ErrorLog::Fatal_Printf( "" );
                break;
			}

			switch( PaTopState::Instance().GetPlatform() )
			{
			case PackerPlatform_WinDX:
				sprintf( pArchive->m_szPackedFilename, "%s%cRelease%c%s.WinDXz",
						   PaTopState::pWorkingPath(),
                            FS, FS,
						   fileInformation.m_szTitle );
				break;

			case PackerPlatform_GLES:
				sprintf( pArchive->m_szPackedFilename, "%s%cRelease%c%s.iPhonez",
						   PaTopState::pWorkingPath(),
                            FS, FS,
						   fileInformation.m_szTitle );
				break;
			
			case PackerPlatform_OSX:
				sprintf( pArchive->m_szPackedFilename, "%s%cRelease%c%s.OSXz",
						   PaTopState::pWorkingPath(),
                            FS, FS,
						   fileInformation.m_szTitle);
				break;

			case PackerPlatform_WinGL:
				sprintf( pArchive->m_szPackedFilename, "%s%cRelease%c%s.WinGLz",
					 	   PaTopState::pWorkingPath(),
                            FS, FS,
						   fileInformation.m_szTitle );
				break;
                    
            default:
                ErrorLog::Fatal_Printf( "" );
                break;

			}

			// Add each archive
			m_archives.push_back( pArchive );

			// Increment the archive count
			m_archiveCount++;
		}
		else
		{
			if( pArchive &&
				( fileInformation.m_bFolder == BtFalse ) &&
				BtStrCompare( pArchive->m_file.m_szFilename, fileInformation.m_szFilename, BtStrLength(pArchive->m_file.m_szFilename) ) == BtTrue )
			{
				// Add these files
				pArchive->AddFile( fileInformation );

				// Increment the number of files
				m_nFiles++;
			}
		}
	}
	
	if( m_archiveCount > 1 )
	{
		ErrorLog::Printf( "Found %d archives and %d files.\n", m_archiveCount, m_nFiles );
	}
	else
	{
		ErrorLog::Printf( "Found %d archive and %d files.\n", m_archiveCount, m_nFiles );
	}
}

////////////////////////////////////////////////////////////////////////////////
// ReadResourceList

void PaPacker::ReadResourceList()
{
	PaMemoryResource resourceList( PMR_Read );

	BtChar *resourceListName = GetResourceBuildListName();

	// Read the file
	if( resourceList.Load( resourceListName ) == BtTrue )
	{
		// Cache the number of previous resources
		BtU32 nResources = resourceList.GetCurrentSize() / sizeof( PaResource );

		for( BtU32 i=0; i<nResources; i++ )
		{
			PaResource* pResource = new PaResource;
			resourceList >> *pResource;

			// Loop through the archives
			for( BtU32 j=0; j<m_archiveCount; j++ )
			{
				// Cache each archive
				PaArchive* pArchive = m_archives[j];
			
				// Find the resource's archive
				if( BtStrCompare( pArchive->pTitle(), pResource->pArchiveName() ) == BtTrue )
				{
					// Check the resource is still a dependency
					if( isDependency( pResource->uncompiledFilename() ) == BtTrue )
					{
						// Add this resource
						pArchive->AddResource( pResource ); 
					}
					else
					{
						int a = 0;
						a++;
						// Remove this resource
						//ErrorLog::Printf( "Removed resource %s.\n", pResource->uncompiledFilename() );
					}
					break;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// BuildAssetList

void PaPacker::BuildAssetList()
{
	// Cache the exporter
	ExExporter& exporter = ExExporter::Instance();

	m_nResources = 0;

	// Loop through the file list and create 
	for( BtU32 i=0; i<m_archiveCount; i++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[i];
		
		// Cache the number of files
		BtU32 numFiles = pArchive->nFiles();

		// Loop through the file list and create 
		for( BtU32 j=0; j<numFiles; j++ )
		{
			// Cache the file information
			const PaFileDetails &file = *pArchive->GetFile(j);

			BaResourceType resourceType = BaRT_Max;

			// Identify the resource
			BtBool isResource = exporter.SetResourceType( file, resourceType );

			if( isResource == BtTrue)
			{
				// Create resource
				PaAsset asset = PaAsset( pArchive, resourceType, file );

				// Add the resource
				pArchive->AddAsset( asset );

				// Increment the resource count
				m_nResources++;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// FlagForExport

void PaPacker::FlagForExport()
{
	m_nToExport = 0;

	// Loop through the file list and create a schedule item per resource
	for( BtU32 i=0; i<m_archiveCount; i++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[i];

		BtU32 numAssets = pArchive->GetNumAssets();

		// Loop through the file list and create 
		for( BtU32 j=0; j<numAssets; j++ )
		{
			// Cache each resource header
			PaAsset* pAsset = pArchive->GetAsset(j);

			// Cache the last time the resource was written
			BtU64 lastResourceModifiedTime = pAsset->m_file.m_tResourceChanged;

			// Cache the number of dependencies
			BtU32 numDependencies = pAsset->GetNumDependencies();

			// Check each dependency to see if the any file has been changed more recently
			for( BtU32 nDependencyIndex=0; nDependencyIndex<numDependencies; nDependencyIndex++ )
			{
				// Cache each dependency
				const PaFilename& dependancyFile = pAsset->GetDependency( nDependencyIndex );

				// Get the file details
				const PaFileDetails* pFileDetails = pArchive->GetFile( dependancyFile.GetFilename() );

				if( pFileDetails != BtNull )
				{
					if( pFileDetails->m_tResourceChanged > lastResourceModifiedTime )
					{
						lastResourceModifiedTime = pFileDetails->m_tResourceChanged;
					}
				}
			}

			// Count the number of dirty flags
			BtU32 nDirtyFlagCount = sizeof(gDirty) / sizeof(BtU32);

			// Check to see if our resource type has been dirtied
			for( BtU32 iIndex=0; iIndex<nDirtyFlagCount; iIndex++ )
			{
				if( ( pAsset->Type() == gDirty[iIndex] ) == BtTrue )
				{
					pAsset->m_bExport = BtTrue;
				}
			}

			if( lastResourceModifiedTime > pAsset->m_lastExportTime )
			{
				pAsset->m_bExport = BtTrue;
			}

			if( pAsset->m_bExport == BtTrue )
			{
				m_nToExport++;
			}
		}
	}
	ErrorLog::Printf( "Found %d assets for export.\n", m_nToExport);
}

////////////////////////////////////////////////////////////////////////////////
// Compile resources

void PaPacker::CompileResources()
{
	m_hasExported = BtFalse;

	// Cache the exporter
	ExExporter& exporter = ExExporter::Instance();

	// Loop through the resources
	for( BtU32 j=0; j<m_pArchive->GetNumAssets(); j++ )
	{
		// Cache each asset
		PaAsset* pAsset = m_pArchive->GetAsset(j);

		if( ( pAsset->m_bExport == BtTrue ) && ( pAsset->m_bDLL == BtFalse ) )
		{	
			// Compile each resource
			exporter.compileResource( pAsset );

			// Flag the resource as exported
			pAsset->bExported( BtTrue );

			// Flag that we have exported
			m_hasExported = BtTrue;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// LoadResource

void PaPacker::LoadResourceHeaders()
{
	// Cache the resource count
	BtU32 nResources = (BtU32) m_pArchive->GetNumResources();

	// Loop through the resources
	for( BtU32 i=0; i<nResources; i++ )
	{
		// Cache each resource
		PaResource* pResource = m_pArchive->GetResource(i);

		FsFile resourceFile;

		// Cache the compiled filename
		BtChar *compiledFilename = pResource->compiledFilename();

		// Open the resource container file
		resourceFile.Open(compiledFilename, FsMode_Read );

		// Read the resource data
		resourceFile.Read( (BtU8*) pResource->GetHeader(), sizeof( BaResourceHeader ) );

		// Close the container file
		resourceFile.Close();
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetDataOffsets

void PaPacker::SetDataOffsets()
{
	// Set the first offset
	BtU32 nResourceOffset = sizeof( BaArchiveHeader );

	// Cache the resource count
	BtU32 nResources = (BtU32) m_pArchive->GetNumResources();

	// Add space for all the resource headers
	nResourceOffset += nResources * sizeof( BaResourceHeader );

	// Loop through the resources
	for( BtU32 i=0; i<nResources; i++ )
	{
		// Cache each resource
		PaResource* pResource = m_pArchive->GetResource(i);

		// Set the offset into the archive
		pResource->GetHeader()->m_nResourceOffset = nResourceOffset;

		// Add space for the resource
		nResourceOffset += pResource->GetHeader()->m_nResourceSize;
	}
	
	// Set the size
	m_pArchive->m_archiveSize = nResourceOffset;

	// Set the total instance size
	m_pArchive->m_archiveHeader.m_nDataSize = m_pArchive->m_archiveSize;
}

////////////////////////////////////////////////////////////////////////////////
// GetFileTime

BtU64 PaPacker::GetFileTime()
{
#ifdef WIN32
    FILETIME ft;
    SYSTEMTIME st;
	BtU64 time;

    GetSystemTime(&st);              // gets current time
    SystemTimeToFileTime(&st, &ft);  // converts to file time format
	
	memcpy( &time, &ft, sizeof(BtU64) );

	return (BtU64) time;
#else
    time_t  timev;
    time(&timev);
    
    return timev;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Pack

void PaPacker::Pack()
{
	// Find the archives
	FindArchivesAndFiles();

	// Load the global id
	LoadGlobalID();

	// Build the list of resource
	BuildAssetList();

	// Load the archive build times
	LoadArchiveBuildTimes();

	// Load the resource build times
	ReadAssetBuildTimes();

	// Read the resource list
	ReadResourceList();

	// Decide which files should be exported
	FlagForExport();

	if( BtTrue )	// We can check whether to export but we can't alter the structure if we do that.
	{
		// Loop through the archives
		for (BtU32 i = 0; i < m_archiveCount; i++)
		{
			// Cache each archive
			m_pArchive = m_archives[i];

			// Compile the resources
			CompileResources();

			// Load the resource headers
			LoadResourceHeaders();

			// Setup the data offsets
			SetDataOffsets();

			// Write out the archive
			m_pArchive->WriteArchive();

			// Write the resource sizes
			m_pArchive->WriteResourceSizes();

			// Deflate each archive using zlib
			DeflateArchive();
		}
	}

	// Write the build times
	WriteArchiveBuildTimes();

	// Write the resource build times
	WriteAssetBuildTimes();

	// Write the resources
	WriteResourceList();

	//if( m_hasExported )
	{
		// Write the global id
		WriteGlobalID();

		// Delete the archives
		DeleteArchives();
	}
}

////////////////////////////////////////////////////////////////////////////////
// DeflateArchive

void PaPacker::DeflateArchive()
{
    BtS32 ret, flush;
    BtU32 have;
    z_stream strm;
	const BtS32 CHUNK = 256 * 1024;
    BtU8 in[CHUNK];
    BtU8 out[CHUNK];

	// Open the source file
	FILE* hSource;
	
	hSource = fopen( m_pArchive->m_szArchiveFilename, "rb" );
	
	fseek(hSource, 0, SEEK_END);
	BtU32 archiveSize = ftell(hSource);
	fseek(hSource, 0, SEEK_SET);

	ErrorLog::Printf("Compressing %s from %d\n", m_pArchive->m_szArchiveFilename, archiveSize);

	// Open the destination file
	FILE* destStream;
	
	destStream = fopen( m_pArchive->m_szPackedFilename, "wb" );

	// Setup the archive header
	BaArchiveHeader archiveHeader;
	archiveHeader.m_nNumResources = m_pArchive->GetNumResources();
	archiveHeader.m_nDataSize = m_pArchive->m_archiveSize;
	BtStrCopy( archiveHeader.m_szTitle, m_pArchive->m_archiveHeader.m_szTitle );
	archiveHeader.m_nPackerVersion = PACKER_VERSION;
	archiveHeader.m_nHeaderCheckSum = BtCRC::GenerateHashCode((BtU8*)&archiveHeader, sizeof(BaArchiveHeader) - sizeof(BtU32));
	fwrite( (void*)&archiveHeader, 1, sizeof(BaArchiveHeader), destStream );

    // Allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit( &strm, 1 );

    if( ret != Z_OK )
	{
		ErrorLog::Fatal_Printf( "deflateInit %d\n", ret );
	}

    /* compress until end of file */
    do {
        strm.avail_in = (BtU32) fread( in, 1, CHUNK, hSource );
        
		if( ferror( hSource ) )
		{
            (void)deflateEnd(&strm);

			ErrorLog::Fatal_Printf( "ferror\n" );
        }

        flush = feof( hSource ) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            
			if (fwrite(out, 1, have, destStream ) != have || ferror(destStream) )
			{
                (void)deflateEnd(&strm);
			
				ErrorLog::Fatal_Printf( "ferror\n" );
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);

    BtAssert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);

	fclose( hSource );
	fclose( destStream );
}

////////////////////////////////////////////////////////////////////////////////
// WriteArchiveBuildTimes

void PaPacker::WriteArchiveBuildTimes()
{
	PaMemoryResource archiveBuildTimes( PMR_Write );

	// Add an integer
	archiveBuildTimes << m_archiveCount;

	// Loop through the file list and create 
	for( BtU32 i=0; i<m_archiveCount; i++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[i];

		// Cache each entry
		ExScheduleItem scheduleItem;

		// Set each archive's details
		BtStrCopy( scheduleItem.m_szFilename, MaxFilePath, pArchive->m_file.m_szFilename );

		// Set the time stamp
		scheduleItem.m_tLastExport = pArchive->m_file.m_tResourceChanged;

		// Add this data
		archiveBuildTimes << scheduleItem;
	}

	archiveBuildTimes.Save( GetArchiveBuildListName() );
}

////////////////////////////////////////////////////////////////////////////////
// WriteAssetBuildTimes

void PaPacker::WriteAssetBuildTimes()
{
	PaMemoryResource AssetBuildTimes1(PMR_Write );
	PaMemoryResource AssetBuildTimes2(PMR_Write );

	BtU32 nResources = 0;

	ExScheduleItem scheduleItem;

	ErrorLog::Printf( "AssetBuildTimes\n" );

	// Loop through the file list and create a schedule item per resource
	for( BtU32 i=0; i<m_archiveCount; i++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[i];

		// Display the archive name
		ErrorLog::Printf( " Archive %s\n", pArchive->pTitle() );

		// Loop through the file list and create 
		for (BtU32 j = 0; j < pArchive->GetNumAssets(); j++)
		{
			// Cache each asset
			PaAsset* pAsset = pArchive->GetAsset(j);

			// Set each archive's details
			BtStrCopy(scheduleItem.m_szFilename, MaxFilePath, pAsset->m_file.m_szFilename);

			// Set the time stamp
            BtU64 fileTime = GetFileTime();
			scheduleItem.m_tLastExport = fileTime;

			// Output the schedule item
			AssetBuildTimes2 << scheduleItem;

			// Cache the number of dependencies
			BtU32 nItems = pAsset->m_dependencies.nItems();

			if (0)
			{
				// Display the dependency name
				for (BtU32 iDependency = 0; iDependency < nItems; iDependency++)
				{
					// Display the archive name
					ErrorLog::Printf(" Dependencies %s\n", pAsset->m_dependencies[iDependency].GetFilename());
				}
			}

			// Serialise the dependencies
			pAsset->m_dependencies.Write( AssetBuildTimes2 );

			// Increment the number of resources
			nResources++;
		}
	}

	// Write the file. TODO
	AssetBuildTimes1 << nResources;
	AssetBuildTimes1.Save( GetAssetBuildListName() );
	AssetBuildTimes2.Append( GetAssetBuildListName() );
}

////////////////////////////////////////////////////////////////////////////////
// WriteResourceList

void PaPacker::WriteResourceList()
{
	BtChar *resourceListName = GetResourceBuildListName();

	PaMemoryResource ResourceList( PMR_Write );

	// Loop through the file list and create a schedule item per resource
	for( BtU32 i=0; i<m_archiveCount; i++ )
	{
		// Cache each archive
		PaArchive* pArchive = m_archives[i];

		// Cache the number of resources
		BtU32 nResources = pArchive->GetNumResources();

		for( BtU32 j=0; j<nResources; j++ )
		{
			// Cache each resource
			PaResource* pResource = pArchive->GetResource( j );

			// Save each resource
			ResourceList << *pResource;
		}
	}

	// Write the file
	ResourceList.Save( resourceListName );
}

////////////////////////////////////////////////////////////////////////////////
// WriteGlobalID

void PaPacker::WriteGlobalID()
{
    char FS = FsFile::GetFileSeparator();
    BtChar szFilename[256];
    sprintf( szFilename, "%s%cBuildFiles%cglobalID.dat", PaTopState::pWorkingPath(), FS, FS );
    
	PaMemoryResource globalID( PMR_Write );
	globalID << m_nGlobalID;
	globalID.Save( szFilename );
}

////////////////////////////////////////////////////////////////////////////////
// DeleteArchives

void PaPacker::DeleteArchives()
{
	// Delete the archives
	for( BtU32 nArchiveIndex=0; nArchiveIndex<m_archiveCount; nArchiveIndex++ )
	{
		delete m_archives[nArchiveIndex];
	}
}

BtU32 RsColour::asWord() const
{
	if( PaTopState::Instance().GetPlatform() == PackerPlatform_WinDX )
	{
		return asABGR();
	}
	return asRGBA();
}
