////////////////////////////////////////////////////////////////////////////////
// ExCollisionMesh

#include "BaArchive.h"
#include "BtCRC.h"
#include "ExCollisionMesh.h"
#include "ExSgNode.h"
#include "ExSgMesh.h"
#include "DyCollisionMesh.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExCollisionMesh::ExCollisionMesh()				
{
}

////////////////////////////////////////////////////////////////////////////////
// NumIndex

BtU32 ExCollisionMesh::NumIndex() const
{
	return (BtU32) m_indices.size();
}

////////////////////////////////////////////////////////////////////////////////
// NumVertex

BtU32 ExCollisionMesh::NumVertex() const
{
	return (BtU32) m_vertices.size();
}

////////////////////////////////////////////////////////////////////////////////
// ExtractMesh

void ExCollisionMesh::ExtractMesh( ExSgNode* pNode )
{
	m_pNode = pNode;
	
	if( strstr( m_pNode->GetTitle(), "convex" ) )
	{
		int a=0;
		a++;
	}

	// Cache the mesh
	ExSgMesh* pMesh = m_pNode->m_pMesh;

	// Cache the number of materials
	BtU32 numMaterials = pMesh->NumMaterialBlocks();

	if( numMaterials != 1 )
	{
		ErrorLog::Printf( "Warning multiple materials in collision file.\r\n" );
	}

	BtU32 vertexBase = 0;

	for( BtU32 iMaterial=0; iMaterial<numMaterials; iMaterial++ )
	{
		ExMaterialBlock* pMaterialBlock = pMesh->pMaterialBlock( iMaterial );

		// Cache the number of render blocks
		BtU32 numRenderBlocks = (BtU32) pMaterialBlock->m_renderBlocks.size();

		if( numRenderBlocks != 1 )
		{
			ErrorLog::Fatal_Printf( "Multiple render blocks in collision file.\r\n" );
		}

		ExRenderBlock* pRenderblock = &pMaterialBlock->m_renderBlocks[0];

		BtU32 numVertices = (BtU32) pRenderblock->m_pOptimisedVertex.size();

		for( BtU32 iVertex=0; iVertex<numVertices; ++iVertex )
		{
			m_vertices.push_back( pRenderblock->m_pOptimisedVertex[iVertex]->Position() );
		}

		BtU32 numIndex = (BtU32) pRenderblock->m_optimisedIndex.size();

		for( BtU32 iIndex=0; iIndex<numIndex; iIndex+= 3 )
		{
			m_indices.push_back( pRenderblock->m_optimisedIndex[iIndex + 0] + vertexBase );
			m_indices.push_back( pRenderblock->m_optimisedIndex[iIndex + 1] + vertexBase );
			m_indices.push_back( pRenderblock->m_optimisedIndex[iIndex + 2] + vertexBase );
		}

		vertexBase = (BtU32)m_vertices.size();
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExCollisionMesh::Export()
{
	PaMemoryResource memoryResource( PMR_Write );

	// Export the file data
	m_fileData.m_startVertex = 0;
	m_fileData.m_startIndex  = 0;
	m_fileData.m_endVertex  = (BtU32)m_vertices.size();
	m_fileData.m_endIndex   = (BtU32)m_indices.size();

	memoryResource << m_fileData;

	// Export the vertex

	// Serialise the vertex
	BtU32 nSize = (BtU32) m_vertices.size();

	BtFloat w = 1;

	for( BtU32 iVertex=0; iVertex<nSize; iVertex++ )
	{
		memoryResource << m_vertices[iVertex].x;
		memoryResource << m_vertices[iVertex].y;
		memoryResource << m_vertices[iVertex].z;
		memoryResource << w;
	}

	// Export the indices

	// Serialise the indices
	nSize = (BtU32) m_indices.size();

	for( BtU32 iIndex=0; iIndex<nSize; iIndex++ )
	{
		memoryResource << m_indices[iIndex];
	}

	// Add the vertex resource
	AddResource(memoryResource,
		BtNull,
		"CollisionMeshWin32",
		BaRT_CollisionMesh);
};
