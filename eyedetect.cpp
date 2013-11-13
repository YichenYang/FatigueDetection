#include "stdafx.h"
#include "eyedetect.h"
#include "highgui.h"
#include "threholdotsu.h"

CvMemStorage * storage = cvCreateMemStorage(0);

double scale = 5;

CvSeq* lefteyes, *righteyes;
CvSeq* faces;
int lefteyewidth = 0;
int righteyewidth = 0;
double ab;
double a, b;

CvPoint lefteyeposition, righteyeposition;
int lefteyeopen, righteyeopen;


void detectfaces( IplImage* frame, IplImage * smallgrayscale, CvHaarClassifierCascade *facecascade, 
				 CvHaarClassifierCascade *lefteyecascade, CvHaarClassifierCascade *righteyecascade, int IsDark)
{//use haar classifier to detect face and eye
	int i, j;
	IplImage * Face, *FaceGray;
	int centerx, centery;
	CvPoint point1, point2;
	CvRect rect, rect1, rect2;
	CvSize size;
	CvPoint lefteye, righteye;
	CvRect* r;
	CvRect* rr;
	int temp, tempnum;
	IplImage * grayframe;



	grayframe = cvCreateImage(cvGetSize(frame), frame->depth, 1);
	cvCvtColor(frame, grayframe, CV_BGR2GRAY);

	faces = cvHaarDetectObjects( smallgrayscale, facecascade, storage,1.2, 2, CV_HAAR_DO_CANNY_PRUNING,cvSize(30, 30) );
	for( i = 0; i < (faces ? faces->total : 0); i++ )
		faces = cvHaarDetectObjects( smallgrayscale, facecascade, storage,1.2, 2, CV_HAAR_DO_CANNY_PRUNING,cvSize(30, 30) );

	for( i = 0; i < (faces ? faces->total : 0); i++ )
	{//如果有人脸,找出人脸
		// if there is face, finde the face
		temp =0;

		r = (CvRect*)cvGetSeqElem( faces, i);
		if((r->width) * (r->height) > temp)
			tempnum = i;
		
	}
	if(faces->total)
	{//如果有人脸
		//if there is face
	
		r = (CvRect*)cvGetSeqElem(faces, tempnum);
		CvRect* rr;
		centerx = int(r->x + r->width / 2);
		centery = int(r->y + r->height / 2);
		point1.x = int((centerx - r->width * 0.4) * scale);
		point1.y = int((centery - r->height * 0.4) * scale);
		point2.x = int((centerx + r->width * 0.4) * scale);
		point2.y = int((centery + r->height * 0.4) * scale);

		//cvRectangle(frame, cvPoint(point1.x, point1.y), cvPoint(point2.x,point2.y), CV_RGB(255, 0, 0), 2, 8, 0);

	
		size.width = int(r->width * scale * 0.5);
		size.height = int(r->height * scale * 0.4);
		rect.x = int(r->x * scale + r->width * 0.5 * scale);
		rect.y = int(r->y * scale + r->height * 0.1 * scale);
		rect.width = size.width;
		rect.height = size.height;

		point1.x = rect.x;
		point1.y = rect.y;
		point2.x = rect.x+rect.width;
		point2.y = rect.y+rect.height;


		Face = cvCreateImage( size,8, 3 );
		FaceGray = cvCreateImage( size,8, 1 );
		GetImageRect(frame, rect, Face);
		//find the left upper part of the face

		cvCvtColor(Face, FaceGray, CV_BGR2GRAY);
		cvEqualizeHist( FaceGray, FaceGray );

		
		//detect the left eye
		lefteyes = cvHaarDetectObjects( FaceGray, lefteyecascade, storage,1.2, 2, CV_HAAR_DO_CANNY_PRUNING,cvSize(22, 22) );

		for( j = 0; j < (lefteyes ? lefteyes->total : 0); j++ )
		{
			temp = 0;
			rr = (CvRect*)cvGetSeqElem( lefteyes, j );
			if(rr->width > temp)
				tempnum = j;
		}

		if (lefteyes->total)
		{
			rr = (CvRect*)cvGetSeqElem( lefteyes, tempnum);

			lefteye.x = int(rr->x + rr->width / 6 * 5)+ rect.x ;
			lefteye.y = int(rr->y + rr->width * 2 / 3) + + rect.y ;

			rect2.x = int(lefteye.x - rr->width/3);
			rect2.y = int(lefteye.y - rr->width/6);
		
			if(rr->width > /*lefteyewidth * 2*/90)
				lefteyewidth = rr->width / 3;
			else
				lefteyewidth = 30;

			
			rect1.x = int(lefteye.x - rr->width/6*5);
			rect1.y = int(lefteye.y - rr->width/6);
			rect1.height = rr->height/3;
			rect1.width = rr->width;

			

			if(IsDark == 1)
			{
				lefteyeopen = GetEyePosition(frame, cvPoint(int(lefteye.x - rr->width/6*5), int(lefteye.y - rr->width/6)), 
					          cvPoint(int(lefteye.x + rr->width/6), int(lefteye.y + rr->width/6)), &lefteyeposition);
				lefteyeposition.x += rect1.x;
				lefteyeposition.y += rect1.y;

				if (lefteyeopen == 1)
				{
					a=1;		
				} 
				else
				{
					a=0;
				}
	
			}
			else
			{				
				lefteyeopen = GetEyePosition(frame, cvPoint(int(lefteye.x - rr->width/3), int(lefteye.y - rr->width/6)), 
					cvPoint(int(lefteye.x + rr->width/3), int(lefteye.y + rr->width/6)), &lefteyeposition);
				a =  howmucheye(frame,cvPoint(int(lefteye.x - rr->width/3), int(lefteye.y - rr->width/6)), 
					cvPoint(int(lefteye.x + rr->width/3), int(lefteye.y + rr->width/6)), &lefteyeposition);
				lefteyeposition.x += rect2.x;
				lefteyeposition.y += rect2.y;
			}
			
		//cvRectangle(frame, cvPoint(rr->x + rect.x, rr->y + rect.y), cvPoint(rr->x + rect.x + rr->width, rr->y + rect.y + rr->height), CV_RGB(255, 0, 0), 2, 8, 0);

			
		}
		rect.x = int(r->x * scale /*+ r->width * 0.1 * scale*/);

		point1.x = rect.x;
		point1.y = rect.y;
		point2.x = rect.x+rect.width;
		point2.y = rect.y+rect.height;


		GetImageRect(frame, rect, Face);
		//find the right upper part of face
		cvCvtColor(Face, FaceGray, CV_BGR2GRAY);
		cvEqualizeHist( FaceGray, FaceGray );


		
		//detect the right eye
		righteyes = cvHaarDetectObjects( FaceGray, righteyecascade, storage,1.2, 2, CV_HAAR_DO_CANNY_PRUNING,cvSize(22, 22) );

		for( j = 0; j < (righteyes ? righteyes->total : 0); j++ )
		{
			temp = 0;
			rr = (CvRect*)cvGetSeqElem( righteyes, j );
			if(rr->width > temp)
				tempnum = j;
		}

		if (righteyes->total)
		{
			rr = (CvRect*)cvGetSeqElem( righteyes, tempnum );

			righteye.x = int(size.width - rr->width / 6 * 5) + rect.x /*- 5*/;
			righteye.y = int(rr->y + rr->width * 2 / 3)  + rect.y /*+ 2*/;
			rect2.x = int(righteye.x - rr->width/3);
			rect2.y = int(righteye.y - rr->height/6);

			
			rect1.x = int(righteye.x - rr->width/2);
			rect1.y = int(righteye.y - rr->width/6);
			rect1.height = rr->height/3;
			rect1.width = rr->width;


			if(rr->width > /*righteyewidth * 2*/90)
				righteyewidth = rr->width / 3;
			else
				righteyewidth = 30;

			if(IsDark == 1)
			{
				righteyeopen = GetEyePosition(frame, cvPoint(int(righteye.x - rr->width/2), int(righteye.y - rr->height/6)),
					cvPoint(int(righteye.x + rr->width/3), int(righteye.y + rr->height/6)), &righteyeposition);
				righteyeposition.x += rect1.x;
				righteyeposition.y += rect1.y;

				if (righteyeopen == 1)
				{
					b=1;
				} 
				else
				{
					b=0;
				}
			}
			else
			{
				righteyeopen = GetEyePosition(frame, cvPoint(int(righteye.x - rr->width/3), int(righteye.y - rr->height/6)),
					cvPoint(int(righteye.x + rr->width/3), int(righteye.y + rr->height/6)), &righteyeposition);
				b =  howmucheye(frame,cvPoint(int(righteye.x - rr->width/3), int(righteye.y - rr->width/6)), 
					cvPoint(int(righteye.x + rr->width/3), int(righteye.y + rr->width/6)), &righteyeposition);
				righteyeposition.x += rect2.x;
				righteyeposition.y += rect2.y;
			}

		
		
			
			//cvRectangle(frame, cvPoint(rr->x + rect.x, rr->y + rect.y), cvPoint(rr->x + rect.x + rr->width, rr->y + rect.y + rr->height), CV_RGB(255, 0, 0), 2, 8, 0);

			cvReleaseImage(&grayframe);

		}
		cvReleaseImage(&Face);
		cvReleaseImage(&FaceGray);
		ab = a/2 +b/2;
	
	}
	
}
