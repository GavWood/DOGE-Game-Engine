////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "SbCamera.h"
#include "GaProject.h"
#include "Game.h"
#include "SbPeers.h"
#include "CgCard.h"
#include "HlTimer.h"
#include <inttypes.h>

class SbMain;
class RsTexture;
class RsShader;
class RsMaterial;
class SgNode;

enum GameState
{
	GameState_Ace,
	GameState_TwoOfClubs,
	GameState_ThreeOfClubs,
	GameState_FourOfClubs,
	GameState_MAX
};

// Class definition
class SbMain : public GaProject
{
public:

	// Public functions
	void							Init();
	void							Reset();
	void							Create();
	void							Update();
	void							Render();
	void							Destroy();
    
	static GameState				GetState();
	static void						SetNextState();
	static void						SetNextState(GameState state);
	static BtBool					IsTurn();

	void							PushNetworkChanges();
	void							PullNetworkChanges();

	// Accessors
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();

private:

	// Private functions
	void							RestoreRenderTarget();
	void							UpdateTests();
	void							RenderTests();
	
	// Private members
	BaArchive						m_utilityArchive;
	
	// Resources
	RsShader					   *m_pShader;
	SbCamera						m_camera;

	// Test classes
	SbPeers							m_peers;
	CgCard							m_card;
	HlTimer							m_closingTimer;

private:
};

inline void SbMain::SetClosing()
{
	m_isClosing = BtTrue;
}

inline BtBool SbMain::IsClosed()
{
	return m_isClosed;
}

inline BtBool SbMain::IsClosing()
{
	return m_isClosing;
}
