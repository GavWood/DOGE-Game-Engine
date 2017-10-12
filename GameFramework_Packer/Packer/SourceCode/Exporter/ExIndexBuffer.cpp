////////////////////////////////////////////////////////////////////////////////
// ExIndexBuffer.cpp

// Includes
#include "ExIndexBuffer.h"
#include "ApConfig.h"
#include "PaTopState.h"
#include "RsIndexBuffer.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExIndexBuffer::ExIndexBuffer()
{
	m_nResourceID = 0;
	m_indexType = RsIndexBuffer::IndType_Short;
}

////////////////////////////////////////////////////////////////////////////////
// AddVertex

void ExIndexBuffer::AddIndex( BtU32 nIndex )
{
	m_indices.push_back( nIndex );
}

////////////////////////////////////////////////////////////////////////////////
// Export

void ExIndexBuffer::Export()
{
	// Cache the number of vertices
	BtU32 nIndices = (BtU32) m_indices.size();

	if( nIndices == 0 )
	{
		return;
	}

	if (nIndices > 65535)
	{
		m_indexType = RsIndexBuffer::IndType_Long;
	}
	else
	{
		m_indexType = RsIndexBuffer::IndType_Short;
	}

	// Write the vertices
	PaMemoryResource indicesResource( PMR_Write );
	BaIndexBufferFileData indexBuffer;

	// Setup the index details
	indexBuffer.m_nIndices = nIndices;
	indexBuffer.m_type = m_indexType;

	if( m_indexType == RsIndexBuffer::IndType_Long )
	{
		// Write out the header
		indicesResource << indexBuffer;

		// Loop through the indices
		for (BtU32 nIndex = 0; nIndex < nIndices; nIndex++)
		{
			// Cache each index
			BtU32 index = m_indices[nIndex];

			// Export each index
			indicesResource << index;
		}
	}
	else
	{
		// Write out the header
		indicesResource << indexBuffer;

		// Loop through the indices
		for (BtU32 nIndex = 0; nIndex < nIndices; nIndex++)
		{
			// Cache each index
			BtU16 index = m_indices[nIndex];

			// Export each index
			indicesResource << index;
		}
	}

	// Add the vertex resource
	AddResource( indicesResource,
		BtNull,
		"IndexBuffer",
		BaRT_IndexBuffer );
}
