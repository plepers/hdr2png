
#include "main.h"


using namespace TCLAP;
using namespace std;



int main(int argc, char* argv[])
{

	CmdLine cmd("hdr2png - convert radiance RGBE to png RGBA", ' ', "0.1");

	ValueArg<string> a_input("i","input","input hdr file",true,"","string", cmd );
	ValueArg<string> a_output("o","output","output png file",true,"","string", cmd );
	
	SwitchArg a_rebase("r","rebase", "maximize exponent range", true);
	cmd.add( a_rebase );

	cmd.parse( argc, argv );
	
	const char* input = a_input.getValue().c_str();
	const char* output = a_output.getValue().c_str();
	
	const bool rebase = a_rebase.getValue();

	//==================================================
	//								       Load hdr file
	//==================================================

	HDRLoaderResult* hdrData = new HDRLoaderResult();

	HDRLoader* hdrLoader = new HDRLoader();
	if( ! hdrLoader->load( input, *hdrData ) ) {
		printf( "error loading %s \n", input );
		return 1;
	}

	printf( "input loaded \n   size : %i*%i \n   range %i>%i \n", hdrData->width, hdrData->height , hdrData->eMin, hdrData->eMax );
	
	double base;
	if( rebase ) 
		base = getNewBase(  hdrData->eMin, hdrData->eMax );
	else
		base = 2.0f;


	
	printf( "f32toRgbe (base : %f)\n", base );
	const unsigned char* rgbe = f32toRgbe( hdrData->cols, hdrData->width, hdrData->height, base );

	printf( "encode png \n" );
	/*Encode the image*/
	unsigned error = lodepng_encode32_file( output, rgbe, hdrData->width, hdrData->height );

	/*if there's an error, display it*/
	if(error) printf("error %u: %s \n", error, lodepng_error_text(error));


	delete hdrLoader;
	delete hdrData;


	return 0;
}



double getNewBase( char min, char max ) {
	double newbaseMax = pow( pow( 2.0, (double)max ), 1.0/128.0 );
	double newbaseMin = pow( pow( 2.0, (double)min ), -1.0/128.0 );
    
    if( newbaseMax > newbaseMin)
        return newbaseMax;
	return newbaseMin;
}



unsigned char* f32toRgbe( float* pixels, int w, int h, double base ) {

	//base = 2.0;

	int j;

	int resSize = w*h;

	unsigned char* rgbe = ( unsigned char* ) malloc( resSize*4*sizeof( unsigned char* ) );

	float r, g, b; 
	double e, re;
	int f;

	double logbase = log( base ); 
	

	int c = 0;
	int fc = 0;
	for (j = 0; j < resSize; j++) {
		
		fc = j*3;
		c = j*4;

		r = pixels[fc];
		g = pixels[fc+1];
		b = pixels[fc+2];

		re = max( r, max( g, b ) );

		f = int( ceil( log( re ) / logbase ) );

		if( f < -128.0f ) f = -128.0f;
		if( f > 127.0f ) f = 127.0f;

		e = pow( base, f );
		
		r = r*255.0f / e;
		g = g*255.0f / e;
		b = b*255.0f / e;

		f += 128.0f;

		
		rgbe[c] = char( r );
		rgbe[c+1] = char( g );
		rgbe[c+2] = char( b );
		rgbe[c+3] = char( f );
	}


	return rgbe;
}