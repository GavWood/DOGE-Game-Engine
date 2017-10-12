////////////////////////////////////////////////////////////////////////////////
// ShKeyboard

// Includes
#include "ShKeyboard.h"
#include "RsUtil.h"
#include "RsMaterial.h"
#include "UiKeyboard.h"
#include "BtString.h"
#include "ApConfig.h"

////////////////////////////////////////////////////////////////////////////////
// statics

// Public functions
BtQueue<ShKeyboardAction, 128> ShKeyboard::m_actions;
BtU32 ShKeyboard::m_length = 8;
BtBool ShKeyboard::m_isVisible = BtFalse;

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShKeyboard::PushAction( ShKeyboardAction action )
{
    ShKeyboardActionType actionType = action.m_type;
    
    // Consume the open and close requests
    if( actionType == ShKeyboardActionType_Open )
    {
        m_isVisible = BtTrue;
    }
    else if( ( action.m_type == ShKeyboardActionType_DoneAndClosed ) ||
             ( action.m_type == ShKeyboardActionType_Closed ) )
    {
        m_isVisible = BtFalse;
    }
    m_actions.Push( action );
}

////////////////////////////////////////////////////////////////////////////////
// GetLength

//static
BtU32 ShKeyboard::GetLength()
{
    return m_length;
}

////////////////////////////////////////////////////////////////////////////////
// SetLength

//static
void ShKeyboard::SetLength( BtU32 length )
{
    m_length = length;
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

//static
BtU32 ShKeyboard::GetNumItems()
{
    return m_actions.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopAction

//static
ShKeyboardAction ShKeyboard::PopAction()
{
    ShKeyboardAction action;
    action = m_actions.Pop();
    return action;
}

////////////////////////////////////////////////////////////////////////////////
// PeekAction

//static
ShKeyboardAction ShKeyboard::PeekAction()
{
	ShKeyboardAction action;
	action = m_actions.Peek();
	return action;
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShKeyboard::Update()
{
	// If there a keyboard action in the stack then we may need to respond to it
	if( ShKeyboard::GetNumItems() && ApConfig::IsWin() )
	{
		ShKeyboardAction action = ShKeyboard::PeekAction();

		if ( (action.m_type == ShKeyboardActionType_Open) &&
			!UiKeyboard::pInstance()->IsPressed(UiKeyCode_RETURN) )
		{
			PopAction();
		}
		if (action.m_type == ShKeyboardActionType_Close)
		{
			PopAction();
			ShKeyboardAction action;
			action.m_type = ShKeyboardActionType_Closed;
			BtStrCopy(action.m_buffer, sizeof(action.m_buffer), "\0");
			ShKeyboard::PushAction(action);
		}
        if (action.m_type == ShKeyboardActionType_Done )
		{
			PopAction();
			ShKeyboardAction action;
			action.m_type = ShKeyboardActionType_DoneAndClosed;
			BtStrCopy(action.m_buffer, sizeof(action.m_buffer), "\0");
			ShKeyboard::PushAction(action);
		}
	}

	if (m_isVisible)
	{
		BtChar keycode[2];

		if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_A))
			BtStrCopy(keycode, sizeof(keycode), "A");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_B))
			BtStrCopy(keycode, sizeof(keycode), "B");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_C))
			BtStrCopy(keycode, sizeof(keycode), "C");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_D))
			BtStrCopy(keycode, sizeof(keycode), "D");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_E))
			BtStrCopy(keycode, sizeof(keycode), "E");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_F))
			BtStrCopy(keycode, sizeof(keycode), "F");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_G))
			BtStrCopy(keycode, sizeof(keycode), "G");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_H))
			BtStrCopy(keycode, sizeof(keycode), "H");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_I))
			BtStrCopy(keycode, sizeof(keycode), "I");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_J))
			BtStrCopy(keycode, sizeof(keycode), "J");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_K))
			BtStrCopy(keycode, sizeof(keycode), "K");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_L))
			BtStrCopy(keycode, sizeof(keycode), "L");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_M))
			BtStrCopy(keycode, sizeof(keycode), "M");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_N))
			BtStrCopy(keycode, sizeof(keycode), "N");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_O))
			BtStrCopy(keycode, sizeof(keycode), "O");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_P))
			BtStrCopy(keycode, sizeof(keycode), "P");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_Q))
			BtStrCopy(keycode, sizeof(keycode), "Q");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_R))
			BtStrCopy(keycode, sizeof(keycode), "R");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_S))
			BtStrCopy(keycode, sizeof(keycode), "S");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_T))
			BtStrCopy(keycode, sizeof(keycode), "T");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_U))
			BtStrCopy(keycode, sizeof(keycode), "U");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_V))
			BtStrCopy(keycode, sizeof(keycode), "V");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_W))
			BtStrCopy(keycode, sizeof(keycode), "W");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_X))
			BtStrCopy(keycode, sizeof(keycode), "X");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_Y))
			BtStrCopy(keycode, sizeof(keycode), "Y");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_Z))
			BtStrCopy(keycode, sizeof(keycode), "Z");
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_DELETE) ||
			UiKeyboard::pInstance()->IsPressed(UiKeyCode_BACK))
		{
			BtStrCopy(keycode, sizeof(keycode), "\0");
		}
		else if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_RETURN))
		{
			// Push the action
			ShKeyboardAction action;
			action.m_type = ShKeyboardActionType_Done;
			BtStrCopy(action.m_buffer, sizeof(action.m_buffer), "\0");
			ShKeyboard::PushAction(action);
			return;
		}
		else
		{
			return;
		}

		// Push the action
		ShKeyboardAction action;
		action.m_type = ShKeyboardActionType_KeyPressed;
		BtStrCopy(action.m_buffer, sizeof(action.m_buffer), keycode);
		ShKeyboard::PushAction(action);
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsVisible

//static
BtBool ShKeyboard::IsVisible()
{
	return m_isVisible;
}
