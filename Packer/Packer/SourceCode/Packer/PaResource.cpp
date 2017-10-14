////////////////////////////////////////////////////////////////////////////////
// Exporter

// Includes
#include <stdio.h>
#include <string.h>
#include "BtMemory.h"
#include "ExExporter.h"
#include "PaResource.h"
#include "PaAsset.h"
#include "PaTopState.h"
#include "FsFile.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaResource::PaResource()
{
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaResource::PaResource( PaAsset* pAsset,
						const BtChar* pFilename,
						const BtChar* pTitle,
					    BaResourceType eType )
{
    BtChar FSBuf[8];
    sprintf( FSBuf, "%c", FsFile::GetFileSeparator() );
    
	// Set the output binary filename
	BtChar szBuffer1[MaxFilePath];
	BtChar szFilename[MaxFilePath];
	BtStrCopy( szBuffer1, MaxFilePath, pFilename );
	BtStringReplace( szBuffer1, szFilename, MaxFilePath, FSBuf, "_" );

	BtChar szPath[MaxFilePath];

	switch( PaTopState::Instance().GetPlatform() )
	{
	case PackerPlatform_WinDX:
		sprintf( szPath, "Win32" );
		break;

	case PackerPlatform_GLES:
		sprintf( szPath, "iPhone" );
		break;

	case PackerPlatform_WinGL:
		sprintf( szPath, "Win32GL" );
		break;

	case PackerPlatform_OSX:
		sprintf(szPath, "osx");
		break;
            
    case PackerPlatform_MAX:
        ErrorLog::Fatal_Printf("Could not pack unknown platform.");
	}

	// Make the compiled filename
    //sprintf( m_compiledFilename, "%s\\BuildFiles\\%s\\%s", PaTopState::pWorkingPath(), szPath, szFilename );

    BtChar FS = FsFile::GetFileSeparator();
	sprintf( m_compiledFilename, "%s%c%s%c%s%c%s", PaTopState::pWorkingPath(), FS, "BuildFiles", FS, szPath, FS, szFilename );

	// Copy the uncompiled filename
	BtStrCopy( m_uncompiledFilename, MaxFilePath, pFilename );
	BtStrCopy( m_resourceHeader.m_szTitle, LMaxResourceName, pTitle );
	m_resourceHeader.m_type = eType;
	m_resourceHeader.m_nInstanceSize = 0;
	m_resourceHeader.m_nInstanceAlignment = 0;
	m_resourceHeader.m_nResourceAlignment = 0;
	m_resourceHeader.m_resourceFlags = 0;

	// Set the archive name in the resource
	m_pArchive = pAsset->pArchive();

	BtStrCopy( m_archive, MaxFilePath, m_pArchive->m_file.m_szTitle );
}
