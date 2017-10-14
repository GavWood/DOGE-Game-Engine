////////////////////////////////////////////////////////////////////////////////
// PaRendering.cpp

// Includes
#include <stdio.h>
#include "PaRendering.h"
#include "BtBase.h"
#include "Errorlog.h"
#include "PaTopState.h"

enum D3D_WRAPPER
{
	D3D_WRAPPER_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
	D3D_WRAPPER_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
	D3D_WRAPPER_PRIMITIVE_TOPOLOGY_LINELIST = 2,
	D3D_WRAPPER_PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
	D3D_WRAPPER_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
	D3D_WRAPPER_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
};

enum GL_WRAPPER
{
	GL_WRAPPER_POINTS = 0x0000,
	GL_WRAPPER_LINES = 0x0001,
	GL_WRAPPER_LINE_LOOP = 0x0002,
	GL_WRAPPER_LINE_STRIP = 0x0003,
	GL_WRAPPER_TRIANGLES = 0x0004,
	GL_WRAPPER_TRIANGLE_STRIP = 0x0005,
	GL_WRAPPER_TRIANGLE_FAN = 0x0006,
	GL_WRAPPER_QUADS = 0x0007,
	GL_WRAPPER_QUAD_STRIP = 0x0008,
	GL_WRAPPER_POLYGON = 0x0009
};

// Platform specific triangle types
BtU32 PaExportSizes::GetTriangleType(ExPrimitiveType primitiveType)
{
	if (PaTopState::Instance().GetPlatform() == PackerPlatform_WinDX)
	{
		if (primitiveType == ExPT_STRIP)
		{
			return D3D_WRAPPER_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		}
		return D3D_WRAPPER_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
	else
	{
		if (primitiveType == ExPT_STRIP)
		{
			return GL_WRAPPER_LINE_STRIP;
		}
		return GL_WRAPPER_TRIANGLES;
	}
	return 0;
}