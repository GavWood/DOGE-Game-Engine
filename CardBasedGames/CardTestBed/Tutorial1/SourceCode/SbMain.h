////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "SbCamera.h"
#include "SbSprite.h"
#include "GaProject.h"
#include "Game.h"
#include "SbPeers.h"
#include "CgCard1.h"
#include "CgCard2.h"
#include "CgCard3.h"
#include "CgCard4.h"
#include "CgCard5.h"
#include "CgCard6.h"
#include "CgCard7.h"

class SbMain;
class RsTexture;
class RsShader;
class RsMaterial;
class SgNode;

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
    
	static BtU32					GetState();
	static void						SetNextState();
	static void						SetNextState(BtU32 state);

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
	CgCard1							m_card1;
	CgCard2							m_card2;
	CgCard3							m_card3;
	CgCard4							m_card4;
	CgCard5							m_card5;
	CgCard6							m_card6;
	CgCard7							m_card7;

private:

	static BtU32					m_currentState;
	static BtU32					m_nextState;
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
