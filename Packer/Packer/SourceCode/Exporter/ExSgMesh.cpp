////////////////////////////////////////////////////////////////////////////////
// ExSgMesh.cpp

// Includes
#include "StdAfx.h"
#include "ExMatrix.h"
#include "ExSgMesh.h"
#include "ExStrip.h"
#include "ExScene.h"
#include "ExIndexBuffer.h"
#include "ExVertexBuffer.h"
#include "ExVertex.h"
#include "SgRigidBody.h"
#include "ApConfig.h"
#include "PaTopState.h"
#include "FCollada.h"
#include "PaRendering.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgMesh::ExSgMesh( ExSgNode *pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgMesh::~ExSgMesh()
{
	BtU32 nSize = (BtU32) m_materialBlocks.size();

	for( BtU32 iMaterialBlock=0; iMaterialBlock<nSize; ++iMaterialBlock )
	{
		delete m_materialBlocks[iMaterialBlock];
	}
}

void ExSgMesh::GroupDrawing()
{
	MakeRenderGroups();
	OptimiseGeometry();
	BoundVertex();
}

////////////////////////////////////////////////////////////////////////////////
// MakeRenderGroups

void ExSgMesh::MakeRenderGroups()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 iMaterialBlock=0; iMaterialBlock<nMaterialBlocks; iMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[iMaterialBlock];

		// Cache the number of indices
		BtU32 nIndices = (BtU32) pMaterialBlock->m_indices.size();

		ExRenderBlock renderBlock;

		for( BtU32 iIndex = 0; iIndex < nIndices; iIndex++ )
		{
			// Add the index
			renderBlock.m_indices.push_back( pMaterialBlock->m_indices[iIndex] );

			// Add the vertex
			renderBlock.m_vertex.push_back( pMaterialBlock->m_pVertex[iIndex] );
		}

		// Add the render block
		pMaterialBlock->m_renderBlocks.push_back( renderBlock );
	}
}

////////////////////////////////////////////////////////////////////////////////
// OptimiseGeometry

void ExSgMesh::OptimiseGeometry()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 iMaterialBlock=0; iMaterialBlock<nMaterialBlocks; iMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[iMaterialBlock];

		// Cache the number of Render blocks
		BtU32 nRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

		// Flatten the Render blocks
		for( BtU32 iRenderBlock=0; iRenderBlock<nRenderBlocks; iRenderBlock++ )
		{
			// Cache each render block
			ExRenderBlock& renderBlock = pMaterialBlock->m_renderBlocks[iRenderBlock];

			// Create the geometry stripper
			ExGeometryOptimise optimise( pMaterialBlock, &renderBlock );

			BtBool optimiseEnabled = m_pNode->isStripped();
            (void)optimiseEnabled;
            
			ErrorLog::Printf( "Merging similar vertex in node %s\r\n", m_pNode->pName() );

			if( BtFalse )//m_pNode->isMergeLikeVertex() == BtTrue )
			{
				optimise.MergeVertex();
			}
			else
			{
				optimise.CopyVertexNoMerge();
			}

			if( m_pNode->isStripped() == BtTrue )
			{
				ErrorLog::Printf( "Stripping node %s\r\n", m_pNode->pName() );
				optimise.Strip();
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// BoundVertex

void ExSgMesh::BoundVertex()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 iMaterialBlock=0; iMaterialBlock<nMaterialBlocks; iMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[iMaterialBlock];

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

				// Cache each position
				MtVector3 v3Position = pVertex->Position();

				// Expand the bounding box
				if( ( iMaterialBlock == 0 ) && ( iRenderBlock == 0 ) && ( iVertex == 0 ) )
				{
					m_AABB = MtAABB( v3Position, v3Position );
					m_sphere = MtSphere( v3Position, 0 );
				}
				else
				{
					m_AABB.Min( v3Position.Min( m_AABB.Min() ) );
					m_AABB.Max( v3Position.Max( m_AABB.Max() ) );
					m_sphere.ExpandBy( v3Position );
				}
			}
		}
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// MoveToVertexBuffer

void ExSgMesh::MoveToVertexBuffers()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 iMaterialBlock=0; iMaterialBlock<nMaterialBlocks; iMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[iMaterialBlock];

		// Cache the number of Render blocks
		BtU32 nRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

		// Flatten the Render blocks
		for( BtU32 iRenderBlock=0; iRenderBlock<nRenderBlocks; iRenderBlock++ )
		{
			// Cache each render block
			ExRenderBlock& renderBlock = pMaterialBlock->m_renderBlocks[iRenderBlock];

			// Cache the number of vertex
			BtU32 nVertices = (BtU32) renderBlock.m_pOptimisedVertex.size();

			// Export only material blocks with primitives
			if( nVertices > 0 )
			{
				BtU32 nBaseVertex = 0;

				// Cache each vertex
				ExVertex* pVertex = renderBlock.m_pOptimisedVertex[0];

				// Get the vertex buffer
				ExVertexBuffer* pVertexBuffer = m_pScene->pVertexBuffer( pVertex->GetVertexType() );

				// Get the base vertex
				nBaseVertex = pVertexBuffer->Size();

				// Add the vertex
				for( BtU32 iVertex=0; iVertex<nVertices; iVertex++ )
				{
					// Cache each vertex
					ExVertex* pVertex = renderBlock.m_pOptimisedVertex[iVertex];

					// Add each vertex
					pVertexBuffer->AddVertex( pVertex );
				}

				BtU32 nBaseIndex = (BtU32) m_pScene->pIndexBuffer()->nSize();

				BtU32 nIndices = 0;

				BtU32 offset = 0;

				if( PaTopState::Instance().IsBaseVertex() )
				{
					offset += nBaseVertex;
				}

				BtChar *pStr = m_pNode->pName();

				if( strstr( pStr, "Wing" ) )
				{
					int a=0;
					a++;
				}

				ExIndexBuffer *pIndexBuffer = m_pScene->pIndexBuffer();

				// Add the indices
				if( m_pNode->isStripped() == BtTrue )
				{
					nIndices = (BtU32) renderBlock.m_strippedIndex.size();

					for( BtU32 iIndex=0; iIndex<nIndices; iIndex++ )
					{
						pIndexBuffer->AddIndex( renderBlock.m_strippedIndex[iIndex] + offset );
					}
				}
				else
				{
					nIndices = (BtU32) renderBlock.m_optimisedIndex.size();

					for( BtU32 iIndex=0; iIndex<nIndices; iIndex++ )
					{
						pIndexBuffer->AddIndex( renderBlock.m_optimisedIndex[iIndex] + offset );
					}
				}

				// Add each primitive
				RsIndexedPrimitive primitive;

				BtU32 numprimitives = nIndices;

				if( renderBlock.m_primitiveType == ExPT_STRIP )
				{
					numprimitives -= 2;
				}
				else if( renderBlock.m_primitiveType == ExPT_FAN )
				{
					numprimitives -= 2;
				}
				else if( renderBlock.m_primitiveType == ExPT_LIST )
				{
					numprimitives /= 3;
				}
                
                primitive.m_primitiveType = PaExportSizes::GetTriangleType(renderBlock.m_primitiveType);
				primitive.m_baseVertexIndex = nBaseVertex;
				primitive.m_startIndex  = nBaseIndex;
				primitive.m_minIndex    = 0;
				primitive.m_numVertices = nVertices;
				primitive.m_primitives  = numprimitives;
				primitive.m_numIndices  = nIndices;
				primitive.m_indexType   = 0;								// Not set for now
				renderBlock.m_primitives.push_back( primitive );
			}

			int a=0;
			a++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgMesh::ChangeCoordinateSystem()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 nMaterialBlock=0; nMaterialBlock<nMaterialBlocks; nMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[nMaterialBlock];

		// Cache the number of vertex
		BtU32 nVertex = (BtU32) pMaterialBlock->m_pVertex.size();

		// Flip the z position
		for( BtU32 i=0; i<nVertex; i++ )
		{
			ExVertex* pVertex = pMaterialBlock->m_pVertex[i];
			
			pVertex->ChangeCoordinateSystem();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgMesh::CopyAttributes()
{
	m_pNode->m_meshFileData.m_AABB = m_AABB;
	m_pNode->m_meshFileData.m_sphere = m_sphere;
	m_pNode->m_meshFileData.m_nMaterials = (BtU32) m_pNode->m_materialBlocks.size();
}
