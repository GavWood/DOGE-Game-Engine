////////////////////////////////////////////////////////////////////////////////
// SbSprite .cpp

// Includes
#include "BaArchive.h"
#include "SbTexture.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsTexture.h"
#include "RsRenderTarget.h"
#include "RsUtil.h"

////////////////////////////////////////////////////////////////////////////////
// Setup

void SbTexture::Setup( BaArchive *pArchive )
{
//	m_pTexture = pArchive->GetTexture( "logo" );
	m_pTexture = pArchive->GetTexture( "vera20" );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void SbTexture::Update()
{
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SbTexture::Render()
{
	return;
	BtFloat width  = (BtFloat)RsUtil::GetWidth();
	BtFloat height = (BtFloat)RsUtil::GetHeight();

	BtFloat x = ( width * 0.5f )  - ( m_pTexture->GetWidth()  * 0.5f );
	BtFloat y = ( height * 0.5f ) - ( m_pTexture->GetHeight() * 0.5f );

	MtVector2 v2Pos( x, y );

//	m_pTexture->Render( v2Pos, MtVector2( m_pTexture->GetWidth(), m_pTexture->GetHeight() ), RsColour::WhiteColour(), MaxSortOrders - 1 );
//	m_pTexture->Render( MtVector2( 0, 0 ), MtVector2( 512.0F, 512.0F ), RsColour::WhiteColour(), MaxSortOrders - 1 );
}
