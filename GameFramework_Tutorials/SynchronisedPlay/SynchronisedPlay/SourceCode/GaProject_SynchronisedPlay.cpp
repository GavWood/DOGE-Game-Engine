////////////////////////////////////////////////////////////////////////////////
// GaProject_Tutorial1.cpp

// Includes
#include "SbMain.h"

SbMain project;

////////////////////////////////////////////////////////////////////////////////
// main

int main()
{
	// Create the game
	Game::Create(&project);

	// Exit the application
	exit(EXIT_SUCCESS);
}
