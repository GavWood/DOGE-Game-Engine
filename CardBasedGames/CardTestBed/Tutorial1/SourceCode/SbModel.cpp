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
	m_pSphere = pGameArchive->GetNode( "sphere" );
    
    // Cache the skybox
    m_pSkybox = pGameArchive->GetNode( "skybox" );
	if (m_pSphere)
	{
		HlModel::SetSortOrders(m_pSphere, ModelSortOrder);
	}

	HlModel::SetMaterialTechnique(m_pSkybox, "RsShaderT");
	HlModel::SetSortOrders( m_pSkybox, ModelSortOrder );
    m_pSkybox = BtNull;
    
	// Cache the font
	m_pFont = pGameArchive->GetFont( "vera20" );

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );

	m_pOwl = pGameArchive->GetNode("snowyowl");
	m_pAnimator = pAnimArchive->GetAnimator("flight");
	m_frameIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbModel::Update()
{
	MtMatrix4 m4Transform;
	m4Transform.SetIdentity();

	if (m_pSphere)
	{
		MtMatrix4 m4Rotate;
		m4Rotate.SetRotationX(MtDegreesToRadians(90.0f));
		m_pSphere->SetLocalTransform(m4Transform);
		MtMatrix4 m4Transform;
		m4Transform.SetTranslation(MtVector3(-5, 0, 10.0f));
		m_pSphere->SetLocalTransform(m4Rotate * m4Transform);
		m_pSphere->Update();
	}
    if( m_pSkybox )
    {
        m_pSkybox->Update();
    }
	if( m_pOwl )
	{
		m_pOwl->Update();

		MtMatrix4 m4Transform;
		m4Transform.SetRotationX(MtDegreesToRadians(90.0f));
		MtMatrix4 m4Translate;
		m4Translate.SetTranslation(MtVector3( 0.25f, 0.5f, 0));
		MtMatrix4 m4Scale;
		m4Scale.SetScale(0.05f, 0.05f, 0.05f);
		m_pOwl->SetLocalTransform(m4Scale * m4Transform * m4Translate );
		m_pOwl->Update();
	}
	if (m_pAnimator)
	{
		m_frameIndex += 1;// BtTime::GetTick();
		
		BtFloat frameRate = m_pAnimator->GetFrameRate();
		BtU32 duration = m_pAnimator->GetDuration() / frameRate;

		if( m_frameIndex >= duration )
		{
			m_frameIndex = 0;
		}
		m_pAnimator->SetTime(m_frameIndex);
		m_pAnimator->Update(m_pOwl->pSkin());
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

	if (m_pSphere)
	{
		m_pSphere->Render();
	}
    if( m_pSkybox )
    {
        m_pSkybox->Render();
    }
	if (m_pOwl)
	{
		m_pOwl->Render();
	}
}
