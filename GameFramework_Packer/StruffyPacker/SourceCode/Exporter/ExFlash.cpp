////////////////////////////////////////////////////////////////////////////////
// ExFont

// Includes
#include <stdlib.h>
#include "BtTypes.h"
#include "BtMemory.h"
#include "ExFlash.h"
#include "PaFileDetails.h"
#include "PaResource.h"
#include "TeTexture.h"
#include "cJSON.h"

#include "BaArchive.h"
#include "RsFlash.h"

#include "FsFile.h"

////////////////////////////////////////////////////////////////////////////////
// Export

//virtual
void ExFlash::Export()
{
    char FS = FsFile::GetFileSeparator();
    
	BtU8 *pMemory = BtNull;

    // Open the file
	FILE *f = fopen( GetFilename(), "r" );

	// Seek the end of the file
	fseek( f, 0, SEEK_END );

	// Get the filesize
	BtU32 fileSize = ftell(f);

	// Seek the start of the file
	fseek( f, 0, 0 );

	// Allocate the memory
	pMemory = BtMemory::Allocate( fileSize );

	// Read the file into the memory
	fread( pMemory, 1, fileSize, f );

	// Close the file
	fclose(f);

	BtU32 maxTextures = 0;

	cJSON *pJSONBody = cJSON_Parse( (BtChar*)pMemory );

	if( pJSONBody )
	{
		cJSON *pFames = cJSON_GetObjectItem( pJSONBody, "frames" );

		BtU32 numFrames = cJSON_GetArraySize( pFames );

		for( BtU32 frame=0; frame<numFrames; frame++ )
		{
			cJSON *pCommands = cJSON_GetArrayItem( pFames, frame );

			BtU32 numCommands = cJSON_GetArraySize( pCommands );

			for( BtU32 command=0; command<numCommands; command=command + 2 )
			{
				// Cache the command
				cJSON *pCommand = cJSON_GetArrayItem( pCommands, command );
				BtChar *pCommandString = pCommand->valuestring;

				// Cache the object
				cJSON *pObject = cJSON_GetArrayItem( pCommands, command + 1 );

				if( BtStrCompare( pCommandString, "frameInfo" ) )
				{
					cJSON *pFrameIndex = cJSON_GetObjectItem( pObject, "frameIndex" );

					BtU32 frameIndex = pFrameIndex->valueint;
					(void)frameIndex;

					int a=0;
					a++;
				}
				else if( BtStrCompare( pCommandString, "texture" ) )
				{
					cJSON *pTextureIndex = cJSON_GetObjectItem( pObject, "textureIndex" );

					// Get the texture index
					BtU32 textureIndex = pTextureIndex->valueint;

					// Count the number of textures in this animation
					maxTextures = MtMax( maxTextures, textureIndex );
				}
			}
		}
	}

	BaFlashData flashData;

	BtMemory::Set( (void*)&flashData, 0, sizeof( BaFlashData ) );

	// Set the number of textures
	flashData.m_numTextures = maxTextures;

	for( BtU32 textureIndex=0; textureIndex<maxTextures; textureIndex++ )
	{
		PaMemoryResource textureResource(PMR_Write);

		// Cache each texture
		TeTexture texture( RsTF_Alpha );;

		BtChar title[256];
		sprintf( title, "%s_%03d", GetTitle(), textureIndex + 1 );

		BtChar filename[256];
		sprintf( filename, "%s%c%s.png", pPath(), FS, title );

		// Load the texture
		texture.LoadTextureFromFile( filename, 0 );

		// Add the texture data
		textureResource << PaMemoryItem( (BtU8*) texture.GetResourceMemory(), texture.GetResourceSize() );

		// Add a new resource
		AddResource( textureResource,
			filename,
			title,
			BaRT_Texture );

		// Set the texture IDs
		flashData.m_nTextures[textureIndex] = m_nResourceID;
	}

	// Add the font file data
	PaMemoryResource flashResource(PMR_Write);
	
	// Export the flash data
	flashResource << flashData;
	
	// Export the flash JSON
	flashResource << PaMemoryItem( pMemory, fileSize );;

	// Add the resource
	AddResource( flashResource, GetFilename(), GetTitle(), BaRT_Flash );

	// Free the memory
	BtMemory::Free( pMemory );
}
