////////////////////////////////////////////////////////////////////////////////
// UiInputWinGL

// Includes
#include <string.h>
#include "BtTypes.h"
#include "UiInputImpl.h"
#include "UiKeyboardImpl.h"

#ifdef WIN32
#include "Ui360.h"
#endif

UiKeyboardWinGL sKeyboardGL;

////////////////////////////////////////////////////////////////////////////////
// Update the controls

void UiInputWinGL::Update()
{
	// Update the keyboard
	sKeyboardGL.Update();

#ifdef WIN32
	// Update the 360 pad
	Ui360::Update();
#endif
}
