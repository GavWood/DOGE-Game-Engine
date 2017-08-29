////////////////////////////////////////////////////////////////////////////////
// RsTextureWinGL.cpp

// Includes
#include <GL/glew.h>
#include "RsTextureWinGL.h"
#include "BtMemory.h"
#include "BtPrint.h"
#include "BtBase.h"
#include "RsUtil.h"
#include "RsImplWinGL.h"
#include "RsShaderWinGL.h"

#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03

BtU32 RsTextureWinGL::GetWidth() const
{
	return m_pMipmaps[0].m_nWidth;
}

BtU32 RsTextureWinGL::GetHeight() const
{
	return m_pMipmaps[0].m_nHeight;
}

MtVector2 RsTextureWinGL::GetDimension() const
{
	return MtVector2( (BtFloat)GetWidth(), (BtFloat)GetHeight() );
}

BtU32 RsTextureWinGL::GetOriginalWidth() const
{
	return m_pMipmaps[0].m_originalWidth;
}

BtU32 RsTextureWinGL::GetOriginalHeight() const
{
	return m_pMipmaps[0].m_originalHeight;
}

MtVector2 RsTextureWinGL::GetOriginalDimension() const
{
	return MtVector2( (BtFloat)GetOriginalWidth(), (BtFloat)GetOriginalHeight() );
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

//virtual
void RsTextureWinGL::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	m_pArchive = pArchive;
	m_pFileData = ( LBaTextureFileData* )pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// GetTextureHandle

BtU32 RsTextureWinGL::GetTextureHandle()
{
	return m_texture;
}

////////////////////////////////////////////////////////////////////////////////
// SetTexture

void RsTextureWinGL::SetTexture()
{
    GLenum error;
	glGetError();
    
	glBindTexture(GL_TEXTURE_2D, m_texture );
    error = glGetError();

    if( m_pFileData->m_flags & RsTF_ClampU )
    {
		glTextureParameteri( m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		error = glGetError();
	}
	if (m_pFileData->m_flags & RsTF_ClampV)
	{
		glTextureParameteri( m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		error = glGetError();
	}
	if (m_pFileData->m_flags & RsTF_WrapU)
	{
		glTextureParameteri( m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT );
		error = glGetError();
	}
	if (m_pFileData->m_flags & RsTF_WrapV)
	{
		glTextureParameteri( m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT );
		error = glGetError();
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetClamped

void RsTextureWinGL::SetClamped(BtBool isEnabled)
{
	if( isEnabled )
	{
		m_pFileData->m_flags |= RsTF_ClampU;
		m_pFileData->m_flags |= RsTF_ClampV;
	}
	else
	{
		m_pFileData->m_flags &= ~RsTF_ClampU;
		m_pFileData->m_flags &= ~RsTF_ClampV;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetWrapped

void RsTextureWinGL::SetWrapped(BtBool isEnabled)
{
	if(isEnabled)
	{
		m_pFileData->m_flags |= RsTF_WrapU;
		m_pFileData->m_flags |= RsTF_WrapV;
	}
	else
	{
		m_pFileData->m_flags &= ~RsTF_WrapU;
		m_pFileData->m_flags &= ~RsTF_WrapV;
	}
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsTextureWinGL::CreateOnDevice()
{
	int error;

	// Find the mipmap data
	BtU8* pMemory = (BtU8*)( m_pFileData );

	// Skip pass the header
	pMemory += sizeof( LBaTextureFileData );

	// Set the mipmap pointer
	m_pMipmaps = ( LBaMipMapFileData* ) pMemory;

	// Go past the mipmap data
	pMemory+= sizeof( LBaMipMapFileData ) * m_pFileData->m_nMipMaps;

	for( BtU32 i=0; i<m_pFileData->m_nMipMaps; i++ )
	{
		// Cache each mipmap
		LBaMipMapFileData& mipMap = m_pMipmaps[i];

		// Mark up each texture
		mipMap.m_pTextureMemory = pMemory;

		// Increment the memory pointer
		pMemory+= mipMap.m_nTextureSize;
	}

	if( m_pFileData->m_flags & RsTF_SystemMemory )
	{
		BtPrint( "Texture %s is loaded into system memory.\r\n", GetTitle() );		
	}
	else
	{
		glGetError();

		BtU32 iMipmapLevel=0; // iMipmapLevel<m_pFileData->m_nMipMaps; iMipmapLevel++ )

		// Cache each mipmap
		LBaMipMapFileData& mipMap = m_pMipmaps[iMipmapLevel];

		// Cache the texture memory
		BtU8* pTextureMemory = mipMap.m_pTextureMemory;

		// Cache the texture size
		BtU32 textureSize = mipMap.m_nTextureSize;

		(void)textureSize;

		//glEnable( GL_TEXTURE_2D );
		//error = glGetError();

		// Load the texture
		GLenum format = (GLenum)0;
		GLenum internalformat = (GLenum)0;

		if( strstr( this->GetTitle(), "ttf") )
		{
			int a=0;
			a++;
		}

		if (m_pFileData->m_flags & RsTF_Red)
		{
			format = GL_ALPHA;
			internalformat = GL_ALPHA;
		}
		else if( m_pFileData->m_flags & RsTF_Alpha )
		{
			format = GL_RGBA;
			internalformat = GL_RGBA8;
		}
		else
		{
			format = GL_RGB;
			internalformat = GL_RGB8;
		}

		// http://www.flashbang.se/archives/48

		// Cache the mipmap
		mipMap = m_pMipmaps[0];

		glGenTextures(1, &m_texture);
		error = glGetError();
		
		glBindTexture(GL_TEXTURE_2D, m_texture);
		error = glGetError();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		error = glGetError();

		error = glGetError();
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}
		if (m_pFileData->m_flags & RsTF_ClampU)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		

		if (m_pFileData->m_flags & RsTF_NearestNeighbour)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if( m_pFileData->m_flags & RsTF_RenderTarget )
		{
			glGenFramebuffers(1, &m_frameBufferObject);

			glGenRenderbuffers(1, &m_renderBufferObject);

			glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mipMap.m_nWidth, mipMap.m_nHeight, 0, GL_RGBA, GL_INT, NULL);

			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, m_texture, 0);

			glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject);
			glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24, mipMap.m_nWidth, mipMap.m_nHeight );
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER, m_renderBufferObject);

			// check FBO status
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		//	BtAssert( status == GL_FRAMEBUFFER_COMPLETE );

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
//			float maximumAnistropy;
//			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maximumAnistropy);
//			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maximumAnistropy);

			glTexImage2D( GL_TEXTURE_2D,		    //target : usually GL_TEXTURE_2D
				0,									//level : usually left to zero
				format,								//internal format
				mipMap.m_nWidth,					//image size
				mipMap.m_nHeight,
				0,									//0 : no border
				format,								//format : usually RGBA
				GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
				pTextureMemory );		  			//picture datas
			//glGenerateMipmap(GL_TEXTURE_2D);

			//checkError( "glTexImage2D" );
		}
	}
	glBindTexture( GL_TEXTURE_2D, 0 );
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void RsTextureWinGL::RemoveFromDevice()
{
	glDeleteTextures( 1, &m_texture );
}

////////////////////////////////////////////////////////////////////////////////
// GetPixel

RsColour RsTextureWinGL::GetPixel( BtU32 x, BtU32 y, BtU8& red, BtU8& green, BtU8& blue, BtU8 &alpha )
{
	// Cache each mipmap
	LBaMipMapFileData& mipMap = m_pMipmaps[0];

	if (m_pFileData->m_flags & (RsTF_Indexed | RsTF_IndexedAlpha))
	{
		BtU8* pAddress = mipMap.m_pTextureMemory;

		BtU32 paletteSize = 0;

		if (m_pFileData->m_flags & RsTF_Indexed)
		{
			paletteSize = 3;
			pAddress += 256 * paletteSize;
		}
		else
		{
			paletteSize = 4;
			pAddress += 256 * paletteSize;
		}

		pAddress += x + (y * mipMap.m_nPitch);

		BtU32 paletteIndex = (*pAddress) * paletteSize;

		// Get the colour from the palette
		BtU8* pPalette = mipMap.m_pTextureMemory + paletteIndex;

		alpha = *(pAddress + 3);
		blue = *(pPalette + 2);
		green = *(pPalette + 1);
		red = *(pPalette + 0);
	}
	else if (m_pFileData->m_flags & RsTF_Alpha)
	{
		BtU8* pAddress = mipMap.m_pTextureMemory + (x * 4) + (y * mipMap.m_nPitch);

		alpha = *(pAddress + 3);
		red = *(pAddress + 2);
		green = *(pAddress + 1);
		blue = *(pAddress + 0);
	}
	else
	{
		BtU8* pAddress = mipMap.m_pTextureMemory + (x * 3) + (y * mipMap.m_nPitch);

		alpha = *(pAddress + 3);
		red = *(pAddress + 2);
		green = *(pAddress + 1);
		blue = *(pAddress + 0);
	}
	return RsColour(red, green, blue, alpha);
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureWinGL::Render( const MtVector2& v2Position, BtU32 sortOrder )
{
	Render( v2Position, GetDimension(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureWinGL::Render( const MtVector2& v2Position,
							 const MtVector2& v2Dimension,
							 BtU32 sortOrder )
{
	Render( v2Position, v2Dimension, RsColour::WhiteColour(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureWinGL::Render( const MtVector2& v2Position,
							 const MtVector2& v2Dimension,
							 RsColour colour,
							 BtU32 sortOrder )
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
	BtFloat fScaleWidth  = 1.0f / Width;
	BtFloat fScaleHeight = 1.0f / Height;

	// Allocate vertex
	RsVertex3 *pStartVertex = pImpl->StartVertex();

	// Set the start vertex
	RsVertex3 *pVertex = pStartVertex;

	// Cache the texture width and height
	BtFloat fTextureWidth  = (BtFloat)m_pMipmaps[0].m_nWidth;
	BtFloat fTextureHeight = (BtFloat)m_pMipmaps[0].m_nHeight;

	// Calculate the texture scalars
	BtFloat fScalarX = 1.0f / fTextureWidth;
	BtFloat fScalarY = 1.0f / fTextureHeight;

	BtFloat minU = fScalarX / 2;
	BtFloat minV = fScalarY / 2;

	BtFloat maxU = ( m_pMipmaps[0].m_originalWidth * fScalarX )  - minU;
	BtFloat maxV = ( m_pMipmaps[0].m_originalHeight * fScalarY ) - minV;

	// Calculate the positions
	BtFloat xmin = v2Position.x;
	BtFloat xmax = v2Position.x + v2Dimension.x;
	BtFloat ymin = v2Position.y;
	BtFloat ymax = v2Position.y + v2Dimension.y;

	RsVertex3 *pQuad = pVertex;

	// Copy these into vertex
	pVertex->m_v3Position = MtVector3(xmin, ymin, 0.1f );
	pVertex->m_v2UV = MtVector2( minU, minV );
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmin, ymax, 0.1f );
	pVertex->m_v2UV = MtVector2( minU, maxV );
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmax, ymin, 0.1f );
	pVertex->m_v2UV = MtVector2( maxU, minV );
	++pVertex;

	pVertex->m_v3Position = MtVector3(xmax, ymax, 0.1f );
	pVertex->m_v2UV = MtVector2( maxU, maxV );
	++pVertex;

	// Scale the position to local screen space -1 to 1
	for( BtU32 i=0; i<4; i++ )
	{
		// Set the colour
		pQuad[ i ].m_colour = colour.asARGB();

		// Flip the y
		pQuad[ i ].m_v3Position.y = Height - pQuad[ i ].m_v3Position.y;

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
	RsPrimitive *pPrimitive = pImpl->AddPrimitive();
	pPrimitive->m_primitiveType = GL_TRIANGLE_STRIP;
	pPrimitive->m_numVertex	    = 4;
	pPrimitive->m_nStartVertex  = pImpl->GetCurrentVertex();

	// End the current vertex
	pImpl->EndVertex( 4 );

	// Make a new font renderable
	RsTextureRenderable *pTextureRenderable = pImpl->AddTexture();
	pTextureRenderable->m_pTexture  = this;
	pTextureRenderable->m_pVertex   = pStartVertex;
	pTextureRenderable->m_primitive = pPrimitive;

	// Validate the shader
	BtAssert( pTextureRenderable->m_pShader != BtNull );

	// Add the font to the renderable list
	RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();
	pCurrentRenderTarget->Add( sortOrder, pTextureRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureWinGL::Render( RsTextureRenderable *pRenderable )
{
	// Cache the current render target
	RsRenderTarget *pRenderTarget = pRenderable->m_pRenderTarget;

	// Set the shader
	RsShaderWinGL *pShader = (RsShaderWinGL*)pRenderable->m_pShader;

	RsPrimitive* pPrimitives = (RsPrimitive*) pRenderable->m_primitive;

	if( pPrimitives->m_primitiveType == 1 )
	{
		int a=0;
		a++;
	}

	// Cache the camera
	const RsCamera &camera = pRenderTarget->GetCamera();

	const MtMatrix4& m4Projection = camera.GetViewProjection();
	const MtMatrix4& m4World      = MtMatrix4::m_identity;
	const MtMatrix4& m4View	      = camera.GetView();
	MtMatrix4 m4WorldViewScreen   = m4World * m4Projection;

	MtMatrix4 m4WorldView = m4World * m4View;
	pShader->SetCamera( camera );
	pShader->SetTechnique( "RsShaderT2" );
	pShader->SetMatrix( RsHandles_WorldViewScreen, m4WorldViewScreen );

	for( BtU32 i=0; i<7; i++ )
	{
		glDisableVertexAttribArray( i );
	}

	// Cache the texture
	RsTextureWinGL* pTexture = (RsTextureWinGL*)pRenderable->m_pTexture;

	// Set the texture
	pTexture->SetTexture();

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

	glEnableVertexAttribArray( 5);
	glVertexAttribPointer( 5, 2, GL_FLOAT, GL_FALSE, stride,  GL_BUFFER_OFFSET( offset ) );
	offset += sizeof( MtVector2 );

	// Draw the primitives
	pShader->Draw( pRenderable->m_primitive );
}

////////////////////////////////////////////////////////////////////////////////
// ReadMemory

BtU8 *RsTextureWinGL::ReadMemory()
{
	// Cache the mipmap
	LBaMipMapFileData& mipMap = m_pMipmaps[0];

	// Cache the texture memory
	return mipMap.m_pTextureMemory;
}

////////////////////////////////////////////////////////////////////////////////
// WriteMemory

void RsTextureWinGL::WriteMemory( BtU8* pMemory, BtU32 size )
{
	// Cache the mipmap
	// Cache each mipmap
	LBaMipMapFileData& mipMap = m_pMipmaps[0];
	memcpy(mipMap.m_pTextureMemory, pMemory, size);
	
	//BtAssert(mipMap.m_nWidth * mipMap.m_nHeight * 4 == size);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	GLenum err = glGetError();
//	if (err != GL_NO_ERROR)
//		printf( "Error creating texture. glError: 0x%04X", err);

	if (m_pFileData->m_flags & RsTF_Red)
	{
		glTexImage2D(GL_TEXTURE_2D,             //target : usually GL_TEXTURE_2D
			0,									//level : usually left to zero
			GL_RGBA,							//internal format
			mipMap.m_nWidth,                    //image size
			mipMap.m_nHeight,
			0,									//0 : no border
			GL_ALPHA,							//format : usually RGB
			GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
			(GLvoid*)pMemory);                  //picture datas
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D,                    //target : usually GL_TEXTURE_2D
			0,									//level : usually left to zero
			GL_RGBA,							//internal format
			mipMap.m_nWidth,                   //image size
			mipMap.m_nHeight,
			0,									//0 : no border
			GL_BGRA,							//format : usually RGB
			GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
			(GLvoid*)pMemory);                //picture datas
	}

	err = glGetError();
//	if (err != GL_NO_ERROR)
//		printf( "Can't update texture. glError: 0x%04X", err);
}
