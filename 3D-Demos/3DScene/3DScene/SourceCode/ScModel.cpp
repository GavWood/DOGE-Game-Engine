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

void ScModel::Setup( BaArchive *pGameArchive, BaArchive *pAnimArchive )
{
	// Cache our model
	m_pFish = pGameArchive->GetNode( "render_model" );
    
    if( m_pFish )
    {
        HlModel::MeasureSphere(m_sphere, m_pFish );
        BtFloat radius = m_sphere.GetRadius();
        (void)radius;
        int a=0;
        a++;
    }
    
	//m_pCube = pGameArchive->GetNode("cube");
	//HlModel::SetSortOrders(m_pCube, ModelSortOrder);
	HlModel::SetSortOrders(m_pFish, ModelSortOrder);

	m_pAnimator = pAnimArchive->GetAnimator("swim");

	// Cache the main shader
	m_pShader = pGameArchive->GetShader( "shader" );

	m_time = 0;
    
    m_m4Transform.SetIdentity();
}

////////////////////////////////////////////////////////////////////////////////
// Update

void ScModel::Update( RsCamera &camera )
{
	MtMatrix4 m4Transform;
	m4Transform.SetIdentity();

	if (m_pCube)
	{
		m4Transform.SetTranslation(MtVector3(0, 0, 2));
		m_pCube->SetLocalTransform(m4Transform);
		m_pCube->Update();
	}
	if (m_pFish)
	{
        static BtBool isFirstTime = BtTrue;
        if( ShTouch::IsPressed() || isFirstTime )
        {
            isFirstTime = BtFalse;
            
            // Get the camera origin
            MtVector3 v3Position = camera.GetPosition();
            
            // Move the fish in front of the camera
            MtMatrix3 m3Rotation = camera.GetRotation().GetInverse();
            MtVector3 v3CentreOfWorld = v3Position + ( MtVector3( 0, 0, 0.20 ) * m3Rotation );
            
            // Set the matrix with this transform
            m4Transform.SetTranslation( v3CentreOfWorld );
            
            // Scale the fish
            MtMatrix4 m4Scale;
            m4Scale.SetScale( 0.10f );  // Make the fish 10cm
            
            // Now face the fish toward the camera
            m_m4Transform = m4Scale * m4Transform;
        }
        m_pFish->SetLocalTransform(m_m4Transform);
        m_pFish->Update();
	}
	if (m_pAnimator)
	{
		if( !ApConfig::IsPaused() )
		{
			m_time += BtTime::GetTick();

			BtFloat frameRate = m_pAnimator->GetFrameRate();
			(void)frameRate;

			BtU32 numFrames = m_pAnimator->GetNumFrames();
			(void)numFrames;

			BtFloat duration = m_pAnimator->GetDuration();

			if (m_time >= duration)
			{
				m_time = 0;
			}
			m_pAnimator->SetTime(m_time);
			m_pAnimator->Update(m_pFish->pSkin());
		}
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

	if (m_pFish )
	{
		m_pFish->Render();

		if (ApConfig::IsDebug())
		{
			// Render the bones
			HlModel::RenderBones(m_pFish);

			// Render the aabb - this is not currently changed by the bones
			MtAABB aabb = m_pFish->pSkin()->AABB();

			// Render its AABB box
			MtMatrix4 m4Transform = m_pFish->GetWorldTransform();
			HlDraw::RenderAABB( aabb, m4Transform, RsColour::GreenColour(), MaxSortOrders - 1);
		}
	}
	if (m_pCube)
	{
		m_pCube->Render();
	}
    
    
    MtVector2 v2Position( 0, 0 );
    MtVector3 v3Position = camera.GetRotation().Col2();
    
    BtChar text[32];
    sprintf( text, "%.2f %.2f %.2f", v3Position.x, v3Position.y, v3Position.z );
    HlFont::Render(v2Position, text, RsColour::BlackColour(), MaxSortOrders - 1);
}
