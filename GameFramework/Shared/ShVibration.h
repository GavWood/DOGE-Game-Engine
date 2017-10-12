////////////////////////////////////////////////////////////////////////////////
// ShVibration

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtQueue.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"

enum ShVibrationActionType
{
	ShVibrationActionType_Start,
	ShVibrationActionType_MAX,
};

struct ShVibrationAction
{
    ShVibrationActionType					m_type;
};

// Class definition
class ShVibration
{
public:

	// Access
	static void                             PushAction( ShVibrationAction action );
	static ShVibrationAction                PopAction();
	static ShVibrationAction                PeekAction();
    static BtU32                            GetNumItems();
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShVibrationAction, 128>	m_actions;
    static BtU32                            m_length;
};
