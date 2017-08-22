////////////////////////////////////////////////////////////////////////////////
// RsFlashWin32

// Includes
#include "RsUtil.h"
#include "RsFlashGLES.h"
#include "BaFileData.h"
#include "BtString.h"
#include "MtVector2.h"
#include "RsVertex.h"
#include "cJSON.h"
#include "BtTime.h"
#include "RsImplGLES.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void RsFlashWinGL::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	// Set the file data
	m_pFileData = (BaFlashData*) pFileData;

	BtU32 texturePages = m_pFileData->m_numTextures;

	for( BtU32 iTexture=0; iTexture<texturePages; iTexture++ )
	{
		// Set the texture
		m_pFileData->m_pTextures[iTexture] = (RsTexture*) pArchive->GetResource( m_pFileData->m_nTextures[iTexture] );
	}

	m_currentFrame = 0;
	m_currentFrameTime = 0;
	m_isLooping = BtTrue;
    m_isFinshed = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

//virtual
void RsFlashWinGL::CreateOnDevice()
{
	BtChar *pJSON = ( (BtChar*)( m_pFileData ) ) + sizeof(BaFlashData);

	m_pJSON = cJSON_Parse( pJSON );
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

//virtual 
void RsFlashWinGL::RemoveFromDevice()
{
	delete m_pJSON;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void RsFlashWinGL::Reset()
{
	m_currentFrameTime = 0;
    m_isFinshed = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsFinished

BtBool RsFlashWinGL::IsFinished()
{
    return m_isFinshed;
}

////////////////////////////////////////////////////////////////////////////////
// GetFrameNumber

BtU32 RsFlashWinGL::GetFrameNumber()
{
	return m_currentFrame;
}

////////////////////////////////////////////////////////////////////////////////
// SetLooping

void RsFlashWinGL::SetLooping( BtBool isLooping )
{
	m_isLooping = isLooping;
}

////////////////////////////////////////////////////////////////////////////////
// GetLooping

BtBool RsFlashWinGL::GetLooping()
{
	return m_isLooping;
}

////////////////////////////////////////////////////////////////////////////////
// Update

// virtual
void RsFlashWinGL::Update( BtFloat tick )
{
	// Cache the frames array
	cJSON *pFames = cJSON_GetObjectItem( m_pJSON , "frames" );

	// Cache the number of frames
	m_numFrames = cJSON_GetArraySize( pFames );

	// Increment the time
	m_currentFrameTime += tick;

	// Work out the current frame
	m_currentFrame = (BtU32)( m_currentFrameTime / BtTime::GetTick() );
 
	if( m_isLooping )
	{
		if( m_currentFrame > m_numFrames - 1 )
		{
			m_currentFrameTime = 0;
		}
	}
	else
	{
		if( m_currentFrame > m_numFrames - 1 )
		{
            m_isFinshed = BtTrue;
			m_currentFrame = m_numFrames - 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

// virtual
void RsFlashWinGL::Render( const MtVector2& v2StartPosition, BtU32 sortOrder )
{
	RsTexture *pTexture = BtNull;

	cJSON *pFames = cJSON_GetObjectItem( m_pJSON, "frames" );

	cJSON *pCommands = cJSON_GetArrayItem( pFames, m_currentFrame );

	if( pCommands == BtNull )
	{
		m_currentFrameTime = 0;
		
		m_currentFrame = 0;

		pCommands = cJSON_GetArrayItem( pFames, m_currentFrame );
	}

	RsColour colour( 1.0f, 1.0f, 1.0f, 1.0f );

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
			BtU32 textureIndex = pTextureIndex->valueint - 1;

			pTexture = m_pFileData->m_pTextures[textureIndex];

			colour = RsColour( 1.0f, 1.0f, 1.0f, 1.0f );
		}
		else if( BtStrCompare( pCommandString, "colour" ) )
		{
			// Get the colour object
			cJSON *pColour = cJSON_GetObjectItem( pObject, "colourArray" );

			cJSON *pR = cJSON_GetArrayItem( pColour, 0 );
			BtFloat r = (BtFloat)pR->valuedouble;

			cJSON *pG = cJSON_GetArrayItem( pColour, 1 );
			BtFloat g = (BtFloat)pG->valuedouble;

			cJSON *pB = cJSON_GetArrayItem( pColour, 2 );
			BtFloat b = (BtFloat) pB->valuedouble;

			colour = RsColour( r, g, b, 1.0f );
		}
		else if( BtStrCompare( pCommandString, "vertex" ) )
		{
			// Get the vertex object
			cJSON *pVertex = cJSON_GetObjectItem( pObject, "vertex" );

			// Cache the number of vertex
			BtU32 numVertex = cJSON_GetArraySize( pVertex );

			// Extract the vertex
			MtVector2 coords[4];

			for( BtU32 vertexIndex=0; vertexIndex<numVertex; vertexIndex++ )
			{
				// Cache each set of coordinates
				cJSON *pCoordinates = cJSON_GetArrayItem( pVertex, vertexIndex );

				cJSON *pX = cJSON_GetObjectItem( pCoordinates, "positionx" );
				cJSON *pY = cJSON_GetObjectItem( pCoordinates, "positiony" );

				coords[vertexIndex].x = (BtFloat)pX->valueint / 20;
				coords[vertexIndex].y = (BtFloat)pY->valueint / 20;
			}

			{
				// Cache the impl
				RsImplWinGL *pImpl = (RsImplWinGL*)RsImpl::pInstance();

				// Cache the render target
				RsRenderTarget *pRenderTarget = RsRenderTarget::GetCurrent();

				// Cache the camera
				RsCamera camera = pRenderTarget->GetCamera();

				// Cache the display width and height
				BtFloat Width  = (BtFloat)camera.GetViewport().m_width;
				BtFloat Height = (BtFloat)camera.GetViewport().m_height;

				// Cache the display width and height
				BtFloat textureWidth  = (BtFloat)pTexture->GetWidth();
				BtFloat textureHeight = (BtFloat)pTexture->GetHeight();

				// Cache the display width and height
				BtFloat fScaleWidth  = 1.0f / Width;
				BtFloat fScaleHeight = 1.0f / Height;

				// Allocate vertex
				RsVertex3 *pStartVertex = pImpl->StartVertex();

				// Set the start vertex
				RsVertex3 *pVertex = pStartVertex;

				RsVertex3 *pQuad = pVertex;

				BtFloat minU = 1.0f / textureWidth;
				BtFloat minV = 1.0f / textureHeight;
				BtFloat maxU = 1.0f - minU;
				BtFloat maxV = 1.0f - minV;

				/*
				coords[0].y = Height - coords[0].y;
				coords[1].y = Height - coords[1].y;
				coords[2].y = Height - coords[2].y;
				coords[3].y = Height - coords[3].y;
				*/

				// Copy these into vertex
				pVertex->m_v3Position = MtVector3(coords[0].x, coords[0].y, 0.1f );
				pVertex->m_v2UV = MtVector2( minU, minV );
				++pVertex;

				pVertex->m_v3Position = MtVector3(coords[1].x, coords[1].y, 0.1f );
				pVertex->m_v2UV = MtVector2( maxU, minV );
				++pVertex;

				pVertex->m_v3Position = MtVector3(coords[2].x, coords[2].y, 0.1f );
				pVertex->m_v2UV = MtVector2( minU, maxV );
				++pVertex;

				pVertex->m_v3Position = MtVector3(coords[3].x, coords[3].y, 0.1f );
				pVertex->m_v2UV = MtVector2( maxU, maxV );
				++pVertex;

				// Scale the position to local screen space -1 to 1
				for( BtU32 i=0; i<4; i++ )
				{
					// Set the colour
					pQuad[ i ].m_colour = colour.asARGB();

					// Use the start position
					pQuad[ i ].m_v3Position.x += v2StartPosition.x;
					pQuad[ i ].m_v3Position.y += v2StartPosition.y;

					// Flip the y
					pQuad[ i ].m_v3Position.y = Height - pQuad[ i ].m_v3Position.y;

					pQuad[ i ].m_v3Position.x -= 0.5f;
					pQuad[ i ].m_v3Position.y -= 0.5f;

					// Scale from 0..width to 0..1
					pQuad[ i ].m_v3Position.x *= fScaleWidth;
					pQuad[ i ].m_v3Position.y *= fScaleHeight;

					// Scale from 0..1 to 0..2
					pQuad[ i ].m_v3Position.x *= 2.0f;
					pQuad[ i ].m_v3Position.y *= 2.0f;

					// Translate from 0..2 to -1..1
					pQuad[ i ].m_v3Position.x -= 1.0f;
					pQuad[ i ].m_v3Position.y -= 1.0f;
				}

				// Setup the primitive
				RsPrimitiveWinGL *primitive = pImpl->AddPrimitive();
				primitive->m_primitiveType = GL_TRIANGLE_STRIP;
				primitive->m_numVertex     = 4;
				primitive->m_nStartVertex  = pImpl->GetCurrentVertex();

				// End the current vertex
				pImpl->EndVertex( 4 );

				// Make a new font renderable
				RsTextureRenderable *pTextureRenderable = pImpl->AddTexture();
				pTextureRenderable->m_pTexture  = (RsTextureWinGL*)pTexture;
				pTextureRenderable->m_pVertex   = pStartVertex;
				pTextureRenderable->m_primitive = primitive;

				// Validate the shader
				BtAssert( pTextureRenderable->m_pShader != BtNull );

				// Add the font to the renderable list
				RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();
				pCurrentRenderTarget->Add( sortOrder, pTextureRenderable );
			}
		}
	}		
}
