
////////////////////////////////////////////////////////////////////////////////
// ShMail

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

enum ShMailActionType
{
    ShMailActionType_Send,
    ShMailActionType_MAX,
};

struct ShMailAction
{
    ShMailActionType                             m_action;
    BtChar                                       m_email[32];
    BtChar                                       m_subject[128];
    BtChar                                       m_body[4096];
    BtChar                                       m_filename[32];
    BtChar                                       m_mimeType[32];
    BtU8                                        *m_pMemory;
};

// Class definition
class ShMail
{
public:
    
    // Access
    static void                                 PushAction( ShMailAction action );
    static ShMailAction                         PopAction();
    static BtU32                                GetNumItems();
    
    // State functions
    
    // Public functions
private:
    
    static BtQueue<ShMailAction, 128>           m_actions;
};

