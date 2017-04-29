////////////////////////////////////////////////////////////////////////////////
// gamemouse.cpp

#include "GameMouse.h"
#include "manymouse.h"

GameMouse mouse;

////////////////////////////////////////////////////////////////////////////////
// CreateOnDevice

void GameMouse::CreateOnDevice()
{
	m_mice = ManyMouse_Init();

	POINT ptCursor;
	GetCursorPos( &ptCursor );

	for( BtU32 device=0; device<4; device++ )
	{	
		GameMouseState& state = m_state[device];
		
		for( BtU32 i=0; i<4; i++ )
		{
			state.m_nUp[i] = BtFalse;
			state.m_nNewDown[i] = BtFalse;
			state.m_nDown[i] = BtFalse;
		}
		state.m_x = ptCursor.x;
		state.m_y = ptCursor.y;
		state.m_v2Position = MtVector2( (BtFloat)state.m_x, (BtFloat)state.m_y );
		state.m_isAlive = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Setup

void GameMouse::Setup( HWND handle )
{
	m_hWindowHandle = handle;
}

////////////////////////////////////////////////////////////////////////////////
// GetPosition

MtVector2& GameMouse::GetPosition( BtU32 device )
{
	GameMouseState& state = m_state[device];
	return state.m_v2Position;
}

////////////////////////////////////////////////////////////////////////////////
// GetMovement

MtVector3& GameMouse::GetMovement( BtU32 device )
{
	GameMouseState& state = m_state[device];
	return state.m_v3Movement;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void GameMouse::Update()
{
    ManyMouseEvent mouseEvent;

	for( BtU32 device=0; device<4; device++ )
	{
		GameMouseState& state = m_state[device];

		for(BtU32 i=0; i<4; i++)
		{
			state.m_nUp[i] = BtFalse;
			state.m_nNewDown[i] = BtFalse;
		}
	}

	while( ManyMouse_PollEvent( &mouseEvent ) )
	{
		// Cache the index
		BtU32 device = mouseEvent.device;

		const BtChar *mouse = ManyMouse_DeviceName( device );

		// Cache the state
		GameMouseState& state = m_state[device];

		if (mouseEvent.type == MANYMOUSE_EVENT_ABSMOTION)
		{
			int a=0;
			a++;
		}
		else if (mouseEvent.type == MANYMOUSE_EVENT_RELMOTION)
		{
			if( mouseEvent.item == 0 )
			{
				state.m_x += mouseEvent.value;
			}
			else
			{
				state.m_y += mouseEvent.value;
			}

			// Get the mouse position
			POINT ptCursor;
			ptCursor.x = (LONG)state.m_x;
			ptCursor.y = (LONG)state.m_y;

			// Set the cursor position for Windows from just one mouse
			SetCursorPos( ptCursor.x, ptCursor.y );

			ScreenToClient( m_hWindowHandle, &ptCursor );

			state.m_v2Position.x = (BtFloat) ptCursor.x;
			state.m_v2Position.y = (BtFloat) ptCursor.y;
			state.m_isAlive = BtTrue;
		}
		else if (mouseEvent.type == MANYMOUSE_EVENT_ABSMOTION)
		{
		}

		else if( mouseEvent.type == MANYMOUSE_EVENT_BUTTON )
		{
			if( mouseEvent.value )
			{
				if( state.m_nDown[mouseEvent.item] == BtFalse )
				{
					state.m_nNewDown[mouseEvent.item] = BtTrue;
				}
				state.m_nDown[mouseEvent.item] = BtTrue;
				state.m_isAlive = BtTrue;
			}
			else
			{
				state.m_nUp[mouseEvent.item]   = BtTrue;
				state.m_nDown[mouseEvent.item] = BtFalse;
			}
		}
		else if (mouseEvent.type == MANYMOUSE_EVENT_SCROLL)
		{
		}

		else if (mouseEvent.type == MANYMOUSE_EVENT_DISCONNECT)
		{
		}
		else
		{
		}
	}
	//return;
}

////////////////////////////////////////////////////////////////////////////////
// RemoveFromDevice

void GameMouse::RemoveFromDevice()
{
	ManyMouse_Quit();
}

////////////////////////////////////////////////////////////////////////////////
// GetNumMice

BtU32 GameMouse::GetNumMice()
{
	return m_mice;
}

////////////////////////////////////////////////////////////////////////////////
// IsAlive

BtBool GameMouse::IsAlive( BtU32 device )
{
	if( m_state[device].m_isAlive > 0 )
	{
		return BtTrue;
	}
	return BtFalse;
}