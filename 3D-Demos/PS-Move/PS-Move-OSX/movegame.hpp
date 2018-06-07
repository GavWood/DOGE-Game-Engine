//
//  movegame.hpp
//  PSMoveOSX
//
//  Created by localadmin on 07/06/2018.
//

#pragma once
#include <stdio.h>

// Thomas Perl's PS-Move-API
#import "psmove.h"

//#define MADGWICK
#ifdef MADGWICK
#include "madgwickAHRS.h"
#endif

class MoveGame
{
public:
    virtual void setup() = 0;
    virtual void reset() = 0;
    virtual void update() = 0;
};
