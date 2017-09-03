////////////////////////////////////////////////////////////////////////////////
// UiInputWinGL

// Includes
#include <string.h>
#include "BtTypes.h"
#include "UiInputWinGL.h"
#include "UiKeyboardWinGL.h"
#include "Ui360.h"

UiKeyboardWinGL sKeyboardGL;

////////////////////////////////////////////////////////////////////////////////
// Update the controls

void UiInputWinGL::Update()
{
	// Update the keyboard
	sKeyboardGL.Update();
}
