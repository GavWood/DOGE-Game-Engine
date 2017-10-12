////////////////////////////////////////////////////////////////////////////////
// ExTexture.cpp

// Includes
#include "ExTexture.h"
#include "TeTexture.h"
#include "PaArchive.h"
#include "UtTokeniser.h"
#include "PaTopState.h"
#include "FsFile.h"

//#include "ImageLibrary.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExTexture::ExTexture()
{
	m_isInverted = BtFalse;
	m_isFlipped = BtFalse;
	m_isMipmapped = BtFalse;
	m_fileData.m_flags = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExTexture::ExTexture( BtBool isInverted )
{
	m_fileData.m_flags = 0;
	m_isInverted = isInverted;
	m_isFlipped = BtFalse;
	m_isMipmapped = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExTexture::Export()
{
    char FS = FsFile::GetFileSeparator();
	PackerPlatform platform = PaTopState::Instance().GetPlatform();

	BtU32 flags = RsTF_Square | RsTF_Alpha;

	if( m_isMipmapped == BtTrue )
	{
		flags |= RsTF_Mipmap;
	}

	BtChar textureFilename[256];
	BtChar floatingPoint[256];

	// Set the material filename
	sprintf( textureFilename, "%s%c%s.tex", pPath(), FS, GetTitle() );

	if( strstr(textureFilename, "tile" ) )
	{
		int a=0;
		a++;
	}

	UtTokeniser tokeniser;

	if( tokeniser.Read( textureFilename ) == BtTrue )
	{
		// Add this as a dependency
		AddDependency( textureFilename );

		BtBool result;
		if( tokeniser.GetBool( "invert", result ) == BtTrue )
		{
			SetInverted( result );
		}
		if( tokeniser.GetBool( "flipped", result ) == BtTrue )
		{
			SetFlipped( result );
		}
		if( tokeniser.GetBool( "nearestneighbour", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_NearestNeighbour;
			}
		}
		if( tokeniser.GetBool( "cubemap", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_CubeMap;
			}
		}
		if( tokeniser.GetBool( "square", result ) == BtTrue )
		{
			if( result == BtFalse )
			{
				flags = flags & ~RsTF_Square;
			}
		}
		if( tokeniser.GetBool( "mipmaps", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_Mipmap;
			}
			else
			{
				flags = flags & ~RsTF_Mipmap;
			}
		}
		if( tokeniser.GetBool( "depthbuffer", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_DepthBuffer;
			}
		}
		if( tokeniser.GetBool( "alpha", result ) == BtTrue )
		{
			if( result == BtFalse )
			{
				flags = flags & ~RsTF_Alpha;
			}
		}
		ErrorLog::Printf( "Texture alpha = %d\n", flags );

		if( tokeniser.GetBool( "indexed", result ) == BtTrue )
		{
			flags = flags | RsTF_Indexed;
		}
		if( tokeniser.GetBool( "indexedalpha", result ) == BtTrue )
		{
			flags = flags | RsTF_IndexedAlpha;
		}
		if( tokeniser.GetBool( "compressed", result ) == BtTrue )
		{
			if( ( result == BtTrue ) && ( platform == PackerPlatform_GLES ) )
			{
				flags = flags | RsTF_PVR4;
			}
		}
		if( tokeniser.GetBool( "systemmemory", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_SystemMemory;
			}
		}
		if (tokeniser.GetBool("clamp", result) == BtTrue)
		{
			if (result == BtTrue)
			{
				flags = flags | RsTF_ClampU;
				flags = flags | RsTF_ClampV;
			}
		}
		if( tokeniser.GetBool( "rendertarget", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_RenderTarget;
			}
		}
		if( tokeniser.GetBool( "editable", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_Editable;
			}
		}
		if( tokeniser.GetBool( "backbuffersize", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				flags = flags | RsTF_BackBufferSize;
			}
		}
		if (tokeniser.GetBool("red", result) == BtTrue)
		{
			if (result == BtTrue)
			{
				flags = flags | RsTF_Red;
			}
		}
		if( tokeniser.GetBool( "dxt", result ) == BtTrue )
		{
			if (result == BtTrue)
			{
				flags = flags | RsTF_DXT;
			}
		}
		if( tokeniser.GetString( "floatingpoint", floatingPoint, 256 ) == BtTrue )
		{
			BtU32 precision;
			BtU32 channels;

			sscanf( floatingPoint, "%d %d", &precision, &channels );

			if( precision == 16 )
			{
				flags = flags | RsTF_FloatingPoint16;
			}
			else if( precision == 32 )
			{
				flags = flags | RsTF_FloatingPoint32;
			}

			switch( channels)
			{
				case 1:
					flags = flags | RsTF_FloatingPoint1;
					break;
				case 2:
					flags = flags | RsTF_FloatingPoint2;
					break;
				case 4:
					flags = flags | RsTF_FloatingPoint4;
					break;
			}
		}		
	}

	if (flags & RsTF_Alpha)
	{
		int a = 0;
		a++;
	}

	if (m_isWrapU)
	{
		flags = flags | RsTF_WrapU;
	}

	if (m_isWrapV)
	{
		flags = flags | RsTF_WrapV;
	}

	TeTexture texture( flags );

	// Set the invert flag
	texture.SetInverted( m_isInverted );

	// Set the flipped flag
	texture.SetFlipped( m_isFlipped );

	// Set the use mipmaps flag
	texture.SetMipmaps( flags & RsTF_Mipmap );

	// Cache the filename
	BtChar *filename = GetFilename();

	// Load the texture
	if( BtStrStr( filename, ".dds" ) )
	{
		int a=0;
		a++;
	}
	else
	{
		texture.LoadTextureFromFile( filename, 1 );

		// Set the is alpha flag
		m_isAlpha = texture.IsAlpha();

		// Create a resource
		PaMemoryResource resource( PMR_Write );

		// Add the texture data
		resource << PaMemoryItem( (BtU8*) texture.m_pResourceMemory, texture.m_nResourceSize );

		// Set the instance size
		AddResource( resource, GetFilename(), GetTitle(), BaRT_Texture );
	}
	int a=0;
	a++;
}
