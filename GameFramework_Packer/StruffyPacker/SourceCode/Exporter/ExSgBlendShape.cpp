///////////////////////////////////////////////////////////////////////////////
// ExSgBlendShape.cpp

// Includes
#include "StdAfx.h"
#include "FCollada.h"
#include "ExMatrix.h"
#include "ExSgBlendShape.h"
#include "ExScene.h"
#include "ExSgMesh.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgBlendShape::ExSgBlendShape( ExSgNode *pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
	m_nItems = 0;
	m_pBlendShapes.clear();
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgBlendShape::~ExSgBlendShape()
{
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgBlendShape::ChangeCoordinateSystem()
{
}

///////////////////////////////////////////////////////////////////////////////
// AddBlendShape

void ExSgBlendShape::AddBlendShape( ExSgNode* pNode )
{
	// Add each blend shape
	m_pBlendShapes.push_back( pNode );
}

///////////////////////////////////////////////////////////////////////////////
// CopyMeshes

void ExSgBlendShape::CopyBlendShapes()
{
	// Set our mesh
	m_pMesh = m_pNode->m_pMesh;

	// Cache the number of blend shapes
	BtU32 nBlendShapes = (BtU32) m_pBlendShapes.size();

	// Cache the number of material blocks
	BtU32 nMaterialBlocks = m_pMesh->NumMaterialBlocks();

	// Flatten the material blocks
	for( BtU32 iMaterialBlock=0; iMaterialBlock<nMaterialBlocks; iMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_pMesh->pMaterialBlock( iMaterialBlock );

		// Cache the number of Render blocks
		BtU32 nRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

		// Flatten the Render blocks
		for( BtU32 iRenderBlock=0; iRenderBlock<nRenderBlocks; iRenderBlock++ )
		{
			// Cache each render block
			ExRenderBlock& renderBlock = pMaterialBlock->m_renderBlocks[iRenderBlock];

			// Cache the number of vertex
			BtU32 nVertices = (BtU32) renderBlock.m_pOptimisedVertex.size();

			// Add the vertex
			for( BtU32 iVertex=0; iVertex<nVertices; iVertex++ )
			{
				// Cache each vertex
				ExVertex* pVertex = (ExVertex*) renderBlock.m_pOptimisedVertex[iVertex];

				for( BtU32 iBlendShape=0; iBlendShape<nBlendShapes; iBlendShape++ )
				{
					// Cache each node
					ExSgNode* pNode = m_pBlendShapes[iBlendShape];

					// Cache each mesh
					ExSgMesh* pMesh = pNode->m_pMesh;

					// Cache each position
					ExVertex* pBlendVertex = pMesh->pMaterialBlock( iMaterialBlock )->m_renderBlocks[iRenderBlock].m_pOptimisedVertex[iVertex];

					if( iBlendShape == 0 )
					{
						pVertex->Position1( pBlendVertex->Position() );
					}
					else if( iBlendShape == 1 )
					{
						pVertex->Position2( pBlendVertex->Position() );
					}
					else if( iBlendShape == 2 )
					{
						pVertex->Position3( pBlendVertex->Position() );
					}
				}
			}

			// Cache any vertex
			ExVertex* pVertex = (ExVertex*) renderBlock.m_pOptimisedVertex[0];

			// Reflect any changes in the material type
			pMaterialBlock->m_pMaterial->VertexType( pVertex->GetVertexType() );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgBlendShape::CopyAttributes()
{
	m_pNode->m_blendShapeFileData.m_weight[0] = 1.0f;
	m_pNode->m_blendShapeFileData.m_weight[1] = 0;
	m_pNode->m_blendShapeFileData.m_weight[2] = 0;
	m_pNode->m_blendShapeFileData.m_weight[3] = 0;
	m_pNode->m_blendShapeFileData.m_items = m_nItems;
}
