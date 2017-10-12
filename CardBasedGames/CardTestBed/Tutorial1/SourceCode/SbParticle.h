////////////////////////////////////////////////////////////////////////////////
// SbParticle.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector2.h"

struct SbParticle
{
	MtVector3									m_v3Position;
	MtVector2									m_v2Dimension;
	MtVector3									m_v3Velocity;
	BtFloat										m_time;
	BtU32										m_frame;
	RsColour									m_colour;
	BtFloat										m_opacityRandom;
	BtFloat										m_speedRandom;
};
