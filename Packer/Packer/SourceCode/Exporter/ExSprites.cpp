////////////////////////////////////////////////////////////////////////////////
// ExSprites

// Includes
#include "BtTypes.h"
#include "BtMemory.h"
#include "BaArchive.h"
#include "TeTexture.h"
#include "ErrorLog.h"
#include "ExSprites.h"
#include "ExExporter.h"
#include "PaTopState.h"
#include "UtTokeniser.h"
#include "BtPrint.h"
#include "PaRendering.h"
#include "FsFile.h"

const BtU32 gap = 8;
const BtU32 doublegap = gap * 2;

////////////////////////////////////////////////////////////////////////////////
// GetColour

BtU32 ExSprites::GetColour( BtU32 x, BtU32 y )
{
	return m_texture.GetColour( 0, x, y ).asWord();
}

////////////////////////////////////////////////////////////////////////////////
// IsBorderColour

BtBool ExSprites::IsBorderColour( BtS32 x, BtS32 y )
{
	if( x < 0 )
	{
		return BtFalse;
	}
	if( y < 0 )
	{
		return BtFalse;
	}
	BtU32 colA = GetColour( x, y );
	BtU32 colB = m_borderColour;

	if( colA == colB )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Mask

void ExSprites::Mask( BtS32 x, BtS32 y )
{
	m_texture.SetColour( 0, x, y, RsColour::NoColour() );
}

////////////////////////////////////////////////////////////////////////////////
// Export

//virtual
void ExSprites::Export()
{
	char *filename = GetFilename();
	(void)filename;
	if( strstr( filename, "mouse" ) )
	{
		int a=0;
		a++;
	}

	// Get the border colour
	m_borderColour = (RsColour::RedColour() ).asWord();

	PackerPlatform platform = PaTopState::Instance().GetPlatform();

	m_flags = RsTF_Square | RsTF_Alpha;

	BtChar textureFilename[256];

	// Set the material filename
    char FS = FsFile::GetFileSeparator();
	sprintf( textureFilename, "%s%c%s.tex", pPath(), FS, GetTitle() );

	UtTokeniser tokeniser;
	BtBool result;

	if( tokeniser.Read( textureFilename ) == BtTrue )
	{
		// Add this as a dependency
		AddDependency( textureFilename );

		if( tokeniser.GetBool( "indexed", result ) == BtTrue )
		{
			if( ( result == BtTrue ) && ( platform == PackerPlatform_GLES ) )
			{
				m_flags = m_flags | RsTF_Indexed;
			}
		}
		if( tokeniser.GetBool( "indexedalpha", result ) == BtTrue )
		{
			if( ( result == BtTrue ) && ( platform == PackerPlatform_GLES ) )
			{
				m_flags = m_flags | RsTF_IndexedAlpha;
			}
		}
	}
	
	m_flags = m_flags & ~RsTF_Square;

	// Load the texture
	m_texture = TeTexture( m_flags );
	m_texture.LoadTextureFromFile( GetFilename(), 1 );

	BtU32 nNumSprites = 0;

	// Set the material filename
	BtChar spriteConfig[256];
	sprintf( spriteConfig, "%s%c%s.config", pPath(), FS, GetTitle() );

	BaSprite* pSprite = NULL;
	BtU32 rows, columns;

	if( tokeniser.Read( spriteConfig ) == BtTrue )
	{
		tokeniser.GetNum( "rows", rows );
		tokeniser.GetNum( "columns", columns );

		m_nMaxWidth  = m_texture.Width(0)  / columns;
		m_nMaxHeight = m_texture.Height(0) / rows;

		// Set the number of sprites
		m_spriteFileData.m_nNumSprites = columns * rows;

		// Set the number of masks
		m_spriteFileData.m_nNumMasks = 0;

		// Set the maximum sprite width
		m_spriteFileData.m_nMaxWidth = m_nMaxWidth;

		// Set the maximum sprite height
		m_spriteFileData.m_nMaxHeight = m_nMaxHeight;

		BtU32 xSpriteStart = 0;
		BtU32 ySpriteStart = 0;

		BtU32 index = 0;

		for( BtU32 y=0; y<rows; y++ )
		{
			xSpriteStart = 0;

			for( BtU32 x=0; x<columns; x++ )
			{
				// Find the sprite positions
				pSprite = &m_spriteFileData.m_sprites[index];

				pSprite->m_fX      = (BtFloat) xSpriteStart;
				pSprite->m_fY      = (BtFloat) ySpriteStart;
				pSprite->m_width   = (BtFloat) m_nMaxWidth;
				pSprite->m_height  = (BtFloat) m_nMaxHeight;

				pSprite->m_nLeft   = xSpriteStart;
				pSprite->m_nTop    = ySpriteStart;
				pSprite->m_nRight  = xSpriteStart + m_nMaxWidth;
				pSprite->m_nBottom = ySpriteStart + m_nMaxHeight;

				++index;

				xSpriteStart += m_nMaxWidth;
			}

			ySpriteStart += m_nMaxHeight;
		}

		PaMemoryResource textureResource(PMR_Write);

		// Copy the texture resource
		textureResource << PaMemoryItem( (BtU8*) m_texture.m_pResourceMemory, m_texture.m_nResourceSize );

		// Add the resource
		m_spriteFileData.m_nTexture = AddResource( textureResource, GetFilename(), GetTitle(), BaRT_Texture );

		PaMemoryResource resource(PMR_Write);

		// Copy the sprite information
		resource << m_spriteFileData;

		// Add the resource
		AddResource( resource, GetFilename(), GetTitle(), BaRT_Sprite );

		// Release the textures
		m_texture.Destroy();
	}
	else
	{
		RsColour colour = m_texture.GetColour( 0, 0, 0 );

		if( colour.Alpha() == 1.0f )
		{
			m_borderColour = m_texture.GetColour( 0, 0, 0 ).asWord();

			m_texture.SetColour( 0, 0, 0, RsColour::NoColour() );
		}
		else
		{
			ErrorLog::Printf("No border colour set for sprite.");
		}

		m_nMaxWidth  = 0;
		m_nMaxHeight = 0;

		// Find the red markers
		BtS32 x, y;
		BtS32 xSpriteStart = 0, ySpriteStart = 0;
		BtS32 nWidth  = (BtS32) m_texture.Width(0);
		BtS32 nHeight = (BtS32) m_texture.Height(0);
		BtS32 xStart = 0;
		BtS32 yStart = 0;
		BtS32 yEnd = -1;

		// Calculate the size of the height markers array
		BtU32 nSize = sizeof(BtU32) * nWidth;
        (void)nSize;
        
		BtBool isFound = BtFalse;

		for(;;)
		{
			// Set the maximum distance of the next red pixel
			BtU32 max = (nWidth * nWidth) + (nHeight * nHeight);

			// Make the nearest the maximum possible red pixel
			BtU32 nearest = max;

			// Get the start of the red column
			for( x=xStart; x<nWidth; x++ )
			{
				BtS32 searchUpToRow = nHeight;

				if( yEnd != -1 )
				{
					searchUpToRow = yEnd;
				}

				for( y=yStart; y<searchUpToRow; y++ )
				{
					if( IsBorderColour( x, y ) == BtTrue )
					{
						if( IsBorderColour( x - 1, y ) == BtTrue )
						{
							continue;
						}
						else if( IsBorderColour( x, y - 1 ) == BtTrue )
						{
							continue;
						}
						else if( IsBorderColour( x, y + 1 ) == BtFalse )
						{
							continue;
						}

						// Find the nearest
						BtU32 hyp = (x * x) + (y * y );

						if( hyp < nearest )
						{
							xSpriteStart = x;
							ySpriteStart = y;

							nearest = hyp;
						}
					}
				}
			}

			// Expand the search downwards
			yEnd = -1;

			if( nearest == max )
			{
				if( isFound == BtFalse )
				{
					break;
				}
				else
				{
					xStart = 0;
					isFound = BtFalse;
					continue;
				}
			}

			// Set the position of the nearest red pixel
			x = xSpriteStart;
			y = ySpriteStart;

		//	ErrorLog::Printf("Position %.2f %.2f\n", xSpriteStart, ySpriteStart );

			// Find the end of the red column
			for( ; y<nHeight; y++ )
			{
				if( IsBorderColour( x, y ) == BtFalse )
				{
					break;
				}
			}

			// Go back onto the column
			y--;

			// Verify the red row
			if( IsBorderColour( x, y ) == BtFalse )
			{
				ErrorLog::Fatal_Printf( "Border broke on sprite %d at %d,%d.\n", nNumSprites + 1, x + 1, y);
			}

			// Find the end of the red row
			for( ; x<nWidth; x++ )
			{
				if( IsBorderColour( x, y ) == BtFalse )
				{
					break;
				}
			}

			// Blank out the red border
			BtU32 x1 = xSpriteStart;
			BtU32 x2 = x;
			BtU32 y1 = ySpriteStart;		// Working from top to bottom. y1 start is first pixel in sprite
			BtU32 y2 = y;					// y2 is position of border line
	
			// Fill in the left y row with appropriate colours
			for (BtU32 newY = y1; newY < y2; newY++)
			{
				RsColour col = RsColour::GreenColour();
				col = m_texture.GetColour( 0, x1 + 1, newY);
				m_texture.SetColour( 0, x1, newY, col );
			}
			// Fill in the bottom horizontal row
			for (BtU32 newX = x1 + 1; newX < x2; newX++)
			{
				RsColour col = RsColour::GreenColour();
				col = m_texture.GetColour(0, newX, y2 - 1 );
				m_texture.SetColour(0, newX, y2, col);
			}
			// Fill in the right vertical row
			for (BtU32 newY = y1; newY < y2; newY++)
			{
				RsColour col = RsColour::GreenColour();
				col = m_texture.GetColour( 0, x2 - 1, newY);
				m_texture.SetColour(0, x2, newY, col);
			}
			// Fill in the top horizontal row
			for (BtU32 newX = x1 + 1; newX < x2; newX++)
			{
				RsColour col = RsColour::GreenColour();
				col = m_texture.GetColour(0, newX, y1 );
				m_texture.SetColour(0, newX, y1-1, col);
			}
			// Do the four corners
			RsColour col = RsColour::BlueColour();
			col = m_texture.GetColour(0, x1 + 1, y1 );
			m_texture.SetColour(0, x1, y1-1, col);
	
			col = m_texture.GetColour(0, x1 + 1, y2 - 1);
			m_texture.SetColour(0, x1, y2, col);
			
			col = m_texture.GetColour(0, x2 - 1, y1 );
			m_texture.SetColour(0, x2, y1-1, col);
			
			col = m_texture.GetColour(0, x2 - 1, y2 - 1);
			m_texture.SetColour(0, x2, y2, col);

			// Increment the start location
			xSpriteStart++;

			// Cache the current sprite or mask
			pSprite = &m_spriteFileData.m_sprites[nNumSprites];
			nNumSprites++;

			// Calculate the width and height of the sprite
			BtU32 nSpriteHeight = y - ySpriteStart;
			BtU32 nSpriteWidth  = x - xSpriteStart;

			// Set the details
			pSprite->m_fX      = (BtFloat) xSpriteStart;
			pSprite->m_fY      = (BtFloat) ySpriteStart;
			pSprite->m_width   = (BtFloat) nSpriteWidth;
			pSprite->m_height  = (BtFloat) nSpriteHeight;

			pSprite->m_nLeft   = xSpriteStart;
			pSprite->m_nTop    = ySpriteStart;
			pSprite->m_nRight  = xSpriteStart + nSpriteWidth;
			pSprite->m_nBottom = ySpriteStart + nSpriteHeight;

			// Keep a track of the maximum width
			if( nSpriteWidth > m_nMaxWidth )
			{
				m_nMaxWidth  = nSpriteWidth;
			}

			// Keep a track of the maximum height
			if( nSpriteHeight > m_nMaxHeight )
			{
				m_nMaxHeight = nSpriteHeight;
			}

			// Set a flag for the stopping case
			isFound = BtTrue;

			// Set the new x starting position
			xStart = xSpriteStart + nSpriteWidth + 1;
			yStart = 0;
			yEnd   = ySpriteStart + nSpriteHeight;
		}

		if( nNumSprites == 0 )
		{
			ErrorLog::Fatal_Printf( "No sprites found. Place a pixel in the top left indicating the boundary colour or ensure your boundaries are red (255, 0, 0, 255)." );
		}

		// Set the number of sprites
		m_spriteFileData.m_nNumSprites = nNumSprites;

		// Set the number of masks
		m_spriteFileData.m_nNumMasks = 0;

		// Set the maximum sprite width
		m_spriteFileData.m_nMaxWidth = m_nMaxWidth;

		// Set the maximum sprite height
		m_spriteFileData.m_nMaxHeight = m_nMaxHeight;

		// Pack the texture
		PackTexture();

		PaMemoryResource textureResource(PMR_Write);

		// Copy the texture resource
		textureResource << PaMemoryItem( (BtU8*) m_resizedTexture.m_pResourceMemory, m_resizedTexture.m_nResourceSize );

		// Add the resource
		m_spriteFileData.m_nTexture = AddResource( textureResource, GetFilename(), GetTitle(), BaRT_Texture );

		PaMemoryResource resource(PMR_Write);

		// Copy the sprite information
		resource << m_spriteFileData;

		// Add the resource
		AddResource( resource, GetFilename(), GetTitle(), BaRT_Sprite );

		// Do we save the textures out
		//if( ExExporter::Instance().saveTextures() == BtTrue )
        BtBool isExport = BtFalse;
        if( isExport )
		{
            /*
			BtChar szFilename[256];
			sprintf( szFilename, "\\temp\\%s_packed.%s", GetTitle(), GetExtension() );
			m_packedTexture.SaveToFile( szFilename );

			sprintf( szFilename, "\\temp\\%s_resized.%s", GetTitle(), GetExtension() );
			m_resizedTexture.SaveToFile( szFilename );
            */
		}

//		for( BtU32 i=0; i<nNumSprites; i++ )
		//{
			//ErrorLog::Printf( "Dimension %.2f %.2f\n", m_spriteFileData.m_sprites[i].m_width, m_spriteFileData.m_sprites[i].m_height );
		//}

		// Display the number of sprites
		ErrorLog::Printf( "Number of sprites %d packed to %d %d\r\n", nNumSprites, m_resizedTexture.Width(0), m_resizedTexture.Height(0) );

		// Release the texture
		m_texture.Destroy();
		m_packedTexture.Destroy();
		m_resizedTexture.Destroy();
	}
}

////////////////////////////////////////////////////////////////////////////////
// CheckFit

BtBool ExSprites::CheckFit( BtS32& x, BtS32& y, BaSprite* pItem )
{
	BtU32 width  = (BtU32)pItem->m_width  + doublegap;
	BtU32 height = (BtU32)pItem->m_height + doublegap;

	// Check the fit at this point
	for( BtU32 checkx=0; checkx<width; checkx++ )
	{
		for( BtU32 checky=0; checky<height; checky++ )
		{
			if( IsUsed( x + checkx, y + checky ) == BtTrue )
			{
				x += checkx + 1;
				return BtFalse;
			}
		}
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// SetUsed

BtBool ExSprites::SetUsed( BtS32 x, BtS32 y, BaSprite* pItem )
{
	BtU32 width  = (BtU32)pItem->m_width  + doublegap;
	BtU32 height = (BtU32)pItem->m_height + doublegap;

	// Check the fit at this point
	for( BtU32 setx=0; setx<width; setx++ )
	{
		for( BtU32 sety=0; sety<height; sety++ )
		{
			SetUsed( x + setx, y + sety );
		}
	}
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// IsUsed

BtBool ExSprites::IsUsed( BtS32 x, BtS32 y )
{
	if( x < 0 )
	{
		BtAssert(0);
	}

	BtU8* pAddress = m_pUsed + x + ( y * m_packedWidth );
	
	if( *pAddress == 1 )
	{
		return BtTrue;
	}
	else
	{
		return BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetUsed

void ExSprites::SetUsed( BtS32 x, BtS32 y )
{
	BtAssert( x >= 0 );
	BtAssert( y >= 0 );
	BtAssert( x < (BtS32)m_packedWidth );
	BtAssert( y < (BtS32)m_packedHeight );

	if( IsUsed( x, y ) == BtTrue )
	{
		int a=0;
		a++;
	}
	BtU8* pAddress = m_pUsed + x + ( y * m_packedWidth );
	*pAddress = 1;
}

////////////////////////////////////////////////////////////////////////////////
// PackItems

void ExSprites::PackItems( BaSprite* pItems, BtU32 numSprites )
{
	// Cache the packed with and packed height
	m_packedWidth = m_packedTexture.Width(0);
	m_packedHeight = m_packedTexture.Height(0);

	// Make a blank area to mark the used texture
	BtU32 size = m_packedWidth * m_packedHeight;
	m_pUsed = BtMemory::Allocate( size );

	BtMemory::Set( m_pUsed, 0, size );

	BtS32 nStartX;
	BtS32 nStartY;

	// Loop through the sprites
	for( BtU32 nIndex=0; nIndex<numSprites; nIndex++ )
	{	
		// Cache each sprite
		BaSprite& item = pItems[nIndex];

		BtU32 nSpriteWidth = (BtU32) item.m_width;
		BtU32 nSpriteHeight = (BtU32) item.m_height;

		if( ExExporter::Instance().packTextures() == BtTrue )
		{
			m_spritePositions.clear();

			BtS32 x, y;

			BtS32 startWidth = m_packedWidth  - ( nSpriteWidth + doublegap );
			BtS32 endHeight  = m_packedHeight - ( nSpriteHeight + doublegap );

			y = 0;

			// Find the first free space
			while( y < endHeight )
			{
				x = 0;

				while( x < startWidth )
				{
					BtBool bFit = CheckFit( x, y, &item );

					if( bFit == BtTrue )
					{
						SpritePosition position;
						position.x = x;
						position.y = y;
						m_spritePositions.push_back( position );
						break;
					}
				}
				++y;
			}

			// Cache the number of sprite positions
			BtU32 spritePositions = (BtU32)m_spritePositions.size();

			BtAssert( spritePositions != 0 ); 

			// Find the best location so we make a small square of sprites
			BtU32 nHyp = m_spritePositions[0].hyp();
			nStartX = m_spritePositions[0].x;
			nStartY = m_spritePositions[0].y;

			for( BtU32 nBestIndex=1; nBestIndex<spritePositions; nBestIndex++ )
			{
				BtU32 nNewHyp = m_spritePositions[nBestIndex].hyp();

				if( nNewHyp < nHyp )
				{
					nHyp = nNewHyp;
					nStartX = m_spritePositions[nBestIndex].x;
					nStartY = m_spritePositions[nBestIndex].y;
				}
			}

			int a=0;
			a++;
		}
		else
		{
			nStartX = item.m_nLeft;
			nStartY = item.m_nTop;
		}

		BtBool bFit = CheckFit( nStartX, nStartY, &item );
		if( bFit == BtFalse )
		{
			int a=0;
			a++;
		}

		SetUsed( nStartX, nStartY, &item );

		// Put it in the center of the space
		nStartX += gap;
		nStartY += gap;

		if (nStartX < 4)
		{
			ErrorLog::Fatal_Printf("Sprites will need a border of their own colour in the atlas to stop artifacts from bilinear filtering");
		}
		if (nStartY < 4)
		{
			ErrorLog::Fatal_Printf("Sprites will need a border of their own colour in the atlas to stop artifacts from bilinear filtering");
		}

		// Copy the sprite but copy a four pixel border with it as well
		BtU32 s = 4;
		BtU32 nToX = nStartX - s;
		BtU32 nToY = nStartY - s;

		BtU32 fromX = item.m_nLeft - s;
		BtU32 fromY = item.m_nTop - s;

		BtU32 width  = (BtU32)(item.m_width)  + s + s;
		BtU32 height = (BtU32)(item.m_height) + s + s;

		// Copy each sprite to the best place
		for( BtU32 x=0; x<width; x++ )
		{
			for( BtU32 y=0; y<height; y++ )
			{
				BtU32 nDestX = nToX + x;
				BtU32 nDestY = nToY + y;

				// Get the colour in the non packed texture
				RsColour colour = m_texture.GetColour( 0, fromX + x , fromY + y );

				// Set the colour in the packed texture
				m_packedTexture.SetColour( 0, nDestX, nDestY, colour );

				//m_packedTexture.SetColour( 0, nDestX, nDestY, RsColour::BlackColour() );
			}
		}
		/*
		// Copy each sprite to the best place
		for (BtU32 x = 0; x < item.m_width; x++)
		{
			for (BtU32 y = 0; y < item.m_height; y++)
			{
				BtU32 nDestX = nStartX + x;
				BtU32 nDestY = nStartY + y;

				// Get the colour in the non packed texture
				RsColour colour = m_texture.GetColour(0, item.m_nLeft + x, item.m_nTop + y);

				// Set the colour in the packed texture
				m_packedTexture.SetColour(0, nDestX, nDestY, colour);

				//m_packedTexture.SetColour( 0, nDestX, nDestY, RsColour::BlackColour() );
			}
		}
		*/
		// Dump out the progress
        BtBool isExport = BtFalse;
        if( isExport )
        {
            /*
            static BtU32 index = 0;
            BtChar test[256];
            sprintf( test, "\\temp\\image%d.png", index );
            if( index >= 4 )
            {
                m_packedTexture.SaveToFile( test );
            }
            ++index;
            */
        }
        
		// Set the packed sprite positions
		item.m_nLeft = nStartX;
		item.m_nTop  = nStartY;
		item.m_fX = (BtFloat) nStartX;
		item.m_fY = (BtFloat) nStartY;
		item.m_nRight  = nStartX + nSpriteWidth;
		item.m_nBottom = nStartY + nSpriteHeight;

		// Record the biggest dimensions
		m_nBiggestX = MtMax( m_nBiggestX, item.m_nRight );
		m_nBiggestY = MtMax( m_nBiggestY, item.m_nBottom  );
	}

	// Free the used texture
	BtMemory::Free( m_pUsed );
}

////////////////////////////////////////////////////////////////////////////////
// PackTexture

void ExSprites::PackTexture()
{
	m_nBiggestX = 0;
	m_nBiggestY = 0;

	// Set the number of mip maps to 1
	m_packedTexture.SetMipmaps( BtFalse );

	// Create a packed texture
	BtU32 width  = m_texture.Width(0)  * 2;
	BtU32 height = m_texture.Height(0) * 2;

	m_packedTexture.Create( width, height, width, height );
	//m_packedTexture.Clear( 0, RsColour::NoColour() );
	m_packedTexture.Clear( 0, RsColour::GreenColour() );

	PackItems( m_spriteFileData.m_sprites, m_spriteFileData.m_nNumSprites );
//	PackItems( m_spriteFileData.m_masks, m_spriteFileData.m_nNumMasks );

	// Calculate the size of the next texture
	//BtU32 nTextureWidth  = BtNearestPowerOf2( m_nBiggestX );
	//BtU32 nTextureHeight = BtNearestPowerOf2( m_nBiggestY );
	BtU32 nTextureWidth  = m_nBiggestX + 32;
	BtU32 nTextureHeight = m_nBiggestY + 32;

	m_resizedTexture = TeTexture( m_flags );

	// Set the number of mip maps to 1
	m_resizedTexture.SetMipmaps( BtFalse );

	// Create the final texture
	m_resizedTexture.Create( nTextureWidth,
							 nTextureHeight,
							 nTextureWidth,
							 nTextureHeight );

	// Copy the final texture
	for( BtU32 x=0; x<nTextureWidth; x++ )
	{
		for( BtU32 y=0; y<nTextureHeight; y++ )
		{
			// Get each colour
			if( ( x < width ) && ( y < height ) )
			{
				const RsColour& colour = m_packedTexture.GetColour( 0, x, y );

				// Set each colour
				m_resizedTexture.SetColour( 0, x, y, colour );
			}
			else
			{
				m_resizedTexture.SetColour( 0, x, y, RsColour::NoColour() );
			}
		}
	}
}
