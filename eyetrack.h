#ifndef EYETRACK_H
#define EYETRACK_H

#include "stdafx.h"
#include "cv.h"
// #include "highgui.h"
#include "cxcore.h"
#include <time.h>
#include <math.h>
#include <iostream>
using namespace std;
#include "eye.h"


extern "C"
{
#include "defs.h"
#include "utils.h"
#include "particles.h"
#include "observation.h"
#include "track.h"
};

// #include "gsl_rng.h"
// #include "gsl_randist.h"

//---gsl的库文件-----------
//#pragma comment (lib, "libgsl.a")

/* command line options */
#define OPTIONS ":p:oah"

/* default number of particles */
#define PARTICLES 100

/* default basename and extension of exported frames */
#define EXPORT_BASE "./frames/frame_"
#define EXPORT_EXTN ".png"

/* maximum number of frames for exporting */
#define MAX_FRAMES 2048


/* maximum number of objects to be tracked */
#define MAX_OBJECTS 1



int eyetrack(int num_objects, IplImage * hsv_frame, IplImage *lasthsv_frame, CvRect regions, gsl_rng* rng, CvPoint * eyeposition, IplImage * frame, int IsDark);
/*
粒子滤波算法跟踪眼睛
num_objects为目标数目
hsv_frame为当前帧
lasthsv_frame为上一帧
regions为跟踪目标范围
eyeposition为眼睛位置
frame为当前帧
*/

double eyetrackmuch(int num_objects, IplImage * hsv_frame, IplImage *lasthsv_frame, CvRect regions, gsl_rng* rng, CvPoint * eyeposition, IplImage * frame, int IsDark);
/*
同粒子滤波跟踪
返回眼睛中心像素值

*/
#endif