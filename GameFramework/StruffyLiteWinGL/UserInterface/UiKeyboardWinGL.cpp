////////////////////////////////////////////////////////////////////////////////
// UiKeyboardWinGL.cpp

#include "BtBase.h"
#include "BtMemory.h"
#include "UiKeyboardWinGL.h"
#include "ErrorLog.h"

const BtU32 struffKeys[] = { (BtU32)UiKeyCode_ESCAPE,
							 (BtU32)UiKeyCode_RETURN,
						     (BtU32)UiKeyCode_A,
							 (BtU32)UiKeyCode_B,
							 (BtU32)UiKeyCode_C,
							 (BtU32)UiKeyCode_D,
							 (BtU32)UiKeyCode_E,
							 (BtU32)UiKeyCode_F,
							 (BtU32)UiKeyCode_G,
							 (BtU32)UiKeyCode_H,
							 (BtU32)UiKeyCode_I,
							 (BtU32)UiKeyCode_J,
							 (BtU32)UiKeyCode_K,
							 (BtU32)UiKeyCode_L,
							 (BtU32)UiKeyCode_M,
							 (BtU32)UiKeyCode_N,
						     (BtU32)UiKeyCode_O,
						     (BtU32)UiKeyCode_P,
							 (BtU32)UiKeyCode_Q,
							 (BtU32)UiKeyCode_R,
							 (BtU32)UiKeyCode_S,
							 (BtU32)UiKeyCode_T,
							 (BtU32)UiKeyCode_U,
						     (BtU32)UiKeyCode_V,
							 (BtU32)UiKeyCode_W,
							 (BtU32)UiKeyCode_X,
							 (BtU32)UiKeyCode_Y,
							 (BtU32)UiKeyCode_Z,
							 (BtU32)UiKeyCode_0,
							 (BtU32)UiKeyCode_1,
							 (BtU32)UiKeyCode_2,
							 (BtU32)UiKeyCode_3,
							 (BtU32)UiKeyCode_4,
							 (BtU32)UiKeyCode_5,
							 (BtU32)UiKeyCode_6,
							 (BtU32)UiKeyCode_7,
							 (BtU32)UiKeyCode_8,
							 (BtU32)UiKeyCode_9,
							 (BtU32)UiKeyCode_SPACE,
							 (BtU32)UiKeyCode_LSHIFT,
							 (BtU32)UiKeyCode_RSHIFT,
							 (BtU32)UiKeyCode_APOSTROPHE,
							 (BtU32)UiKeyCode_SLASH,
							 (BtU32)UiKeyCode_ALT,
							 (BtU32)UiKeyCode_F1,
							 (BtU32)UiKeyCode_F2,
							 (BtU32)UiKeyCode_F3,
							 (BtU32)UiKeyCode_F4,
							 (BtU32)UiKeyCode_F5,
							 (BtU32)UiKeyCode_F6,
							 (BtU32)UiKeyCode_F7,
							 (BtU32)UiKeyCode_F8,
							 (BtU32)UiKeyCode_F9,
							 (BtU32)UiKeyCode_F10,
							 (BtU32)UiKeyCode_F11,
							 (BtU32)UiKeyCode_F12,
							 (BtU32)UiKeyCode_COMMA,
							 (BtU32)UiKeyCode_PERIOD,
							 (BtU32)UiKeyCode_DELETE,
							 (BtU32)UiKeyCode_DOWN,
							 (BtU32)UiKeyCode_UP,
							 (BtU32)UiKeyCode_LEFT,
							 (BtU32)UiKeyCode_RIGHT,
							 (BtU32)UiKeyCode_PLUS,
						 	 (BtU32)UiKeyCode_MINUS
};

////////////////////////////////////////////////////////////////////////////////
// Constructor

UiKeyboardWinGL::UiKeyboardWinGL()
{
	m_pInstance = this;
	BtMemory::Set( m_nKeyDown, 0, sizeof( m_nKeyDown ) );
	BtMemory::Set( m_nKeyNewDown, 0, sizeof( m_nKeyNewDown ) );
	BtMemory::Set( m_nKeyUp, 0, sizeof( m_nKeyUp ) );
}

////////////////////////////////////////////////////////////////////////////////
// GetNumKeys

BtU32 UiKeyboardWinGL::GetNumKeys()
{
	return sizeof(struffKeys) / sizeof(BtU32);
}

////////////////////////////////////////////////////////////////////////////////
// GetKeyCode

BtU32 UiKeyboardWinGL::GetKeyCode( BtU32 index )
{
	return struffKeys[index];
}

////////////////////////////////////////////////////////////////////////////////
// SetPressed

void UiKeyboardWinGL::SetPressed(int myKey)
{
	m_isKeyPressed[myKey] = BtTrue;

	if (m_isKeyPressed[UiKeyCode_DOWN])
	{
		int a = 0;
		a++;
	}
	if (m_isKeyPressed[UiKeyCode_UP])
	{
		int a = 0;
		a ++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetReleased

void UiKeyboardWinGL::SetReleased(int myKey)
{
	m_isKeyPressed[myKey] = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void UiKeyboardWinGL::Update()
{
	if (m_isKeyPressed[UiKeyCode_F1])
	{
		int a = 0;
		a++;
	}
	if (m_isKeyPressed[UiKeyCode_W])
	{
		int a = 0;
		a++;
	}
	if (m_isKeyPressed[UiKeyCode_DOWN])
	{
		int a = 0;
		a++;
	}

	for (BtU32 i = 0; i < 256; i++)
	{
		// Was the key pressed?
		BtBool isKeyPressed = m_isKeyPressed[i];	

		// Was it released
		m_nKeyUp[i] = !isKeyPressed && m_nKeyDown[i];

		// Is it a new key press?
		m_nKeyNewDown[i] = isKeyPressed && !m_nKeyDown[i];

		if (m_nKeyNewDown[i])
		{
			int a = 0;
			a++;
		}

		// Set this key as pressed
		m_nKeyDown[i] = isKeyPressed;
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsHeld

BtBool UiKeyboardWinGL::IsHeld( UiKeyCode key)
{
	return m_nKeyDown[key];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

BtBool UiKeyboardWinGL::IsPressed( UiKeyCode key )
{
	if( key == UiKeyCode_SPACE )
	{
		int a=0;
		a++;
	}
	return m_nKeyNewDown[key];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

BtBool UiKeyboardWinGL::IsPressed()
{
	BtU32 numKeys = sizeof(struffKeys) / sizeof(BtU32);
	
	for (BtU32 i = 0; i < numKeys; i++)
	{
		if( m_nKeyDown[i] )
		{
			return BtTrue;
		}
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsReleased

BtBool UiKeyboardWinGL::IsReleased( UiKeyCode key )
{
	return m_nKeyUp[key];
}
