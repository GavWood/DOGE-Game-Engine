////////////////////////////////////////////////////////////////////////////////
// ShAchievement

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

enum ShAchievementActionType
{
	ShAchievementActionType_Add,
    ShAchievementActionType_Done,
    ShAchievementActionType_Reset,
	ShAchievementActionType_MAX,
};

struct ShAchievementAction
{
	ShAchievementActionType					m_type;
    BtChar                                  m_id[64];
};


// Class definition
class ShAchievement
{
public:

	// Access
	static void                             PushAction( ShAchievementAction action );
	static ShAchievementAction              PopAction();
	static ShAchievementAction              PeekAction();
    static BtU32                            GetNumItems();
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShAchievementAction, 128>      m_actions;
};
