////////////////////////////////////////////////////////////////////////////////
// ApConfig.h

#pragma once
#include "BtTypes.h"
#include "MtVector2.h"
#include "RsColour.h"

enum ApPlatform
{
	ApPlatform_DX11,
	ApPlatform_WinGL,
	ApPlatform_GLES,
	ApPlatform_Max,
};

enum ApDevice
{
    ApDevice_iPad,
    ApDevice_iPhone,
    ApDevice_WIN,
    ApDevice_AppleTV,
    ApDevice_OSX,
    ApDevice_Max,
};

// Class declaration
class ApConfig
{
public:

	ApConfig();

	static void					Init();
	static void					CheckResourcePath(const BtChar *name);

	static ApPlatform			GetPlatform();
	static void					SetPlatform( ApPlatform orientation );
	static const BtChar		   *GetPlatformName();

    static const BtChar		   *GetDocuments();                         // For writable area
	static const BtChar		   *GetResourcePath();                      // For package install area
    static const BtChar		   *GetExtension();
    static ApDevice             GetDevice();
	
	static BtBool				IsWin();
	static BtBool				IsPhone();
	static BtBool				IsAppleTV();
    static BtBool               IsOSX();
    static BtBool               IsDesktop();

	static void					SetTitle(const BtChar* title);
	static const BtChar		   *GetTitle();

	static void					SetResourcePath( const BtChar* pResourcePath );
	static void					SetExtension( const BtChar* pExtension );
    static void					SetDocuments( const BtChar* documents );
    static void					SetDevice( ApDevice device );
    
    static BtBool               IsDebug();
    static void                 SetDebug( BtBool isDebug );
   
	static BtBool               IsDebugBuild();
	static void                 SetDebugBuild(BtBool isDebug);

    static BtBool               IsAR();
    static void                 SetAR(BtBool isAR);

	static BtBool               IsPaused();
	static void                 SetPaused(BtBool isPaused);

	static BtBool               IsInitialised();

	static BtBool               IsCheatMode();
	static void                 SetCheatMode( BtBool isCheatMode );

    static BtBool               IsSimulator();
    static void                 SetSimulator( BtBool isSimulator );

	static BtU32                GetNumInstances();
	static void                 SetInstance(BtU32 numInstances);

private:
	
	static ApPlatform			m_platform;
	static BtChar				m_resourcePath[256];
	static BtChar				m_extension[256];
	static BtChar				m_documents[256];
	static BtChar				m_title[256];
    static ApDevice             m_device;
    static BtBool               m_isDebug;
    static BtBool               m_isCheatMode;
    static BtBool               m_isAR;
    static BtBool               m_isSimulator;
	static BtBool				m_isDebugBuild;
	static BtBool				m_isPaused;
	static BtU32				m_instances;
	static BtBool				m_isInitialised;
};
