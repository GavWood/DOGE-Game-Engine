
////////////////////////////////////////////////////////////////////////////////
// ShRecorder

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

enum ShRecorderActionType
{
    ShRecorderActionType_Record,
    ShRecorderActionType_Stop,
};

struct ShRecorderAction
{
    ShRecorderActionType                         m_action;
    BtChar                                       m_filename[32];
};

// Class definition
class ShRecorder
{
public:
    
    // Access
    static void                                 PushAction( ShRecorderAction action );
    static ShRecorderAction                     PopAction();
    static BtU32                                GetNumItems();
    
    // State functions
    
    // Public functions
private:
    
    static BtQueue<ShRecorderAction, 128>       m_actions;
};

