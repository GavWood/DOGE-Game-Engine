////////////////////////////////////////////////////////////////////////////////
// SbModel.cpp

// Includes
#include "BaArchive.h"
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
#include "SgSkin.h"
#include "ShTouch.h"
#include "ScModel.h"
#include "ScWorld.h"
#include "HlFont.h"
#include "HlModel.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void ScModel::Setup( BaArchive *pGameArchive )
{
    m_pLargeCube = pGameArchive->GetNode("cube");
	m_pSmallCube = pGameArchive->GetNode("cube")->GetDuplicate();

	HlModel::SetSortOrders(m_pLargeCube, ModelSortOrder);
	HlModel::SetSortOrders(m_pSmallCube, ModelSortOrder);

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );

	m_time = 0;
}
	
////////////////////////////////////////////////////////////////////////////////
// Update

void ScModel::Update( RsCamera &camera )
{
	MtMatrix4 m4Transform;
	m4Transform.SetIdentity();

	if (m_pLargeCube)
	{
		m4Transform.SetTranslation(MtVector3(0, 0, 0));
		m_pLargeCube->SetLocalTransform(m4Transform);
		m_pLargeCube->Update();

		MtMatrix4 m4Scale;
		m4Scale.SetScale(0.5f, 0.5f, 0.5f);
		m4Transform.SetTranslation(MtVector3(0, 1, 0));
		m_pSmallCube->SetLocalTransform( m4Scale * m4Transform );
		m_pSmallCube->Update();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void ScModel::Render( RsCamera &camera )
{
	// Set the light direction
	MtVector3 v3LightDirection( -1, -1, 0 );

	m_pShader->SetDirectionalLight( v3LightDirection );

	// Set the lights ambient level
	m_pShader->SetAmbient( RsColour( 0.5f, 0.4f, 0.3f, 0 ) );

	// Apply the shader
	m_pShader->Apply();

	if (m_pLargeCube)
	{
		m_pLargeCube->Render();
	}

	if (m_pSmallCube)
	{
		m_pSmallCube->Render();
	}
    //MtVector2 v2Position( 0, 0 );
    //MtVector3 v3Position = camera.GetRotation().Col2();
    //BtChar text[32];
    //sprintf( text, "%.2f %.2f %.2f", v3Position.x, v3Position.y, v3Position.z );
    //HlFont::Render(v2Position, text, RsColour::BlackColour(), MaxSortOrders - 1);
}
