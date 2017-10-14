////////////////////////////////////////////////////////////////////////////////
// PaTopState

#pragma once
#include "BtSingleton.h"

enum PackerPlatform
{
	PackerPlatform_WinDX,
	PackerPlatform_GLES,
	PackerPlatform_WinGL,
	PackerPlatform_OSX,
	PackerPlatform_MAX = -1
};

// Class definition
class PaTopState : public BtSingleton<PaTopState> 
{
public:

	PaTopState();

    static void                 HookIntoMain( const char *path, int platformIndex );
    
	void						Create();
	void						Update();
	void						Destroy();

	static const BtChar*		pWorkingPath();
	
	static const BtChar		   *GetPlatformName();
	static PackerPlatform		GetPlatform();

	BtBool						IsBaseVertex() const;
	BtBool						IsVertexBase2() const;

private:

	BtBool						ReadConfig();
	void						CreateFolders();

	static BtChar				m_szWorkingPath[256];
	static PackerPlatform		m_platform;
};
