///////////////////////////////////////////////////////////////////////////////
// RsPrimitiveWin32GL.h

#pragma once
#include "BtTypes.h"
#include "RsPrimitive.h"
#include "glfw.h"

struct RsPrimitiveWinGL : public RsPrimitive
{
	GLenum			 					m_primitiveType;
	BtU32								m_numVertex;
	BtU32								m_nStartVertex;
};

struct RsIndexedPrimitiveWinGL : public RsIndexedPrimitive
{
	GLenum			 					m_primitiveType;
    BtU32 								m_baseVertexIndex;
    BtU32 								m_minIndex;
    BtU32 								m_numVertices;
    BtU32 								m_startIndex;
    BtU32								m_primitives;
	BtU32								m_numIndices;
	BtU32								m_indexType;
};
