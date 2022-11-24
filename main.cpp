
#include "main.h"


using namespace TCLAP;
using namespace std;


inline float toneMap( const Tonemap *tm, float value ){
    return tm->expo * pow( value, tm->gamma );
}


int main(int argc, char* argv[])
{

	CmdLine cmd("hdr2png - convert radiance RGBE to png RGBA", ' ', "0.1");

	ValueArg<string> a_input("i","input","input hdr file",true,"","string", cmd );
	ValueArg<string> a_output("o","output","output png file",true,"","string", cmd );

	SwitchArg a_rebase("r","rebase", "maximize exponent range", false);
	SwitchArg a_verbose("v","verbose", "print infos", false);
    
    vector<string> formats;
    formats.push_back("rgbe");
    formats.push_back("rgbm");
    ValuesConstraint<string> allowedformats( formats );
    
    ValueArg<string> a_format("f","format","Ecoding format, default to rgbe", false,"rgbe",&allowedformats);

    TCLAP::ValueArg<float> a_expo("e","expo","exposure", false, 1.0, "expo");
    TCLAP::ValueArg<float> a_gamma("g","gamma","gamma", false, 1.0, "gamma");



    cmd.add( a_rebase );
    cmd.add( a_verbose );
    cmd.add( a_format );
    cmd.add( a_expo );
    cmd.add( a_gamma );

	cmd.parse( argc, argv );

	const char* input = a_input.getValue().c_str();
    const char* output = a_output.getValue().c_str();
    const char* format = a_format.getValue().c_str();

    const bool rebase  = a_rebase.getValue();
    const bool verbose = a_verbose.getValue();

    const Tonemap tm = {
        a_expo.getValue(),
        a_gamma.getValue()
    };
	//==================================================
	//								       Load hdr file
	//==================================================

	HDRLoaderResult* hdrData = new HDRLoaderResult();

	HDRLoader* hdrLoader = new HDRLoader();
	if( ! hdrLoader->load( input, *hdrData ) ) {
		printf( "error loading %s \n", input );
		return 1;
	}

	if( verbose )
		printf( "input loaded \n   size : %i*%i \n   range %i>%i \n", hdrData->width, hdrData->height , hdrData->eMin, hdrData->eMax );
    
    
    const unsigned char* rgba;
    if( strcmp(format, "rgbe") == 0 ) {
        float base;
        if( rebase )
            base = getNewBase( &tm, hdrData );
        else
            base = 2.0f;


        if( verbose )
            printf( "f32toRgbe (base : %f)\n", base );
        
        rgba = f32toRgbe( hdrData->cols, hdrData->width, hdrData->height, base, &tm );
    } else if( strcmp(format, "rgbm") == 0 ) {
        
        rgba = f32toRgbm( hdrData->cols, hdrData->width, hdrData->height );
        
    } else {
        printf("error unkown format: %s \n", a_format.getValue().c_str());
        exit(1);
    }
    
	if( verbose )
		printf( "encode png \n" );
	/*Encode the image*/
	unsigned error = lodepng_encode32_file( output, rgba, hdrData->width, hdrData->height );

	/*if there's an error, display it*/
	if(error) printf("error %u: %s \n", error, lodepng_error_text(error));


	delete hdrLoader;
	delete hdrData;


	return 0;
}



float getNewBase( const Tonemap *tm, HDRLoaderResult* hdrData ) {
    float fmin = pow( 2.0, (float)hdrData->eMin );
    float fmax = pow( 2.0, (float)hdrData->eMax );

    fmin = toneMap( tm, fmin );
    fmax = toneMap( tm, fmax );

	float newbaseMax = pow( fmin, 1.0/128.0 );
	float newbaseMin = pow( fmax, -1.0/128.0 );

	return max( newbaseMin, newbaseMax );
}



unsigned char* f32toRgbe( float* pixels, int w, int h, float base, const Tonemap *tm ) {

    //base = 2.0;

    int j;

    int resSize = w*h;

    unsigned char* rgbe = ( unsigned char* ) malloc( resSize*4*sizeof( unsigned char* ) );

    float r, g, b;
    float e, re;
    int f;

    float logbase = log( base );


    int c = 0;
    int fc = 0;
    for (j = 0; j < resSize; j++) {

        fc = j*3;
        c = j*4;

        r = toneMap( tm, pixels[fc]    );
        g = toneMap( tm, pixels[fc+1]  );
        b = toneMap( tm, pixels[fc+2]  );

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



unsigned char* f32toRgbm( float* pixels, int w, int h ) {

    //base = 2.0;

    int j;

    int resSize = w*h;

    unsigned char* rgbm = ( unsigned char* ) malloc( resSize*4*sizeof( unsigned char* ) );
    float mFactor = 16.0f;
    float r, g, b, m;
    
    int c = 0;
    int fc = 0;
    for (j = 0; j < resSize; j++) {

        fc = j*3;
        c = j*4;

        r = pixels[fc]   / mFactor;
        g = pixels[fc+1] / mFactor;
        b = pixels[fc+2] / mFactor;

        m = max( max(1e-6f, r), max( g, b ) );
        m = min( 1.0f, m );
        m = ceil( m * 255.0 ) / 255.0;
                
        r = r / m;
        g = g / m;
        b = b / m;

        rgbm[c+0] = char( r*255.0f );
        rgbm[c+1] = char( g*255.0f );
        rgbm[c+2] = char( b*255.0f );
        rgbm[c+3] = char( m*255.0f );
    }


    return rgbm;
}


