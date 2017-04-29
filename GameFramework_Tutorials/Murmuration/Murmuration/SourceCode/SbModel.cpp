////////////////////////////////////////////////////////////////////////////////
// SbModel.cpp

// Includes
#include "BaArchive.h"
#include "SbModel.h"
#include "MtVector2.h"
#include "RsCamera.h"
#include "RsColour.h"
#include "RsFont.h"
#include "RsShader.h"
#include "RsUtil.h"
#include "RsRenderTarget.h"
#include "SgNode.h"
#include "MtAABB.h"
#include "SgMesh.h"
#include "SgAnimator.h"
#include "BtTime.h"
#include <stdio.h>
#include "UiKeyboard.h"
#include "HlDraw.h"
#include "ApConfig.h"
#include "SgBone.h"
#include "HlModel.h"
#include "SbWorld.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbModel::Setup( BaArchive *pGameArchive, BaArchive *pAnimArchive )
{
	// Cache our model
	m_pDebugCube = pGameArchive->GetNode( "cube" );
	HlModel::SetSortOrders(m_pDebugCube, ModelSortOrder);
	
	// Cache the font
	m_pFont = pGameArchive->GetFont( "vera20" );

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbModel::Update()
{
	MtMatrix4 m4Transform;
	m4Transform.SetIdentity();

	if (m_pDebugCube)
	{
		MtMatrix4 m4Scale;
		m4Scale.SetScale( MtVector3(256.0f, 0.1f, 256.0f) );
		m_pDebugCube->SetLocalTransform(m4Transform);
		MtMatrix4 m4Transform;
		m4Transform.SetTranslation(MtVector3(0, -128.0f, 0 ));
		m_pDebugCube->SetLocalTransform(m4Scale * m4Transform);
		m_pDebugCube->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbModel::Render()
{
	// Set the light direction
	MtVector3 v3LightDirection( -1, 0, 0 );

	m_pShader->SetDirectionalLight( v3LightDirection );

	// Set the lights ambient level
	m_pShader->SetAmbient( RsColour( 0.5f, 0.4f, 0.3f, 0 ) );

	// Apply the shader
	m_pShader->Apply();

	if (m_pDebugCube && ApConfig::IsDebug())
	{
		m_pDebugCube->Render();
	}
}
