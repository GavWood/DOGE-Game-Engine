////////////////////////////////////////////////////////////////////////////////
/// SgMaterialsWinGL.cpp

#include "SgNodeWinGL.h"
#include "SgMeshWinGL.h"
#include "RsImplWinGL.h"
#include "RsMaterial.h"
#include "SgMaterialsWinGL.h"
#include "RsShaderWinGL.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void SgMaterialsWinGL::FixPointers( BaArchive *pArchive, BtU8* pMemory )
{
	// Mark up the material file data
	m_pMaterialsFileData = (BaMaterialsFileData*) pMemory;

	// Advance past the number of materials
	pMemory += sizeof( BaMaterialsFileData );

	// Mark up the material blocks
	m_pMaterialBlocks = (BaMaterialBlockFileData*) pMemory;

	// Cache the number of materials
	BtU32 nMaterials = NumMaterials();

	// Loop through each of the materials
	for( BtU32 iMaterialBlock=0; iMaterialBlock<nMaterials; iMaterialBlock++ )
	{
		// Cache each material block
		BaMaterialBlockFileData& materialBlock = m_pMaterialBlocks[iMaterialBlock];

		if( m_pNode->IsDuplicate() == BtFalse )
		{
			// Find the material
			RsMaterialWinGL* pMaterial = (RsMaterialWinGL*)pArchive->GetResource( materialBlock.m_nMaterial );

			// Set the material block
			materialBlock.m_pMaterial = pMaterial;
		}
		else
		{
			// Duplicate the material block
			//materialBlock.m_pMaterial = (RsMaterialWin32*)materialBlock.m_pMaterial->pDuplicate();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Render

void SgMaterialsWinGL::Render()
{
	// Cache the renderer
	RsImplWinGL *pImpl = (RsImplWinGL*)RsImpl::pInstance();

	// Cache the current render target
	RsRenderTargetWinGL *pCurrentRenderTarget = (RsRenderTargetWinGL*)RsRenderTarget::GetCurrent();

	// Get the shader
	RsShaderWinGL *pShader = (RsShaderWinGL*)RsShader::GetCurrent();

	// Cache the number of materials
	BtU32 numMaterials = m_pMaterialsFileData->m_nMaterials;

	if( m_pNode->NodeType() & SgType_Mesh )
	{
		// Loop through the materials
		for( BtU32 iMaterialBlock=0; iMaterialBlock<numMaterials; iMaterialBlock++ )
		{
			// Cache each material block
			BaMaterialBlockFileData& materialBlock = m_pMaterialBlocks[iMaterialBlock];

			// Cache the sort order
			BtU32 sortOrder = materialBlock.m_pMaterial->GetSortOrder();

			// Set the next renderable
			SgMeshRenderable* pRenderable = (SgMeshRenderable*) pImpl->AddMesh();
			pRenderable->m_pMesh = m_pNode->GetMesh();
			pRenderable->m_pMaterialBlock = &materialBlock;

			// Add the node to the renderable list
			pCurrentRenderTarget->Add( sortOrder, pRenderable );
		}
	}
	else if( m_pNode->NodeType() & SgType_Skin )
	{
		// Loop through the materials
		for( BtU32 iMaterialBlock=0; iMaterialBlock<numMaterials; iMaterialBlock++ )
		{
			// Cache each material block
			BaMaterialBlockFileData& materialBlock = m_pMaterialBlocks[iMaterialBlock];

			// Cache the sort order
			BtU32 sortOrder = materialBlock.m_pMaterial->GetSortOrder();

			// Set the next renderable
			SgSkinRenderable* pRenderable = (SgSkinRenderable*) pImpl->AddSkin();
			pRenderable->m_pSkin = m_pNode->pSkin();
			pRenderable->m_pMaterialBlock = &materialBlock;

			// Add the node to the renderable list
			pCurrentRenderTarget->Add( sortOrder, pRenderable );
		}
	}
}