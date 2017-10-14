////////////////////////////////////////////////////////////////////////////////
// ExVertexBuffer

#pragma once
#include "PaAsset.h"
#include "BaArchive.h"
#include "ExResource.h"

class ExVertex;

class ExVertexBuffer : public ExResource
{
public:

	ExVertexBuffer();
	~ExVertexBuffer();

	void 									AddVertex( ExVertex* pVertex );
	void 									Export();

	// Accessors
	BtU32									Size();
	void									SetStatic();

private:

	BaVertexBufferFileData					m_fileData;
	std::vector<ExVertex*>					m_vertices;
	BtBool									m_isDynamic;
};

////////////////////////////////////////////////////////////////////////////////
// Size

inline BtU32 ExVertexBuffer::Size()
{
	return (BtU32) m_vertices.size();
}

////////////////////////////////////////////////////////////////////////////////
// SetStatic

inline void ExVertexBuffer::SetStatic()
{
	m_isDynamic = BtTrue;
}
