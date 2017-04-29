////////////////////////////////////////////////////////////////////////////////
// UiKeyboardWinGL.h

#pragma once
#include "BtTypes.h"
#include "UiKeyboard.h"

// Class definition
class UiKeyboardWinGL : public UiKeyboard
{
	public:
  
		// Constructor
		UiKeyboardWinGL();

		// Public functions
		void					Update();

		// Accessors
		BtBool					IsPressed ( UiKeyCode code );
		BtBool					IsHeld    ( UiKeyCode code );
		BtBool					IsReleased( UiKeyCode code );
		BtBool					IsPressed();

	private:

		BtBool					m_nLastKeyDown[512];
		BtBool					m_nKeyDown[512];
		BtBool					m_nKeyUp[512];
		BtBool					m_nKeyNewDown[512];
};
