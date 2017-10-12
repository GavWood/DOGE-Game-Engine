////////////////////////////////////////////////////////////////////////////////
// TeTexture.cpp

// Includes
#include "TeTexture.h"
#include "BaArchive.h"
#include "BtMemory.h"
#include "ErrorLog.h"
#include "PaPacker.h"
#include "PaTopState.h"
#include "UtTokeniser.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

TeTexture::TeTexture()
{
	m_isInvert = BtFalse;
	m_isFlipped = BtFalse;
	m_bUseMipmaps = BtFalse;
	m_pResourceMemory = BtNull;
	m_flags = RsTF_Square | RsTF_Alpha;
	m_isAlpha = BtFalse;
	m_pixelFormat = corona::PF_R8G8B8A8;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

TeTexture::TeTexture( BtU32 flags )
{
	m_isInvert = BtFalse;
	m_isFlipped = BtFalse;
	m_bUseMipmaps = BtFalse;
	m_pResourceMemory = BtNull;
	m_flags = flags;
	m_isAlpha = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

TeTexture::~TeTexture()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
// SetFormat

void TeTexture::SetFormat()
{
	if( IsCompressed() == BtTrue )
	{
		m_flags &= ~RsTF_Indexed;
		m_flags &= ~RsTF_IndexedAlpha;
	}

	if( m_flags & RsTF_PVR4 )
	{
		m_pixelFormat = corona::PF_R8G8B8A8;
	}
	else if( m_flags & ( RsTF_Indexed | RsTF_IndexedAlpha ) )
	{
		m_pixelFormat = corona::PF_I8;
	}
	else if( m_flags & RsTF_Alpha )
	{
		switch( PaTopState::Instance().GetPlatform() )
		{
		case PackerPlatform_WinDX:
			m_pixelFormat = corona::PF_B8G8R8A8;
			break;
		case PackerPlatform_WinGL:
			m_pixelFormat = corona::PF_R8G8B8A8;
			break;
		case PackerPlatform_GLES:
			m_pixelFormat = corona::PF_R8G8B8A8;
			break;
		case PackerPlatform_OSX:
			m_pixelFormat = corona::PF_R8G8B8A8;
			break;
        default:
            break;
        }
	}
	else
	{
		switch( PaTopState::Instance().GetPlatform() )
		{
		case PackerPlatform_WinDX:
			m_pixelFormat = corona::PF_B8G8R8A8;
			break;
		case PackerPlatform_OSX:
			m_pixelFormat = corona::PF_R8G8B8;
			break;
		case PackerPlatform_GLES:
			m_pixelFormat = corona::PF_R8G8B8;
			break;
		case PackerPlatform_WinGL:
			m_pixelFormat = corona::PF_R8G8B8;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Create

void TeTexture::Create( BtU32 nWidth,
					    BtU32 nHeight,
						BtU32 originalWidth,
						BtU32 originalHeight )
{
	SetFormat();

	BtU32 paletteSize = 0;

	if( m_flags & RsTF_Indexed )
	{
		paletteSize += 3 * 256;
	}
	else if( m_flags & RsTF_IndexedAlpha )
	{
		paletteSize += 4 * 256;
	}

	BtU32 iMipMapIndex = 0;

	for( ;; )
	{
		BtAssert( iMipMapIndex < MaxMipMapLevels );

		// Cache each mipmapc
		TeMipMap& mipMap = m_mipMaps[iMipMapIndex];

		// Cache the filedata
		LBaMipMapFileData& fileData = mipMap.m_fileData;

		BtU32 stride = GetStride( nWidth, m_pixelFormat, IsCompressed() );

		// Calculate the pitch
		fileData.m_nPitch = stride;

		// Set the pixel stride
		fileData.m_nBytesPerPixel = stride / nWidth;

		// Set the width
		fileData.m_nWidth = nWidth;

		// Set the height
		fileData.m_nHeight = nHeight;

		// Set the original width
		fileData.m_originalWidth = originalWidth;

		// Set the original height
		fileData.m_originalHeight = originalHeight;

		// Set the aspect ratio
		fileData.m_aspectRatio = (BtFloat)nWidth / (BtFloat)nHeight;

		// Calculate the texture size
		fileData.m_nTextureSize = fileData.m_nPitch * nHeight;

		// Increment the size
		if( !( m_flags & RsTF_RenderTarget ) )
		{
			paletteSize+= fileData.m_nTextureSize;
		}

		nWidth  = nWidth >> 1;
		nHeight = nHeight >> 1;

		++iMipMapIndex;

		if( ( nHeight == 8 ) || ( m_bUseMipmaps == BtFalse ) )
		{
			break;
		}
	}

	// Set the number of mipmaps
	m_nMipMaps = iMipMapIndex;

	// Set the size of the texture
	m_nResourceSize = sizeof(LBaTextureFileData) + ( sizeof(LBaMipMapFileData) * m_nMipMaps ) + paletteSize;

	// Allocate some memory
	m_pResourceMemory = BtMemory::Allocate( m_nResourceSize );

	// Calculate the offset to the texture data
	BtU32 nOffset = sizeof(LBaTextureFileData) + ( sizeof(LBaMipMapFileData) * m_nMipMaps );

	// Set the texture data pointers
	for( BtU32 iMipMapIndex=0; iMipMapIndex<m_nMipMaps; iMipMapIndex++ )
	{
		// Cache each mipmap
		TeMipMap& mipMap = m_mipMaps[iMipMapIndex];

		// Cache the filedata
		LBaMipMapFileData& fileData = mipMap.m_fileData;

		// Set the memory pointer for each mip map
		fileData.m_pTextureMemory = m_pResourceMemory + nOffset;

		// Move the memory on
		if( !( m_flags & RsTF_RenderTarget ) )
		{
			nOffset+= fileData.m_nTextureSize;
		}
	}

	BtU8* pMemory = m_pResourceMemory;

	// Cache the texture file data pointer
	m_pFileData = (LBaTextureFileData*) pMemory;

	// Set the number of mip maps
	m_pFileData->m_nMipMaps = m_nMipMaps;

	// Set the flags
	m_pFileData->m_flags = m_flags;

	// Increment the memory pointer
	pMemory+=sizeof(LBaTextureFileData);

	// Cache each mipmap
	TeMipMap& mipMap = m_mipMaps[0];
    (void)mipMap;
    
	LBaMipMapFileData* pMipMap = (LBaMipMapFileData*) pMemory;

	// Setup the texture file data
	for( iMipMapIndex=0; iMipMapIndex<m_nMipMaps; iMipMapIndex++ )
	{
		pMipMap[iMipMapIndex] = m_mipMaps[iMipMapIndex].m_fileData;
	}
}

BtBool TeTexture::IsCompressed() const
{
	if( m_flags & RsTF_PVR4 )
	{
		return BtTrue;
	}
	return BtFalse;
}

BtBool TeTexture::IsIndexed() const
{
	if( m_flags & RsTF_Indexed )
	{
		return BtTrue;
	}
	return BtFalse;
}

BtBool TeTexture::IsAlphaIndexed() const
{
	if( m_flags & RsTF_IndexedAlpha )
	{
		return BtTrue;
	}
	return BtFalse;
}

BtBool TeTexture::IsSquare() const
{
	if( m_flags & RsTF_Square )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetStride

BtU32 TeTexture::GetStride( BtU32 width, corona::PixelFormat pixelFormat, BtBool compressed )
{
	if( compressed )
	{
		return width / 2;
	}

	switch( m_pixelFormat )
	{
		case corona::PF_I8:
		{
			return width;
		}
		case corona::PF_B8G8R8A8:
		{
			return width * 4;
		}
		case corona::PF_R8G8B8A8:
		{
			return width * 4;
		}
		case corona::PF_R8G8B8:
		{
			return width * 3;
		}
        default:
        {
            // TODO
            int a=0;
            a++;
            return width;
        }
	}
	return width;
}

////////////////////////////////////////////////////////////////////////////////
// LoadTextureFromFile

void TeTexture::LoadTextureFromFile( const BtChar *szFilename, BtU32 nMipMaps )
{
	SetFormat();
    
    printf( "Filename - %s\n", szFilename );

	corona::Image* pImage = corona::OpenImage( szFilename, m_pixelFormat );
	if( pImage == BtNull )
	{
		ErrorLog::Fatal_Printf( "Could not load image %s\n", szFilename );
	}

	// Invert the image if required
	if( m_isInvert == BtTrue )
	{
		corona::FlipImage( pImage, corona::CA_X );
	}

	// Flip the image from left to right
	if( m_isFlipped == BtTrue )
	{
		corona::FlipImage( pImage, corona::CA_Y );
	}

	BtU32 originalWidth  = pImage->getWidth();
	BtU32 originalHeight = pImage->getHeight();
	BtU32 nWidth, height;

	// Calculate the dimensions
	if( m_flags & RsTF_Square )
	{
		nWidth  = BtNearestPowerOf2( originalWidth  );
		height = BtNearestPowerOf2( originalHeight );
	}
	else
	{
		nWidth  = originalWidth;
		height = originalHeight;
	}

	// Create the texture
	Create( nWidth, height, originalWidth, originalHeight );

	// Cache the mipmap
	TeMipMap& mipMap = m_mipMaps[0];

	// Copy the memory
	if( !( m_flags & RsTF_RenderTarget ) )
	{
		BtU8* pTexture = mipMap.m_fileData.m_pTextureMemory;

		if( m_flags & ( RsTF_Indexed | RsTF_IndexedAlpha ) )
		{
			corona::PixelFormat paletteFormat = pImage->getPaletteFormat();

			if( m_flags & RsTF_Indexed )
			{
				if( paletteFormat != corona::PF_B8G8R8 )
				{
					ErrorLog::Fatal_Printf( "Palette has alpha channel. Ensure tex file does indicates alpha.\r\n" );
				}
			}
			else if( m_flags & RsTF_IndexedAlpha )
			{
				if( paletteFormat != corona::PF_B8G8R8A8 )
				{
					ErrorLog::Fatal_Printf( "Palette has no alpha channel. Ensure tex file does not indicate alpha.\r\n" );
				}
			}

			BtU8* pPalette = (BtU8*)pTexture;

			BtU32 paletteSize = pImage->getPaletteSize();
			(void)paletteSize;

			BtU8* pPaletteIn = (BtU8*)pImage->getPalette();

			// Save out the palette
			for( BtU32 iPaletteIndex=0; iPaletteIndex<256; iPaletteIndex++ )
			{
				// Cache the components
				BtU8 blue   = *(pPaletteIn + 2 );
				BtU8 green  = *(pPaletteIn + 1 );
				BtU8 red    = *(pPaletteIn + 0 );

				switch( paletteFormat )
				{
					case corona::PF_B8G8R8:
					{
						*pPalette++ = blue;
						*pPalette++ = green;
						*pPalette++ = red;

						pPaletteIn += 3;
						break;
					}

					case corona::PF_B8G8R8A8:
					{
						BtU8 alpha  = *(pPaletteIn + 3 );

						*pPalette++ = blue;
						*pPalette++ = green;
						*pPalette++ = red;
						*pPalette++ = alpha;

						pPaletteIn += 4;
						break;
					}
				}
			}
			pTexture = pPalette;
		}

		// Calculate the original row stride
		BtU32 originalRowStride;

		BtU32 rowStride = GetStride( nWidth, m_pixelFormat, BtFalse );

		if( IsCompressed() == BtTrue )
		{
			BtU32 tempStride = GetStride( nWidth, corona::PF_R8G8B8A8, BtFalse );

			pTexture = BtMemory::Allocate( tempStride * height );

			originalRowStride = GetStride( originalWidth, m_pixelFormat, BtFalse );
			
			rowStride = tempStride;
		}
		else
		{
			originalRowStride = GetStride( originalWidth, m_pixelFormat, BtFalse );
		}

		// Copy the rows across to ensure we cope with a different row stride in the original and destination
		BtU8* pPixelSource = (BtU8*)pImage->getPixels();

		// Copy the top left corner if it's now a square texture
		// Otherwise this copies the full area
		BtU32 textureSize = rowStride * height;

		BtMemory::Set( pTexture, 0, textureSize);

		if( m_flags & RsTF_DXT )
		{
			int a=0;
			a++;
		}

		if( m_flags & RsTF_FloatingPoint32 )
		{
			// Save the data by copying across the rows (one at a time)
			for (BtU32 y = 0; y < originalHeight; y++)
			{
				for (BtU32 x = 0; x< originalWidth; x++)
				{
					// Get the colour at this location
					BtU32 colour = *( pPixelSource + (x * 4 ) );

					colour = colour & 0xff;

					if( colour > 0 )
					{
						int a=0;
						a++;
					}
					
					// Calculate the destination pixel
					BtFloat* pLocation = (BtFloat*)( pTexture + (x * 4) + (rowStride * y) );

					// Set the new floating point pixel
					*pLocation = (BtFloat)colour;
				}
				pPixelSource += originalRowStride;
			}

			//SaveToFile( "\\temp\\test.png" );
		}
		else
		{
			// Save the data by copying across the rows (one at a time)
			for( BtU32 y=0; y<originalHeight; y++ )
			{
				BtU8* pLocation = pTexture + ( rowStride * y );

				BtMemory::Copy( pLocation, pPixelSource, originalRowStride );

				pPixelSource += originalRowStride;
			}

			// Flag alpha
			SetAlpha();

			// Generate the mipmaps
			GenerateMipMaps();
		}

		if( IsCompressed() == BtTrue )
		{
			// Add compression code here
			int a=0;
			a++;
		}
	}

	// Delete the image
	delete pImage;
}

////////////////////////////////////////////////////////////////////////////////
// SetPixel

void TeTexture::SetPixel( BtU32 iMipmap, BtU32 x, BtU32 y, const RsColour& colour )
{
	// Set the colour at this pixel
	SetColour( iMipmap, x, y, colour );
}

////////////////////////////////////////////////////////////////////////////////
// SetAlpha

void TeTexture::SetAlpha()
{
	// Cache each mipmap
	TeMipMap& mipMap = m_mipMaps[0];

	BtU32 width  = mipMap.m_fileData.m_nWidth;
	BtU32 height = mipMap.m_fileData.m_nHeight;

	for( BtU32 u = 0; u < width; u++)
	{
		for( BtU32 v = 0; v < height; v++)
		{
			// Get the colour
			RsColour colour = GetColour( 0, u, v );

			if( colour.Alpha() < 1.0f )
			{
				m_isAlpha = BtTrue;
				return;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GenerateMipMaps

void TeTexture::GenerateMipMaps()
{
	for( BtU32 iMipMapIndex=1; iMipMapIndex<m_nMipMaps; iMipMapIndex++ )
	{
		// Cache each mipmap
		TeMipMap& mipMap = m_mipMaps[iMipMapIndex];

		BtU32 nParentMipMap = iMipMapIndex - 1;

		for( BtU32 u = 0; u < mipMap.m_fileData.m_nWidth; u++)
		{
			for( BtU32 v = 0; v < mipMap.m_fileData.m_nHeight; v++)
			{
				// Get the colour
				RsColour colour = GetColour( nParentMipMap, u * 2  , v * 2  )
                                + GetColour( nParentMipMap, u * 2 + 1, v * 2  )
                                + GetColour( nParentMipMap, u * 2  , v * 2 + 1)
                                + GetColour( nParentMipMap, u * 2 + 1, v * 2 + 1);

				// Average the colour
				colour *= 0.25f;

				// Set the colour
				SetColour( iMipMapIndex, u, v, colour );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void TeTexture::Destroy()
{
	// Free the memory
	if( m_pResourceMemory != BtNull )
	{
		BtMemory::Free( m_pResourceMemory );
		m_pResourceMemory = BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Width

BtU32 TeTexture::Width( BtU32 iMipMap )
{
	return m_mipMaps[iMipMap].m_fileData.m_nWidth;
}

////////////////////////////////////////////////////////////////////////////////
// Height

BtU32 TeTexture::Height( BtU32 iMipMap )
{
	return m_mipMaps[iMipMap].m_fileData.m_nHeight;
}

////////////////////////////////////////////////////////////////////////////////
// Pitch

BtU32 TeTexture::Pitch( BtU32 iMipMap )
{
	return m_mipMaps[iMipMap].m_fileData.m_nPitch;
}

////////////////////////////////////////////////////////////////////////////////
// pTextureMemory

BtU8* TeTexture::pTextureMemory( BtU32 iMipMap )
{
	return m_mipMaps[iMipMap].m_fileData.m_pTextureMemory;
}

////////////////////////////////////////////////////////////////////////////////
// TextureSize

BtU32 TeTexture::GetTextureSize( BtU32 iMipMap )
{
	return m_mipMaps[iMipMap].m_fileData.m_nTextureSize;
}

////////////////////////////////////////////////////////////////////////////////
// GetColour

RsColour TeTexture::GetColour( BtU32 iMipMap, BtU32 x, BtU32 y )
{
	TeMipMap& mipMap = m_mipMaps[iMipMap];

	BtAssert( x < mipMap.m_fileData.m_nWidth );
	BtAssert( y < mipMap.m_fileData.m_nHeight );

	BtU8* pTextureData = mipMap.m_fileData.m_pTextureMemory;

	// Calculate the address

    BtU8 nAlpha = 0, nRed = 0, nGreen = 0, nBlue = 0;

	if( m_pixelFormat == corona::PF_B8G8R8A8 )
	{
		BtU8 *pAddress = pTextureData + (x * 4) + (mipMap.m_fileData.m_nPitch * y);
		nBlue  = *(pAddress + 0 );
		nGreen = *(pAddress + 1 );
		nRed   = *(pAddress + 2 );
		nAlpha = *(pAddress + 3 );
	}
	else if( m_pixelFormat == corona::PF_R8G8B8A8 )
	{
		BtU8 *pAddress = pTextureData + (x * 4 ) + (mipMap.m_fileData.m_nPitch * y);
		nRed   = *(pAddress + 0 );
		nGreen = *(pAddress + 1 );
		nBlue  = *(pAddress + 2 );
		nAlpha = *(pAddress + 3 );
	}
	if( m_pixelFormat == corona::PF_R8G8B8 )
	{
		BtU8 *pAddress = pTextureData + (x * 3 ) + (mipMap.m_fileData.m_nPitch * y);
		nBlue  = *(pAddress + 0 );
		nGreen = *(pAddress + 1 );
		nRed   = *(pAddress + 2 );
		nAlpha = 255;
	}
	return RsColour( nRed, nGreen, nBlue, nAlpha );
}

////////////////////////////////////////////////////////////////////////////////
// SetColour

void TeTexture::SetColour( BtU32 iMipMap, BtU32 x, BtU32 y, const RsColour& colour )
{
	TeMipMap& mipMap = m_mipMaps[iMipMap];

	BtU8* pTextureData = mipMap.m_fileData.m_pTextureMemory;

	if( m_flags & RsTF_FloatingPoint1 )
	{
		// Calculate the address
		BtFloat *pAddress = (BtFloat*) ( pTextureData + ( x * sizeof(BtFloat) ) + ( mipMap.m_fileData.m_nPitch * y ) );

		*pAddress = colour.Red();
	}
	else
	{
		// Calculate the address
		BtU8 *pAddress = (BtU8*) ( pTextureData + ( x * sizeof(BtU32) ) + ( mipMap.m_fileData.m_nPitch * y ) );

		BtU8 nBlue  = (BtU8) ( colour.Blue()  * 255.0f );
		BtU8 nGreen = (BtU8) ( colour.Green() * 255.0f );
		BtU8 nRed   = (BtU8) ( colour.Red()   * 255.0f );
		BtU8 nAlpha = (BtU8) ( colour.Alpha() * 255.0f );

		if( m_pixelFormat == corona::PF_B8G8R8A8 )
		{
			*(pAddress + 0 ) = nBlue;
			*(pAddress + 1 ) = nGreen;
			*(pAddress + 2 ) = nRed;
			*(pAddress + 3 ) = nAlpha;
		}
		else if( m_pixelFormat == corona::PF_R8G8B8A8 )
		{
			*(pAddress + 0 ) = nRed;
			*(pAddress + 1 ) = nGreen;
			*(pAddress + 2 ) = nBlue;
			*(pAddress + 3 ) = nAlpha;
		}
		if( m_pixelFormat == corona::PF_R8G8B8 )
		{
			*(pAddress + 0 ) = nRed;
			*(pAddress + 1 ) = nGreen;
			*(pAddress + 2 ) = nBlue;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Clear

void TeTexture::Clear()
{
	for( BtU32 iMipMap=0; iMipMap<m_nMipMaps; iMipMap++ )
	{
		Clear( iMipMap, RsColour::BlackColour() );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Clear

void TeTexture::Clear( BtU32 iMipMap, const RsColour& clearColour )
{
	TeMipMap& mipMap = m_mipMaps[iMipMap];

	for( BtU32 x=0; x<mipMap.m_fileData.m_nWidth; x++ )
	{
		for( BtU32 y=0; y<mipMap.m_fileData.m_nHeight; y++ )
		{
			SetColour( iMipMap, x, y, clearColour );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// SaveToFile

void TeTexture::SaveToFile( BtChar* szFilename )
{
	corona::PixelFormat format = corona::PF_R8G8B8A8;

	// Cache each mipmapc
	TeMipMap& mipMap = m_mipMaps[0];

	// Cache the filedata
	LBaMipMapFileData& fileData = mipMap.m_fileData;

	// Create an image
	corona::Image* pImage = corona::CreateImage( mipMap.m_fileData.m_nWidth,
												 mipMap.m_fileData.m_nHeight,
												 format,
												 fileData.m_pTextureMemory );

	if( pImage == BtNull )
	{
		ErrorLog::Fatal_Printf( "Create image failed. Was your texture size a power of two?\n" );
	}

	// Save the texture to a file
	corona::SaveImage( szFilename, corona::FF_PNG, pImage );

	// Delete the image
	delete( pImage );
}
