////////////////////////////////////////////////////////////////////////////////
// HlKeyboard

// Includes
#include "HlKeyboard.h"
#include "UiKeyboard.h"

////////////////////////////////////////////////////////////////////////////////
// IsNumericPressed
//static

BtBool HlKeyboard::IsNumericPressed()
{
	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_0 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_1 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_2 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_3 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_4 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_5 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_6 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_7 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_8 ) ||
		UiKeyboard::pInstance()->IsPressed( UiKeyCode_9 ) )
	{
		return BtTrue;
	}
	return BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsReloadMenuPressed
//static

BtBool HlKeyboard::IsReloadMenuPressed()
{
	if (UiKeyboard::pInstance()->IsPressed(ReloadMenuKey) )
	{
		return BtTrue;
	}
	return BtFalse;
}
