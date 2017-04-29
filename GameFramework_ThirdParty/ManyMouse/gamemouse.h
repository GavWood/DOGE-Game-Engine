////////////////////////////////////////////////////////////////////////////////
// GameMouse.h

#pragma once

#include "BtTypes.h"
#include "MtVector2.h"
#include "MtVector3.h"
#include <wtypes.h>

struct GameMouseState
{
		BtS32						m_x;
		BtS32						m_y;

		MtVector3					m_v3Movement;
		MtVector2					m_v2Position;
		BtFloat						m_isAlive;

		BtBool						m_nDown[4];
		BtBool						m_nNewDown[4];
		BtBool						m_nUp[4];
};

// Class definition
class GameMouse
{
	public:
  
		// Public functions
		void 						CreateOnDevice();
		void 						RemoveFromDevice();
		void						Setup( HWND handle );
		void 						Update();

		// Accessors
		MtVector3&					GetMovement( BtU32 device );
		MtVector2&					GetPosition( BtU32 device );

		BtBool						Pressed ( BtU32 nIndex, BtU32 device );
		BtBool						Held    ( BtU32 nIndex, BtU32 device );
		BtBool						Released( BtU32 nIndex, BtU32 device );

		BtU32						GetNumMice();

		BtBool						IsAlive( BtU32 device );

	private:

		HWND						m_hWindowHandle;
		GameMouseState				m_state[4];		
		BtU32						m_mice;
};

////////////////////////////////////////////////////////////////////////////////
// Held

inline BtBool GameMouse::Held( BtU32 nIndex, BtU32 device )
{
	GameMouseState& state = m_state[device];

	return state.m_nDown[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// Pressed

inline BtBool GameMouse::Pressed( BtU32 nIndex, BtU32 device )
{
	GameMouseState& state = m_state[device];

	return state.m_nNewDown[nIndex];
}

////////////////////////////////////////////////////////////////////////////////
// Released

inline BtBool GameMouse::Released( BtU32 nIndex, BtU32 device )
{
	GameMouseState& state = m_state[device];

	return state.m_nUp[nIndex];
}
