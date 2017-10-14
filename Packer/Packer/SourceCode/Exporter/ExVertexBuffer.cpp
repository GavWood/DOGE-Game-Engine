////////////////////////////////////////////////////////////////////////////////
// ExVertexBuffer.cpp

// Includes
#include "ExTexture.h"
#include "TeTexture.h"
#include "PaArchive.h"
#include "ExVertexBuffer.h"
#include "ExVertex.h"
#include "RsVertex.h"
#include "PaTopState.h"
#include "PaRendering.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

ExVertexBuffer::ExVertexBuffer()
{
	m_nResourceID = 0;
	m_isDynamic = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Destructor

ExVertexBuffer::~ExVertexBuffer()
{
	BtU32 nVertices = (BtU32) m_vertices.size();

	for( BtU32 iVertex=0; iVertex<nVertices; iVertex++ )
	{
		// delete m_vertices[iVertex];
	}
}

////////////////////////////////////////////////////////////////////////////////
// AddVertex

void ExVertexBuffer::AddVertex( ExVertex* pVertex )
{
	m_vertices.push_back( pVertex );
}

///////////////////////////////////////////////////////////////////////////////
// Export

void ExVertexBuffer::Export()
{
	// Cache the number of vertices
	BtU32 nVertices = (BtU32) m_vertices.size();

	if( nVertices == 0 )
	{
		return;
	}

	// Write the vertices
	PaMemoryResource verticesResource( PMR_Write );
	BaVertexBufferFileData VertexBuffer;

	// Cache the first vertex
	ExVertex* pVertex = m_vertices[0];

	// Setup the vertex details
	VertexBuffer.m_nVertices = nVertices;
	VertexBuffer.m_nStride = pVertex->GetStride();

	if( m_isDynamic )
	{
		VertexBuffer.m_flags = RsVB_Dynamic;
	}
	else
	{
		VertexBuffer.m_flags = 0;
	}

	if( PaTopState::Instance().IsVertexBase2() )
	{
		VertexBuffer.m_nStride = BtNearestPowerOf2( VertexBuffer.m_nStride );
	}

	VertexBuffer.m_type = pVertex->GetVertexType();
	BtStrCopy( VertexBuffer.m_description, 256, pVertex->Description() );
	verticesResource << VertexBuffer;

	// Loop through the scene nodes and check the vertex types
	BtU32 flag = m_vertices[0]->GetVertexType();
	BtChar *description = m_vertices[0]->Description();
	for( BtU32 nVertex=1; nVertex<nVertices; nVertex++ )
	{
		if( flag != m_vertices[nVertex]->GetVertexType() )
		{
			BtPrint( "Vertex are not necessarily the same type" );
		}
		flag = m_vertices[nVertex]->GetVertexType();
	}

	// Loop through the scene nodes
	for( BtU32 nVertex=0; nVertex<nVertices; nVertex++ )
	{
		// Export the vertex
		m_vertices[nVertex]->Export( verticesResource );
	}

	BtPrint( "Added vertex buffer of type %s with stride %d", description, m_vertices[0]->GetStride() );

	// Add the vertex resource
	AddResource( verticesResource,
				 BtNull,
				 description,
				 BaRT_VertexBuffer );
}
