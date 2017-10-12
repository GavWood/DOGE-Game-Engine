////////////////////////////////////////////////////////////////////////////////
// ErrorLog.h

// Include guard
#ifndef __ErrorLog_H
#define __ErrorLog_H

// Includes
#include <stdio.h>
#include "BtTypes.h"

// Class definition
class ErrorLog
{
public:

	// Public functions
	static void		Create();

	static void		Printf( const BtChar* pFormat, ... );

	static void		Fatal_Printf( const BtChar* pDescription, ... );

	static void		Filename( const BtChar* pFilename );

private:

	// Private functions
	static void		Print( const BtChar* pString );
};

#endif // __ErrorLog_H
