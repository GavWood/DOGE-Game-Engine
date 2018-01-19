////////////////////////////////////////////////////////////////////////////////
/// SgSkinWin32GL.cpp

#include "RsPlatform.h"
#include "BtMemory.h"
#include "RsMaterial.h"
#include "RsMaterialImpl.h"
#include "RsCamera.h"
#include "RsManagerImpl.h"
#include "RsShaderImpl.h"
#include "SgNodeImpl.h"
#include "SgMaterialsImpl.h"
#include "SgBoneImpl.h"
#include "SgSkinImpl.h"
#include "RsSceneImpl.h"
#include "RsRenderTarget.h"
#include "RsVertexBufferImpl.h"
#include "RsIndexBufferImpl.h"
#include "SgBoneImpl.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgSkinImpl::FixPointers( BaArchive *pArchive, BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgSkinFileData*)pMemory;

	// Mark up the skeleton
	for (BtU32 i = 0; i < m_pFileData->m_numJoints; i++)
	{
		m_pFileData->m_skeleton[i].m_pJoint = (SgNodeImpl*)pArchive->GetResource(m_pFileData->m_skeleton[i].m_nJoint);
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetDuplicate

void SgSkinImpl::GetDuplicate()
{
	// Mark up the skeleton
	for (BtU32 i = 0; i < m_pFileData->m_numJoints; i++)
	{
		m_pFileData->m_skeleton[i].m_pJoint = m_pFileData->m_skeleton[i].m_pJoint->GetDuplicate();
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

//virtual
void SgSkinImpl::Render()
{
    m_pSkinNode->m_pMaterials->Render();
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SgSkinImpl::Render( SgSkinRenderable* pRenderable )
{
	RsSceneImpl* pScene = (RsSceneImpl*)m_pSkinNode->m_pFileData->m_pScene;

	// Cache each material block
	BaMaterialBlockFileData* pMaterialBlock = pRenderable->m_pMaterialBlock;

	// Cache the material
	RsMaterialImpl* pMaterial = (RsMaterialImpl*) pMaterialBlock->m_pMaterial;

	// Set the vertex buffer
	pScene->pVertexBuffer( pMaterial->GetVertexType() )->SetStream();

	// Set the index stream
	pScene->pIndexBuffer()->SetIndices();

	// Cache the effect
	RsShaderImpl* pShader = (RsShaderImpl*)pRenderable->m_pShader;

	// Render the blend shape
	if( m_pSkinNode->pBlendShape() != BtNull )
	{
		SgBlendShapeImpl* pBlendShape = (SgBlendShapeImpl*)  m_pSkinNode->pBlendShape();
		(void)pBlendShape;
	}

	// Set the the shader from the material
	pShader->SetMaterial( pMaterial );

	// Cache the current render target
	RsRenderTarget* pRenderTarget = pRenderable->m_pRenderTarget;

	// Cache the camera
	const RsCamera &camera = pRenderTarget->GetCamera();

	const MtMatrix4& m4ViewScreen = pRenderTarget->GetCamera().GetViewProjection();
	const MtMatrix4& m4World      = m_pSkinNode->m_pFileData->m_m4World;
	const MtMatrix4& m4View	      = camera.GetView();
	MtMatrix4 m4WorldViewScreen   = m4World * m4ViewScreen;
	MtMatrix4 m4WorldView		  = m4World * m4View;

	// Cache the technique
	const BtChar* pTechnique = pMaterial->GetTechniqueName();

	if (strstr(pTechnique, "RsShaderZLT"))
	{
		int a = 0;
		a++;
	}

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

		BtAssert( pRenderBlock->m_bonePaletteSize <= MaxBoneMatrices );

		// Loop through the bones in the bone palette
		for (BtU32 iBone = 0; iBone < pRenderBlock->m_bonePaletteSize; iBone++)
		{
			// Cache each bone index
			BtU32 iBoneIndex = pRenderBlock->m_bonePalette[iBone];

			// Cache the node
			SgBoneImpl* pBone = (SgBoneImpl*)m_pFileData->m_skeleton[iBoneIndex].m_pJoint->pBone();
			
			// Validate the node
			BtAssert(pBone != BtNull);

			// Copy the bone matrices
			BtMemory::Copy(&m4BoneMatrices[iBone], &pBone->m_boneTransform, sizeof(MtMatrix4));
		}

		// Set the bone matrix on the shader
		pShader->SetMatrix( "s_m4BoneMatrices", m4BoneMatrices, MaxBoneMatrices );

		// Loop through the materials
		for( BtU32 nPrimitiveBlock=pRenderBlock->m_nStartPrimitiveBlock; nPrimitiveBlock<pRenderBlock->m_nEndPrimitiveBlock; nPrimitiveBlock++ )
		{
			// Cache each primitive block
			RsIndexedPrimitive* pPrimitiveBlock = pScene->pPrimitiveBlock( nPrimitiveBlock );

			// Draw the primitives using the effect
			pShader->Draw( pPrimitiveBlock );
		}
	}
}
