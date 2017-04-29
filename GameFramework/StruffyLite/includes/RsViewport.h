///////////////////////////////////////////////////////////////////////////////
// RsViewport.h

#pragma once
#include "BtTypes.h"

struct RsViewport
{
	RsViewport()
	{};

	RsViewport( BtU32 x, BtU32 y, BtU32 width, BtU32 height )
	{
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
	}

	BtU32 					m_x;
	BtU32 					m_y;
	BtU32 					m_width;
	BtU32 					m_height;
};
