////////////////////////////////////////////////////////////////////////////////
// RsRenderTargetWin32GL.cpp

#include "RsGLES.h"
#include "RsRenderTargetGLES.h"
#include "RsImpl.h"
#include "RsFontGLES.h"
#include "RsSpriteGLES.h"
#include "RsImplGLES.h"
#include "SgMesh.h"
#include "SgMeshGLES.h"
#include "SgSkinGLES.h"
#include "SgNode.h"
#include <new>

////////////////////////////////////////////////////////////////////////////////
// Reset

void RsRenderTargetWinGL::Reset()
{
	// Empty the renderables
	for( BtU32 sortOrder=0; sortOrder<MaxSortOrders; sortOrder++ )
	{
		m_fonts[sortOrder].Empty();
		m_textures[sortOrder].Empty();
		m_materials[sortOrder].Empty();
		m_nodes[sortOrder].Empty();
		m_sprites[sortOrder].Empty();
        m_skinNodes[sortOrder].Empty();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Add

void RsRenderTargetWinGL::Add( BtU32 sortOrder, RsFontRenderable *pFontRenderable )
{
	m_fonts[sortOrder].AddTail( pFontRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Add

void RsRenderTargetWinGL::Add( BtU32 sortOrder, RsTextureRenderable *pTextureRenderable )
{
	m_textures[sortOrder].AddTail( pTextureRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Add

void RsRenderTargetWinGL::Add( BtU32 sortOrder, RsSpriteRenderable *pSpriteRenderable )
{
	m_sprites[sortOrder].AddTail( pSpriteRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Add

void RsRenderTargetWinGL::Add( BtU32 sortOrder, RsMaterialRenderable *pMaterialRenderable )
{
	m_materials[sortOrder].AddTail( pMaterialRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Add

void RsRenderTargetWinGL::Add( BtU32 sortOrder, SgSkinRenderable *pSkinRenderable )
{
	m_skinNodes[sortOrder].AddTail( pSkinRenderable );
}

////////////////////////////////////////////////////////////////////////////////
// Add

void RsRenderTargetWinGL::Add( BtU32 sortOrder, SgMeshRenderable *pNodeRenderable )
{
	m_nodes[sortOrder].AddTail( pNodeRenderable );
}

int defaultFBO;

////////////////////////////////////////////////////////////////////////////////
// Render

void RsRenderTargetWinGL::Render()
{
    GLenum err = 0;

	// Clear the render target so we don't use it in the deferred rendering pass
	m_pCurrentRenderTarget = BtNull;
    
	if( m_pTexture != BtNull )
	{
		RsTextureWinGL *pTexture = (RsTextureWinGL*)m_pTexture;

        if( pTexture->m_pFileData->m_flags & RsTF_RenderTarget )
		{
            glBindFramebuffer(GL_FRAMEBUFFER, pTexture->m_frameBufferObject );

			glViewport( 0, 0, m_pTexture->GetWidth(), pTexture->GetHeight() );
            
            err = glGetError();
            if (err != GL_NO_ERROR)
                printf( "RsRenderTargetWinGL::Render to texture. glError: 0x%04X", err);
		}
	}
	else
	{
        const RsViewport &currentViewport = m_camera.GetViewport();
        
        // Set the viewport
        glViewport( currentViewport.m_x, currentViewport.m_y, currentViewport.m_width, currentViewport.m_height );
	}
    
	// Clear the viewport
	if( IsCleared() == BtTrue )
	{
		// Set the clear colour
		glClearColor( m_clearColour.Red(), m_clearColour.Green(), m_clearColour.Blue(), m_clearColour.Alpha() );

		// Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        err = glGetError();
        if (err != GL_NO_ERROR)
            printf( "RsRenderTargetWinGL::Render. glError: 0x%04X", err);
	}

	for( BtU32 sortOrder=0; sortOrder<MaxSortOrders; sortOrder++ )
	{
		// Render the materials
		RsMaterialRenderable *pMaterialRenderable = m_materials[sortOrder].pHead();

		while( pMaterialRenderable )
		{
			// Render the material
			RsMaterialWinGL *pMaterial = (RsMaterialWinGL*)pMaterialRenderable->m_pMaterial;

            if( pMaterial )
            {
			// Render the material
			pMaterial->Render( pMaterialRenderable );
            }
            
			// Go to the next renderable
			pMaterialRenderable = pMaterialRenderable->pNext();
		}

		// Render the nodes
		SgMeshRenderable *pNodeRenderable = m_nodes[sortOrder].pHead();

		while( pNodeRenderable )
		{
			// Render the texture
			SgMeshWinGL *pMesh = (SgMeshWinGL*)pNodeRenderable->m_pMesh;

			// Render the node
			pMesh->Render( pNodeRenderable );
            
			// Go to the next renderable
			pNodeRenderable = pNodeRenderable->pNext();
		}
        
		// Render the skin
		SgSkinRenderable *pSkinRenderable = m_skinNodes[sortOrder].pHead();
    
		while( pSkinRenderable )
		{
			SgSkinWin32GL *pSkin = (SgSkinWin32GL*)pSkinRenderable->m_pSkin;
            
			// Render the skin
			pSkin->DeferredRender( pSkinRenderable );
            
			// Go to the next renderable
			pSkinRenderable = (SgSkinRenderable*)pSkinRenderable->pNext();
		}
        
        // Render the textures
		RsTextureRenderable *pTextureRenderable = m_textures[sortOrder].pHead();

		while( pTextureRenderable )
		{
			// Render the texture
			RsTextureWinGL *pTexture = (RsTextureWinGL*)pTextureRenderable->m_pTexture;

			pTexture->Render( pTextureRenderable );
            
            err = glGetError();
            if (err != GL_NO_ERROR)
                printf( "RsTextureRenderable. glError: 0x%04X", err);
            
			// Go to the next renderable
			pTextureRenderable = pTextureRenderable->pNext();
		}

		// Render the sprites
		RsSpriteRenderable *pSpriteRenderable = m_sprites[sortOrder].pHead();

		while( pSpriteRenderable )
		{
			RsSpriteWinGL *pSprite = (RsSpriteWinGL*) pSpriteRenderable->m_pSprite;

			// Render the texture
			pSprite->Render( pSpriteRenderable );

			// Go to the next renderable
			pSpriteRenderable = pSpriteRenderable->pNext();
		}

		// Render the fonts
		RsFontRenderable *pFontRenderable = m_fonts[sortOrder].pHead();

		while( pFontRenderable )
		{
			// Cache the font
			RsFontGLES *pFont = (RsFontGLES*)pFontRenderable->m_pFont;

			// Render the font
			pFont->Render( pFontRenderable );

			// Go to the next renderable
			pFontRenderable = pFontRenderable->pNext();
		}
  	}
}
