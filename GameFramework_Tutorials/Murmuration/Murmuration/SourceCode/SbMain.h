////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "RsVertex.h"
#include "SbCamera.h"
#include "SdSound.h"
#include "SbRenderSkybox.h"
#include "SbModel.h"
#include "GaProject.h"
#include "SdSound.h"
#include "Game.h"
#include "HlJoysticks.h"
#include "MuMurmuration.h"

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
	void							PreRender();
	void							PostRender();
	void							Destroy();
 
    void                            RenderIntoHMD( RsMaterial *pMaterial,
                                                   const MtVector2 &v2Position,
                                                   MtVector2 &v2Dimension,
                                                   const MtVector2 &v2ScreenDimension,
                                                   BtU32 sortOrder
                                                 );
    
	// Accessors
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();

private:

	// Private functions
	void							RestoreRenderTarget();
	void							Render2DScene();
	void							Render3DScene();
	void							Render2DInto3D( RsCamera &camera );

	void							SetupRenderToTexture( RsTexture *pTexture, RsCamera camera );
	void							SetupRender();
	void							RenderFinal();
	void							PlaySound();
	void							UpdateTest();
	void							RenderTests();
	void							RenderFont();
	void							DrawRenderTarget();
	
	// Private members
	BaArchive						m_gameArchive;
	BaArchive						m_utilityArchive;
	BaArchive						m_hmdArchive;
	
	// Resources
	HlJoysticks						m_joysticks;
	RsMaterial					   *m_pLeftEye;
	RsMaterial					   *m_pRightEye;

	RsMaterial					   *m_pTouch;
	RsShader					   *m_pShader;
	MtVector3						m_v3Position;
	SbCamera						m_camera;
	RsMaterial					   *m_pWhite2;
	RsMaterial					   *m_pWhite3;
	RsMaterial					   *m_pGUIRenderTarget;
	RsShader					   *m_pBloom;
	SdSound						   *m_pSound;

	// Test classes
	SbModel							m_model;
	SbRenderSkybox					m_skybox;

	RsVertex3						m_v3Vertex[6];
	BtChar							m_text[256];

	SbMurmuration					m_murmuration;
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
