////////////////////////////////////////////////////////////////////////////////
// UiKeyboardWinGL.cpp

#include "BtBase.h"
#include "BtMemory.h"
#include "UiKeyboardWinGL.h"
#include "ErrorLog.h"

const BtU32 asciiKeys[] = {
                            (BtU32)0,
                            (BtU32)1,
                            (BtU32)2,
                            (BtU32)3,
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
                            (BtU32)'a',
                            (BtU32)'b',
                            (BtU32)'c',
                            (BtU32)'d',
                            (BtU32)'e',
                            (BtU32)'f',
                            (BtU32)'g',
                            (BtU32)'h',
                            (BtU32)'i',
                            (BtU32)'j',
                            (BtU32)'k',
                            (BtU32)'l',
                            (BtU32)'m',
                            (BtU32)'n',
                            (BtU32)'o',
                            (BtU32)'p',
                            (BtU32)'q',
                            (BtU32)'r',
                            (BtU32)'s',
                            (BtU32)'t',
                            (BtU32)'u',
                            (BtU32)'v',
                            (BtU32)'w',
                            (BtU32)'x',
                            (BtU32)'y',
                            (BtU32)'z',
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
                            (BtU32)' ',
                            (BtU32)UiKeyCode_LSHIFT,
                            (BtU32)UiKeyCode_RSHIFT,
                            (BtU32)';',
                            (BtU32)'/',
                            (BtU32)UiKeyCode_ALT,
                            (BtU32)UiKeyCode_F1,
                            (BtU32)UiKeyCode_F2,
                            (BtU32)UiKeyCode_F3,
                            (BtU32)UiKeyCode_F4,
                            (BtU32)UiKeyCode_F5,
                            (BtU32)',',
                            (BtU32)'.',
                            (BtU32)10,
                            (BtU32)UiKeyCode_MAX,
};

const BtU32 struffKeys[] = {
                             (BtU32)UiKeyCode_UP,           // 0
                             (BtU32)UiKeyCode_DOWN,         // 1
                             (BtU32)UiKeyCode_LEFT,         // 2
                             (BtU32)UiKeyCode_RIGHT,        // 3
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
							 (BtU32)UiKeyCode_COMMA,
							 (BtU32)UiKeyCode_PERIOD,
                             (BtU32)UiKeyCode_DELETE,
                             (BtU32)UiKeyCode_MAX
};

////////////////////////////////////////////////////////////////////////////////
// Constructor

UiKeyboardWinGL::UiKeyboardWinGL()
{
	m_pInstance = this;
	BtMemory::Set( m_nKeyDown, 0, sizeof( m_nKeyDown ) );
	BtMemory::Set( m_nKeyNewDown, 0, sizeof( m_nKeyNewDown ) );
	BtMemory::Set( m_nKeyUp, 0, sizeof( m_nKeyUp ) );
    BtMemory::Set( m_isKeyPressed, 0, sizeof( m_isKeyPressed ) );
}

////////////////////////////////////////////////////////////////////////////////
// FindKeyCode

BtU32 UiKeyboardWinGL::FindKeyCode( BtU32 key)
{
    BtU32 numKeys = sizeof(asciiKeys) / sizeof(BtU32);
    
    for( BtU32 i=0; i<numKeys; i++ )
    {
        if( asciiKeys[i] == key )
        {
            return i;
        }
    }
    return numKeys - 1;
}

////////////////////////////////////////////////////////////////////////////////
// SetKeyUp

void UiKeyboardWinGL::SetKeyUp( BtChar key )
{
    BtU32 keyIndex = FindKeyCode( key );
    
    // Get the key code mapping
    BtU32 myKey = struffKeys[keyIndex];
   
    // Is it pressed
    m_isKeyPressed[myKey] = BtFalse;
    
    int a=0;
    a++;
}

////////////////////////////////////////////////////////////////////////////////
// SetKeyDown

void UiKeyboardWinGL::SetKeyDown( BtChar key )
{
    BtU32 keyIndex = FindKeyCode( key );
    
    // Get the key code mapping
    BtU32 myKey = struffKeys[keyIndex];
    
    // Is it pressed
    m_isKeyPressed[myKey] = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// Update

void UiKeyboardWinGL::Update()
{
    BtU32 numKeys = sizeof(struffKeys) / sizeof(BtU32);
	
	for( BtU32 i=0; i<numKeys; i++ )
	{
        // Look at each of my key mappings
        BtU32 myKey = struffKeys[i];
        
        // Was the key pressed?
        BtBool isKeyPressed = m_isKeyPressed[myKey];
        
        if( myKey == UiKeyCode_R )
        {
            if( isKeyPressed )
            {
                int a=0;
                a++;
            }
        }
        
		// Was it released
		m_nKeyUp[myKey] = !isKeyPressed && m_nKeyDown[myKey];

		// Is it a new key press?
		m_nKeyNewDown[myKey] = isKeyPressed && !m_nKeyDown[myKey];

		// Set this key as pressed
		m_nKeyDown[myKey] = isKeyPressed;
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
