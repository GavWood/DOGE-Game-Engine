////////////////////////////////////////////////////////////////////////////////
// UtFilename.cpp

#include "BtString.h"
#include "UtFilename.h"
#include "FsFile.h"
#include "stdio.h"
#include "string.h"

////////////////////////////////////////////////////////////////////////////////
// UtFilename

UtFilename::UtFilename( BtChar* filename )
{
	// Empty the strings
	BtStrEmpty( m_pathname, 256 );
	BtStrEmpty( m_extension, 256 );
	BtStrEmpty( m_filename, 256 );
	BtStrEmpty( m_title, 256 );
	BtChar copy[256];

	BtStrCopy( copy, 256, filename, 256 );
	BtStrCopy( m_filename, 256, copy, 256 );

	// Find the filename
    BtChar* pFilename = strrchr( copy, FsFile::GetFileSeparator() );

	if( pFilename != BtNull )
	{
		BtStrCopy( m_title, 256, pFilename + 1, 256 );
		
		*pFilename = '\0';
		BtStrCopy( m_pathname, 256, copy );
	}
	else
	{
		BtStrCopy( m_title, 256, copy, 256 );
		BtStrEmpty( m_pathname, 256 );
	}

	// Find the extension
	BtChar* pExtension = strrchr( m_title, '.' );

	if( pExtension != BtNull )
	{
		BtStrCopy( m_extension, 256, pExtension + 1, 256 );
		*pExtension = 0;
	}
}
