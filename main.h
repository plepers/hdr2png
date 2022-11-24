


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

typedef struct _Tonemap{
    float expo;
    float gamma;
} Tonemap;

float getNewBase( const Tonemap*, HDRLoaderResult* );

unsigned char* f32toRgbe( float* faces, int w, int h, float base, const Tonemap* );
unsigned char* f32toRgbm( float* faces, int w, int h );

#endif

