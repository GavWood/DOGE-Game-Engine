////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "SbCamera.h"
#include "SdSound.h"
#include "SbRenderSkybox.h"
#include "SbSprite.h"
#include "SbMaterial.h"
#include "SbTexture.h"
#include "SbModel.h"
#include "GaProject.h"
#include "SdSound.h"
#include "SbSkippingRope.h"

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
    void                            Resize();

	// Accessors
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();
    
    static double                   GetCounter() { return m_counter; }
    static void                     SetCounter( double counter ) { m_counter = counter; }
    
private:

	// Private functions
	void							RestoreRenderTarget();
	void							RenderScene();

	void							SetupRenderToTexture( RsTexture *pTexture, RsCamera camera );
	void							SetupRender();
	void							PlaySound();
	void							UpdateTest();
	void							RenderTests();
	void							RenderFont();
	void							DrawRenderTarget();
    
	void							RenderUpsideDown( RsMaterial *pMaterial,
                                                      const MtVector2 &v2Position,
                                                      const MtVector2 &v2Dimension,
                                                      const MtVector2 &v2ScreenDimension );
	
	// Private members
	BaArchive						m_gameArchive;
	BaArchive						m_animArchive;
	BaArchive						m_utilityArchive;
	BaArchive						m_oculusArchive;

	// Resources
	RsMaterial					   *m_pTouch;
	RsShader					   *m_pShader;
	BtBool							m_isClosing;
	BtBool							m_isClosed;
	MtVector3						m_v3Position;
	ShCamera						m_camera;
	RsMaterial					   *m_pWhite2;
	RsMaterial					   *m_pWhite3;
	RsShader					   *m_pBloom;
	SdSound						   *m_pSound;

	// Test classes
	SbSprite						m_sprite;
	SbMaterial						m_wolf2;
	SbTexture						m_texture;
	SbModel							m_model;
    SbSkippingRope                  m_skippingRope;
	SbRenderSkybox					m_skybox;
    
    BtBool                          m_isSkippingRope;
    
    static double                   m_counter;
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
