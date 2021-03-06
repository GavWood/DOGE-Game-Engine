////////////////////////////////////////////////////////////////////////////////
// SbMain.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "MtVector3.h"
#include "RsVertex.h"
#include "SdSound.h"
#include "GaProject.h"
#include "SdSound.h"
#include "Game.h"

#include "ScCamera.h"
#include "ScRenderSkybox.h"
#include "ScModel.h"

class ScMain;
class RsTexture;
class RsShader;
class RsMaterial;
class SgNode;

// Class definition
class ScMain : public GaProject
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
 
	// Accessors
	void							SetClosing();
	BtBool							IsClosed();
	BtBool							IsClosing();

private:

	// Private functions
	void							RestoreRenderTarget();
	void							Render2DScene();
	void							Render3DScene();

	void							UpdateTest();
	void							RenderTests();
	
	// Private members
	BaArchive						m_gameArchive;
	BaArchive						m_utilityArchive;
	
	// Resources
	RsShader					   *m_pShader;
	SbCamera						m_camera;
	RsMaterial					   *m_pWhite2;
	RsMaterial					   *m_pWhite3;

	// Test classes
	ScModel							m_model;
	ScRenderSkybox					m_skybox;
};

inline void ScMain::SetClosing()
{
	m_isClosing = BtTrue;
}

inline BtBool ScMain::IsClosed()
{
	return m_isClosed;
}

inline BtBool ScMain::IsClosing()
{
	return m_isClosing;
}
