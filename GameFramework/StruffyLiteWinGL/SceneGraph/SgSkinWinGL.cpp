////////////////////////////////////////////////////////////////////////////////
/// SgSkinWin32GL.cpp

#include "RsPlatform.h"
#include "BtMemory.h"
#include "RsMaterial.h"
#include "RsMaterialWinGL.h"
#include "RsCamera.h"
#include "RsImplWinGL.h"
#include "RsShaderWinGL.h"
#include "SgNodeWinGL.h"
#include "SgMaterialsWinGL.h"
#include "SgBoneImpl.h"
#include "SgSkinWinGL.h"
#include "RsSceneWinGL.h"
#include "RsRenderTarget.h"
#include "RsVertexBufferWinGL.h"
#include "RsIndexBufferWinGL.h"
#include "SgBoneWinGL.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgSkinWinGL::FixPointers( BaArchive *pArchive, BtU8* pMemory )
{
	// Set the file data
	m_pFileData = (BaSgSkinFileData*) pMemory;

	// Mark up the skeleton
	for( BtU32 i=0; i<m_pFileData->m_numJoints; i++ )
	{
		m_pFileData->m_skeleton[i].m_pJoint = (SgNodeWinGL*)  pArchive->GetResource( m_pFileData->m_skeleton[i].m_nJoint );
		//m_pFileData->m_skeleton[i].m_pJoint = m_pFileData->m_skeleton[i].m_pJoint->GetDuplicate();
		int a = 0;
		a++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

//virtual
void SgSkinWinGL::Render()
{
	if( m_pFileData->m_bVisible == BtTrue )
	{
		// Cache the camera frustum
		const RsFrustum& frustum = RsRenderTargetWinGL::GetCurrent()->GetCamera().GetFrustum();

		// Cache the position
		const MtVector3& v3Position = m_pNode->GetWorldTransform().GetTranslation();

		// Get the AABB
		MtAABB AABB = m_pFileData->m_AABB;

		// Transform it by the world transform of this node
		AABB.Transform( m_pNode->GetWorldTransform() );

		// Get the max and min
		MtVector3 v3Min = AABB.Min();
		MtVector3 v3Max = AABB.Max();

		// Get the center
		MtVector3 v3Center = AABB.Center();

		// Get the diagonal
		MtVector3 v3Diff = v3Max - v3Min;

		// Get the radius
		BtFloat fRadius = v3Diff.Magnitude() * 0.5f;

		// Are we inside or outside the frustum
		if( 1 ) //frustum.IsInside( v3Center, fRadius ) == BtTrue )
		{
			m_pNode->m_pMaterials->Render();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void SgSkinWinGL::Render( SgSkinRenderable* pRenderable )
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

	// Cache the current render target
	RsRenderTarget* pRenderTarget = pRenderable->m_pRenderTarget;

	// Cache the camera
	const RsCamera &camera = pRenderTarget->GetCamera();

	const MtMatrix4& m4ViewScreen = pRenderTarget->GetCamera().GetViewProjection();
	const MtMatrix4& m4World      = m_pNode->m_pFileData->m_m4World;
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

	// Set the the shader from the material
	pShader->SetMaterial(pMaterial);

	pShader->SetMatrix( RsHandles_WorldViewScreen, m4WorldViewScreen );
	pShader->SetMatrix( RsHandles_WorldViewInverseTranspose, m4WorldView.GetInverse().GetTranspose() );
	pShader->SetMatrix( RsHandles_ViewInverseTranspose, m4View.GetInverse().GetTranspose() );
	pShader->SetMatrix( RsHandles_World, m4World);

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
			SgBoneWinGL* pBone = (SgBoneWinGL*)m_pFileData->m_skeleton[iBoneIndex].m_pJoint->pBone();
			
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
			RsIndexedPrimitiveWinGL* pPrimitiveBlock = pScene->pPrimitiveBlock( nPrimitiveBlock );

			// Draw the primitives using the effect
			pShader->Draw( pPrimitiveBlock );
		}
	}
}
