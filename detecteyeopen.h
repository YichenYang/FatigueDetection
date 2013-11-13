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
#include "haarfaces.h"
#include "eyetrack.h"
#include "highgui.h"






// void detectfaces( IplImage* img, IplImage * small_img, CvHaarClassifierCascade *facecascade, CvHaarClassifierCascade *lefteyecascade, CvHaarClassifierCascade *righteyecascade);
/*
使用haar分类器检测图像上的人脸，并在人脸部分检测人眼
img为输入图像
small_img为输入图像缩小后的图像
facecascade为人脸检测器
lefteyecascade为左眼检测器
righteyecascade为右眼检测器
*/

// void detect(CvCapture * capture);
/*
检测有没有疲劳驾驶
capture为输入图像，从摄像头或者视屏中读入
*/

int detectframe(IplImage * frame, gsl_rng* rng, CvHaarClassifierCascade *facecascade, CvHaarClassifierCascade *lefteyecascade,
				 CvHaarClassifierCascade *righteyecascade, int * lefteyeclose, int * righteyeclose);

#endif
