////////////////////////////////////////////////////////////////////////////////
// PaTopState.cpp

//#include <conio.h>
#include <stdio.h>
#include "BtString.h"
#include "BtTime.h"
#include "BtTypes.h"
#include "BtBase.h"
#include "FsFindFile.h"
#include "BaArchive.h"
#include "PaPacker.h"
#include "PaTopState.h"
#include "ExExporter.h"
#include "ErrorLog.h"
#include "ApConfig.h"
#include "FsFile.h"
#include "PaRendering.h"
#include "FCollada.h"
#include "UtXML.h"

#ifdef WIN32
#include "shlwapi.h"
#include <tchar.h>
#endif

PaPacker packer;
PackerPlatform g_platform = PackerPlatform_MAX;
PackerPlatform PaTopState::m_platform = PackerPlatform_MAX;
const BtChar *g_path;

////////////////////////////////////////////////////////////////////////////////
// BtStringReplace

BtChar* BtStringReplace( const BtChar* szSource,
						BtChar* szDest,
						BtU32 nDestSize,
						const BtChar* szSearchString,
						const BtChar *szReplaceString )
{
	BtMemory::Set( szDest, 0, nDestSize );

	BtChar* pDest = szDest;

	const BtChar* pCurrent = szSource;

	while( pCurrent != BtNull )
	{
		const BtChar* pStart = strstr( pCurrent, szSearchString );

		if( pStart )
		{
			BtU32 nLength = (BtU32) ( pStart - pCurrent );
			if( nLength > 0 )
			{
				BtU32 nNewDestSize = szDest + nDestSize - pDest;
				BtStrCopy( pDest, nNewDestSize, pCurrent, nLength );
				pDest+= nLength;
			}
			BtU32 nNewDestSize = szDest + nDestSize - pDest;
			BtStrCopy( pDest, nNewDestSize, szReplaceString );
			pDest+=strlen(szReplaceString);
			pCurrent = pStart + strlen( szSearchString );
		}
		else
		{
			BtU32 nNewDestSize = szDest + nDestSize - pDest;
			BtStrCopy( pDest, nNewDestSize, pCurrent );
			pCurrent = BtNull;
		}
	}

	return szDest;
}


////////////////////////////////////////////////////////////////////////////////
// BtStrTrimNewLines

void BtStrTrimSpaces( BtChar* szSource, BtU32 nSize )
{
	BtU32 length = BtStrLength( szSource );

	for( BtS32 i=length - 1; i >= 0; i-- )
	{
		BtChar& myChar = szSource[i];

		switch( myChar )
		{
		case ' ' :
		case '\n' :
		case '\r' :
		case '\0' :
		case '\t' :
			myChar = '\0';
			break;

		default:
			i = -1;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// BtStrTrimNewLines

void BtStrTrimNewLines( BtChar* szSource, BtU32 nSize )
{
	BtChar* pSource;

	pSource = strchr( szSource, '\n' );

	if( pSource )
	{
		*pSource = '\0';
	}

	pSource = strchr( szSource, '\r' );

	if( pSource )
	{
		*pSource = '\0';
	}
}

// Defines
PaTopState s_GameState;
BtChar PaTopState::m_szWorkingPath[256];

////////////////////////////////////////////////////////////////////////////////
// Constructor

PaTopState::PaTopState()
{
}

////////////////////////////////////////////////////////////////////////////////
// CreateFolders

void PaTopState::CreateFolders()
{
    char FS = FsFile::GetFileSeparator();
	BtChar szFilename[256];
	sprintf( szFilename, "%s%cBuildFiles", m_szWorkingPath, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildFiles%ciPhone", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildFiles%cWin32", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildFiles%cOSX", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder(szFilename);
	sprintf( szFilename, "%s%cBuildFiles%cWin32GL", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cRelease", m_szWorkingPath, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildNotes", m_szWorkingPath, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildNotes%ciPhone", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildNotes%cWin32", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder( szFilename );
	sprintf( szFilename, "%s%cBuildNotes%cWin32GL", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder( szFilename );
	sprintf(szFilename, "%s%cBuildNotes%cOSX", m_szWorkingPath, FS, FS );
	FsFile::CreateFolder(szFilename);
}

////////////////////////////////////////////////////////////////////////////////
// ReadConfig

BtBool PaTopState::ReadConfig()
{		
	UtXML xml;
	xmlNode *cur;

#ifdef WIN32
	cur = xml.Open("\\github\\DOGE-Game-Engine\\Packer\\config.xml", "config" );
#else
	cur = xml.Open("/Users/localadmin/Documents/DOGE-Game-Engine/Packer/config.xml", "config");
#endif
	if (!cur)
	{
		cur = xml.Open("config.xml", "config");
	}

	BtU32 projectId  = xml.GetUintFromNode(cur, "project");
	m_platform = (PackerPlatform)xml.GetUintFromNode(cur, "platform");

	xmlNode *pID = xml.FindXMLNodeByUint(cur, "id", projectId);
	BtAssert(projectId == xml.GetUintFromChildNode(pID));

	xmlNode *pPath = xml.FindXMLNode(pID->parent, "path" );

	if (pPath)
	{
		BtStrCopy(m_szWorkingPath, xml.GetStringFromChildNode(pPath ) );
        ErrorLog::Printf( "Setting working path %s", m_szWorkingPath );
	}
	else
	{
		BtStrCopy(m_szWorkingPath, 256, ".");
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Create

void PaTopState::Create()
{
	FCollada::Initialize();
    
	// Get the project setting from the configuration file
	if( g_platform == PackerPlatform_MAX )
	{
		ReadConfig();
	}
	else
	{
		ErrorLog::Printf( "Platform from command line is %d.\r\n", g_platform );
		strcpy( m_szWorkingPath, g_path );
		m_platform = g_platform;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void PaTopState::Update()
{
	ErrorLog::Printf( "Gamedata path is %s\r\n", m_szWorkingPath );
#ifdef WIN32
    char buffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buffer);
    ErrorLog::Printf( "Working path is %s\r\n", buffer );
    ErrorLog::Printf( "Together: %s\\%s.\r\n", buffer, m_szWorkingPath);
#endif
    
	switch( m_platform )
	{
		case PackerPlatform_WinDX:
			ErrorLog::Printf( "Platform is Win32.\r\n" );
			break;

		case PackerPlatform_OSX:
			ErrorLog::Printf("Platform is OSX.\r\n");
			break;

		case PackerPlatform_GLES:
			ErrorLog::Printf( "Platform is iPhone.\r\n" );
			break;

		case PackerPlatform_WinGL:
			ErrorLog::Printf( "Platform is Win32GL.\r\n" );
			break;
            
        default:
            break;
	}

	// Create the output folder
	CreateFolders();

	// Pack the resources
	packer.Pack();
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void PaTopState::Destroy()
{
#ifdef WIN32
	FCollada::Release();
#endif
}

////////////////////////////////////////////////////////////////////////////////
// pWorkingPath

// static
const BtChar* PaTopState::pWorkingPath()
{
	return m_szWorkingPath;
}

////////////////////////////////////////////////////////////////////////////////
// HookIntoMain

void PaTopState::HookIntoMain( const char *path, int platformIndex )
{   
    // Now pack it
    g_platform = PackerPlatform_MAX;
	g_path = BtNull;

	if( path != BtNull )
	{
		g_platform = (PackerPlatform)platformIndex;
		g_path = path;

		//ErrorLog::Printf( "Setting platform and folder from command line is %s %s.\n", argv[1], argv[2] );
	}

    BtChar FS = FsFile::GetFileSeparator();
    BtChar filename[MaxPath];
    sprintf(filename, "%ctemp%cpacker.txt", FS, FS );
	ErrorLog::Filename( filename );
	ErrorLog::Create();
	s_GameState.Create();
	s_GameState.Update();
	s_GameState.Destroy();

	// Cache the day
	BtU32 nDay = BtTime::GetDay();

	// Cache the month
	BtU32 nMonth = BtTime::GetMonth();

	// Cache the year
	BtU32 nYear  = BtTime::GetYear();

	ErrorLog::Printf( "%s packed %s on %d-%d-%d.\n",
			PaTopState::GetPlatformName(), PaTopState::pWorkingPath(), nDay, nMonth, nYear );
}

////////////////////////////////////////////////////////////////////////////////
// GetPlatformName

//static
const BtChar *PaTopState::GetPlatformName()
{
	switch( m_platform )
	{
	case PackerPlatform_GLES:
		return "GLES";

	case PackerPlatform_OSX:
		return "OSX";

	case PackerPlatform_WinDX:
		return "WinDX";

	case PackerPlatform_WinGL:
		return "WinGL";

	default:
		return "Unknown";
	}
	return "Unknown";
}

PackerPlatform PaTopState::GetPlatform()
{
	return m_platform;
}

BtBool PaTopState::IsBaseVertex() const
{
	if( ( m_platform == PackerPlatform_WinGL ) || ( m_platform == PackerPlatform_GLES ) || ( m_platform == PackerPlatform_OSX ) )
	{
		return BtTrue;
	}
	return BtFalse;
}

BtBool PaTopState::IsVertexBase2() const
{
	if( ( m_platform == PackerPlatform_WinGL ) || ( m_platform == PackerPlatform_GLES ) || ( m_platform == PackerPlatform_OSX ) )
	{
		return BtTrue;
	}
	return BtFalse;
}
