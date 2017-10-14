////////////////////////////////////////////////////////////////////////////////
// ExIndexBuffer.h

#pragma once
#include "PaAsset.h"
#include "BaArchive.h"
#include "ExResource.h"

class ExIndexBuffer : public ExResource
{
public:

	ExIndexBuffer();

	void					AddIndex( BtU32 nIndex );
	void					Export();

	// Accessors
	BtU32					nSize() const;
	BtU32					GetIndexType() { return m_indexType;  }

private:

	BaIndexBufferFileData	m_fileData;
	std::vector<BtU32>		m_indices;
	BtU32					m_indexType;
};

inline BtU32 ExIndexBuffer::nSize() const
{
	return (BtU32) m_indices.size();
}
