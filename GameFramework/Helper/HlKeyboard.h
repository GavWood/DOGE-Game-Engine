////////////////////////////////////////////////////////////////////////////////
// HlKeyboard

// Include guard
#pragma once

// Includes
#include "BtTypes.h"
#include "UiKeyboard.h"

const UiKeyCode DebugKey			= UiKeyCode_F1;
const UiKeyCode SaveCameraKey		= UiKeyCode_F2;
const UiKeyCode LoadCameraKey		= UiKeyCode_F3;
const UiKeyCode CloseGameKey		= UiKeyCode_F4;
const UiKeyCode ReloadMenuKey		= UiKeyCode_F5;
const UiKeyCode CameraKey			= UiKeyCode_F6;
const UiKeyCode ShutdownPeersKey	= UiKeyCode_F10;
const UiKeyCode ScreenShotKey		= UiKeyCode_F11;
const UiKeyCode ResetGameKey		= UiKeyCode_R;
const UiKeyCode RecentreViewGameKey = UiKeyCode_R;
const UiKeyCode PauseKey			= UiKeyCode_P;
const UiKeyCode MirrorKey			= UiKeyCode_M;

// Class definition
class HlKeyboard
{
public:
	static BtBool IsNumericPressed();
	static BtBool IsReloadMenuPressed();
};
