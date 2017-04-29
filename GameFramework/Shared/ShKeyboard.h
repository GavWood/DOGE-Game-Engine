////////////////////////////////////////////////////////////////////////////////
// ShKeyboard

// Include guard
#pragma once

// Includes
#include "BaArchive.h"
#include "BtBase.h"
#include "BtQueue.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"

enum ShKeyboardActionType
{
	ShKeyboardActionType_Open,					// Ask the keyboard to open
    ShKeyboardActionType_KeyPressed,			// Signals what key has been pressed
	ShKeyboardActionType_Done,					// Signals when done has been pressed
	ShKeyboardActionType_Close,					// Close the keyboard
    ShKeyboardActionType_Closed,				// This is pushed when the keyboard is dismissed by the user
	ShKeyboardActionType_DoneAndClosed,			// This is pushed when the keyboard is dismissed by the user
	ShKeyboardActionType_MAX,
};

struct ShKeyboardAction
{
    ShKeyboardActionType					m_type;
    BtChar                                  m_buffer[64];
	MtVector2								m_v2Position;
	MtVector2								m_v2Dimension;
};

// Class definition
class ShKeyboard
{
public:

	// Access
	static void                             PushAction( ShKeyboardAction action );
	static ShKeyboardAction                 PopAction();
	static ShKeyboardAction                 PeekAction();
    static BtU32                            GetNumItems();
    static BtU32                            GetLength();
    static void                             SetLength( BtU32 length );
	static void								Update();
	  
	// State functions
    static BtBool							IsVisible();
	// Public functions

private:

    static BtQueue<ShKeyboardAction, 128>	m_actions;
    static BtU32                            m_length;
	static BaArchive					   *m_pArchive;
	static BtBool						    m_isVisible;
};

