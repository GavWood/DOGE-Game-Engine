////////////////////////////////////////////////////////////////////////////////
// ApConfig.cpp

// Includes
#include "ApConfig.h"
#include "BtString.h"
#include "BtPrint.h"
#include "ErrorLog.h"
#include "stdio.h"
#include "stdlib.h"

////////////////////////////////////////////////////////////////////////////////
// Statics
ApConfig config;
ApPlatform ApConfig::m_platform = ApPlatform_Max;
BtChar ApConfig::m_resourcePath[256];
BtChar ApConfig::m_extension[256];
BtChar ApConfig::m_documents[256];
BtChar ApConfig::m_title[256];
ApDevice ApConfig::m_device = ApDevice_Max;
BtBool ApConfig::m_isDebug = BtFalse;
BtBool ApConfig::m_isCheatMode = BtFalse;
BtBool ApConfig::m_isSimulator = BtFalse;
BtBool ApConfig::m_isDebugBuild = BtFalse;
BtBool ApConfig::m_isPaused = BtFalse;
BtBool ApConfig::m_isAR = BtFalse;
BtBool ApConfig::m_isInitialised = BtFalse;
BtU32 ApConfig::m_instances = 0;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ApConfig::ApConfig()
{
	BtStrCopy( m_documents, 256, "" );
    BtStrCopy( m_resourcePath, 256, "" );
	BtStrCopy( m_title, 256, "blank" );
    m_isSimulator = BtFalse;

#ifdef _DEBUG
	m_isDebugBuild = BtTrue;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// Init

void ApConfig::Init()
{
	m_isInitialised = BtTrue;

    ApDevice device = ApConfig::GetDevice();
    
	// Load the game archive
	if( device == ApDevice_WIN )
	{
		if (ApConfig::GetPlatform() == ApPlatform_DX11)
		{
			ApConfig::SetExtension(".windxz");
		}
		else
		{
			ApConfig::SetExtension(".winglz");
		}
	}
	else if( device == ApDevice_iPhone)
	{
		ApConfig::SetExtension(".iPhonez");
	}
    else if( device == ApDevice_OSX )
    {
        ApConfig::SetExtension(".OSXz");
    }
}

////////////////////////////////////////////////////////////////////////////////
// IsSimulator

BtBool ApConfig::IsSimulator()
{
    return m_isSimulator;
}

////////////////////////////////////////////////////////////////////////////////
// SetSimulator

void ApConfig::SetSimulator( BtBool isSimulator )
{
    m_isSimulator = isSimulator;
}

////////////////////////////////////////////////////////////////////////////////
// GetInstance

BtU32 ApConfig::GetNumInstances()
{
	return m_instances;
}

////////////////////////////////////////////////////////////////////////////////
// SetInstance

void ApConfig::SetInstance(BtU32 numInstances)
{
	m_instances = numInstances;
}

////////////////////////////////////////////////////////////////////////////////
// IsInitialised

BtBool ApConfig::IsInitialised()
{
	return m_isInitialised;
}

////////////////////////////////////////////////////////////////////////////////
// IsCheatMode

BtBool ApConfig::IsCheatMode()
{
	return m_isCheatMode;
}

////////////////////////////////////////////////////////////////////////////////
// SetCheatMode

void ApConfig::SetCheatMode( BtBool isCheatMode )
{
	m_isCheatMode = isCheatMode;
}

////////////////////////////////////////////////////////////////////////////////
// IsDebugBuild

BtBool ApConfig::IsDebugBuild()
{
	return m_isDebugBuild;
}

////////////////////////////////////////////////////////////////////////////////
// SetDebugBuild

void ApConfig::SetDebugBuild(BtBool isDebug)
{
	m_isDebugBuild = isDebug;
}

////////////////////////////////////////////////////////////////////////////////
// IsDebug

BtBool ApConfig::IsDebug()
{
    return m_isDebug;
}

////////////////////////////////////////////////////////////////////////////////
// SetDebug

void ApConfig::SetDebug( BtBool isDebug )
{
    m_isDebug = isDebug;
}

////////////////////////////////////////////////////////////////////////////////
// IsAR

BtBool ApConfig::IsAR()
{
    return m_isAR;
}

////////////////////////////////////////////////////////////////////////////////
// SetAR

void ApConfig::SetAR(BtBool isAR)
{
    m_isAR = isAR;
}

////////////////////////////////////////////////////////////////////////////////
// IsPaused

BtBool ApConfig::IsPaused()
{
	return m_isPaused;
}

////////////////////////////////////////////////////////////////////////////////
// SetPaused

void ApConfig::SetPaused(BtBool isPaused)
{
	m_isPaused = isPaused;
}

////////////////////////////////////////////////////////////////////////////////
// SetTitle

void ApConfig::SetTitle( const BtChar* title )
{
	BtStrCopy(m_title, sizeof(m_title), title);
}

////////////////////////////////////////////////////////////////////////////////
// GetTitle

const BtChar *ApConfig::GetTitle()
{
	return m_title;
}

////////////////////////////////////////////////////////////////////////////////
// GetPlatform

//static
ApPlatform ApConfig::GetPlatform()
{
	return m_platform;
}

////////////////////////////////////////////////////////////////////////////////
// GetPlatformName

//static
const BtChar *ApConfig::GetPlatformName()
{
	switch( m_platform )
	{
		case ApPlatform_GLES:
			return "GLES";

		case ApPlatform_DX11:
			return "WinDX";

		case ApPlatform_WinGL:
			return "WinGL";
            
        default:
            return "Platform not set";
	}
	return "Platform not set";
}

////////////////////////////////////////////////////////////////////////////////
// SetOrientation

//static
void ApConfig::SetPlatform( ApPlatform platform )
{
	m_platform = platform;
}

///////////////////////////////////////////////////////////////////////////////
// SetExtension

void ApConfig::SetExtension( const BtChar* pExtension )
{
	BtStrCopy( m_extension, 256, pExtension );
}

///////////////////////////////////////////////////////////////////////////////
// SetResourcePath

void ApConfig::SetResourcePath( const BtChar* pResourcePath )
{
	BtStrCopy( m_resourcePath, 256, pResourcePath );
    BtPrint( "Resource path set to: %s\n", m_resourcePath );
}

///////////////////////////////////////////////////////////////////////////////
// SetDocuments

void ApConfig::SetDocuments( const BtChar* documents )
{
	BtStrCopy( m_documents, 256, documents );
    BtPrint( "Document path set to: %s\n", documents );
}

///////////////////////////////////////////////////////////////////////////////
// CheckResourcePath

void ApConfig::CheckResourcePath( const BtChar *name )
{
	BtChar filename[256];
	sprintf( filename, "%s%s%s", GetResourcePath(), name, ApConfig::GetExtension() );
	FILE *f = fopen( filename, "rb" );
    printf( "Filename %s", filename );
	if( f != BtNull )
	{
		fclose(f);
	}
	else
	{
		BtStrCopy( m_resourcePath, 256, "" );
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetResourcePath

const BtChar* ApConfig::GetResourcePath()
{
	return m_resourcePath;
}

///////////////////////////////////////////////////////////////////////////////
// GetDocuments

const BtChar* ApConfig::GetDocuments()
{
	return m_documents;
}

///////////////////////////////////////////////////////////////////////////////
// GetExtension

const BtChar* ApConfig::GetExtension()
{
	return m_extension;
}

///////////////////////////////////////////////////////////////////////////////
// GetDevice

ApDevice ApConfig::GetDevice()
{
    return m_device;
}

///////////////////////////////////////////////////////////////////////////////
// IsAppleTV

BtBool ApConfig::IsAppleTV()
{
    if( m_device == ApDevice_AppleTV )
    {
        return BtTrue;
    }
    return BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// IsPhone

BtBool ApConfig::IsPhone()
{
	if (m_device == ApDevice_iPhone )
	{
		return BtTrue;
	}
	return BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// IsOSX

BtBool ApConfig::IsOSX()
{
    if( m_device == ApDevice_OSX )
    {
        return BtTrue;
    }
    return BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// IsDesktop

BtBool ApConfig::IsDesktop()
{
    return IsWin() || IsOSX();
}

///////////////////////////////////////////////////////////////////////////////
// IsWin

BtBool ApConfig::IsWin()
{
    if( m_device == ApDevice_WIN )
	{
		return BtTrue;
	}
	return BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// SetDevice

void ApConfig::SetDevice( ApDevice device )
{
    m_device = device;
}
