////////////////////////////////////////////////////////////////////////////////
/// DyCollisionMesh.cpp

#include "DyCollisionMesh.h"

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void DyCollisionMesh::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	// Set the file data
	m_pFileData = (BaSgCollisionMesh*) pFileData;

	BtU8* pMemory = (BtU8*) m_pFileData;

	// Advance past the file data
	pMemory += sizeof(BaSgCollisionMesh);

	// Set a pointer to the vertex
	m_pVertex = (BtFloat*) pMemory;

	// Advance past the vectors
	BtU32 nSize = sizeof(BtFloat) * m_pFileData->m_endVertex * 4;

	// Skip past the vertex
	pMemory+= nSize;

	// Set a pointer to the index
	m_pIndex = (BtU32*) pMemory;
};

////////////////////////////////////////////////////////////////////////////////
// pVertex

BtFloat* DyCollisionMesh::pVertex() const
{
	return m_pVertex;
}

////////////////////////////////////////////////////////////////////////////////
// pIndex

BtU32* DyCollisionMesh::pIndex() const
{
	return m_pIndex;
}
