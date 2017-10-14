////////////////////////////////////////////////////////////////////////////////
// TeTexture.h

#pragma once
#include "BtTypes.h"
#include "BaArchive.h"
#include "BaFileData.h"
#include "BaFileData.h"
#include "RsColour.h"
#include "corona.h"

const BtU32 MaxMipMapLevels = 12;

struct TeMipMap
{
	LBaMipMapFileData	m_fileData;
};

class TeTexture
{
public:

	TeTexture();
	TeTexture( BtU32 flags );
	~TeTexture();

	// Public functions
	void				Create( BtU32 nWidth,
								BtU32 nHeight,
								BtU32 originalWidth,
								BtU32 originalHeight );

	void				SetPixel( BtU32 iMipmap, BtU32 x, BtU32 y, const RsColour& colour );
	void				LoadTextureFromFile( const BtChar* szFilename, BtU32 nMipMap );
	void				GenerateMipMaps();
	void				SetAlpha();
	void				Destroy();
	void				SaveToFile( BtChar* szFilename );

	void				Clear();

	void				Clear( BtU32 iMipMap, const RsColour& colour );

	// Get a colour at the location
	RsColour			GetColour( BtU32 iMipMap, BtU32 x, BtU32 y );

	// Set the colour at the location
	void				SetColour( BtU32 iMipMap, BtU32 x, BtU32 y, const RsColour& colour );

	// Accessors
	BtU32				Width( BtU32 iMipMap );
	BtU32				Height( BtU32 iMipMap );
	BtU32				Pitch( BtU32 iMipMap );
	BtU32				GetStride( BtU32 width, corona::PixelFormat pixelFormat, BtBool compressed );
	BtU32				GetTextureSize( BtU32 iMipMap );
	BtU8*				pTextureMemory( BtU32 iMipMap );

	BtU8*				GetResourceMemory();
	BtU32				GetResourceSize();

	void				SetInverted( BtBool isInverted );
	void				SetFlipped( BtBool isFlipped );
	void				SetMipmaps( BtBool hasMipmaps );

	BtBool				IsCompressed() const;
	BtBool				IsAlpha() const;
	BtBool				IsIndexed() const;
	BtBool				IsAlphaIndexed() const;
	BtBool				IsSquare() const;

private:

	void				SetFormat();

	friend class ExExporter;
	friend class ExSprites;
	friend class ExTexture;
	friend class ExMask;
	friend class ExFont;

	BtChar				m_szFilename[256];
	BtU8*				m_pResourceMemory;
	BtU32				m_nResourceSize;
	LBaTextureFileData*	m_pFileData;
	TeMipMap			m_mipMaps[MaxMipMapLevels];
	BtU32				m_nMipMaps;

	BtBool				m_isInvert;
	BtBool				m_isFlipped;
	BtBool				m_bUseMipmaps;

	BtU32				m_flags;
	BtBool				m_isAlpha;
	corona::PixelFormat	m_pixelFormat;
};

////////////////////////////////////////////////////////////////////////////////
// pMemory

inline BtU8* TeTexture::GetResourceMemory()
{
	return m_pResourceMemory;
}

////////////////////////////////////////////////////////////////////////////////
// GetResourceSize

inline BtU32 TeTexture::GetResourceSize()
{
	return m_nResourceSize;
}

////////////////////////////////////////////////////////////////////////////////
// SetInverted

inline void TeTexture::SetInverted( BtBool isInverted )
{
	m_isInvert = isInverted;
}

////////////////////////////////////////////////////////////////////////////////
// SetFlipped

inline void TeTexture::SetFlipped( BtBool isFlipped )
{
	m_isFlipped = isFlipped;
}

////////////////////////////////////////////////////////////////////////////////
// IsAlpha

inline BtBool TeTexture::IsAlpha() const
{
	return m_isAlpha;
}

////////////////////////////////////////////////////////////////////////////////
// bUseMipmaps

inline void TeTexture::SetMipmaps( BtBool hasMipmaps )
{
	m_bUseMipmaps = hasMipmaps;
}
