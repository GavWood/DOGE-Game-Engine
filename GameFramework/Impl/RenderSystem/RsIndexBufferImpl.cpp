////////////////////////////////////////////////////////////////////////////////
/// RsIndexBufferWinGL.cpp

#include "RsImpl.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "BtMemory.h"
#include "BtBase.h"
#include "RsIndexBufferImpl.h"

////////////////////////////////////////////////////////////////////////////////
// RsIndexBufferWinGL

RsIndexBufferImpl::RsIndexBufferImpl()
{
	m_pData = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void RsIndexBufferImpl::FixPointers( BtU8 *pFileData, BaArchive *pArchive )
{
	m_pFileData = (BaIndexBufferFileData*) pFileData;
	m_pData = ( (BtU8*) m_pFileData ) + sizeof( BaIndexBufferFileData );
}

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void RsIndexBufferImpl::CreateOnDevice()
{
	if( m_pFileData->m_type & IndType_Short )
	{
		m_indexSize = sizeof(BtU16);
	}
	else
	{
		m_indexSize = sizeof(BtU32);
	}

	// generate a new buffer and get the associated ID
	glGenBuffers(1, &m_indexBuffer);

	// bind VBO in order to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	BtU32 dataSize = m_pFileData->m_nIndices * m_indexSize;

	// upload data to VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, m_pData, GL_STATIC_DRAW);

	// Unbind vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////////////////////////////////
// Copy

void RsIndexBufferImpl::Copy( BtU8* pData, BtU32 nIndices )
{
}

////////////////////////////////////////////////////////////////////////////////
// SetIndices

void RsIndexBufferImpl::SetIndices()
{
	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void RsIndexBufferImpl::RemoveFromDevice()
{
	glDeleteBuffers(1, &m_indexBuffer);
}
