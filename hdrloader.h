
/***********************************************************************************
	Created:	17:9:2002
	FileName: 	hdrloader.h
	Author:		Igor Kravtchenko
	
	Info:		Load HDR image and convert to a set of float32 RGB triplet.
************************************************************************************/

#ifndef HDR_LOADER_H
#define HDR_LOADER_H

class HDRLoaderResult {
public:
	int width, height;
	char eMax, eMin;
	// each pixel takes 3 float32, each component can be of any value...
	float *cols;
};

class HDRLoader {
public:
	static bool load(const char *fileName, HDRLoaderResult &res);
};

#endif // HDR_LOADER_H

