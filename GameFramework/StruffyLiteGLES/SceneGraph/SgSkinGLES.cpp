////////////////////////////////////////////////////////////////////////////////
/// SgSkinWin32GL.cpp

#include "BtMemory.h"
#include "RsMaterial.h"
#include "RsMaterialGLES.h"
#include "RsCamera.h"
#include "RsImplGLES.h"
#include "SgNodeGLES.h"
#include "SgMaterialsGLES.h"
#include "SgBoneImpl.h"
#include "SgSkinGLES.h"
#include "RsSceneGLES.h"
#include "RsShaderGLES.h"
#include "RsVertexBufferGLES.h"
#include "RsIndexBufferGLES.h"
#include "SgBoneGLES.h"

namespace SgSkinTechniques32GL
{
	const BtChar* pTechniques[] = { "RcMeshUnTextured",
							        "RcMeshTextured",
                                    "RcMeshLitUnTextured",
							        "RcMeshLitTextured"
							      };
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgSkinWin32GL::FixPointers( BaArchive *pArchive, BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgSkinFileData*) pMemory;
    
    // Mark up the skeleton
    for( BtU32 i=0; i<m_pFileData->m_numJoints; i++ )
    {
        m_pFileData->m_skeleton[i].m_pJoint = (SgNodeWinGL*)pArchive->GetResource( m_pFileData->m_skeleton[i].m_nJoint );
    }
}

////////////////////////////////////////////////////////////////////////////////
// Render

//virtual
void SgSkinWin32GL::Render()
{
	if( m_pFileData->m_bVisible == BtTrue )
	{
        m_pNode->m_pMaterials->Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
// DeferredRender

void SgSkinWin32GL::DeferredRender( SgSkinRenderable* pRenderable )
{
	RsSceneWinGL* pScene = (RsSceneWinGL*)m_pNode->m_pFileData->m_pScene;
    
	// Cache each material block
	BaMaterialBlockFileData* pMaterialBlock = pRenderable->m_pMaterialBlock;
    
	// Cache the material
	RsMaterialWinGL* pMaterial = (RsMaterialWinGL*) pMaterialBlock->m_pMaterial;
    
	// Set the vertex buffer
	pScene->pVertexBuffer( pMaterial->GetVertexType() )->SetStream();
    
	// Set the index stream
	pScene->pIndexBuffer()->SetIndices();
    
	// Cache the effect
	RsShaderWinGL* pShader = (RsShaderWinGL*)pRenderable->m_pShader;
    
	// Render the blend shape
	if( m_pNode->pBlendShape() != BtNull )
	{
		SgBlendShapeImpl* pBlendShape = (SgBlendShapeImpl*)  m_pNode->pBlendShape();
		(void)pBlendShape;
	}
    
	// Set the the shader from the material
	pShader->SetMaterial( pMaterial );
    
	// Cache the current render target
	RsRenderTarget* pRenderTarget = pRenderable->m_pRenderTarget;
    
	// Cache the camera
	const RsCamera &camera = pRenderTarget->GetCamera();
    
	const MtMatrix4& m4ViewScreen = pRenderTarget->GetCamera().GetViewProjection();
	const MtMatrix4& m4World      = m_pNode->m_pFileData->m_m4World;
	const MtMatrix4& m4View	      = camera.GetView();
	MtMatrix4 m4WorldViewScreen   = m4World * m4ViewScreen;
	MtMatrix4 m4WorldView		  = m4World * m4View;
    
	// Set the technique
	pShader->SetTechnique( "RsShaderZLTS" );
	pShader->SetMatrix( RsHandles_WorldViewScreen, m4WorldViewScreen );
	pShader->SetMatrix( RsHandles_WorldViewInverseTranspose, m4WorldView.GetInverse().GetTranspose() );
	pShader->SetMatrix( RsHandles_ViewInverseTranspose, m4View.GetInverse().GetTranspose() );
    
	// Ensure we have a material
	BtAssert( pMaterial != BtNull );
    
	MtMatrix4 m4BoneMatrices[MaxBoneMatrices];
    
	// Loop through the materials
	for( BtU32 nRenderBlock=pMaterialBlock->m_nStartRenderBlock; nRenderBlock<pMaterialBlock->m_nEndRenderBlock; nRenderBlock++ )
	{
		// Cache each render block
		BaRenderBlockFileData* pRenderBlock = pScene->pRenderBlock( nRenderBlock );
        
		// Loop through the bones in the bone palette
		for( BtU32 iBone=0; iBone<pRenderBlock->m_bonePaletteSize; iBone++ )
		{
			// Cache each bone index
			BtU32 iBoneIndex = pRenderBlock->m_bonePalette[iBone];
            
			// Cache the node
			SgBoneDX11* pBone = (SgBoneDX11*)m_pFileData->m_skeleton[iBoneIndex].m_pJoint->pBone();
            
			// Validate the node
			BtAssert( pBone != BtNull );
            
			// Copy the bone matrices
			BtMemory::Copy( &m4BoneMatrices[iBone], &pBone->m_boneTransform, sizeof(MtMatrix4) );
		}
        
		// Set the bone matrix on the shader
		pShader->SetMatrix( "s_m4BoneMatrices", m4BoneMatrices, MaxBoneMatrices );

		// Loop through the materials
		for( BtU32 nPrimitiveBlock=pRenderBlock->m_nStartPrimitiveBlock; nPrimitiveBlock<pRenderBlock->m_nEndPrimitiveBlock; nPrimitiveBlock++ )
		{
			// Cache each primitive block
			RsIndexedPrimitiveWinGL* pPrimitiveBlock = pScene->pPrimitiveBlock( nPrimitiveBlock );
            
			// Draw the primitives using the effect
			pShader->Draw( pPrimitiveBlock );
		}
	}
}
