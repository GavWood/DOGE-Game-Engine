////////////////////////////////////////////////////////////////////////////////
/// RsVertexBufferWinGL.cpp

#include "glee.h"
#include "BtBase.h"
#include "BaArchive.h"
#include "BtMemory.h"
#include "RsVertexBufferWinGL.h"
#include "RsVertex.h"
#include "glfw.h"

RsVertexBufferWinGL VertexBuffer;

//const GLubyte *PSTR = glGetString(GL_VERSION);
// http://www.songho.ca/opengl/gl_vbo.html

////////////////////////////////////////////////////////////////////////////////
// Constructor

RsVertexBufferWinGL::RsVertexBufferWinGL()
{
	m_pData = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void RsVertexBufferWinGL::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	m_pFileData = (BaVertexBufferFileData*) pFileData;
	m_pData = ( (BtU8*) m_pFileData ) + sizeof( BaVertexBufferFileData );
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsVertexBufferWinGL::CreateOnDevice()
{
	// generate a new VBO and get the associated ID
	glGenBuffers(1, &m_vertexBuffer);

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

	BtU32 dataSize = m_pFileData->m_nVertices * m_pFileData->m_nStride;

	// upload data to VBO
	glBufferData(GL_ARRAY_BUFFER, dataSize, m_pData, GL_STATIC_DRAW);

	// Unbind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////
// SetStream

void RsVertexBufferWinGL::SetStream()
{
	// http://www.opengl.org/wiki/Vertex_Buffer_Object
	for( BtU32 i=0; i<7; i++ )
	{
		glDisableVertexAttribArray( i );
	}

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);         // for vertex coordinates

	BtU32 offset = 0;
	BtU32 stride = m_pFileData->m_nStride;
	BtU32 vertexType = m_pFileData->m_type;

	BtU32 index = 0;

	if ( vertexType & VT_Position )
	{
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, GL_BUFFER_OFFSET( offset ) );

		offset += sizeof( MtVector3 );
		++index;
	}

	if ( vertexType & VT_Normal )
	{
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride, GL_BUFFER_OFFSET( offset ) );

		offset += sizeof( MtVector3 );
		++index;
	}
	if (vertexType & VT_Colour)
	{
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, GL_BUFFER_OFFSET(offset));

		offset += sizeof(BtU32);
		++index;
	}

	if (vertexType & VT_UV0)
	{
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, stride, GL_BUFFER_OFFSET(offset));

		offset += sizeof(MtVector2);
		++index;
	}

	if (vertexType & VT_UV1)
	{
		offset += sizeof(MtVector2);
		++index;
	}

	if ( vertexType & VT_Tangent )
	{
		offset += sizeof( MtVector3 );
		++index;
	}
	
	if ( vertexType & VT_Binormal )
	{
		offset += sizeof( MtVector3 );
		++index;
	}

	if ( vertexType & VT_Skin )
	{
		glEnableVertexAttribArray( 6 );
		glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, stride, GL_BUFFER_OFFSET( offset ) );
	
		GLenum error = glGetError();
		(void)error;

		offset += sizeof( BtFloat ) * 4;
		++index;

		glEnableVertexAttribArray( 7 );
		glVertexAttribPointer( 7, 4, GL_UNSIGNED_BYTE, GL_FALSE, stride, GL_BUFFER_OFFSET( offset ) );

		error = glGetError();
		(void)error;

		offset += sizeof( BtU8 ) * 4;
		++index;
	}

	if ( vertexType & VT_BlendShape )
	{
		offset += sizeof( MtVector3 );
		offset += sizeof( MtVector3 );
		offset += sizeof( MtVector3 );
	}
}

////////////////////////////////////////////////////////////////////////////////
// Copy

void RsVertexBufferWinGL::Copy(BtU8* pData, BtU32 numVertices )
{
	// Copy the vertices across
	// BtMemory::Copy( pVertices, pData, numVertices * m_pFileData->m_nStride );
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void RsVertexBufferWinGL::RemoveFromDevice()
{
	glDeleteBuffers(1, &m_vertexBuffer);
}
