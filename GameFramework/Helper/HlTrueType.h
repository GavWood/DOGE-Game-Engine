////////////////////////////////////////////////////////////////////////////////
// HlTrueType.h

// Include guard
#pragma once

/*
// Includes
#include "BaArchive.h"
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "stb.h"
#include "stb_truetype.h"
#include "RsColour.h"
#include "RsFont.h"

class BaUserData;
class BaArchive;
class RsMaterial;

struct HlTrueTypeDimension
{
	MtVector2					m_v2Dimension;
	int							m_ascent;
};

// Class definition
class HlTrueType : public RsTrueTypeFont
{
public:
	
	// Public functions		
	void						 Setup( BaArchive *pArchive, const BtChar *fontText, const BtChar *pTextMaterial );

	MtVector2					 GetDimension(const BtChar *text);

	void						 Render(const BtChar *text, RsColour colour);
	
	void						 Destroy();

	// Accessors
	RsMaterial					*GetTextureMaterial() { return m_pTextTexture;  }

private:

	MtVector2					 GetUnscaledDimension(const BtChar *text);

	RsMaterial				    *m_pTextTexture;
	BaUserData					*m_pUserData;
	stbtt_fontinfo				 m_stbFontInfo;
	BaArchive					*m_pArchive;

	BtU32						 m_width;
	BtU32						 m_height;
	BtU8						*m_pTempTextMemory;
};
*/