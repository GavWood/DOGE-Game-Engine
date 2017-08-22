#ifndef CORONA_OPEN_H
#define CORONA_OPEN_H


#include "corona.h"


namespace corona {
  Image* OpenBMP (File* file); // OpenBMP.cpp
#ifndef NO_JPEG
  Image* OpenJPEG(File* file); // OpenJPEG.cpp
#endif
  Image* OpenPCX (File* file); // OpenPCX.cpp
#ifndef NO_PNG
  Image* OpenPNG (File* file); // OpenPNG.cpp
#endif
  Image* OpenTGA (File* file); // OpenTGA.cpp
  Image* OpenGIF (File* file); // OpenGIF.cpp
}


#endif
