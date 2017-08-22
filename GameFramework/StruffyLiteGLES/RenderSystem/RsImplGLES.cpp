////////////////////////////////////////////////////////////////////////////////
// RsImplWin32GL.cpp

// Includes
#include "ApConfig.h"
#include "BtString.h"
#include "RsImplGLES.h"
#include "RsRenderTargetGLES.h"
#include "SgNode.h"
#include "RsCapsGLES.h"
#include "RsShaderGLES.h"

RsImplWinGL implWin32GL;
RsCapsWinGL capsWinGL;
BtU32 RsImplWinGL::m_vertexBuffer = 0;

////////////////////////////////////////////////////////////////////////////////
// GetCaps

RsCaps *RsImplWinGL::GetCaps()
{
	return &capsWinGL;
}

////////////////////////////////////////////////////////////////////////////////
// Create

void RsImplWinGL::Create()
{
	RsImpl::Create();

	// Create the vertex and index buffers
	CreateOnDevice();

	ResetRenderables();

	capsWinGL.GetModes();
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsImplWinGL::CreateOnDevice()
{
	glGenBuffers(1, &m_vertexBuffer);
}

////////////////////////////////////////////////////////////////////////////////
// GetVertexBuffer

BtU32 RsImplWinGL::GetVertexBuffer()
{
	return m_vertexBuffer;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void RsImplWinGL::RemoveFromDevice()
{
	glDeleteBuffers(1, &m_vertexBuffer);
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void RsImplWinGL::Destroy()
{
	RemoveFromDevice();
}

////////////////////////////////////////////////////////////////////////////////
// ResetRenderables

void RsImplWinGL::ResetRenderables()
{
	m_runTimeHeap = 0;

	for( BtU32 i=0; i<m_renderTargets.GetNumItems(); i++ )
	{
		m_renderTargets[i].Reset();
	}

	m_primitives.Empty();
}

////////////////////////////////////////////////////////////////////////////////
// EmptyRenderTargets

void RsImplWinGL::EmptyRenderTargets()
{
	m_renderTargets.Empty();
	m_currentVertex = 0;
	m_currentIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////
// BeginScene

void RsImplWinGL::BeginScene()
{
}

////////////////////////////////////////////////////////////////////////////////
// GetNewRenderTarget

RsRenderTarget *RsImplWinGL::GetNewRenderTarget()
{
	// Add a render target
	RsRenderTargetWinGL *pRenderTarget = m_renderTargets.Add();

	// Return the new render target
	return pRenderTarget;
}

////////////////////////////////////////////////////////////////////////////////
// AddFont

RsFontRenderable *RsImplWinGL::AddFont()
{
	RsFontRenderable *pFontRenderable = (RsFontRenderable*)AllocateGraphicsMemory( sizeof( RsFontRenderable ) );
	pFontRenderable->m_pRenderTarget = RsRenderTarget::GetCurrent();
	pFontRenderable->m_pShader = RsShader::GetCurrent();
	return pFontRenderable;
}

////////////////////////////////////////////////////////////////////////////////
// AddSprite

RsSpriteRenderable *RsImplWinGL::AddSprite()
{
	RsSpriteRenderable *pSpriteRenderable = (RsSpriteRenderable*)AllocateGraphicsMemory( sizeof( RsSpriteRenderable ) );
	pSpriteRenderable->m_pRenderTarget = RsRenderTarget::GetCurrent();
	pSpriteRenderable->m_pShader = RsShader::GetCurrent();
	return pSpriteRenderable;
}

////////////////////////////////////////////////////////////////////////////////
// AddTexture

RsTextureRenderable *RsImplWinGL::AddTexture()
{
	RsTextureRenderable *pTextureRenderable = (RsTextureRenderable*)AllocateGraphicsMemory( sizeof( RsTextureRenderable ) );
	pTextureRenderable->m_pRenderTarget = RsRenderTarget::GetCurrent();
	pTextureRenderable->m_pShader = RsShader::GetCurrent();
	return pTextureRenderable;
}

////////////////////////////////////////////////////////////////////////////////
// AddMaterial

RsMaterialRenderable *RsImplWinGL::AddMaterial()
{
	RsMaterialRenderable *pMaterialRenderable = (RsMaterialRenderable*)AllocateGraphicsMemory( sizeof( RsMaterialRenderable ) );
	pMaterialRenderable->m_pRenderTarget = RsRenderTarget::GetCurrent();
	pMaterialRenderable->m_pShader = RsShader::GetCurrent();

	BtAssert( pMaterialRenderable->m_pShader != BtNull );

	return pMaterialRenderable;
}

////////////////////////////////////////////////////////////////////////////////
// AddMesh

SgMeshRenderable *RsImplWinGL::AddMesh()
{
	SgMeshRenderable *pNodeRenderable = (SgMeshRenderable*)AllocateGraphicsMemory( sizeof( SgMeshRenderable ) );
	pNodeRenderable->m_pRenderTarget = RsRenderTarget::GetCurrent();
	pNodeRenderable->m_pShader = RsShader::GetCurrent();
	return pNodeRenderable;
}

////////////////////////////////////////////////////////////////////////////////
// AddSkin

SgSkinRenderable *RsImplWinGL::AddSkin()
{
	SgSkinRenderable *pNodeRenderable = (SgSkinRenderable*)AllocateGraphicsMemory( sizeof( SgSkinRenderable ) );
	pNodeRenderable->m_pRenderTarget = RsRenderTarget::GetCurrent();
	pNodeRenderable->m_pShader = RsShader::GetCurrent();
	return pNodeRenderable;
}

////////////////////////////////////////////////////////////////////////////////
// AddPrimitive

RsPrimitiveWinGL *RsImplWinGL::AddPrimitive()
{
	return m_primitives.Add();
}

////////////////////////////////////////////////////////////////////////////////
// StartVertex

RsVertex3 *RsImplWinGL::StartVertex()
{
	return &m_vertex[m_currentVertex];
}

////////////////////////////////////////////////////////////////////////////////
// EndVertex

void RsImplWinGL::EndVertex( BtU32 vertexCount )
{
	m_currentVertex += vertexCount;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void RsImplWinGL::Render()
{
	BeginScene();

    GLint defaultFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    
	// Clear the render target so we don't use it in the deferred rendering pass
	m_pCurrentRenderTarget = BtNull;
    
	// Cache the number of render targets
	BtU32 numRenderTargets = m_renderTargets.GetNumItems();

	for( BtU32 i=0; i<numRenderTargets; i++ )
	{
		// Cache each render target
		RsRenderTargetWinGL &renderTarget = m_renderTargets[i];

        // Always restore the default frame buffer at the start before changing it
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO );
        
		// render each render target
		renderTarget.Render();
	}

	EndScene();

	ResetRenderables();
}
