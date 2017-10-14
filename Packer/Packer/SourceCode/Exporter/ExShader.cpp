////////////////////////////////////////////////////////////////////////////////
// ExShader.cpp

// Includes
#include "ExShader.h"
#include "PaArchive.h"
#include "PaMemoryResource.h"
#include "PaTopState.h"
#include "PaRendering.h"
#include "FsFile.h"

////////////////////////////////////////////////////////////////////////////////
// Globals

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExShader::ExShader()
{
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExShader::Export()
{
    char FS = FsFile::GetFileSeparator();
    
	if( PaTopState::Instance().GetPlatform() == PackerPlatform_WinDX )
	{		
		PaMemoryResource resource(PMR_Write);

		BaShaderFileData shaderFileData;

		// Open the shader
		BtChar shaderPath[256];
		sprintf(shaderPath, "%s", GetFilename());
		FILE *f = fopen(shaderPath, "rb");
		if (f)
		{
			BtU32 currentIndex = 0;

			while (!feof(f))
			{
				BtChar vertex[32];
				if (!fgets(vertex, 32, f))
				{
					break;
				}
				BtChar frag[32];
				if (!fgets(frag, 32, f))
				{
					break;
				}

				BtStrTrimNewLines(frag, 0);
				BtStrTrimNewLines(vertex, 0);

				BtStrCopy(shaderFileData.m_shaders[currentIndex++].m_name, 32, vertex, 32);
				BtStrCopy(shaderFileData.m_shaders[currentIndex++].m_name, 32, frag, 32);
			}

			// Set the shader count
			shaderFileData.m_count = currentIndex;
			fclose(f);

			// Output the shader file data
			resource << shaderFileData;

			// Instead of outputting each shader. On the PC we can store them in a single FX file for convenience.
			// Let's just read this one in and output that

			for (BtU32 i = 0; i < shaderFileData.m_count; i++)
			{
				// Cache each shader
				BaShader &shaderData = shaderFileData.m_shaders[i];

				// Set the current offset
				shaderData.m_offset = resource.Tell();
			}

			// Add the FX file here
          	sprintf(shaderPath, "%s%c%s.fx", pPath(), FS, GetTitle() );
			PaMemoryResource shader(PMR_Read);
			shader.Load(shaderPath);
			AddDependency(shaderPath);

			// Output the shader
			resource << PaMemoryItem(shader.GetBase(), shader.GetCurrentSize());
			resource << BtU32(0);

			for (BtU32 i = 0; i < shaderFileData.m_count; i++)
			{
				// Cache each shader
				BaShader &shaderData = shaderFileData.m_shaders[i];

				// Set the size
				shaderData.m_size = shader.GetCurrentSize();
			}

			BaShaderFileData *pShaderFileData = (BaShaderFileData*)resource.GetBase();
			*pShaderFileData = shaderFileData;

			AddResource(resource, GetFilename(), GetTitle(), BaRT_Shader);
		}
	}
	else
	{
		PaMemoryResource resource(PMR_Write);
		
		BaShaderFileData shaderFileData;
		
		// Open the shader
		BtChar shaderPath[256];
		sprintf( shaderPath, "%s", GetFilename() );
		FILE *f = fopen( shaderPath, "rb" );
		if( f )
		{
			BtU32 currentIndex = 0;

			while( !feof( f ) )
			{
				BtChar vertex[32];
				if (!fgets(vertex, 32, f))
				{
					break;
				}
				BtChar frag[32];
				if( !fgets( frag, 32, f ) )
				{
					break;
				}

				BtStrTrimNewLines( frag, 0 );
				BtStrTrimNewLines( vertex, 0 );

				BtStrCopy( shaderFileData.m_shaders[currentIndex++].m_name, 32, vertex, 32 );
				BtStrCopy( shaderFileData.m_shaders[currentIndex++].m_name, 32, frag,   32 );
			}

			// Set the shader count
			shaderFileData.m_count = currentIndex;
			fclose(f);

			// Output the shader file data
			resource << shaderFileData;

			for( BtU32 i=0; i<shaderFileData.m_count; i++ )
			{
				// Cache each shader
				BaShader &shaderData = shaderFileData.m_shaders[i];

				// Set the current offset
				shaderData.m_offset = resource.Tell();

				sprintf(shaderPath, "%s%c%s", pPath(), FS, shaderData.m_name );
				PaMemoryResource shader(PMR_Read);
				shader.Load(shaderPath);

				AddDependency(shaderPath);
				shaderData.m_size = shader.GetCurrentSize();
			
				// Output the shader
				resource << PaMemoryItem(shader.GetBase(), shader.GetCurrentSize() );
				resource << BtU32(0);
	
				// Tell the length of each resource
				shaderData.m_size = shader.GetCurrentSize() + sizeof(BtU32);

				int a = 0;
				a++;
			}

			BaShaderFileData *pShaderFileData = (BaShaderFileData*)resource.GetBase();
			*pShaderFileData = shaderFileData;

			AddResource( resource, GetFilename(), GetTitle(), BaRT_Shader );
		}
	}
}
