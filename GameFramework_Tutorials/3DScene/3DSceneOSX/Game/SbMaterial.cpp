////////////////////////////////////////////////////////////////////////////////
// SbMaterial.cpp

// Includes
#include "BaArchive.h"
#include "SbMaterial.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "SgNode.h"
#include "RsShader.h"
#include "HlModel.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbMaterial::Setup( BaArchive *pArchive )
{
	m_pWolf2 = pArchive->GetMaterial( "wolf2" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbMaterial::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbMaterial::Render()
{
	m_pWolf2->Render( MtVector2( 0, 0 ), RsColour::WhiteColour(), MaxSortOrders-1 );
}
