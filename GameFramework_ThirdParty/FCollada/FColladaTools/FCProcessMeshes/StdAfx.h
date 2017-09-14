/*
    Copyright (C) 2005-2007 Feeling Software Inc.
    MIT License: http://www.opensource.org/licenses/mit-license.php

    Portions of the code surrounded by PREMIUM conditional compilation
    blocks are copyright Feeling Software Inc., strictly confidential and released
    to Premium Support licensees only. Licensees are allowed to modify and extend
    the Premium code for their internal use. Redistributing the Premium source code
    or any of its derivative product (e.g. binary versions) is forbidden.
*/

#pragma once

#define UNICODE
#define _UNICODE

//See StdAfx.h in the FCProcessImages folder for explanation.

#ifndef WIN32
#include <iostream>
#endif

#define NO_LIBXML
#include "FCollada.h"

#ifdef WIN32
#include <iostream>
#endif
