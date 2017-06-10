////////////////////////////////////////////////////////////////////////////////
// SbSprite.cpp

// Includes
#include "BaArchive.h"
#include "SbSprite.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsSprite.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbSprite::Setup( BaArchive *pArchive )
{
	m_pSprite = pArchive->GetSprite( "hermes" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbSprite::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbSprite::Render()
{
	m_pSprite->Render( BtFalse, MtVector2( 100, 100 ), 0, 0, RsColour::WhiteColour(), 4 );
	m_pSprite->Render( BtFalse, MtVector2( 140, 100 ), 6, 0, RsColour::WhiteColour(), 4 );
}
