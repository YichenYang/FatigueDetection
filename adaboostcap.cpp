// adaboost.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "observation.h"

#include "frameshow.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>

extern CvMemStorage * storage;

using namespace std;


int _tmain(/*int argc, _TCHAR* argv[]*/)
{
    IplImage * frame,* lastframe;
	int t1;
	int k;
	gsl_rng * rng;
	double scale;
	CvSeq* lefteyes, *righteyes;
	CvSeq* faces;
	CvCapture *capture;
	int lefteyeclose[100], righteyeclose[100];//the recent 100 frame of left eye and right eye situation
	int lefteyeclosep[100], righteyeclosep[100];
	
	
	gsl_rng_env_setup();
	rng = gsl_rng_alloc( gsl_rng_mt19937 );
	gsl_rng_set( rng, time(NULL) );


	cvNamedWindow( "camera", CV_WINDOW_AUTOSIZE );// output the image



	CvHaarClassifierCascade *facecascade;
	CvHaarClassifierCascade *lefteyecascade;
	CvHaarClassifierCascade *righteyecascade;

	facecascade = cvLoadHaarClassifierCascade("haarcascade_frontalface_alt2.xml", cvSize(30,30));
	lefteyecascade = cvLoadHaarClassifierCascade("haarcascade_lefteye_2splits.xml",cvSize(12,12));
	righteyecascade = cvLoadHaarClassifierCascade("haarcascade_righteye_2splits.xml",cvSize(12,12));


	cvClearMemStorage( storage );
	scale = 5;




	//use the camera of the computer
	capture = cvCreateCameraCapture( 0 );
	//use the video in the computer
 	// capture = cvCreateFileCapture("c:\\学习\\毕业设计\\demo6.avi"/*"e:\\test3.avi"*/);




	for (k = 0; k < 100; k ++)
	{
		lefteyeclose[k] = 1;
		righteyeclose[k] = 1;
	}

	
	for ( k = 0; k < 100; k++)
	{
		lefteyeclosep[k]=1;
		righteyeclosep[k]=1;
	}

	

	while(frame = cvQueryFrame( capture ))
	{
		t1 = detectframe(frame, rng, facecascade, lefteyecascade, righteyecascade, lefteyeclose, righteyeclose);
		if(t1 == 0)
			break;
	}




	cvDestroyWindow( "camera" );

	cvReleaseCapture( &capture );
	return 0;
}

