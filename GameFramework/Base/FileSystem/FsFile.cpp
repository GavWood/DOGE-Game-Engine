////////////////////////////////////////////////////////////////////////////////
// FsFile.cpp

#include "FsFile.h"
#include "ErrorLog.h"
#include "BtBase.h"
#include "ApConfig.h"
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <stdarg.h>
#include <stdio.h>
#include "BtString.h"
#include "BtBase.h"

#ifdef WIN32
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// Constructor

FsFile::FsFile()
{
	m_fileHandle = BtNull;
	m_bOpened = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// CreateFolder

BtBool FsFile::CreateFolder(const BtChar *filename)
{
#ifdef WIN32
	_mkdir(filename);
#else
    mkdir(filename, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetFileSeparator

BtChar FsFile::GetFileSeparator()
{
#ifdef WIN32
    return '\\';
#else
    return '/';
#endif
}

////////////////////////////////////////////////////////////////////////////////
// LoadString

BtU32 FsFile::LoadString( BtChar* filename, BtChar *contents )
{
	FsFile file;
	file.Open(filename, FsMode_Read);
	if( file.IsOpen() )
	{
		BtU32 fileSize = file.GetSize();
		BtAssert(fileSize < 1024 * 32);
		file.Read((BtU8*)contents, fileSize);
		file.Close();
		*(contents + fileSize) = '\0';
		return fileSize;
	}
	else
	{
		ErrorLog::Printf( "Could not open %s\r\n", filename );
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Open a file

void FsFile::Open( BtChar* pFilename, FsMode mode )
{
	m_mode = mode;

	if( mode == FsMode_Read )
	{
		m_fileHandle = fopen( pFilename, "rb" );
	}
	else if( mode == FsMode_Write )
	{
		m_fileHandle = fopen( pFilename, "wb" );
	}
	else if( mode == FsMode_WriteAscii )
	{
		m_fileHandle = fopen( pFilename, "w" );
	}
	else if( mode == FsMode_Append )
	{
		m_fileHandle = fopen( pFilename, "ab" );
	}

	if( m_fileHandle == BtNull )
	{
		m_bOpened = BtFalse;
	}
	else
	{
		m_bOpened = BtTrue;
	}
}

////////////////////////////////////////////////////////////////////////////////
// bOpened

BtBool FsFile::IsOpen()
{
	return m_bOpened;
}

////////////////////////////////////////////////////////////////////////////////
// Read data to a file

BtU32 FsFile::Read( BtU8* pMemory, BtU32 nSize )
{
	if( m_bOpened == BtFalse )
	{
		//LgErrorLog::Fatal_Printf( "FsFile: Read failed as no file is open." );
	}

	BtAssert( m_mode == FsMode_Read );

	// Read the bytes from the file
	BtU32 nBytesRead = (BtU32)fread( pMemory, 1, nSize, m_fileHandle );

	// Return the number of bytes read
	return nBytesRead;
}

////////////////////////////////////////////////////////////////////////////////
// Write string to a file

void FsFile::Write(const char *pFormat, ...)
{
	if(m_bOpened == BtFalse)
	{
		//LgErrorLog::Fatal_Printf( "FsFile: Write failed as no file is open." );
	}
	else
	{
		BtChar buffer[256];
		va_list marker;
		va_start(marker, pFormat);
		vsprintf(buffer, pFormat, marker);
		va_end(marker);

		fprintf( m_fileHandle, "%s", buffer );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Write data to a file

void FsFile::Write( BtU8* pMemory, BtU32 nSize )
{
	if( m_bOpened == BtFalse )
	{
		//LgErrorLog::Fatal_Printf( "FsFile: Write failed as no file is open." );
	}
    else
    {
        fwrite( pMemory, 1, nSize, m_fileHandle );
    }
}

////////////////////////////////////////////////////////////////////////////////
// GetSize

BtU32 FsFile::GetSize()
{
	if( m_bOpened == BtTrue )
	{
		BtU32 nCurrentPosition = GetPosition();

		fseek( m_fileHandle, 0, SEEK_END );

		BtU32 nEndPosition = GetPosition();

		fseek( m_fileHandle, nCurrentPosition, SEEK_SET );

		return nEndPosition;
	}
	else
	{
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Skip

void FsFile::Skip( BtU32 nBytes )
{
	fseek( m_fileHandle, nBytes, SEEK_CUR );
}

////////////////////////////////////////////////////////////////////////////////
// Position

BtU32 FsFile::GetPosition()
{
	return (BtU32)ftell( m_fileHandle );
}

////////////////////////////////////////////////////////////////////////////////
// Close a file

void FsFile::Close()
{
	if( m_bOpened == BtTrue )
	{
		// Close the handle
		fclose(m_fileHandle);

		// Flag the file as closed
		m_bOpened = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Close a file

BtBool FsFile::IsEndOfFile()
{
	if( m_bOpened == BtFalse )
	{
		return BtFalse;
	}

	if( feof( m_fileHandle) )
	{
		return BtTrue;
	}
	return BtFalse;
}
