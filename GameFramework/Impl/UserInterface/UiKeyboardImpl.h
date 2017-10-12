////////////////////////////////////////////////////////////////////////////////
// UiKeyboardWinGL.h

#pragma once
#include "BtTypes.h"
#include "UiKeyboard.h"

// Class definition
class UiKeyboardWin : public UiKeyboard
{
	public:
  
		// Constructor
		UiKeyboardWin();

		// Public functions
		void					Update();

		// Accessors
		BtBool					IsPressed ( UiKeyCode code );
		BtBool					IsHeld    ( UiKeyCode code );
		BtBool					IsReleased( UiKeyCode code );
		BtBool					IsPressed();
    
        void                    SetKeyUp(BtU32 key );
        void                    SetKeyDown(BtU32 key );
    
		static BtU32			GetNumKeys();
		static BtU32		    GetKeyCode( BtU32 index);

		BtBool					m_isKeyPressed[512];

	private:

		BtBool					m_nLastKeyDown[512];
		BtBool					m_nKeyDown[512];
		BtBool					m_nKeyUp[512];
		BtBool					m_nKeyNewDown[512];
};
