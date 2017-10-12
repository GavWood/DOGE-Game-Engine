////////////////////////////////////////////////////////////////////////////////
// ExMaterial.cpp

#include "ExMaterial.h"
#include "ExSgNode.h"
#include "ExTexture.h"
#include "PaMemoryResource.h"
#include "PaArchive.h"
#include "PaTopState.h"
#include "UtTokeniser.h"
#include "FsFile.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExMaterial::ExMaterial()
{
	BtStrCopy( m_name, MaxMaterialName, GetTitle() );
	BtStrCopy( m_fileData.m_name, MaxMaterialName, m_name );
	m_fileData.m_diffuseColour = RsColour::WhiteColour();
	m_fileData.m_flags = 0;
	m_fileData.m_nSortOrder = 0;
	m_repeatU = 1;
	m_repeatV = 1;
	m_offsetU = 0;
	m_offsetV = 0;
	// Clear the textures
	for( BtU32 iTexture=0; iTexture<MaxTextures; ++iTexture )
	{
		m_textures[iTexture] = BtNull;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExMaterial::Export()
{
    char FS = FsFile::GetFileSeparator();
    
	BtChar TechniqueName[64];
	BtChar materialFilename[256];
	BtChar foundString[256];
	BtChar filename[MaxFilePath];
	BtBool isSortOrderSupplied = BtFalse;

	// Clear the technique name
	BtStrCopy( TechniqueName, 64, "" );

	// Set a default number of passes
	m_fileData.m_nPasses = 1;

	// Set the material filename
	sprintf( materialFilename, "%s%c%s.material", pPath(), FS, m_name );

	// Print the material name
	ErrorLog::Printf( "%s\n", materialFilename );

	UtTokeniser tokeniser;
	BtBool result;

	if( tokeniser.Read( materialFilename ) == BtTrue )
	{	
		if( tokeniser.GetToken( "alpha" ) == BtTrue )
		{
			ErrorLog::Fatal_Printf( "Texture flags must be placed in a .tex file" );
		}
		if( tokeniser.GetBool( "lit", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_Lit;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_Lit;
			}
		}

		if( tokeniser.GetNum( "sortorder", m_fileData.m_nSortOrder ) == BtTrue )
		{
			isSortOrderSupplied = BtTrue;
		}

		if( tokeniser.GetBool( "mipmaps", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_Mipmap;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_Mipmap;
			}
		}

		if( tokeniser.GetBool( "transparent", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_Transparent;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_Transparent;
			}
		}

		if( tokeniser.GetBool( "ztest", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_ZTest;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_ZTest;
			}
		}

		if( tokeniser.GetBool( "zwrite", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_ZWrite;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_ZWrite;
			}
		}
		
		if( tokeniser.GetBool( "cull", result ) == BtTrue )
		{
			if( result == BtFalse )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_DoubleSided;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_DoubleSided;
			}
		}

		if( tokeniser.GetBool( "sprite", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_Sprite;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_Sprite;
			}
		}

		if( tokeniser.GetBool( "environmentMap", result ) == BtTrue )
		{
			if( result == BtTrue )
			{
				m_fileData.m_flags = m_fileData.m_flags | RsMaterial_EnvironmentMap;
			}
			else
			{
				m_fileData.m_flags = m_fileData.m_flags & ~RsMaterial_EnvironmentMap;
			}
		}

		for( BtU32 textureIndex=0; textureIndex<8; textureIndex++ )
		{
			BtChar textureName[256];
			sprintf( textureName, "texture%d", textureIndex );

			if( tokeniser.GetString( textureName, foundString, 256 ) == BtTrue )
			{
				// Export the mask
				ExTexture* pTexture = new ExTexture( BtFalse );

				// Construct the texture filename
				sprintf( filename, "%s%c%s", pPath(), FS, foundString );
                
                //printf( "%s", filename );

				// Set the texture filename
				pTexture->GetFilename( filename );

				// Add the texture
				pTexture = m_textureList.Add( pTexture );

				// Set the texture pointer
				m_textures[textureIndex] = pTexture;
			}
		}
		tokeniser.GetString( "shader", TechniqueName, 256 );
	}

	// Create an output resource
	PaMemoryResource resource( PMR_Write );

	// Check it isn't a sprite
	if( ( m_fileData.m_flags & RsMaterial_Sprite ) == 0 )
	{
		// Check the diffuse colour
		if( DiffuseColour().Alpha() < 1.0f )
		{
			m_fileData.m_flags = m_fileData.m_flags | RsMaterial_Transparent;
		}
	}

	// Clear the texture array
	for(BtU32 iTexture = 0; iTexture < MaxTextures; iTexture++)
	{
		m_fileData.m_pTexture[iTexture] = BtNull;
		m_fileData.m_nTexture[iTexture] = BtNull;
	}

	// Map the textures and test for alpha in the texture
	for( BtU32 iTexture=0; iTexture<MaxTextures; iTexture++ )
	{
		// Cache each texture
		ExTexture* pTexture = m_textures[iTexture];

		if( pTexture != BtNull )
		{
			if( iTexture == SpecularTextureIndex )
			{
				m_fileData.m_flags |= RsMaterial_SpecularMap;
				m_fileData.m_flags |= RsMaterial_EnvironmentMap;
			}

			// Check it isn't a sprite
			if( ( m_fileData.m_flags & RsMaterial_Sprite ) == 0 )
			{
				if( pTexture->IsAlpha() == BtTrue )
				{
					m_fileData.m_flags = m_fileData.m_flags | RsMaterial_Transparent;
				}
			}
			m_fileData.m_nTexture[iTexture] = pTexture->GetResourceID();
		}
	}

	// Set the sort order to something high if it is not specified, but there is transparency
	if( ( isSortOrderSupplied == BtFalse ) && ( m_fileData.m_flags & RsMaterial_Transparent ) )
	{
		m_fileData.m_nSortOrder = 1;
	}

	if( ( BtStrLength( TechniqueName ) == 0 ) )
	{
		SetTechniqueName( TechniqueName );
	}

	if( BtStrCompare( TechniqueName, "RsShaderT" ) == BtTrue )
	{
		int a=0;
		a++;
	}

	// Set the technique name
	BtStrCopy( m_fileData.m_techniqueName, 64, TechniqueName );

	// Set the file data size
	m_fileData.m_nFileDataSize = sizeof( BaMaterialFileData );

	// Serialise the scene node
	resource << m_fileData;

	// Add the resource
	AddResource( resource, BtNull, m_name, BaRT_Material );
}

////////////////////////////////////////////////////////////////////////////////
// SetTechniqueName

void ExMaterial::SetTechniqueName( BtChar* pTechniqueName )
{
	// Set the technique name
	BtStrCopy( pTechniqueName, 64, "RsShader" );

	if( m_fileData.m_flags & RsMaterial_ZTest )
	{
		BtStrCat( pTechniqueName, 64, "Z" );
	}

	if( m_textures[LightMapIndex] == BtNull )
	{
		if( m_fileData.m_flags & RsMaterial_Lit )
		{
			BtStrCat( pTechniqueName, 64, "L" );
		}
	}

	if( m_textures[DiffuseTextureIndex] != BtNull )
	{
		BtStrCat( pTechniqueName, 64, "T" );
	}

	if( m_textures[LightMapIndex] != BtNull )
	{
		BtStrCat( pTechniqueName, 64, "M" );
	}

	if( m_fileData.m_flags & RsMaterial_Transparent )
	{
		BtStrCat( pTechniqueName, 64, "G" );
		m_fileData.m_nPasses = 3;
	}

	if( m_fileData.m_flags & RsMaterial_DoubleSided )
	{
		BtStrCat( pTechniqueName, 64, "D" );
	}

	if( m_fileData.m_flags & RsMaterial_Fog )
	{
		BtAssert(0);
	}

	if( m_fileData.m_flags & RsMaterial_BlendShape )
	{
		BtStrCat( pTechniqueName, 64, "B" );
	}

	if( m_fileData.m_flags & RsMaterial_Sprite )
	{
		BtStrCat( pTechniqueName, 64, "2" );
	}

	if( m_fileData.m_flags & RsMaterial_EnvironmentMap )
	{
		BtStrCat( pTechniqueName, 64, "E" );
	}

	if( m_fileData.m_flags & RsMaterial_SpecularMap )
	{
		BtStrCat( pTechniqueName, 64, "S" );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void ExMaterial::Destroy()
{
	m_textures.Empty();
}

////////////////////////////////////////////////////////////////////////////////
// SetTextureIndex

void ExMaterial::SetTextureIndex( BtU32 ID, ExTexture* pTexture )
{
	m_textures[ID] = pTexture;
}
