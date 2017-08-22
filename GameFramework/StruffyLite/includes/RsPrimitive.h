///////////////////////////////////////////////////////////////////////////////
// RsPrimitive.h

#pragma once
#include "BtTypes.h"

struct RsPrimitive
{
	BtU32			 					m_primitiveType;
	BtU32								m_numVertex;
	BtU32								m_nStartVertex;
	BtU32								m_indexType;
};

struct RsIndexedPrimitive
{
	BtU32			 					m_primitiveType;
	BtU32 								m_baseVertexIndex;
	BtU32 								m_minIndex;
	BtU32 								m_numVertices;
	BtU32 								m_startIndex;
	BtU32								m_primitives;
	BtU32								m_numIndices;
	BtU32								m_indexType;
};
