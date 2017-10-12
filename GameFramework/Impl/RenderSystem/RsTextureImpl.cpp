////////////////////////////////////////////////////////////////////////////////
// RsTextureGLES.cpp

// Includes
#include "RsTextureImpl.h"
#include "BtMemory.h"
#include "BtPrint.h"
#include "BtBase.h"
#include "RsUtil.h"
#include "RsManagerImpl.h"
#include "RsShaderImpl.h"
#include "RsImpl.h"
#include "BaFileData.h"
#include <stdio.h>

#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03

BtU32 RsTextureImpl::GetTextureHandle()
{
    return m_texture;
}

BtU32 RsTextureImpl::GetWidth() const
{
	return m_pMipmaps[0].m_nWidth;
}

BtU32 RsTextureImpl::GetHeight() const
{
	return m_pMipmaps[0].m_nHeight;
}

MtVector2 RsTextureImpl::GetDimension() const
{
	return MtVector2( (BtFloat)GetWidth(), (BtFloat)GetHeight() );
}

BtU32 RsTextureImpl::GetOriginalWidth() const
{
	return m_pMipmaps[0].m_originalWidth;
}

BtU32 RsTextureImpl::GetOriginalHeight() const
{
	return m_pMipmaps[0].m_originalHeight;
}

MtVector2 RsTextureImpl::GetOriginalDimension() const
{
	return MtVector2( (BtFloat)GetOriginalWidth(), (BtFloat)GetOriginalHeight() );
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

//virtual
void RsTextureImpl::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	m_pArchive = pArchive;
	m_pFileData = ( LBaTextureFileData* )pFileData;
}

////////////////////////////////////////////////////////////////////////////////
// GetDuplicate

RsTexture* RsTextureImpl::GetDuplicate() const
{
    return BtNull;
}

void checkFrameBuffer()
{
    GLuint returned = (glCheckFramebufferStatus(GL_FRAMEBUFFER));
    
    if( returned != GL_FRAMEBUFFER_COMPLETE )
    {
        switch (returned)
		{
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                printf("Incomplete: Missing Attachment\r\n");
                break;
                
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                printf("Incomplete: Attachment\r\n");
                break;
                
            default:
                //printf("Complete\r\n");
                break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsTextureImpl::CreateOnDevice()
{
    GLenum err = 0;
    
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
        int a=0;
        a++;
        BtPrint( "Texture %s is loaded into system memory.\r\n", GetTitle() );
	}
    
    {
		BtU32 iMipmapLevel=0; // iMipmapLevel<m_pFileData->m_nMipMaps; iMipmapLevel++ )

		// Cache each mipmap
		LBaMipMapFileData& mipMap = m_pMipmaps[iMipmapLevel];

		// Cache the texture memory
		BtU8* pTextureMemory = mipMap.m_pTextureMemory;

		// Cache the texture size
		BtU32 textureSize = mipMap.m_nTextureSize;

		(void)textureSize;

		// Cache the mipmap
		mipMap = m_pMipmaps[0];

		glEnable( GL_TEXTURE_2D );
    
        BtChar *title = GetTitle();
        
        //printf( "%s", title );
        
        if( strstr( title, "cam4" ) )
        {
            int a=0;
            a++;
        }
        
        err = glGetError();
        
        // glgConvertTo_32
        glGenTextures( 1, &m_texture );
        err = glGetError();
        
        if( m_texture == 11 )
        {
            int a=0;
            a++;
        }
        
        glBindTexture( GL_TEXTURE_2D, m_texture );
        err = glGetError();
        
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
        err = glGetError();
        
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        err = glGetError();
        
        if( m_pFileData->m_flags & RsTF_NearestNeighbour )
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
     
        // Clamp all textures to edge
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
		if( m_pFileData->m_flags & RsTF_RenderTarget )
		{
            // http://stackoverflow.com/questions/7313734/rendering-to-texture-on-ios-opengl-esworks-on-simulator-but-not-on-device
            // http://www.opengl.org/wiki/Framebuffer_Object_Examples
            
            //NULL means reserve texture memory, but texels are undefined
 			glTexImage2D( GL_TEXTURE_2D,		    //target : usually GL_TEXTURE_2D
                         0,									//level : usually left to zero
                         GL_RGBA,							//internal format
                         mipMap.m_nWidth,                    //image size
                         mipMap.m_nHeight,
                         0,									//0 : no border
                         GL_RGBA,							//format : usually RGB
                         GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
                         (GLvoid*)pTextureMemory );          //picture datas
            
            err = glGetError();
            if (err != GL_NO_ERROR)
                printf( "Error creating texture. glError: 0x%04X\n", err);
            
            checkFrameBuffer();
            
            //-------------------------
            glGenFramebuffers(1, &m_frameBufferObject);
            glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
            //Attach 2D texture to this FBO
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
            
            checkFrameBuffer();
            
            //-------------------------
            glGenRenderbuffers( 1, &m_renderBufferObject);
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject);
            glRenderbufferStorage(GL_RENDERBUFFER, RS_GL_DEPTH_COMPONENT, mipMap.m_nWidth, mipMap.m_nHeight );
            
            checkFrameBuffer();
            //-------------------------
            //Attach depth buffer to FBO
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject);
        
            checkFrameBuffer();
            
            err = glGetError();
            if (err != GL_NO_ERROR)
                printf( "Error creating texture. glError: 0x%04X\n", err);
		}
		else
		{
    
            BtU32 format = GL_RGBA;
            
            if (m_pFileData->m_flags & RsTF_Red)
            {
                format = GL_ALPHA;
            }
            else if( ( m_pFileData->m_flags & RsTF_Alpha ) == 0 )
            {
                format = GL_RGB;
            }
            
			glTexImage2D( GL_TEXTURE_2D,		    //target : usually GL_TEXTURE_2D
				0,									//level : usually left to zero
				format,                             //internal format
				mipMap.m_nWidth,                    //image size
				mipMap.m_nHeight,
				0,                                  //0 : no border
				format ,                             //format : usually RGB
				GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
				(GLvoid*)pTextureMemory );          //picture datas

            err = glGetError();
            if (err != GL_NO_ERROR)
            {
               printf( "Error uploading texture %d %d. glError: 0x%04X\n",
                      mipMap.m_nWidth, mipMap.m_nHeight, err);
            }
		}
        
        // Unbind this texture
        glBindTexture( GL_TEXTURE_2D, 0 );
        
        err = glGetError();
        
        int a=0;
        a++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetTexture

void RsTextureImpl::SetTexture( BtU32 newTextureID )
{
    m_texture = newTextureID;
}

////////////////////////////////////////////////////////////////////////////////
// ReadMemory

BtU8 *RsTextureImpl::ReadMemory()
{
	// Cache the mipmap
	LBaMipMapFileData& mipMap = m_pMipmaps[0];
    
	// Cache the texture memory
	return mipMap.m_pTextureMemory;
}

////////////////////////////////////////////////////////////////////////////////
// WriteMemory

void RsTextureImpl::WriteMemory( BtU8* pMemory, BtU32 size )
{
    // Cache the mipmap
    LBaMipMapFileData& mipMap = m_pMipmaps[0];
    
    if( size == 0 )
    {
        size = mipMap.m_nWidth * mipMap.m_nHeight * 4;
    }
    
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf( "Error creating texture. glError: 0x%04X\n", err);
    
    //printf( "%s", GetTitle() );

    if ( m_pFileData->m_flags & RsTF_Red)
    {
        BtAssert( size == mipMap.m_nWidth * mipMap.m_nHeight );
 
        glTexImage2D(GL_TEXTURE_2D,                     //target : usually GL_TEXTURE_2D
                     0,									//level : usually left to zero
                     GL_ALPHA,							//internal format
                     mipMap.m_nWidth,                   //image size
                     mipMap.m_nHeight,
                     0,									//0 : no border
                     GL_ALPHA,							//format : usually RGB
                     GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
                     (GLvoid*)pMemory);                 //picture datas
    }
    else
    {
        BtAssert( size == mipMap.m_nWidth * mipMap.m_nHeight * 4 );
        
        glTexImage2D( GL_TEXTURE_2D,                    //target : usually GL_TEXTURE_2D
                     0,									//level : usually left to zero
                     GL_RGBA,							//internal format
                     mipMap.m_nWidth,                   //image size
                     mipMap.m_nHeight,
                     0,									//0 : no border
                     GL_BGRA,							//format : usually RGB
                     GL_UNSIGNED_BYTE,					//data type : pixels are made of byte
                     (GLvoid*)pMemory );                //picture datas
    }
    
    err = glGetError();
    if (err != GL_NO_ERROR)
        printf( "Can't update texture. glError: 0x%04X\n", err);
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void RsTextureImpl::RemoveFromDevice()
{
	glDeleteTextures( 1, &m_texture );
}

////////////////////////////////////////////////////////////////////////////////
// GetPixel

RsColour RsTextureImpl::GetPixel( BtU32 x, BtU32 y, BtU8& red, BtU8& green, BtU8& blue, BtU8 &alpha )
{
	// Cache each mipmap
	LBaMipMapFileData& mipMap = m_pMipmaps[0];

	if( m_pFileData->m_flags & ( RsTF_Indexed | RsTF_IndexedAlpha ) )
	{
		BtU8* pAddress = mipMap.m_pTextureMemory;

		BtU32 paletteSize = 0;

		if( m_pFileData->m_flags & RsTF_Indexed )
		{
			paletteSize = 3;
			pAddress += 256 * paletteSize;
		}
		else
		{
			paletteSize = 4;
			pAddress += 256 * paletteSize;
		}

		pAddress += x + ( y * mipMap.m_nPitch );

		BtU32 paletteIndex = ( *pAddress ) * paletteSize;

		// Get the colour from the palette
		BtU8* pPalette = mipMap.m_pTextureMemory + paletteIndex;

		alpha = *(pAddress + 3 );
		blue  = *(pPalette + 2 );
		green = *(pPalette + 1 );
		red   = *(pPalette + 0 );
	}
	else if( m_pFileData->m_flags & RsTF_Alpha )
	{
		BtU8* pAddress = mipMap.m_pTextureMemory + ( x * 4 ) + ( y * mipMap.m_nPitch );

        alpha = *(pAddress + 3 );
		red   = *(pAddress + 2 );
		green = *(pAddress + 1 );
		blue  = *(pAddress + 0 );
	}
	else
	{
		BtU8* pAddress = mipMap.m_pTextureMemory + ( x * 3 ) + ( y * mipMap.m_nPitch );
        
		alpha = *(pAddress + 3 );
		red   = *(pAddress + 2 );
		green = *(pAddress + 1 );
		blue  = *(pAddress + 0 );
	}
	return RsColour( red, green, blue, alpha );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureImpl::Render( const MtVector2& v2Position, BtU32 sortOrder )
{
	Render( v2Position, GetDimension(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureImpl::Render( const MtVector2& v2Position,
							 const MtVector2& v2Dimension,
							 BtU32 sortOrder )
{
	Render( v2Position, v2Dimension, RsColour::WhiteColour(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsTextureImpl::Render( const MtVector2& v2Position,
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
		pQuad[ i ].m_colour = colour.asABGR();
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

void RsTextureImpl::Render( RsTextureRenderable *pRenderable )
{
	// Cache the current render target
	RsRenderTarget *pRenderTarget = pRenderable->m_pRenderTarget;

	// Set the shader
	RsShaderImpl *pShader = (RsShaderImpl*)pRenderable->m_pShader;

	RsPrimitive* pPrimitives = (RsPrimitive*)pRenderable->m_primitive;

	if( pPrimitives->m_primitiveType == 1 )
	{
		int a=0;
		a++;
	}

	// Cache the camera
	const RsCamera &camera = pRenderTarget->GetCamera();

	const MtMatrix4& m4Projection = camera.GetViewProjection();
	const MtMatrix4& m4World      = MtMatrix4::m_identity;
	MtMatrix4 m4WorldViewScreen   = m4World * m4Projection;

	pShader->SetCamera( camera );
	pShader->SetTechnique( "RsShaderT2" );
	pShader->SetMatrix( RsHandles_WorldViewScreen, m4WorldViewScreen );

	for( BtU32 i=0; i<7; i++ )
	{
		glDisableVertexAttribArray( i );
	}

	// Cache the texture
	RsTextureImpl* pTexture = (RsTextureImpl*)pRenderable->m_pTexture;

    // Set the texture
    glActiveTexture(GL_TEXTURE0 );
 
    // Bind the texture to the handle
    glBindTexture(GL_TEXTURE_2D, pTexture->GetTextureHandle() );
            
    // Set the shader sampler
    pShader->SetSampler(0);
    
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

