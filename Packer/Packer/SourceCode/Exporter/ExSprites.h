////////////////////////////////////////////////////////////////////////////////
// ExSprites

#ifndef __ExSprites_h__
#define __ExSprites_h__

#include "BtTypes.h"
#include "TeTexture.h"
#include "PaPacker.h"
#include "ExResource.h"

struct SpritePosition
{
	BtU32 x;
	BtU32 y;

	BtU32 hyp();
};

inline BtU32 SpritePosition::hyp()
{
	return ( x * x ) + ( y * y );
}

class ExSprites : public ExResource
{
public:
	
	void							Export();

	void							PackItems( BaSprite* pItems, BtU32 nCount );

	void							PackTexture();

private:

	// Private functions
	BtBool							IsUsed(  BtS32 x, BtS32 y );
	void							SetUsed( BtS32 x, BtS32 y );

	BtBool							CheckFit(  BtS32& x, BtS32& y, BaSprite* pItem );
	BtBool							SetUsed( BtS32 x, BtS32 y, BaSprite* pItem );
	BtU32							GetColour( BtU32 x, BtU32 y );
	BtBool							IsBorderColour( BtS32 x, BtS32 y );
	void							Mask( BtS32 x, BtS32 y );

	// Members
	BtU32							m_nMaxWidth;
	BtU32							m_nMaxHeight;

	LBaSpriteFileData				m_spriteFileData;

	TeTexture						m_texture;				// Source texture
	TeTexture						m_packedTexture;		// Packed texture
	TeTexture						m_resizedTexture;		// Resized texture
	
	BtU32							m_packedWidth;
	BtU32							m_packedHeight;

	BtU32							m_nBiggestX;
	BtU32							m_nBiggestY;

	std::vector<SpritePosition>		m_spritePositions;
	BtU32							m_borderColour;

	BtU8*							m_pUsed;

	BtU32							m_flags;
};

#endif // __ExSprites_h__
