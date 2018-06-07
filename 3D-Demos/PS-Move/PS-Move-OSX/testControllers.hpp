//
//  testControllers.hpp
//  PSMoveOSX
//
//  Created by localadmin on 07/06/2018.
//

#pragma once
#include <stdio.h>
#include "movegame.hpp"

class TestControllers : public MoveGame
{
public:
    BtU32 numControllers;
    PSMove *moveArr[16];
    int lastTrigger[16];
    BtBool magnet[16];
    
#ifdef MADGWICK
    Madgwick madgwick[16];
#endif
    
    void reset();
    void setup();
    void update();
};
