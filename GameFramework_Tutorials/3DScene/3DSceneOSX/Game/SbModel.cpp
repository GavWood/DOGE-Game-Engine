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
#include "HlLines.h"
#include "ApConfig.h"
#include "SgBone.h"
#include "HlModel.h"
#include "ShIMU.h"
#include "HlFont.h"

int MaxControllers = 6;

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbModel::Setup( BaArchive *pGameArchive, BaArchive *pAnimArchive )
{
	// Cache our model
	SgNode *pCube = pGameArchive->GetNode( "cube" );
    
    for( BtU32 i=0; i<MaxControllers; i++ )
    {
        m_pCubes[i] = pCube->GetDuplicate();
    }
    
	// Cache the font
	m_pFont = pGameArchive->GetFont( "vera20" );

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbModel::Update()
{
	MtMatrix4 m4Position[6];
	 
    BtFloat width = 3.0f;
    BtFloat x = 0; //-3.0f * width;
    
    for( BtU32 i=0; i<MaxControllers; i++ )
    {
        if (m_pCubes[i])
        {
            m4Position[i].SetTranslation( MtVector3( x, 0, 20 ) );
            m_pCubes[i]->SetLocalTransform( m4Position[i] );
            m_pCubes[i]->Update();
            x += width;
    
            MtMatrix4 m4Transform = ShIMU::GetTransform(i) * m4Position[i];
            m_pCubes[i]->SetLocalTransform( m4Transform );
            m_pCubes[i]->Update();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbModel::Render()
{
	// Set the light direction
	MtVector3 v3LightDirection( -0.5f, 0, 0 );

	m_pShader->SetDirectionalLight( v3LightDirection );

	// Set the lights ambient level
	m_pShader->SetAmbient( RsColour( 0.8f, 0.8f, 0.8f, 0 ) );

	// Apply the shader
	m_pShader->Apply();

    for( BtU32 i=0; i<MaxControllers; i++ )
    {
        if( m_pCubes[i] )
        {
            m_pCubes[i]->Render();
        }
	}
}
