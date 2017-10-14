////////////////////////////////////////////////////////////////////////////////
// ExFont

// Includes
#include <stdlib.h>
#include "BtTypes.h"
#include "BtMemory.h"
#include "BaArchive.h"
#include "ExFont.h"
#include "PaFileDetails.h"
#include "PaResource.h"
#include "TeTexture.h"
#include "PaTopState.h"
#include "BaUserData.h"

////////////////////////////////////////////////////////////////////////////////
// Example "Arial32.fnt"

// info face="Arial" size=32 bold=0 italic=0 charset="ANSI" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
// common lineHeight=32 base=26 scaleW=256 scaleH=256 pages=1 packed=0
// page id=0 file="Arial100_00.tga"
// page id=1 file="Arial100_01.tga"
// page id=2 file="Arial100_02.tga"
// page id=3 file="Arial100_03.tga"
// char id=32   x=0     y=0     width=1     height=0     xoffset=0     yoffset=32    xadvance=8     page=0  chnl=0 
// kerning first=32  second=65  amount=-2  
// kerning first=32  second=84  amount=-1  

//info face="Arial" size=20 bold=1 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1 outline=0
//common lineHeight=19 base=15 scaleW=256 scaleH=256 pages=1 packed=0 encoded=0
//page id=0 file="Arial20Bold_00.tga"
//chars count=210
//char id=32   x=254   y=68    width=1     height=0     xoffset=0     yoffset=19    xadvance=5     page=0  chnl=15

////////////////////////////////////////////////////////////////////////////////
// Export

//virtual
void ExFont::Export()
{
	BtChar *extension = GetExtension();

	if (strstr(extension, "ttf") || strstr(extension, "otf"))
	{
		ExportTTFFont();
	}
	else
	{
		ExportAngelCodeFont();
	}
}

////////////////////////////////////////////////////////////////////////////////
// ExportTTFFont

//virtual
void ExFont::ExportTTFFont()
{
	LBaFontFileData fontFileData;

// Add the raw font file to user data

	// Read the resource in
	PaMemoryResource memoryResource(PMR_Read);
	memoryResource.Load(GetFilename());

	BtU32 dataSize = memoryResource.GetCurrentSize();

	BtU32 nullWord = 0;

	// Write the resource out
	PaMemoryResource resource(PMR_Write);
	resource << dataSize;
	resource << PaMemoryItem(memoryResource.pCurrent(), dataSize);
	resource << nullWord;

	AddResource(resource, GetFilename(), GetTitle(), BaRT_UserData );

// Add the font instance

	// Clear the memory with zeros
	BtMemory::Set(&fontFileData, 0, sizeof(LBaFontFileData));
	fontFileData.m_isTTFFont = BtTrue;

	// Add the font file data
	PaMemoryResource font(PMR_Write);
	font << fontFileData;

	AddResource(font, GetFilename(), GetTitle(), BaRT_Font);

	int a = 0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// ExportAngelCodeFont

//virtual
void ExFont::ExportAngelCodeFont()
{
	BtChar *filename = GetFilename();

	LBaFontFileData fontFileData;
	
	// Clear the memory with zeros
	BtMemory::Set( &fontFileData, 0, sizeof(LBaFontFileData ) );

	BtChar szLine1[256];
	BtChar szLine2[256];
	TeTexture textures[MaxFontTexturePages];
	PaFileDetails fileDetails[MaxFontTexturePages];
	
	// Open the file
	FILE* f = fopen( filename, "r" );

	// Read the first two lines
	fgets( szLine1, 256, f );
	fgets( szLine2, 256, f );

	// Get the string for the line height
	BtChar* szLineHeight = strstr( szLine2, "common lineHeight=" ) + strlen("common lineHeight=");

	// Get the string for the number of pages
	BtChar* szPages = strstr( szLine2, "pages=" ) + strlen("pages=");

	// Set the text height
	fontFileData.m_nTextHeight = atoi( szLineHeight );

	// Set the number of texture pages
	fontFileData.m_nTexturePages = atoi( szPages );

	// Set the texture names
	for( BtU32 TextureIndex=0; TextureIndex<fontFileData.m_nTexturePages; TextureIndex++ )
	{
		// Cache each texture
		TeTexture& texture = textures[TextureIndex];

		// Get each page id line
		fgets( szLine1, 256, f );

		// Get the name of each texture
		BtChar* szTexture = strstr( szLine1, "file=" ) + strlen("file=\"");

		// Terminate the texture name before the quote \"
		*strstr(szTexture, "\"" ) = 0;

		fileDetails[TextureIndex] = PaFileDetails( pPath(), szTexture );

		// Load the texture
		texture.LoadTextureFromFile( fileDetails[TextureIndex].m_szFilename, 1 );
	}

	// Get each character's information
	fgets( szLine1, 256, f );

	while( !feof( f ) )
	{
		// Get each character's information
		fgets( szLine1, 256, f );

		if( !strstr( szLine1, "char id=" ) )
		{
			break;
		}

		// Find the char id
		BtU32 charId = atoi( strstr( szLine1, "char id=" ) + strlen( "char id=" ) );

		if( charId < 256 )
		{
			// Cache each font's letters
			LBaFontChar &fontCharacter = fontFileData.m_characters[charId];

			// Find x
			fontCharacter.m_U0 = (BtFloat) atoi( strstr( szLine1, "x=" ) + strlen( "x=" ) );

			// Find y
			fontCharacter.m_V0 = (BtFloat) atoi( strstr( szLine1, "y=" ) + strlen( "y=" ) );

			// Find width
			fontCharacter.m_fWidth = (BtFloat) atoi( strstr( szLine1, "width=" ) + strlen( "width=" ) );

			// Find height
			fontCharacter.m_fHeight = (BtFloat) atoi( strstr( szLine1, "height=" ) + strlen( "height=" ) );

			// Set the second u
			fontCharacter.m_U1 = fontCharacter.m_U0 + fontCharacter.m_fWidth;

			// Set the second v
			fontCharacter.m_V1 = fontCharacter.m_V0 + fontCharacter.m_fHeight;

			// Find xoffset
			fontCharacter.m_nXOffset = atoi( strstr( szLine1, "xoffset=" ) + strlen( "xoffset=" ) );

			// Find yoffset
			fontCharacter.m_nYOffset = atoi( strstr( szLine1, "yoffset=" ) + strlen( "yoffset=" ) );

			// Find x advance
			fontCharacter.m_nXAdvance = atoi( strstr( szLine1, "xadvance=" ) + strlen( "xadvance=" ) );

			// Find page
			fontCharacter.m_nPage = atoi( strstr( szLine1, "page=" ) + strlen( "page=" ) );
		}
	}	

	// Close the file
	fclose(f);

	for( BtU32 TextureIndex=0; TextureIndex<fontFileData.m_nTexturePages; TextureIndex++ )
	{
		PaMemoryResource textureResource(PMR_Write);

		// Cache each texture
		TeTexture& texture = textures[TextureIndex];

		// Add the texture data
		textureResource << PaMemoryItem( (BtU8*) texture.GetResourceMemory(), texture.m_nResourceSize );

		// Add a new resource
		AddResource( textureResource,
					 fileDetails[TextureIndex].m_szFilename,
				     fileDetails[TextureIndex].m_szTitle,
					 BaRT_Texture );

		// Set the texture IDs
		fontFileData.m_nTextures[TextureIndex] = m_nResourceID;
	}

	// Add the font file data
	PaMemoryResource font(PMR_Write);
	font << fontFileData;

	AddResource( font, GetFilename(), GetTitle(), BaRT_Font );
}
