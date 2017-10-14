
///////////////////////////////////////////////////////////////////////////////
// ExSgSkin.cpp

// Includes
#include "StdAfx.h"
#include "ExMatrix.h"
#include "ExSgSkin.h"
#include "ExStrip.h"
#include "ExScene.h"
#include "ExIndexBuffer.h"
#include "ExVertexBuffer.h"
#include "ExVertex.h"
#include "SgRigidBody.h"
#include "PaTopState.h"
#include "FCollada.h"
#include "PaRendering.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExSgSkin::ExSgSkin( ExSgNode *pNode, ExScene *pScene )
{
	m_pNode = pNode;
	m_pScene = pScene;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExSgSkin::~ExSgSkin()
{
}

void ExSgSkin::GroupDrawing()
{
	MakeBoneGroups();
	OptimiseGeometry();
	BoundVertex();
}

////////////////////////////////////////////////////////////////////////////////
// addBoneToGroup

BtS32 ExRenderBlock::addBoneToPalette( BtS32 iBoneIndex )
{
	for( BtU32 iBone = 0; iBone < MaxBoneMatrices; ++iBone )
	{
		if( m_bonePalette[ iBone ] == iBoneIndex )
		{
			return iBone;
		}
		else if( m_bonePalette[ iBone ] == -1 )
		{
			m_bonePalette[ iBone ] = iBoneIndex;

			// Increment the number of bone palette spaces used
			++m_bonePaletteSize;

			return iBone;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////
// addFaceToBoneGroup

BtBool ExRenderBlock::addTriangleToGroup( ExMaterialBlock* pMaterialBlock, BtU32 iIndex )
{
	ExVertex vertex[3];

	for( BtU32 iVertex = 0; iVertex < 3; iVertex++ )
	{
		// Cache the index
		BtU32 iVertexIndex = pMaterialBlock->m_indices[iIndex + iVertex];

		// Cache each vertex
		ExVertex* pVertex = (ExVertex*) pMaterialBlock->m_pVertex[iVertexIndex];

		// Copy the vertex attributes across
		vertex[iVertex] = *pVertex;

		// Loop through the bones
		for( BtU32 iBone = 0; iBone < MaxBoneInfluence; ++iBone )
		{
			BtS32 iBoneIndex = pVertex->Bone( iBone );

			if( iBoneIndex != -1 )
			{
				BtS32 iRelativeIndex = addBoneToPalette( iBoneIndex );
				
				if( iRelativeIndex == -1 )
				{
					return BtFalse;
				}
				else
				{
					BtAssert( iRelativeIndex < MaxBoneMatrices );

					// Base the new vertex on the existing one
					vertex[iVertex].Bone( iBone, iRelativeIndex );
				}
			}
		}

		int a=0;
		a++;
	}

	// Add the triangle
	for( BtU32 iVertex = 0; iVertex < 3; iVertex++ )
	{
		// Add the index
		m_indices.push_back( (BtU32) m_vertex.size() );

		ExVertex* pVertex = new ExVertex;
		*pVertex = vertex[iVertex];
		
		// Copy the relative bone indices across
		for( BtU32 iBone=0; iBone<MaxBoneInfluence; ++iBone )
		{
			BtU32 iCurrentBone = vertex[iVertex].Bone( iBone );

			pVertex->Bone( iBone, iCurrentBone );

			if( iCurrentBone != -1 )
			{
				// Validate the bone indices
				BtAssert( iCurrentBone < MaxBoneMatrices );

				// Ensure the bone palette has enough bones for this index
				BtAssert( iCurrentBone < (BtU32) m_bonePaletteSize );
			}
		}

		m_vertex.push_back( pVertex );
	}

	// Ensure that the number of vertices equals the number of indices
	BtAssert( m_indices.size() == m_vertex.size() );
	return BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// MakeBoneGroups

void ExSgSkin::MakeBoneGroups()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 nMaterialBlock=0; nMaterialBlock<nMaterialBlocks; nMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[nMaterialBlock];
		
		// Cache the number of indices
		BtU32 nIndices = (BtU32) pMaterialBlock->m_indices.size();

		// Validate that we have a whole number of triangles
		BtAssert( ( nIndices % 3 ) == 0 );

		// Calculate the number of triangles
		BtU32 nTriangles = nIndices / 3;

		ExRenderBlock renderBlock;

		BtU32 nSize = sizeof(BtBool) * nTriangles;
		BtBool* pTrianglesFitted = (BtBool*) BtMemory::Allocate( nSize );
		BtMemory::Set( pTrianglesFitted, 0, nSize );

		BtU32 TrianglesFitted = 0;

		while( TrianglesFitted < nTriangles )
		{
			// Try and fit any of the triangles into the bone group
			for( BtU32 iTriangle=0; iTriangle<nTriangles; iTriangle++ )
			{
				if( pTrianglesFitted[iTriangle] == BtFalse )
				{
					if( renderBlock.addTriangleToGroup( pMaterialBlock, iTriangle * 3 ) == BtTrue )
					{
						// Flag the triangle as fitted
						pTrianglesFitted[iTriangle] = BtTrue;

						// Increment the number of triangles
						TrianglesFitted++;
					}
				}
			}

			// Add the render block
			pMaterialBlock->m_renderBlocks.push_back( renderBlock );

			// Empty the bone matrix palette and try until all the triangles are assigned
			renderBlock.Empty();
		}

		BtMemory::Free( pTrianglesFitted );
	}
}

////////////////////////////////////////////////////////////////////////////////
// OptimiseGeometry

void ExSgSkin::OptimiseGeometry()
{
	// Cache the number of material blocks
	BtU32 nMaterialBlocks = (BtU32) m_materialBlocks.size();

	// Flatten the material blocks
	for( BtU32 nMaterialBlock=0; nMaterialBlock<nMaterialBlocks; nMaterialBlock++ )
	{
		// Cache each material block
		ExMaterialBlock* pMaterialBlock = m_materialBlocks[nMaterialBlock];

		// Cache the number of Render blocks
		BtU32 nRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

		// Flatten the Render blocks
		for( BtU32 nRenderBlock=0; nRenderBlock<nRenderBlocks; nRenderBlock++ )
		{
			// Cache each render block
			ExRenderBlock& renderBlock = pMaterialBlock->m_renderBlocks[nRenderBlock];

			// Create the geometry stripper
			ExGeometryOptimise strip( pMaterialBlock, &renderBlock );

			strip.MergeVertex();
			
			strip.Strip();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// BoundVertex

void ExSgSkin::BoundVertex()
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
				}
				else
				{
					m_AABB.Min( v3Position.Min( m_AABB.Min() ) );
					m_AABB.Max( v3Position.Max( m_AABB.Max() ) );
				}
			}
		}
	}

	// Set the AABB in the Skin file data
	m_pNode->m_skinFileData.m_AABB = m_AABB;
	m_pNode->m_skinFileData.m_sphere = MtSphere( m_AABB.Center(), m_AABB.Radius() );
}

////////////////////////////////////////////////////////////////////////////////
// MoveToVertexBuffers

void ExSgSkin::MoveToVertexBuffers()
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
				// Cache each vertex
				ExVertex* pVertex = renderBlock.m_pOptimisedVertex[0];

				BtU32 vertexType = pVertex->GetVertexType();

				// Cache the vertex buffer
				ExVertexBuffer* pVertexBuffer = m_pScene->pVertexBuffer( vertexType );

				BtU32 nBaseVertex = 0;

				// Get the base vertex
				nBaseVertex = pVertexBuffer->Size();

				// Add the vertex
				for( BtU32 iVertex=0; iVertex<nVertices; iVertex++ )
				{
					// Cache each vertex
					ExVertex* pCopyVertex = renderBlock.m_pOptimisedVertex[iVertex];

					// Add each vertex
					pVertexBuffer->AddVertex( pCopyVertex );
				}
				
				// Add the indices
				BtU32 nIndices = (BtU32) renderBlock.m_optimisedIndex.size();

				// If we are packing for OpenGL we need to add on a base
				if( ( PaTopState::GetPlatform() == PackerPlatform_GLES ) ||
					( PaTopState::GetPlatform() == PackerPlatform_WinGL ) ||
					( PaTopState::GetPlatform() == PackerPlatform_OSX )
				  )
				{
					for( BtU32 iIndex=0; iIndex<nIndices; iIndex++ )
					{
						renderBlock.m_optimisedIndex[iIndex] += nBaseVertex;
					}
				}

				BtU32 nBaseIndex = (BtU32) m_pScene->pIndexBuffer()->nSize();

				for( BtU32 iIndex=0; iIndex<nIndices; iIndex++ )
				{
					m_pScene->pIndexBuffer()->AddIndex( renderBlock.m_optimisedIndex[iIndex] );
				}

				// Add each primitive
				RsIndexedPrimitive primitive;
				primitive.m_primitiveType = PaExportSizes::GetTriangleType(ExPT_LIST);
				primitive.m_baseVertexIndex = nBaseVertex;
				primitive.m_startIndex = nBaseIndex;
				primitive.m_minIndex = 0;
				primitive.m_numVertices = nVertices;
				primitive.m_primitives = nIndices / 3;
				primitive.m_numIndices = nIndices;
				renderBlock.m_primitives.push_back( primitive );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ChangeCoordinateSystem

void ExSgSkin::ChangeCoordinateSystem()
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
			// Cache each vertex
			ExVertex* pVertex = pMaterialBlock->m_pVertex[i];

			// Change the coordinate system
			pVertex->ChangeCoordinateSystem();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// CopyAttributes

void ExSgSkin::CopyAttributes()
{
	// Set the skin to be visible via default
	m_pNode->m_skinFileData.m_bVisible = BtTrue;
	
	// Get the size of the skin file data
	BaSgSkinFileData skinFileData;
	BtU32 skeletonSize = sizeof( skinFileData.m_skeleton );

	// Copy across each of the skeleton nodes
	for( BtU32 i=0; i<skeletonSize; i++ )
	{
		if( i < m_numJoints )
		{
			m_pNode->m_skinFileData.m_skeleton[i].m_nJoint = m_skeleton[i]->GetResourceID();
		}
	}
	m_pNode->m_skinFileData.m_numJoints = m_numJoints;

	// Set the number of materials
	m_pNode->m_skinFileData.m_nMaterials = (BtU32) m_pNode->m_materialBlocks.size();
}
