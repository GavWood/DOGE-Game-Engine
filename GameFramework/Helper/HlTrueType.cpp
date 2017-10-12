////////////////////////////////////////////////////////////////////////////////
// HlTrueType
/*
#include "HlTrueType.h"
#include "RsMaterial.h"
#include "RsTexture.h"
#include "BtMemory.h"
#include "BaUserData.h"
#include "HlDraw.h"
#include "RsUtil.h"
#include "RsCamera.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb.h"
#include "stb_truetype.h"

// https://github.com/nothings/stb

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlTrueType::Setup( BaArchive *pArchive, const BtChar *fontText, const BtChar *pTextureMaterial )
{
	m_pArchive = pArchive;
	m_pUserData = m_pArchive->GetUserData(fontText);

	if(!stbtt_InitFont(&m_stbFontInfo, m_pUserData->GetData(), 0))
	{
		printf("failed\n");
	}

	m_pTextTexture = pArchive->GetMaterial(pTextureMaterial);

	m_width  = m_pTextTexture->GetTexture(0)->GetWidth();
	m_height = m_pTextTexture->GetTexture(0)->GetHeight();

	m_pTempTextMemory = (BtU8*)BtMemory::Allocate(m_width * m_height);
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void HlTrueType::Destroy()
{
	BtMemory::Free(m_pTempTextMemory);
}

////////////////////////////////////////////////////////////////////////////////
// GetUnscaledDimension

MtVector2 HlTrueType::GetUnscaledDimension(const BtChar *text)
{
	HlTrueTypeDimension trueTypeDimension;

	BtU32 textLength = (BtU32)strlen(text);

	// Get the font metrics
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&m_stbFontInfo, &ascent, &descent, &lineGap);

	// Clear the temporary font memory
	BtMemory::Set(m_pTempTextMemory, 0, m_width * m_height);

	// Calculate the width of the font

	MtVector2 v2UnscaledDimension(0, 0);
	int screenx = 0;
	int screeny = 0;

	// Get the size of the 
	for (BtU32 i = 0; i < textLength; ++i)
	{
		BtChar current = text[i];

		if (current == '\n')
		{
			screenx = 0;
			screeny += ascent - descent;
		}
		else
		{
			// how wide is this character
			int advance;
			stbtt_GetCodepointHMetrics(&m_stbFontInfo, current, &advance, 0);
			screenx += advance;

			// get bounding box for character (may be offset to account for chars that dip above or below the line
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&m_stbFontInfo, current, 1, 1, &c_x1, &c_y1, &c_x2, &c_y2);

			// compute y (different characters have different heights
			int y = screeny + ascent + c_y1;

			// add kerning
			int kern;

			if (text[i + 1])
			{
				BtChar next = text[i + 1];
				kern = stbtt_GetCodepointKernAdvance(&m_stbFontInfo, current, next);
				screenx += kern;
			}
			else
			{
				screenx -= (int)(advance);
				screenx += (c_x2 - c_x1);
			}

			int candidateMaxX = screenx;
			int candidateMaxY = y;

			v2UnscaledDimension.x = MtMax(v2UnscaledDimension.x, (BtFloat)candidateMaxX);
			v2UnscaledDimension.y = MtMax(v2UnscaledDimension.y, (BtFloat)candidateMaxY);
		}
	}
	return v2UnscaledDimension;
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

MtVector2 HlTrueType::GetDimension(const BtChar *text)
{
	MtVector2 v2UnscaledDimension = GetUnscaledDimension(text);

	BtFloat biggestDimension = MtMax(v2UnscaledDimension.x, v2UnscaledDimension.y);
	BtFloat scale = m_width / biggestDimension;

	MtVector2 v2Dimension(v2UnscaledDimension);
	v2Dimension *= scale;

	return v2Dimension;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlTrueType::Render( const BtChar *text, RsColour colour )
{
	HlTrueTypeDimension trueTypeDimension;

	BtU32 textLength = (BtU32)strlen(text);

	// Get the font metrics
	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&m_stbFontInfo, &ascent, &descent, &lineGap);

	// Clear the temporary font memory
	BtMemory::Set(m_pTempTextMemory, 0, m_width * m_height );

	// Calculate the width of the font

	MtVector2 v2UnscaledDimension = GetUnscaledDimension( text );

	BtFloat biggestDimension = MtMax( v2UnscaledDimension.x, v2UnscaledDimension.y );
	BtFloat scale = m_width / biggestDimension;
	
	// Get the length of each text row
	BtFloat screenx = 0;
	BtFloat screeny = 0;

	BtU32 row = 0;
	for(BtU32 i = 0; i < textLength; ++i)
	{
		BtChar current = text[i];

		if(current == '\n')
		{
			++row;
			screenx = 0;
		}
		else
		{
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&m_stbFontInfo, current, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

			int advance;
			stbtt_GetCodepointHMetrics(&m_stbFontInfo, current, &advance, 0);
			screenx += (int)(advance * scale);

			if(text[i + 1])
			{
				BtChar next = text[i + 1];
				BtFloat kern = (BtFloat) stbtt_GetCodepointKernAdvance(&m_stbFontInfo, current, next);
				screenx += (int)(kern * scale);
			}
			else
			{
				screenx -= (int)(advance * scale);
				screenx += (c_x2 - c_x1)*scale;
			}
		}
	}

	// Render the font into the bitmap
	MtVector2 v2Dimension(0, 0);
	ascent = (int)((float)ascent * scale);
	descent = (int)((float)descent * scale);
	row = 0;
	screenx = 0;
	screeny = 0;
	for( BtU32 i = 0; i < textLength; ++i )
    {
		BtChar current = text[i];

		if( current == '\n' )
		{
			row++;
			screenx = 0;
			screeny += ascent - descent;
		}
		else
		{
			// get bounding box for character (may be offset to account for chars that dip above or below the line
			int c_x1, c_y1, c_x2, c_y2;
			stbtt_GetCodepointBitmapBox(&m_stbFontInfo, current, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
			// compute y (different characters have different heights
			int y = screeny + ascent + c_y1;

			if (y < 0)
			{
				y = 0;
			}
        
			// render character (stride and offset is important here)
			int byteOffset = screenx + (y  * m_width);
			stbtt_MakeCodepointBitmap(&m_stbFontInfo, m_pTempTextMemory + byteOffset, c_x2 - c_x1, c_y2 - c_y1, m_width, scale, scale, current );
        
			// how wide is this character 
			int advance;
			stbtt_GetCodepointHMetrics(&m_stbFontInfo, current, &advance, 0);
			screenx += (int)( advance * scale );
        
			// add kerning
			int kern;
		
			if( text[i + 1] )
			{
				BtChar next = text[i + 1];
				kern = stbtt_GetCodepointKernAdvance(&m_stbFontInfo, current, next );
				screenx += (int)( kern * scale );
			}
			else
			{
				screenx -= (int)(advance * scale);
				screenx += (c_x2 - c_x1)*scale;
			}

			int candidateMaxX = screenx;
			int candidateMaxY = y;

			v2Dimension.x = MtMax( v2Dimension.x, (BtFloat)candidateMaxX);
			v2Dimension.y = MtMax( v2Dimension.y, (BtFloat)candidateMaxY);
		}
    }
	trueTypeDimension.m_ascent = ascent;
	trueTypeDimension.m_v2Dimension = v2Dimension;

	m_pTextTexture->GetTexture(0)->WriteMemory((BtU8*)m_pTempTextMemory, m_width * m_height );
}
*/