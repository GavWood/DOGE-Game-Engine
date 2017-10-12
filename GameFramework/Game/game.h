////////////////////////////////////////////////////////////////////////////////
// Game.h

// Include guard
#pragma once

#include "BtTypes.h"
#include "ApConfig.h"

class RsFlash;
class GaProject;

// Class definition
class Game
{
public:

	// Constructor
	Game( GaProject *);

	// Public functions
	void Create();

	static void Create( GaProject *pProject);
};
