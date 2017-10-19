////////////////////////////////////////////////////////////////////////////////
// UiKeyboardImpl.cpp

#include "BtBase.h"
#include "BtMemory.h"
#include "UiKeyboardImpl.h"
#include "ErrorLog.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

UiKeyboardWin::UiKeyboardWin()
{
	m_pInstance = this;
	BtMemory::Set( m_nKeyDown, 0, sizeof( m_nKeyDown ) );
	BtMemory::Set( m_nKeyNewDown, 0, sizeof( m_nKeyNewDown ) );
	BtMemory::Set( m_nKeyUp, 0, sizeof( m_nKeyUp ) );
    BtMemory::Set( m_isKeyPressed, 0, sizeof( m_isKeyPressed ) );
}

////////////////////////////////////////////////////////////////////////////////
// SetKeyUp

void UiKeyboardWin::SetKeyUp( BtU32 keyIndex)
{
    BtAssert( keyIndex < 512 );
    
    // Is it pressed
    m_isKeyPressed[keyIndex] = BtFalse;
    
    int a=0;
    a++;
}

////////////////////////////////////////////////////////////////////////////////
// SetKeyDown

void UiKeyboardWin::SetKeyDown(BtU32 keyIndex)
{
    BtAssert( keyIndex < 512 );

    // Is it pressed
    m_isKeyPressed[keyIndex] = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void UiKeyboardWin::Update()
{
	if (m_isKeyPressed[UiKeyCode_LEFT])
	{
		int a = 0;
		a++;
	}
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

BtBool UiKeyboardWin::IsHeld( UiKeyCode key)
{
	return m_nKeyDown[key];
}

////////////////////////////////////////////////////////////////////////////////
// IsPressed

BtBool UiKeyboardWin::IsPressed( UiKeyCode key )
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

BtBool UiKeyboardWin::IsPressed()
{
    BtU32 numKeys = sizeof(m_nKeyDown);
    
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

BtBool UiKeyboardWin::IsReleased( UiKeyCode key )
{
	return m_nKeyUp[key];
}
