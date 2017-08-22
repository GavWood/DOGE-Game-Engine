////////////////////////////////////////////////////////////////////////////////
// RsSpriteWin32GL.cpp

// Includes
#include "RsSpriteGLES.h"
#include "RsTextureGLES.h"
#include "RsPrimitiveGLES.h"
#include "RsImplGLES.h"
#include "RsUtil.h"
#include "BtMemory.h"
#include "BtPrint.h"
#include "RsGLES.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// FixPointers

//virtual
void RsSpriteWinGL::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	// Set the file data address
	m_pFileData = (LBaSpriteFileData*)pFileData;

	// Set the file data
	m_pFileData->m_pTexture = (RsTexture*)pArchive->GetResource( m_pFileData->m_nTexture );
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsSpriteWinGL::CreateOnDevice()
{
	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void RsSpriteWinGL::RemoveFromDevice()
{
	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumSprites

BtU32 RsSpriteWinGL::GetNumSprites() const
{
	return m_pFileData->m_nNumSprites;
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

MtVector2 RsSpriteWinGL::GetDimension( BtU32 iSprite ) const
{
	BaSprite& sprite = m_pFileData->m_sprites[iSprite];
	return MtVector2( sprite.m_width, sprite.m_height );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsSpriteWinGL::Render( BtBool isFlipX, const MtVector2& v2CurrentPosition, BtU32 spriteIndex, BtFloat rotation, BtU32 sortOrder )
{
	Render( isFlipX, v2CurrentPosition, spriteIndex, rotation, RsColour::WhiteColour(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsSpriteWinGL::Render( BtBool isFlipX, const MtVector2& v2CurrentPosition, const MtVector2& v2Dimension, BtU32 spriteIndex, BtFloat rotation, const RsColour& colour, BtU32 sortOrder )
{
	// Cache the texture
	RsTextureWinGL *pTexture = (RsTextureWinGL*)m_pFileData->m_pTexture;
    
	// Cache the sprite
	BaSprite& sprite = m_pFileData->m_sprites[spriteIndex];
    
	// Set the position
	MtVector2 v2Position( v2CurrentPosition.x, v2CurrentPosition.y );
    
	// Cache the texture width
	const BtFloat fTextureWidth = (BtFloat)pTexture->GetWidth();
    
	// Cache the texture height
	const BtFloat fTextureHeight = (BtFloat)pTexture->GetHeight();
    
	// Calculate the texture scalars
	BtFloat fScalarX = 1.0f / fTextureWidth;
	BtFloat fScalarY = 1.0f / fTextureHeight;
    
	BtFloat minU = sprite.m_fX * fScalarX;
	BtFloat minV = sprite.m_fY * fScalarY;
    
	BtFloat maxU = ( sprite.m_fX + sprite.m_width ) * fScalarX;
	BtFloat maxV = ( sprite.m_fY + sprite.m_height ) * fScalarY;
    
	// Setup the dimension
	MtVector3 v3HalfDimension( v2Dimension.x * 0.5f, v2Dimension.y * 0.5f, 0 );
	MtVector3 v3RotatedDimension[4];
    
	MtVector3 v3Position = MtVector3( v2Position, 0 );
    
	v3RotatedDimension[ 0 ] = MtVector3( -v3HalfDimension.x, -v3HalfDimension.y, 0.1f );
	v3RotatedDimension[ 1 ] = MtVector3( -v3HalfDimension.x,  v3HalfDimension.y, 0.1f );
	v3RotatedDimension[ 2 ] = MtVector3(  v3HalfDimension.x, -v3HalfDimension.y, 0.1f );
	v3RotatedDimension[ 3 ] = MtVector3(  v3HalfDimension.x,  v3HalfDimension.y, 0.1f );
    
	if( rotation != 0 )
	{
		MtMatrix4 m4Rotation;
		m4Rotation.SetRotationZ( MtDegreesToRadians( -rotation ) );
        
		for( BtU32 i=0; i<4; i++ )
		{
			v3RotatedDimension[ i ] *= m4Rotation;
			v3RotatedDimension[ i ] += v3Position + v3HalfDimension;
		}
	}
	else
	{
		for( BtU32 i=0; i<4; i++ )
		{
			v3RotatedDimension[ i ] += v3Position + v3HalfDimension;
		}
	}
    
	// Cache the render target
	RsRenderTarget *pRenderTarget = RsRenderTarget::GetCurrent();
    
	// Cache the camera
	RsCamera camera = pRenderTarget->GetCamera();
    
	// Cache the display width and height
	BtFloat Width  = (BtFloat)camera.GetViewport().m_width;
	BtFloat Height = (BtFloat)camera.GetViewport().m_height;
    
	// Cache the display width and height
	BtFloat scaleWidth  = 1.0f / Width;
	BtFloat scaleHeight = 1.0f / Height;
    
	// Cache the impl
	RsImplWinGL *pImpl = (RsImplWinGL*)RsImpl::pInstance();
    
	// Allocate vertex
	RsVertex3 *pStartVertex = pImpl->StartVertex();
    
	// Set the start vertex
	RsVertex3 *pVertex = pStartVertex;
    
	RsVertex3 *pQuad = pVertex;
    
	// Copy these into vertex
	pVertex->m_v3Position = v3RotatedDimension[0];
	pVertex->m_v2UV = MtVector2( minU, minV );
	pVertex->m_colour = colour.asWord();
	++pVertex;
    
	pVertex->m_v3Position = v3RotatedDimension[1];
	pVertex->m_v2UV = MtVector2( minU, maxV );
	pVertex->m_colour = colour.asWord();
	++pVertex;
    
	pVertex->m_v3Position = v3RotatedDimension[2];
	pVertex->m_v2UV = MtVector2( maxU, minV );
	pVertex->m_colour = colour.asWord();
	++pVertex;
    
	pVertex->m_v3Position = v3RotatedDimension[3];
	pVertex->m_v2UV = MtVector2( maxU, maxV );
	pVertex->m_colour = colour.asWord();
	++pVertex;
    
	// Scale the position to local screen space -1 to 1
	for( BtU32 i=0; i<4; i++ )
	{
		// Flip the y
		pQuad[ i ].m_v3Position.y = Height - pQuad[ i ].m_v3Position.y;
 
		// Scale from 0..width to 0..1
		pQuad[ i ].m_v3Position.x *= scaleWidth;
		pQuad[ i ].m_v3Position.y *= scaleHeight;
        
		// Scale from 0..1 to 0..2
		pQuad[ i ].m_v3Position.x *= 2.0f;
		pQuad[ i ].m_v3Position.y *= 2.0f;
        
		// Translate from 0..2 to -1..1
		pQuad[ i ].m_v3Position.x -= 1.0f;
		pQuad[ i ].m_v3Position.y -= 1.0f;
	}
    
	// Setup the primitive
	RsPrimitiveWinGL *pPrimitive = pImpl->AddPrimitive();
	pPrimitive->m_primitiveType = GL_TRIANGLE_STRIP;
	pPrimitive->m_numVertex     = 4;
	pPrimitive->m_nStartVertex  = pImpl->GetCurrentVertex();
    
	// End the current vertex
	pImpl->EndVertex( 4 );
    
	// Make a new font renderable
	RsSpriteRenderable *pSpriteRenderable = pImpl->AddSprite();
	pSpriteRenderable->m_pSprite   = this;
	pSpriteRenderable->m_pVertex   = pQuad;
	pSpriteRenderable->m_primitive = pPrimitive;
    
	// Validate the shader
	BtAssert( pSpriteRenderable->m_pShader != BtNull );
    
	// Add the font to the renderable list
	RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();
	pCurrentRenderTarget->Add( sortOrder, pSpriteRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsSpriteWinGL::Render( BtBool isFlipX, const MtVector2& v2CurrentPosition, BtU32 spriteIndex, BtFloat rotation, const RsColour& colour, BtU32 sortOrder )
{
	// Cache the sprite
	BaSprite& sprite = m_pFileData->m_sprites[spriteIndex];

	// Setup the dimension
	MtVector2 v2Dimension( sprite.m_width, sprite.m_height );

	Render( isFlipX, v2CurrentPosition, v2Dimension, spriteIndex, rotation, colour, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsSpriteWinGL::Render( RsSpriteRenderable *pRenderable )
{
	// Cache the shader
	RsShader *pShader = pRenderable->m_pShader;
    
	// Validate the shader
	BtAssert( pShader );
    
	// Apply the shader
	pShader->SetTechnique( "RsShaderTG2" );

	// Cache the texture
	RsTextureWinGL* pTexture = (RsTextureWinGL*)m_pFileData->m_pTexture;
    
	// Set the texture
	pTexture->SetTexture();
    
	RsPrimitiveWinGL* pPrimitives = (RsPrimitiveWinGL*) pRenderable->m_primitive;
    
	// Set vertex arrays
	BtU32 stride = sizeof(RsVertex3);
    
	// Cache the vertex buffer
	BtU32 vertexBuffer = RsImplWinGL::GetVertexBuffer();
    
	// Get the size
	BtU32 dataSize = pPrimitives->m_numVertex * stride;
    
	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer );
	
	GLenum error;
	error = glGetError();
	(void)error;
    
	// upload data to VBO
	glBufferData(GL_ARRAY_BUFFER, dataSize, pRenderable->m_pVertex, GL_DYNAMIC_DRAW);
	error = glGetError();
	(void)error;
    
	/*
     struct RsVertex3
     {
     MtVector3		m_v3Position;
     MtVector3		m_v3Normal;
     BtU32			m_colour;
     MtVector2		m_v2UV;
     MtVector2		m_v2UV2;
     };
     */
    
	BtU32 offset = 0;
	
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, GL_BUFFER_OFFSET( offset ) );
	offset += sizeof( MtVector3 );
    
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride, GL_BUFFER_OFFSET( offset ) );
	offset += sizeof( MtVector3 );
    
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, GL_BUFFER_OFFSET( offset ) );
	offset += sizeof( BtU32 );
    
	glEnableVertexAttribArray( 5 );
	glVertexAttribPointer( 5, 2, GL_FLOAT, GL_FALSE, stride,  GL_BUFFER_OFFSET( offset ) );
	offset += sizeof( MtVector2 );
    
	// Draw the primitives
	pShader->Draw( pRenderable->m_primitive );
}
