////////////////////////////////////////////////////////////////////////////////
// UiKeyboardWinGL.cpp

#include "BtBase.h"
#include "BtMemory.h"
#include "UiKeyboardWinGL.h"
#include "ErrorLog.h"
#include "glfw.h"

const BtU32 glfwKeys[] = { (BtU32)GLFW_KEY_ESC,
						   (BtU32)GLFW_KEY_ENTER,
						   (BtU32)'A',
						   (BtU32)'B',
						   (BtU32)'C',
						   (BtU32)'D',
						   (BtU32)'E',
						   (BtU32)'F',
						   (BtU32)'G',
						   (BtU32)'H',
						   (BtU32)'I',
						   (BtU32)'J',
						   (BtU32)'K',
						   (BtU32)'L',
						   (BtU32)'M',
						   (BtU32)'N',
						   (BtU32)'O',
						   (BtU32)'P',
						   (BtU32)'Q',
						   (BtU32)'R',
						   (BtU32)'S',
						   (BtU32)'T',
						   (BtU32)'U',
						   (BtU32)'V',
						   (BtU32)'W',
						   (BtU32)'X',
						   (BtU32)'Y',
						   (BtU32)'Z',
						   (BtU32)'0',
						   (BtU32)'1',
						   (BtU32)'2',
						   (BtU32)'3',
						   (BtU32)'4',
						   (BtU32)'5',
						   (BtU32)'6',
						   (BtU32)'7',
						   (BtU32)'8',
						   (BtU32)'9',
						   (BtU32)GLFW_KEY_SPACE,
						   (BtU32)GLFW_KEY_LSHIFT,
						   (BtU32)GLFW_KEY_RSHIFT,
						   (BtU32)39,
						   (BtU32)47,
						   (BtU32)GLFW_KEY_LALT,
						   (BtU32)GLFW_KEY_F1,
						   (BtU32)GLFW_KEY_F2,
						   (BtU32)GLFW_KEY_F3,
						   (BtU32)GLFW_KEY_F4,
						   (BtU32)GLFW_KEY_F5,
						   (BtU32)GLFW_KEY_F6,
						   (BtU32)GLFW_KEY_F7,
						   (BtU32)GLFW_KEY_F8,
						   (BtU32)GLFW_KEY_F9,
						   (BtU32)GLFW_KEY_F10,
						   (BtU32)GLFW_KEY_F11,
						   (BtU32)GLFW_KEY_F12,
						   (BtU32)',',
						   (BtU32)'.',
						   (BtU32)295,
						   (BtU32)284,
						   (BtU32)283,
						   (BtU32)285,
						   (BtU32)286,
							(BtU32)'=',
							(BtU32)'-',
						};

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
// Update

void UiKeyboardWinGL::Update()
{
	BtU32 numKeys  = sizeof(glfwKeys) / sizeof(BtU32);
	BtU32 numKeys2 = sizeof(struffKeys) / sizeof(BtU32);
	BtAssert( numKeys == numKeys2 );

	for( BtU32 i=0; i<numKeys; i++ )
	{
		BtU32 glfwKey = glfwKeys[i];
		BtU32 myKey = struffKeys[i];

		BtBool isKeyPressed = BtFalse;

		if( glfwGetKey( glfwKey ) == GLFW_PRESS )
		{
			isKeyPressed = BtTrue;
		}

		// Was it released
		m_nKeyUp[myKey] = !isKeyPressed && m_nKeyDown[myKey];

		// Is it a new key press?
		m_nKeyNewDown[myKey] = isKeyPressed && !m_nKeyDown[myKey];

		// Set this key as pressed
		m_nKeyDown[myKey] = isKeyPressed;
	}

	if( m_nKeyNewDown[UiKeyCode_F4] )
	{
		int a=0;
		a++;
	}

	for( BtU32 i=0; i<512; i++ )
	{
		// For capturing unknown key codes
		if( glfwGetKey( i ) == GLFW_PRESS )
		{
			int a=0;
			a++;
		}
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
	BtU32 numKeys = sizeof(glfwKeys) / sizeof(BtU32);
	
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
