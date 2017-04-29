////////////////////////////////////////////////////////////////////////////////
// ErrorLog.cpp

// Includes
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <stdarg.h>
#include <stdio.h>
#include "BtString.h"
#include "BtBase.h"

#ifdef _WIN32
#include <process.h>
#include <windows.h>
#endif

#include "ErrorLog.h"

const BtU32 MaxErrorBuffer = 1024;

// Private members
BtChar	m_szFilename[256];
FILE*	m_handle;

BtChar buffer1[MaxErrorBuffer];
BtChar buffer2[MaxErrorBuffer];

////////////////////////////////////////////////////////////////////////////////
// Filename

void ErrorLog::Filename( const BtChar* pFilename )
{
	BtStrCopy( m_szFilename, 256, pFilename );
}

////////////////////////////////////////////////////////////////////////////////
// Create

void ErrorLog::Create()
{
	FILE* f;
	f = fopen( m_szFilename, "w" );
	if( f )
	{
		fclose(f);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Printf

void ErrorLog::Printf( const BtChar* pFormat, ... )
{
	va_list marker;
	va_start( marker, pFormat );
	vsprintf( buffer2, pFormat, marker );
	Print( buffer2 );
	va_end ( marker );
}

////////////////////////////////////////////////////////////////////////////////
// Fatal_Printf

void ErrorLog::Fatal_Printf( const BtChar* pFormat, ... )
{
	va_list marker;
	va_start( marker, pFormat );
	vsprintf( buffer1, pFormat, marker );
	Print( buffer1 );
	va_end ( marker );
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////
// Print

void ErrorLog::Print( const BtChar* pString )
{
#ifdef _WIN32
	OutputDebugString(pString);

	FILE* f;
	f = fopen(m_szFilename, "a+");
	if(f)
	{
		fprintf( f, pString );
		fclose(f);
	}
#endif
	printf( "%s\n", pString );
}
