////////////////////////////////////////////////////////////////////////////////
// SbRenderSKybox.cpp

// Includes
#include <stdio.h>
#include "SgNode.h"
#include "SbRenderSkybox.h"
#include "RsVertex.h"
#include "RsMaterial.h"
#include "RsShader.h"
#include "RsRenderTarget.h"
#include "SbWorld.h"

////////////////////////////////////////////////////////////////////////////////
// Setup
 
void SbRenderSkybox::Setup( BaArchive *pArchive )
{
	m_pBackMaterial  = pArchive->GetMaterial( "back" );
	m_pFrontMaterial = pArchive->GetMaterial( "front" );
	m_pLeftMaterial  = pArchive->GetMaterial( "left" );
	m_pRightMaterial = pArchive->GetMaterial( "right" );
	m_pUpMaterial    = pArchive->GetMaterial( "up" );

	m_pBackMaterial->SetTechniqueName( "RsShaderT" );
	m_pFrontMaterial->SetTechniqueName( "RsShaderT" );
	m_pLeftMaterial->SetTechniqueName( "RsShaderT" );
	m_pRightMaterial->SetTechniqueName( "RsShaderT" );
	m_pUpMaterial->SetTechniqueName( "RsShaderT" );

	// Cache the shader
	m_pShader = pArchive->GetShader( "shader" );
}

////////////////////////////////////////////////////////////////////////////////
// Rotate

void SbRenderSkybox::Rotate( RsVertex3* pVertex )
{
	for( BtU32 i=0; i<4; i++)
	{
		pVertex->m_v3Position = pVertex->m_v3Position - m_v3Translation;

		++pVertex;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbRenderSkybox::Update( MtVector3 v3Translate )
{
	m_v3Translation = -v3Translate;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbRenderSkybox::Render()
{
	RsVertex3 pVertex[4];
	BtFloat width = 512.0f / 2;
	BtFloat min = 1.0f / width;
	BtFloat max = 1.0f - min;

	// Allocate 3 vertex
	BtU32 Colour = 0xffffffff;

	m_pShader->Apply();

	pVertex[1].m_v3Position.x  = -width;
	pVertex[1].m_v3Position.y  = width;
	pVertex[1].m_v3Position.z  = width;
	pVertex[1].m_v2UV.x = min;
	pVertex[1].m_v2UV.y = min;
	pVertex[1].m_colour = Colour;

	pVertex[0].m_v3Position.x = -width;
	pVertex[0].m_v3Position.y = -width;
	pVertex[0].m_v3Position.z =  width;
	pVertex[0].m_v2UV.x = min;
	pVertex[0].m_v2UV.y = max;
	pVertex[0].m_colour = Colour;

	pVertex[3].m_v3Position.x =  width;
	pVertex[3].m_v3Position.y =  width;
	pVertex[3].m_v3Position.z =  width;
	pVertex[3].m_v2UV.x = max;
	pVertex[3].m_v2UV.y = min;
	pVertex[3].m_colour = Colour;

	pVertex[2].m_v3Position.x =  width;
	pVertex[2].m_v3Position.y = -width;
	pVertex[2].m_v3Position.z =  width;
	pVertex[2].m_v2UV.x = max;
	pVertex[2].m_v2UV.y = max;
	pVertex[2].m_colour = Colour;

	Rotate( pVertex );

	// Render these triangles
	m_pFrontMaterial->Render( RsPT_TriangleStrip, pVertex, 4, SkyBoxSortOrder );

	// Allocate 3 vertex
	pVertex[1].m_v3Position.x =  width;
	pVertex[1].m_v3Position.y = -width;
	pVertex[1].m_v3Position.z = -width;
	pVertex[1].m_v2UV.x = max;
	pVertex[1].m_v2UV.y = max;
	pVertex[1].m_colour = Colour;

	pVertex[0].m_v3Position.x =  width;
	pVertex[0].m_v3Position.y =  width;
	pVertex[0].m_v3Position.z = -width;
	pVertex[0].m_v2UV.x = max;
	pVertex[0].m_v2UV.y = min;
	pVertex[0].m_colour = Colour;

	pVertex[3].m_v3Position.x =  width;
	pVertex[3].m_v3Position.y = -width;
	pVertex[3].m_v3Position.z =  width;
	pVertex[3].m_v2UV.x = min;
	pVertex[3].m_v2UV.y = max;
	pVertex[3].m_colour = Colour;

	pVertex[2].m_v3Position.x = width;
	pVertex[2].m_v3Position.y = width;
	pVertex[2].m_v3Position.z = width;
	pVertex[2].m_v2UV.x = min;
	pVertex[2].m_v2UV.y = min;
	pVertex[2].m_colour = Colour;

	Rotate( pVertex );

	// Render these triangles
	m_pRightMaterial->Render( RsPT_TriangleStrip, pVertex, 4, SkyBoxSortOrder );

	// Allocate 3 vertex
	pVertex[0].m_v3Position.x = -width;
	pVertex[0].m_v3Position.y = -width;
	pVertex[0].m_v3Position.z = -width;
	pVertex[0].m_v2UV.x = min;
	pVertex[0].m_v2UV.y = max;
	pVertex[0].m_colour = Colour;

	pVertex[1].m_v3Position.x =-width;
	pVertex[1].m_v3Position.y = width;
	pVertex[1].m_v3Position.z =-width;
	pVertex[1].m_v2UV.x = min;
	pVertex[1].m_v2UV.y = min;
	pVertex[1].m_colour = Colour;

	pVertex[2].m_v3Position.x =-width;
	pVertex[2].m_v3Position.y =-width;
	pVertex[2].m_v3Position.z = width;
	pVertex[2].m_v2UV.x = max;
	pVertex[2].m_v2UV.y = max;
	pVertex[2].m_colour = Colour;

	pVertex[3].m_v3Position.x =-width;
	pVertex[3].m_v3Position.y = width;
	pVertex[3].m_v3Position.z = width;
	pVertex[3].m_v2UV.x = max;
	pVertex[3].m_v2UV.y = min;
	pVertex[3].m_colour = Colour;

	Rotate( pVertex );

	// Render these triangles
	m_pLeftMaterial->Render( RsPT_TriangleStrip, pVertex, 4, SkyBoxSortOrder );

	// Allocate 3 vertex
	pVertex[1].m_v3Position.x = -width;
	pVertex[1].m_v3Position.y =  width;
	pVertex[1].m_v3Position.z = -width;
	pVertex[1].m_v2UV.x = min;
	pVertex[1].m_v2UV.y = min;
	pVertex[1].m_colour = Colour;

	pVertex[0].m_v3Position.x =-width;
	pVertex[0].m_v3Position.y = width;
	pVertex[0].m_v3Position.z = width;
	pVertex[0].m_v2UV.x = min;
	pVertex[0].m_v2UV.y = max;
	pVertex[0].m_colour = Colour;

	pVertex[3].m_v3Position.x = width;
	pVertex[3].m_v3Position.y = width;
	pVertex[3].m_v3Position.z =-width;
	pVertex[3].m_v2UV.x = max;
	pVertex[3].m_v2UV.y = min;
	pVertex[3].m_colour = Colour;

	pVertex[2].m_v3Position.x = width;
	pVertex[2].m_v3Position.y = width;
	pVertex[2].m_v3Position.z = width;
	pVertex[2].m_v2UV.x = max;
	pVertex[2].m_v2UV.y = max;
	pVertex[2].m_colour = Colour;

	Rotate( pVertex );

	m_pUpMaterial->Render( RsPT_TriangleStrip, pVertex, 4, SkyBoxSortOrder );

	// Allocate 4 vertex
	pVertex[0].m_v3Position.x = -width;
	pVertex[0].m_v3Position.y =  width;
	pVertex[0].m_v3Position.z = -width;
	pVertex[0].m_v2UV.x = max;
	pVertex[0].m_v2UV.y = min;
	pVertex[0].m_colour = Colour;

	pVertex[1].m_v3Position.x = -width;
	pVertex[1].m_v3Position.y = -width;
	pVertex[1].m_v3Position.z = -width;
	pVertex[1].m_v2UV.x = max;
	pVertex[1].m_v2UV.y = max;
	pVertex[1].m_colour = Colour;

	pVertex[3].m_v3Position.x =  width;
	pVertex[3].m_v3Position.y = -width;
	pVertex[3].m_v3Position.z = -width;
	pVertex[3].m_v2UV.x = min;
	pVertex[3].m_v2UV.y = max;
	pVertex[3].m_colour = Colour;

	pVertex[2].m_v3Position.x =  width;
	pVertex[2].m_v3Position.y =  width;
	pVertex[2].m_v3Position.z = -width;
	pVertex[2].m_v2UV.x = min;
	pVertex[2].m_v2UV.y = min;
	pVertex[2].m_colour = Colour;

	Rotate( pVertex );

	m_pBackMaterial->Render( RsPT_TriangleStrip, pVertex, 4, SkyBoxSortOrder );
}
