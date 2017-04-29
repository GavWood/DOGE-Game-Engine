//----------------------------------------------------------------------------
// RsVertex.h

#pragma once
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtVector3.h"

////////////////////////////////////////////////////////////////////////////////
// RsVertex2

struct RsVertex2
{
	MtVector2		m_v2Position;
	BtU32			m_colour;
	MtVector2		m_v2UV;
	MtVector2		m_v2UV2;
};

////////////////////////////////////////////////////////////////////////////////
// RsVertex3

struct RsVertex3
{
	MtVector3		m_v3Position;
	MtVector3		m_v3Normal;
	BtU32			m_colour;
	MtVector2		m_v2UV;
	MtVector2		m_v2UV2;
};
