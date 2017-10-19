////////////////////////////////////////////////////////////////////////////////
/// RsMaterialImpl.cpp

#include "BtBase.h"
#include "BaResource.h"
#include "BtString.h"
#include "BtMemory.h"
#include "MtVector4.h"
#include "RsVertex.h"
#include "RsShader.h"
#include "RsImpl.h"
#include "RsMaterialImpl.h"
#include "RsTextureImpl.h"
#include "RsManagerImpl.h"
#include "RsShaderImpl.h"
#include <new>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructor

RsMaterialImpl::RsMaterialImpl()
{
}

////////////////////////////////////////////////////////////////////////////////
// GetDuplicate

//virtual
RsMaterial* RsMaterialImpl::GetDuplicate() const
{
    // Allocate the memory
    BtU8* pMemory = BtMemory::Allocate( sizeof(RsMaterialImpl) + m_pFileData->m_nFileDataSize );
    
    // Create the class
    RsMaterialImpl* pMaterial = new( pMemory ) RsMaterialImpl;
    
    // Advance pass the instance size
    pMemory += sizeof(RsMaterialImpl);
    
    // Copy the memory
    BtMemory::Copy( pMemory, m_pFileData, sizeof( BaMaterialFileData ) );
    
    // Flag as a duplicate
    pMaterial->m_isDuplicate = BtTrue;
    
    // Add the duplicate
    m_pArchive->AddDuplicate( pMaterial );
    
    // Set the file data
    pMaterial->m_pFileData = (BaMaterialFileData*) pMemory;
    
    pMaterial->SetTitle( GetTitle() );
    
    // Return the memory
    return pMaterial;
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void RsMaterialImpl::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	// Set the archive
	m_pArchive = pArchive;

	// Fix the pointers to the textures
	m_pFileData = (BaMaterialFileData*) pFileData;

	for( BtU32 iTexture=0; iTexture<MaxTextures; ++iTexture )
	{
        // Fix up the texture
		m_pFileData->m_pTexture[iTexture] = (RsTexture*) pArchive->GetResource(m_pFileData->m_nTexture[iTexture] );
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetTechnique

BtChar* RsMaterialImpl::GetTechniqueName() const
{
	return m_pFileData->m_techniqueName;
}

////////////////////////////////////////////////////////////////////////////////
// SetTechniqueName

void RsMaterialImpl::SetTechniqueName( const BtChar* name )
{
	BtStrCopy( m_pFileData->m_techniqueName, 64, name );
}

////////////////////////////////////////////////////////////////////////////////
// SetZTest

void RsMaterialImpl::SetZTest( BtBool isEnabled )
{
    m_pFileData->m_flags |= RsMaterial_ZTest;
}

////////////////////////////////////////////////////////////////////////////////
// GetZTest

BtBool RsMaterialImpl::GetZTest()
{
    return m_pFileData->m_flags & RsMaterial_ZTest;
}

////////////////////////////////////////////////////////////////////////////////
// Width

// virtual
BtU32 RsMaterialImpl::Width()
{
	RsTexture* pTexture = GetTexture(0);
	BtAssert( pTexture != BtNull );
	return pTexture->GetWidth();
}

////////////////////////////////////////////////////////////////////////////////
// Height

// virtual
BtU32 RsMaterialImpl::Height()
{
	RsTexture* pTexture = GetTexture(0);
	BtAssert( pTexture != BtNull );
	return pTexture->GetHeight();
}

//----------------------------------------------------------------------------
// Render

void RsMaterialImpl::Render( RsPrimitiveType primitiveType, RsVertex3 *pVertex, BtU32 vertexCount, BtU32 sortOrder, BtBool isCopy )
{
	// Cache the impl
	RsImplWinGL *pImpl = (RsImplWinGL*)RsImpl::pInstance();

	// Setup the primitive
	RsPrimitive *pPrimitive = pImpl->AddPrimitive();

	pPrimitive->m_numVertex = vertexCount;
	pPrimitive->m_nStartVertex = pImpl->GetCurrentVertex();		

	if( primitiveType == RsPT_TriangleStrip )
	{
		pPrimitive->m_primitiveType = GL_TRIANGLE_STRIP;
	}
	else if( primitiveType == RsPT_TriangleList )
	{
		pPrimitive->m_primitiveType = GL_TRIANGLES;
	}
	else if( primitiveType == RsPT_LineList )
	{
		pPrimitive->m_primitiveType = GL_LINES;
	}
	else if( primitiveType == RsPT_LineStrip )
	{
		pPrimitive->m_primitiveType = GL_LINE_STRIP;
	}
    
	// Start the vertex
    RsVertex3 *pVertexToRender = pVertex;
    
    if( isCopy )
    {
        // Get the start of the vertex and overwrite the original vertex pointer since this is a copy
        pVertexToRender = pImpl->StartVertex();

        // How much room did our vertex take
        BtU32 memsize = sizeof(RsVertex3) * vertexCount;

        // Copy the vertex
        BtMemory::Copy( pVertexToRender, pVertex, memsize );

        // End the vertx
        pImpl->EndVertex( vertexCount );
    }
    
	// Make a new material renderable
	RsMaterialRenderable *pMaterialRenderable = pImpl->AddMaterial();
	pMaterialRenderable->m_pMaterial = this;
	pMaterialRenderable->m_pVertex   = pVertexToRender;
	pMaterialRenderable->m_primitive = pPrimitive;

	// Validate the shader
	BtAssert( pMaterialRenderable->m_pShader != BtNull );

	// Add the font to the renderable list
	RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();
	pCurrentRenderTarget->Add( sortOrder, pMaterialRenderable );
}

//----------------------------------------------------------------------------
// Render

void RsMaterialImpl::Render( RsPrimitiveType primitiveType, RsVertex2 *pVertex, BtU32 vertexCount, BtU32 sortOrder )
{
	// Cache the impl
	RsImplWinGL *pImpl = (RsImplWinGL*)RsImpl::pInstance();

	// Setup the primitive
	RsPrimitive *primitive = pImpl->AddPrimitive();

	primitive->m_numVertex = vertexCount;
	primitive->m_nStartVertex = pImpl->GetCurrentVertex();

	if (primitiveType == RsPT_TriangleStrip)
	{
		primitive->m_primitiveType = GL_TRIANGLE_STRIP;
	}
	else if (primitiveType == RsPT_TriangleList)
	{
		primitive->m_primitiveType = GL_TRIANGLES;
	}
	else if (primitiveType == RsPT_TriangleFan)
	{
		primitive->m_primitiveType = GL_TRIANGLE_FAN;
	}
    else if (primitiveType == RsPT_LineList)
	{
		primitive->m_primitiveType = GL_LINES;
	}
    
	// Cache the render target
	RsRenderTarget *pRenderTarget = RsRenderTarget::GetCurrent();

	// Cache the camera
	RsCamera camera = pRenderTarget->GetCamera();

	// Cache the display width and height
	BtFloat width = (BtFloat)camera.GetViewport().m_width;
	BtFloat height = (BtFloat)camera.GetViewport().m_height;

	// Cache the display width and height
	BtFloat scaleWidth = 1.0f / width;
	BtFloat scaleHeight = 1.0f / height;

	// Start the vertex
	RsVertex3 *pEngineVertex = pImpl->StartVertex();

	// Copy the vertex
	for (BtU32 i = 0; i < vertexCount; i++)
	{
		pEngineVertex[i].m_colour = pVertex[i].m_colour;

		pEngineVertex[i].m_v3Position.x = pVertex[i].m_v2Position.x;
		pEngineVertex[i].m_v3Position.y = height - pVertex[i].m_v2Position.y;
		pEngineVertex[i].m_v3Position.z = 0.1f;

		pEngineVertex[i].m_v2UV = pVertex[i].m_v2UV;

		// Scale from 0..width to 0..1
		pEngineVertex[i].m_v3Position.x *= scaleWidth;
		pEngineVertex[i].m_v3Position.y *= scaleHeight;

		// Scale from 0..1 to 0..2
		pEngineVertex[i].m_v3Position.x *= 2.0f;
		pEngineVertex[i].m_v3Position.y *= 2.0f;

		// Translate from 0..2 to -1..1
		pEngineVertex[i].m_v3Position.x -= 1.0f;
		pEngineVertex[i].m_v3Position.y -= 1.0f;
	}

	// End the vertx
	pImpl->EndVertex(vertexCount);

	// Make a new material renderable
	RsMaterialRenderable *pMaterialRenderable = pImpl->AddMaterial();
	pMaterialRenderable->m_pMaterial = this;
	pMaterialRenderable->m_pVertex = pEngineVertex;
	pMaterialRenderable->m_primitive = primitive;

	// Validate the shader
	BtAssert(pMaterialRenderable->m_pShader != BtNull);

	// Add the font to the renderable list
	RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();
	pCurrentRenderTarget->Add(sortOrder, pMaterialRenderable);
}

//----------------------------------------------------------------------------
// GetPasses

BtU32 RsMaterialImpl::GetPasses() const
{
	return m_pFileData->m_nPasses;
}

//----------------------------------------------------------------------------
// GetFlags

BtU32 RsMaterialImpl::GetFlags() const
{
	return m_pFileData->m_flags;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsMaterialImpl::Render( const MtVector2& v2Position, const RsColour &colour, BtU32 sortOrder )
{
	Render( v2Position, m_pFileData->m_pTexture[0]->GetDimension(), colour, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsMaterialImpl::Render( const MtVector2& v2Position,
							  const MtVector2& v2Dimension,
							  const RsColour &colour,
							  BtU32 sortOrder )
{
	// Cache the impl
	RsImplWinGL *pImpl = (RsImplWinGL*)RsImpl::pInstance();
    
	BtU32 rgbColour = colour.asWord();
    
	// Cache the render target
	RsRenderTarget *pRenderTarget = RsRenderTarget::GetCurrent();
    
	// Cache the camera
	RsCamera camera = pRenderTarget->GetCamera();
    
	// Allocate vertex
	RsVertex3 *pStartVertex = pImpl->StartVertex();
    
	// Set the start vertex
	RsVertex3 *pVertex = pStartVertex;
    
	// Cache the texture
	RsTexture *pTexture = m_pFileData->m_pTexture[0];
    
	// Calculate the texture scalars
	BtFloat fScalarX = 1.0f / pTexture->GetWidth();
	BtFloat fScalarY = 1.0f / pTexture->GetHeight();
    
	BtFloat minU = fScalarX / 2;
	BtFloat minV = fScalarY / 2;
    
	BtFloat maxU = ( pTexture->GetOriginalWidth()  * fScalarX ) - minU;
	BtFloat maxV = ( pTexture->GetOriginalHeight() * fScalarY ) - minV;
    
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
		pQuad[ i ].m_colour = rgbColour;
	}
    
	// Cache the display width and height
	BtFloat width  = (BtFloat)camera.GetViewport().m_width;
	BtFloat height = (BtFloat)camera.GetViewport().m_height;
    
	// Cache the display width and height
	BtFloat fScaleWidth  = 1.0f / width;
	BtFloat fScaleHeight = 1.0f / height;
    
	// Scale the position to local screen space -1 to 1
	for( BtU32 i=0; i<4; i++ )
	{
		// Set the colour
		pQuad[ i ].m_colour = rgbColour;
        
		pQuad[ i ].m_v3Position.x -= 0.5f;
		pQuad[ i ].m_v3Position.y -= 0.5f;
        
		// Flip the y
		pQuad[ i ].m_v3Position.y = height - pQuad[ i ].m_v3Position.y;
        
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
	pPrimitive->m_numVertex     = 4;
	pPrimitive->m_nStartVertex  = pImpl->GetCurrentVertex();
    
	// End the current vertex
	pImpl->EndVertex( 4 );
    
	// Make a new material renderable
	RsMaterialRenderable *pMaterialRenderable = pImpl->AddMaterial();
	pMaterialRenderable->m_pMaterial = this;
	pMaterialRenderable->m_pVertex   = pStartVertex;
	pMaterialRenderable->m_primitive = pPrimitive;
    
	// Validate the shader
	BtAssert( pMaterialRenderable->m_pShader != BtNull );
    
	// Add the font to the renderable list
	RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();
	pCurrentRenderTarget->Add( sortOrder, pMaterialRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsMaterialImpl::Render( RsMaterialRenderable *pRenderable )
{
	// Cache the current render target
	RsRenderTarget *pRenderTarget = pRenderable->m_pRenderTarget;
    
	// Set the shader
	RsShaderImpl *pShader = (RsShaderImpl*)pRenderable->m_pShader;
    
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
	MtMatrix4 m4WorldViewScreen   = m4World * m4Projection;
    
	pShader->SetCamera( camera );
	pShader->SetTechnique( m_pFileData->m_techniqueName );
	pShader->SetMatrix( RsHandles_WorldViewScreen, m4WorldViewScreen );
    
	for( BtU32 i=0; i<7; i++ )
	{
		glDisableVertexAttribArray( i );
	}
    
	// Set the texture
    for( BtU32 i=0; i<MaxTextures; i++ )
    {
        // Set the texture
        glActiveTexture(GL_TEXTURE0 + i );
        
        RsTextureImpl* pTexture = (RsTextureImpl*)m_pFileData->m_pTexture[i];
        if( pTexture )
        {
            GLenum error = glGetError();
            (void)error;
            
            // Cache the texture handle
            BtU32 textureHandle = pTexture->GetTextureHandle();
            
            // Bind the texture to the handle
            glBindTexture(GL_TEXTURE_2D, textureHandle );
            
            // Set the shader sampler
            pShader->SetSampler(i);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, 0 );
        }
    }

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
