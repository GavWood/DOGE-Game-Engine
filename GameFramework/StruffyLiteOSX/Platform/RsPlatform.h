//
//  RsPlatform.h
//  Packer_OSX
//
//  Created by localadmin on 23/07/2017.
//  Copyright © 2017 localadmin. All rights reserved.
//

#pragma once

#ifndef GL_BUFFER_OFFSET
#define GL_BUFFER_OFFSET(i)((char *)NULL + (i))
#endif

#ifdef WIN32

#include <GL/glew.h>

#else

#include "GLee.h"

#endif
