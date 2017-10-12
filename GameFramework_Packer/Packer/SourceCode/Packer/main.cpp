//
//  main.cpp
//
// Platform independent main for getting access to passed in parameters
//

#include <iostream>

#ifndef WIN32
#include <unistd.h>
#else
#include <tchar.h>
#endif

#include "PaTopState.h"

#ifdef WIN32

int _tmain(int argc, _TCHAR* argv[])
{
	BtU32 platformIndex = -1;
	if (argv[1])
	{
		platformIndex = atoi(argv[1]);
	}
	PaTopState::HookIntoMain(argv[2], platformIndex );
}

#else

int main(int argc, const char * argv[])
{
//    chdir( "/Users/localadmin/Documents/GameFramework/GameFramework_Packer" );
    chdir( "/Users/localadmin/Documents/GameFramework/GavinWood_Packer" );

    PaTopState::HookIntoMain( NULL, 0 );
    return 0;
}

#endif

