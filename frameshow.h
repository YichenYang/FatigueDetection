#ifndef DETECTEYEOPEN_H
#define DETECTEYEOPEN_H


// #ifdef MYLIBDLL
// #define MYLIBDLL extern "C" _declspec(dllimport) 
// #else
// #define MYLIBDLL extern "C" _declspec(dllexport) 
// #endif


#include "stdafx.h"
#include "cv.h"

#include "cxcore.h"
#include <iostream>
using namespace std;
#include <time.h>
#include "math.h"
#include "threholdotsu.h"
#include "eye.h"
#include "eyedetect.h"
#include "eyetrack.h"
#include "highgui.h"




int detectframe(IplImage * frame, gsl_rng * rng, CvHaarClassifierCascade *facecascade, CvHaarClassifierCascade *lefteyecascade,
				 CvHaarClassifierCascade *righteyecascade, int * lefteyeclose, int * righteyeclose);

#endif