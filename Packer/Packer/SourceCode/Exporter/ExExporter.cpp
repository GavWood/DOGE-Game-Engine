////////////////////////////////////////////////////////////////////////////////
// ExExporter

// Includes
#include <string.h>
#include "BtTypes.h"
#include "BaArchive.h"
#include "BaArchive.h"
#include "ExExporter.h"
#include "ExFont.h"
#include "ExSprites.h"
#include "ExStrings.h"
#include "ExTexture.h"
#include "ExSound.h"
#include "ExShader.h"
#include "ExMaterial.h"
#include "ExFlash.h"
#include "ExUserData.h"
#include "FsFindFile.h"
#include "ErrorLog.h"
#include "BaUserData.h"
#include "PaTopState.h"
#include "ExScene.h"

////////////////////////////////////////////////////////////////////////////////
// Declarations
ExExporter exporter;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExExporter::ExExporter()
{
	m_isSaveTextures = BtTrue;
	m_isPackTextures = BtTrue;
	// m_bPackTextures = BtFalse;
}

BtBool IsImage( const BtChar *extension )
{
    if( BtStrCompare( "jpg", extension ) )
    {
        return BtTrue;
    }
    if( BtStrCompare( "png", extension ) )
    {
        return BtTrue;
    }
    if( BtStrCompare( "bmp", extension ) )
    {
        return BtTrue;
    }
    if( BtStrCompare( "dds", extension ) )
    {
        return BtTrue;
    }
    return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// SetResourceType

BtBool ExExporter::SetResourceType( const PaFileDetails &file, BaResourceType& eResourceType )
{
    // Pack some userdata
	eResourceType = BaRT_NotSet;
    
    //printf ("extension = %s\n", file.m_szExtension );
    
	if( BtStrCompare( file.m_szFolder, "shaders" ) == BtTrue )
	{
		// Pack a sound
		eResourceType = BaRT_Shader;

		if( PaTopState::Instance().GetPlatform() == PackerPlatform_WinDX )
		{
			if ( BtStrCompare("dx11", file.m_szExtension))
			{
				return BtTrue;
			}
		}
		else if( PaTopState::Instance().GetPlatform() == PackerPlatform_GLES )
		{
			if( BtStrCompare( "gles", file.m_szExtension ) )
			{
				return BtTrue;
			}
		}
		else if(PaTopState::Instance().GetPlatform() == PackerPlatform_WinGL )
		{
			if( BtStrCompare("wingl", file.m_szExtension))
			{
				return BtTrue;
			}
		}
		else if(PaTopState::Instance().GetPlatform() == PackerPlatform_OSX )
		{
			if( BtStrCompare("osx", file.m_szExtension))
			{
				return BtTrue;
			}
		}
	}
	else if( BtStrCompare( file.m_szFolder, "flash" ) == BtTrue )
	{
		// Pack a sound
		eResourceType = BaRT_Flash;

		if( strstr( ".flash", file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "sounds" ) == BtTrue )
	{
		// Pack a sound
		eResourceType = BaRT_Sound;

		if( strstr( "wav", file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "userinterface" ) == BtTrue )
	{
		// Pack a texture
		eResourceType = BaRT_Input;

		if (BtStrCompare("ttf", file.m_szExtension))
		{
			return BtTrue;
		}
		else if( BtStrCompare( "txt", file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "sprites" ) == BtTrue )
	{
		// Pack a texture
		eResourceType = BaRT_Sprite;

		if( IsImage( file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "textures" ) == BtTrue )
	{
		// Pack a texture
		eResourceType = BaRT_Texture;

		if( IsImage( file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "materials" ) == BtTrue )
	{
		// Pack a texture
		eResourceType = BaRT_Material;

		if( BtStrCompare( ".mat", file.m_szExtension ) )
		{
			return BtTrue;
		}

		if( BtStrCompare( "material", file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "models" ) == BtTrue )
	{
		// Pack a texture
		eResourceType = BaRT_Scene;

		if( BtStrCompare( "dae", file.m_szExtension ) )
		{
			return BtTrue;
		}
		else if( BtStrCompare( "mqo", file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "strings" ) == BtTrue )
	{
		// Pack a string
		eResourceType = BaRT_Strings;

		if( BtStrCompare( "txt", file.m_szExtension ) )
		{
			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "fonts" ) == BtTrue )
	{
		if( BtStrCompare("fnt", file.m_szExtension) )
		{
			eResourceType = BaRT_Font;

			return BtTrue;
		}

		if( BtStrCompare( "ttf", file.m_szExtension) ||
			BtStrCompare( "otf", file.m_szExtension)
		  )
		{
			eResourceType = BaRT_Font;

			return BtTrue;
		}
	}
	else if( BtStrCompare( file.m_szFolder, "userdata" ) == BtTrue )
	{
		// Pack some userdata
		eResourceType = BaRT_UserData;

		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Compile resource

void ExExporter::compileResource( PaAsset* pAsset )
{
	if( pAsset->m_bDLL == BtTrue )
	{
		return;
	}

	// Clear the dependencies for this asset. These will be re-evaluated as the asset has changed.
	pAsset->m_dependencies.Empty();

	// Remove old dependencies
	pAsset->pArchive()->RemoveOldDependencies();

	// Set the current asset
	m_pCurrentAsset = pAsset;

	// Create the export class
	ExResource* pResource = BtNull;

	switch( pAsset->Type() )
	{
		case BaRT_Flash:
			{
				pResource = new ExFlash;
				ErrorLog::Printf( "Exporting flash" );
			}
			break;

		case BaRT_Strings:
			{
				pResource = new ExStrings;
				ErrorLog::Printf( "Exporting string" );
			}
			break;

		case BaRT_Sound:
			{
				ErrorLog::Printf( "Exporting sound");
				pResource = new ExSound;
			}
			break;

		case BaRT_Texture:
			{
				ErrorLog::Printf( "Exporting texture" );
				pResource = new ExTexture;
			}
			break;

		case BaRT_Font:
			{
				ErrorLog::Printf( "Exporting font" );
				pResource = new ExFont;
			}
			break;

		case BaRT_Sprite:
			{
				ErrorLog::Printf( "Exporting sprites" );
				pResource = new ExSprites;
			}
			break;

		case BaRT_Scene:
			{
				ErrorLog::Printf( "Exporting scene" );
				pResource = new ExScene;
			}
			break;

		case BaRT_UserData:
			{
				ErrorLog::Printf( "Exporting userdata" );
				pResource = new ExUserData;
			}
			break;

		case BaRT_Material:
			{
				ErrorLog::Printf( "Exporting material" );
				pResource = new ExMaterial;
			}
			break;


		case BaRT_Shader:
			{
				ErrorLog::Printf( "Exporting shader" );
				pResource = new ExShader;
			}
			break;
            
        default:
            ErrorLog::Fatal_Printf( "Cannot compile unknown resource" );
            break;
	}

	// Set the common variables
	pResource->nBaseResourceID( PaPacker::GlobalID() );

	// Display a message about the file
	ErrorLog::Printf( " %s\n", pResource->GetFilename() );

	// Create the resource
	pResource->Create();

	// Export the resource
	pResource->Export();

	// Destroy the resource
	pResource->Destroy();

	// Delete the resource
	delete pResource;
}
