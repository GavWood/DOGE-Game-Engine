////////////////////////////////////////////////////////////////////////////////
// HlFont.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector2.h"
#include "RsColour.h"

class RsFont;
class BaArchive;

// Class definition
class HlFont
{
public:

	// Public functions
	static void					Setup(BaArchive *pArchive, const BtChar *font);
	static void					Render(const MtVector2 &v2Position, const BtChar *text, const RsColour &colour, BtU32 sortOrder);
	static void					Render(const MtVector2 &v2Position, BtFloat scale, const BtChar *text, const RsColour &colour, BtU32 sortOrder);
	static void					RenderHeavy(const MtVector2 &v2Position, const BtChar *text, BtU32 sortOrder);
	static MtVector2            GetDimension(const BtChar *text);
    
	// Accessors

private:

	// Private functions
	static RsFont			    *m_pFont;
};
