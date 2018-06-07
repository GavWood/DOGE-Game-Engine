//
//  itsabomb.hpp
//  PSMoveOSX
//
//  Created by localadmin on 07/06/2018.
//

#pragma once
#include <stdio.h>
#include "movegame.hpp"

class ItsABombGame : public MoveGame
{
public:
    BtU32 numControllers;
    PSMove *moveArr[16];
    int lastTrigger[16];
    BtBool magnet[16];
    
    void reset();
    void setup();
    void update();
    
    BtBool bang = BtFalse;
    BtBool m_reset = BtTrue;
    BtFloat startTime;
    BtBool stop = BtFalse;
    BtU32 lastTime = 0;
};
