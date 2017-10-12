////////////////////////////////////////////////////////////////////////////////
// BtEnum.h

// Include guard
#pragma once
#include "BtBase.h"

// Class definition
enum class BtEnum
{
public:
private:
};


// Special behavior for ++GameState
BtEnum& operator++(BtEnum &c)
{
	int iv = static_cast<int>(c);
	if (iv < static_cast<int>(GameState::GameState_MAX))
	{
		iv++;
	}
	c = static_cast<GameState>(iv);
	return c;
}

// Special behavior for GameState++
GameState operator++(GameState &c, int) {
	GameState result = c;
	++c;
	return c;
}