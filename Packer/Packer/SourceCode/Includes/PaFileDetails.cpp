////////////////////////////////////////////////////////////////////////////////
// PaFileDetails.cpp

#include <string.h>
#include <stdio.h>
#include "PaFileDetails.h"
#include "FsFile.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaFileDetails::PaFileDetails( )
{
}

PaFileDetails::PaFileDetails( const BtChar* pPath, const BtChar* pName )
{
    char FS = FsFile::GetFileSeparator();
    
	// Set the name
	BtStrCopy( m_szName, MaxFilePath, pName );

	// Copy the pathname
	BtStrCopy( m_szPath, MaxFilePath, pPath );

	// Construct the filename
	if( strlen( m_szPath ) )
	{
		sprintf( m_szFilename, "%s%c%s", m_szPath, FS, m_szName );

		// Construct the folder name
		BtChar *szFolder = strrchr( m_szPath, FS );

		if( szFolder )
		{
			sprintf( m_szFolder, "%s", szFolder + 1 );
		}
		else
		{
			BtStrCopy( m_szFolder, MaxFilePath, m_szPath );
		}
	}
	else
	{
		sprintf( m_szFilename, "%s", m_szName );
		BtStrCopy( m_szFolder, MaxFilePath, "" );
	}

	// Find the extension
	char *szExtension = strrchr( m_szName, '.' ) ;

	// Copy the extension
	if( szExtension )
	{
		BtStrCopy( m_szExtension, MaxFilePath, szExtension + 1 );

		// Copy up to the extension
		BtU32 nLength = (BtU32) strlen(m_szName) - (BtU32) strlen(szExtension);
		BtStrCopy( m_szTitle, MaxFilePath, m_szName, nLength );
		*(m_szTitle + nLength) = 0;
	}
	else
	{
		BtStrCopy( m_szExtension, MaxFilePath, "" );
		BtStrCopy( m_szTitle, MaxFilePath, m_szName );
	}
	
	// Set the extension to lower case
	BtStrLower( m_szExtension, MaxFilePath );
	BtStrLower( m_szName, MaxFilePath );
	BtStrLower( m_szTitle, MaxFilePath );
	BtStrLower( m_szFilename, MaxFilePath );
	BtStrLower( m_szPath, MaxFilePath );
	BtStrLower( m_szFolder, MaxFilePath );

	m_bFolder = BtFalse;
	m_tResourceChanged = 0;
}
