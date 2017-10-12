////////////////////////////////////////////////////////////////////////////////
// HlFont.cpp

// Includes
#include "BaArchive.h"
#include "HlFont.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsUtil.h"
#include "RsFont.h"

//static
RsFont *HlFont::m_pFont = BtNull;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlFont::Setup( BaArchive *pArchive, const BtChar *title )
{
	m_pFont = pArchive->GetFont( title );
	BtAssert(m_pFont);
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlFont::Render( const MtVector2 &v2Position, const BtChar *text, const RsColour &colour, BtU32 sortOrder )
{
	m_pFont->Render( v2Position, 1.0f, colour, text, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlFont::Render(const MtVector2 &v2Position, BtFloat scale, const BtChar *text, const RsColour &colour, BtU32 sortOrder)
{
	m_pFont->Render(v2Position, scale, colour, text, sortOrder);
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

MtVector2 HlFont::GetDimension( const BtChar *text )
{
    return m_pFont->GetDimension(text);
}

////////////////////////////////////////////////////////////////////////////////
// RenderHeavy

void HlFont::RenderHeavy( const MtVector2 &v2Position, const BtChar *text, BtU32 sortOrder)
{
	BtFloat scale = 1.0f;
	m_pFont->Render(v2Position + MtVector2(-1, -1 ), scale, RsColour::WhiteColour(), text, sortOrder);
	m_pFont->Render(v2Position + MtVector2( 0,  1 ), scale, RsColour::WhiteColour(), text, sortOrder);
	m_pFont->Render(v2Position + MtVector2( 1,  0 ), scale, RsColour::WhiteColour(), text, sortOrder);
	m_pFont->Render(v2Position + MtVector2( 1,  1 ), scale, RsColour::WhiteColour(), text, sortOrder);
	m_pFont->Render(v2Position + MtVector2( 1,  1 ), scale, RsColour::BlackColour(), text, sortOrder);
}