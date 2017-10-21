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
#include "ShIMU.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void ScModel::Setup( BaArchive *pGameArchive )
{
	// Cache our model
	m_pCube = pGameArchive->GetNode("cube");
	HlModel::SetSortOrders(m_pCube, ModelSortOrder);
	
	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );
}
	
////////////////////////////////////////////////////////////////////////////////
// Update

void ScModel::Update( RsCamera &camera )
{
	if (m_pCube)
	{
        MtQuaternion quaternion = ShIMU::GetQuaternion(0);
        //quaternion.SetIdentity();
        MtMatrix4 m4Transform( quaternion );
		m_pCube->SetLocalTransform(m4Transform);
		m_pCube->Update();
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

	if (m_pCube)
	{
		m_pCube->Render();
	}

    //MtVector2 v2Position( 0, 0 );
    //BtChar text[32];
    //sprintf( text, "Hello World!" );
    //HlFont::Render(v2Position, text, RsColour::BlackColour(), MaxSortOrders - 1);
}
