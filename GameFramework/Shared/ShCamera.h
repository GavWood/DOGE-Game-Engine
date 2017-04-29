////////////////////////////////////////////////////////////////////////////////
// ShCamera

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

enum ShCameraActionType
{
	ShCameraActionType_CaptureFront,
    ShCameraActionType_CaptureBack,
	ShCameraActionType_CaptureStop,
	ShCameraActionType_SaveToAlbums,
	ShCameraActionType_MAX,
};

struct ShCameraAction
{
    ShCameraActionType                          m_action;
    BtU8                                       *m_pMemory;
};

// Class definition
class ShCamera
{
public:

	// Access
	static void                                 PushAction( ShCameraActionType action );
    static void                                 PushAction( ShCameraAction action );
	static ShCameraAction                       PopAction();
    static BtU32                                GetNumItems();
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShCameraAction, 128>         m_actions;
};
