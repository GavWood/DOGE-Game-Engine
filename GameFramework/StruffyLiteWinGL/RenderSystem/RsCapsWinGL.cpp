////////////////////////////////////////////////////////////////////////////////
// RsCapsWinGL.cpp

// Includes
#include "RsCapsWinGL.h"
#include "MtMath.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

RsCapsWinGL::RsCapsWinGL()
{
}

////////////////////////////////////////////////////////////////////////////////
// IsMultipleSimultaneousRenderTargets

BtBool RsCapsWinGL::IsMultipleSimultaneousRenderTargets() const
{
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumModes

BtU32 RsCapsWinGL::GetNumModes()
{
	return m_modes.GetNumItems();
}

////////////////////////////////////////////////////////////////////////////////
// GetMode

RsCapsMode RsCapsWinGL::GetMode( BtU32 modeIndex )
{
	return m_modes[modeIndex];
}

////////////////////////////////////////////////////////////////////////////////
// GetModes

void RsCapsWinGL::GetModes()
{
	RsCapsMode mode;
	mode.m_index = 0;
	mode.m_width = 800;
	mode.m_height = 600;
	mode.m_refreshMode = 60;

	if( ( mode.m_width > 640 ) && ( mode.m_height > 480 ) )
	{
		// Cache the current number of modes
		BtU32 numModes = m_modes.GetNumItems();

		BtBool isFound = BtFalse;

		// Iterate through the modes
		for( BtU32 i=0; i<numModes; i++ )
		{
			// Cache each found mode
			RsCapsMode& foundMode = m_modes[i];

			// If we've found the mode don't add - we have the option to update the mode if the resolution is better
			if( ( foundMode.m_width == mode.m_width ) && ( foundMode.m_height == mode.m_height ) )
			{
				if( mode.m_refreshMode > foundMode.m_refreshMode )
				{
					foundMode.m_width  = mode.m_width;
					foundMode.m_height = mode.m_height;
					foundMode.m_refreshMode = mode.m_refreshMode;
					isFound = BtTrue;
				}
			}
		}

		// If it's not found then add a mode
		if( isFound == BtFalse )
		{
			m_modes.Add( mode );
		}
	}
}
