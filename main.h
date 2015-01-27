


#ifndef HDR2PNG_MAIN_H
#define HDR2PNG_MAIN_H

#include "hdrloader.h"
#include "lodepng.h"
#include "tclap/CmdLine.h"

#ifdef WIN
#include <Windows.h>
#endif


#ifdef __APPLE__
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#endif

double getNewBase( char min, char max );

unsigned char* f32toRgbe( float* faces, int w, int h, double base );

#endif

