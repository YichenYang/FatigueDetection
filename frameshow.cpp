#include "frameshow.h"







/********************************** Globals **********************************/

//eyetrack.cpp中的变量
extern int num_particles;    /* number of particles */
extern int exported;               /* TRUE to exported tracking sequence */



//eyedetect.cpp variables
extern CvMemStorage * storage;
extern double scale;
extern CvSeq* lefteyes, *righteyes;
extern CvSeq* faces;
extern int lefteyewidth, righteyewidth;
extern CvPoint lefteyeposition, righteyeposition;
extern int lefteyeopen, righteyeopen;
extern double ab,a,b;




int trackwidth/* = 25*/;//eyes(the width of detect object)
int trackheight/* = 25*/;//eyes(the height of detect object)

IplImage * hsv_frame,*lasthsv_frame;
CvRect regions;
int  num_objects = 0;
int i=0;
int j;//
int  trackframenum = 0;
int  maxtrackframenum = 50;

IplImage * lastframe;
int lefteyealarm, righteyealarm;//the count of left eye and right eye closed time

int lefteyeclosenum = 0;//the number of left eye closed time
int righteyeclosenum = 0;//the number of right eye closed time
int closenum = 0;//the number of close eye time
double fatigue = 0;//whether it is fatigue



IplImage *frame2;//use as current frame
IplImage* smallframe;//smaller color image
IplImage *smallgrayscale;//smaller gray image 


// i = 0;// current frame

int IsDark=0;
int addwidth;
double  width_s,width_l,shownum;


int detectframe(IplImage * frame, gsl_rng* rng, CvHaarClassifierCascade *facecascade, CvHaarClassifierCascade *lefteyecascade,
				 CvHaarClassifierCascade *righteyecascade, int * lefteyeclose, int * righteyeclose)
{
	int eyedetection = 1;
	char c, d;
	int red ;
	int percentage ;
	char szNum[10] = {0};
	CvFont font;

	IsDark = 1;//  DetectIsDark(frame);
	if(IsDark == 1)
		addwidth = 0;
	else
		addwidth = 0;

	i++;

	

	lefteyeclosenum = 0;
	righteyeclosenum = 0;
	lefteyealarm = 0;
	righteyealarm = 0;

	lastframe = cvCreateImage(cvSize( cvRound (frame->width/scale),cvRound (frame->height/scale)), 8, 3);
	frame2 = cvCreateImage(cvSize( cvRound (frame->width/scale),cvRound (frame->height/scale)), 8, 3);

	smallgrayscale = cvCreateImage(cvSize( cvRound (frame->width/scale),cvRound (frame->height/scale)), 8, 1);
	if( !frame ) 
		eyedetection = 0;

	smallframe = cvCreateImage( cvSize( cvRound (frame->width/scale),cvRound (frame->height/scale)),8, 3 );

	cvResize(frame, smallframe, CV_INTER_LINEAR);
	cvCopy(smallframe, frame2);// copy the current frame

	cvCvtColor(smallframe, smallgrayscale, CV_BGR2GRAY);
	// 		cvEqualizeHist(smallgrayscale, smallgrayscale);



	detectfaces(frame,smallgrayscale, facecascade, lefteyecascade, righteyecascade, IsDark);//人脸检测 face detection


	if (faces->total)
	{
		trackframenum = 0;
	}

	if(trackframenum >= maxtrackframenum)
	{
		cvShowImage( "camera", frame);// display the image

		c = cvWaitKey(1);

		if( c == 27 ) 
		{
			eyedetection = 0;
		}

		cvReleaseImage(&lastframe);
		cvReleaseImage(&frame2);
		cvReleaseImage(&smallframe);
		cvReleaseImage(&smallgrayscale);
		
	}


	if(( (faces->total == 0) || ( (faces->total != 0) && ((lefteyes->total == 0) && (righteyes->total == 0)) ) )&& (i > 20) && (trackframenum < maxtrackframenum))
	{//如果人脸检测结果没有发现人脸，或者发现人脸，但没有发现任何一只眼睛，跟踪人眼
		//if the face detect cannot find the eye, or find the eye but no eye, than tract the eye
		
		trackframenum ++;
		lasthsv_frame = bgr2hsv(lastframe);
		hsv_frame = bgr2hsv(frame2);
	

		// left eye
		// 			trackwidth = lefteyewidth / 2;
		// 			trackheight = lefteyewidth / 2;

		trackwidth = lefteyewidth / 2 + addwidth;
		trackheight = lefteyewidth / 2 + addwidth;

		regions.x = cvRound((lefteyeposition.x-trackwidth) / scale);
		regions.y = cvRound((lefteyeposition.y-trackheight) / scale);
		regions.width = cvRound(trackwidth/ scale) * 2 + 1;
		regions.height = cvRound(trackheight/ scale) * 2 + 1;

		

		if((regions.x >= 0) && (regions.y >= 0) && ((regions.x + regions.width) < smallframe->width) && ((regions.y + regions.height) < smallframe->height))
		{
			// 				trackframenum ++;
		
			lefteyeopen = eyetrack(num_objects, hsv_frame, lasthsv_frame, regions, rng, &lefteyeposition, frame, IsDark);
			
			
			if(lefteyeopen == 1)
			{
				if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
					((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
				{
					lefteyeclose[i%100] = 1;//1 means open eye

				}
				else
					lefteyeclose[i%100] = -1;//-1 means error
			}
			else
			{
				if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
					((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
				{
					lefteyeclose[i%100] = 0;//0 means closed eye

				}
				else
					lefteyeclose[i%100] = -1;//-1 means error
			}

			if (IsDark == 0)
			{
				a = eyetrackmuch(num_objects,hsv_frame,lasthsv_frame,regions,rng,&lefteyeposition,frame,IsDark);
			}
			else
			{
				if (lefteyeopen == 1)
				{
					a=1;
				} 
				else
				{
					a=0;
				}
			}
			//cvRectangle(frame, cvPoint(lefteyeposition.x - lefteyewidth, lefteyeposition.y - lefteyewidth), cvPoint(lefteyeposition.x + lefteyewidth, lefteyeposition.y + lefteyewidth), CV_RGB(255, 0, 0), 2, 8, 0);
			


		}

		// right eye
		// 			trackwidth = righteyewidth / 2;
		// 			trackheight = righteyewidth / 2;
		trackwidth = righteyewidth / 2 + addwidth;
		trackheight = righteyewidth / 2 + addwidth;

		regions.x = cvRound((righteyeposition.x-trackwidth) / scale);
		regions.y = cvRound((righteyeposition.y-trackheight) / scale);
		regions.width = cvRound(trackwidth/ scale) * 2 + 1;
		regions.height = cvRound(trackheight/ scale) * 2 + 1;
		if((regions.x >= 0) && (regions.y >= 0) && (regions.x + regions.width < smallframe->width) && (regions.y + regions.height < smallframe->height))
		{

			righteyeopen = eyetrack(num_objects, hsv_frame, lasthsv_frame, regions, rng, &righteyeposition, frame, IsDark);

 			if(righteyeopen == 1)
 			{
 				if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
					((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
				{
					righteyeclose[i%100] = 1;

				}
				else
					righteyeclose[i%100] = -1;
			}
			else
			{
				if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
					((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
				{
					righteyeclose[i%100] = 0;

				}
				else
					righteyeclose[i%100] = -1;
			}
			if (IsDark == 0)
			{
				b = eyetrackmuch(num_objects,hsv_frame,lasthsv_frame,regions,rng,&righteyeposition,frame,IsDark);
			}
			else
			{
				if (righteyeopen == 1)
				{
					b=1;
				} 
				else
				{
					b=0;
				}
			}
			//cvRectangle(frame, cvPoint(righteyeposition.x - righteyewidth, righteyeposition.y - righteyewidth), cvPoint(righteyeposition.x + righteyewidth, righteyeposition.y + righteyewidth), CV_RGB(255, 0, 0), 2, 8, 0);
			

			
		
		}
		ab = a/2 + b/2;
		cvReleaseImage( &hsv_frame );
		cvReleaseImage(&lasthsv_frame);
	}

	else if(i > 20)// find the face and at least one eye
	{
		trackframenum = 0;
		if(lefteyes->total)//change the count of closed eyes
		{
			if(lefteyeopen)
				lefteyeclose[i%100] = 1;
			else
				lefteyeclose[i%100] = 0;
		}

		if(righteyes->total)
		{
			if(righteyeopen)
				righteyeclose[i%100] = 1;
			else
				righteyeclose[i%100] = 0;
		}

		
		//found the face, but didn't detect the left eye
		if ((lefteyes->total == 0) && (trackframenum < maxtrackframenum))
		{
			

			trackwidth = lefteyewidth / 2 + addwidth;
			trackheight = lefteyewidth / 2 + addwidth;
			regions.x = cvRound((lefteyeposition.x-trackwidth) / scale);
			regions.y = cvRound((lefteyeposition.y-trackheight) / scale);
			regions.width = cvRound(trackwidth/ scale) * 2 + 1;
			regions.height = cvRound(trackheight/ scale) * 2 + 1;
			if((regions.x >= 0) && (regions.y >= 0) && ((regions.x + regions.width) < smallframe->width) && ((regions.y + regions.height) < smallframe->height))
			{
				lasthsv_frame = bgr2hsv(lastframe);
				hsv_frame = bgr2hsv(frame2);
				lefteyeopen = eyetrack(num_objects, hsv_frame, lasthsv_frame, regions, rng, &lefteyeposition, frame, IsDark);

 				if(lefteyeopen == 1)
 				{
 					if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
 						((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
 					{
 						lefteyeclose[i%100] = 1;
 
 					}
 					else
 						lefteyeclose[i%100] = -1;
 				}
 				else
 				{
 					if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
 						((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
 					{
 						lefteyeclose[i%100] = 0;
 
 					}
 					else
 						lefteyeclose[i%100] = -1;
				}
				if (IsDark == 0)
				{
					a = eyetrackmuch(num_objects,hsv_frame,lasthsv_frame,regions,rng,&lefteyeposition,frame,IsDark);
				}
				else
				{
					if (lefteyeopen == 1)
					{
						a=1;
					} 
					else
					{
						a=0;
					}
				}
				
				cvReleaseImage( &hsv_frame );
				cvReleaseImage(&lasthsv_frame);
			}	
			ab=a/2 +b/2;
		}
		
		//if it didn't detect the right eye
		if ((righteyes->total == 0) && (trackframenum < maxtrackframenum))
		{



			trackwidth = righteyewidth / 2 + addwidth;
			trackheight = righteyewidth / 2 + addwidth;
			regions.x = cvRound((righteyeposition.x-trackwidth) / scale);
			regions.y = cvRound((righteyeposition.y-trackheight) / scale);
			regions.width = cvRound(trackwidth/ scale) * 2 + 1;
			regions.height = cvRound(trackheight/ scale) * 2 + 1;			
			if((regions.x >= 0) && (regions.y >= 0) && (regions.x + regions.width < smallframe->width) && (regions.y + regions.height < smallframe->height))
			{
				lasthsv_frame = bgr2hsv(lastframe);
				hsv_frame = bgr2hsv(frame2);
				righteyeopen = eyetrack(num_objects, hsv_frame, lasthsv_frame, regions, rng, &righteyeposition, frame, IsDark);

				if(righteyeopen == 1)
				{
					if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
						((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
					{
						righteyeclose[i%100] = 1;

					}
					else
						righteyeclose[i%100] = -1;
				}
				else
				{
					if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
						((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
					{
						righteyeclose[i%100] = 0;

					}
					else
						righteyeclose[i%100] = -1;
				}
				if (IsDark == 0)
				{
					b = eyetrackmuch(num_objects,hsv_frame,lasthsv_frame,regions,rng,&righteyeposition,frame,IsDark);
				}
				else
				{
					if (righteyeopen == 1)
					{
						b=1;
					} 
					else
					{
						b=0;
					}
				}
			
				cvReleaseImage( &hsv_frame );
				cvReleaseImage(&lasthsv_frame);
			}
			ab = a/2 + b/2;
		}
	}


	
 	if (i<5)
	{
		width_s = ab;
		width_l = ab;
		shownum = 1;
	} 
	else
 	{
		if (width_s<ab)
		{
			if (width_l<ab)
			{
				width_l = ab;
				shownum = (ab - width_s)/(width_l - width_s); 
			} 
			else
			{
				shownum = (ab - width_s)/(width_l - width_s);
			}
		} 
		else
		{
			width_s = ab;
			shownum = (ab - width_s)/(width_l - width_s);
		}
	}
	

	percentage = int(shownum*100);
	

	if (IsDark == 0 )
	{
		sprintf_s(szNum, "%d", percentage );	    
	    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 2.f, 2.f, 0.0, 7, CV_AA);
	    cvPutText(frame, szNum, cvPoint(50, 50), &font,CV_RGB(255, 0, 0));
	}
	
	if (shownum < 0.2)
	{
		closenum += 1;

	}

	fatigue =closenum/i; 

    red = int(shownum*255);
	cvCopy(frame2, lastframe);

	if(trackframenum < maxtrackframenum)
	{

		
		//mark the position of eye
		if (IsDark == 0)
		{
			if((lefteyeopen == 1) ||( righteyeopen == 1))
			{
				if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
				((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
				{
				    cvLine(frame, cvPoint(lefteyeposition.x - lefteyewidth, lefteyeposition.y), cvPoint(lefteyeposition.x + lefteyewidth, lefteyeposition.y), CV_RGB(red, 0, 0), 2, 8, 0);
				    cvLine(frame, cvPoint(lefteyeposition.x, lefteyeposition.y - lefteyewidth / 2), cvPoint(lefteyeposition.x, lefteyeposition.y + lefteyewidth / 2), CV_RGB(red, 0, 0), 2, 8, 0);
			    }

			    if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
				((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
		    	{
			    	cvLine(frame, cvPoint(righteyeposition.x - righteyewidth, righteyeposition.y), cvPoint(righteyeposition.x + righteyewidth, righteyeposition.y), CV_RGB(red, 0, 0), 2, 8, 0);
				    cvLine(frame, cvPoint(righteyeposition.x, righteyeposition.y - righteyewidth / 2), cvPoint(righteyeposition.x, righteyeposition.y + righteyewidth / 2), CV_RGB(red, 0, 0), 2, 8, 0);
			    }
		    }
		    else
		    {
			    if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
				((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
			    {
				    cvLine(frame, cvPoint(lefteyeposition.x - lefteyewidth, lefteyeposition.y), cvPoint(lefteyeposition.x + lefteyewidth, lefteyeposition.y), CV_RGB(0, 0, 0), 2, 8, 0);
				    cvLine(frame, cvPoint(lefteyeposition.x, lefteyeposition.y - lefteyewidth / 2), cvPoint(lefteyeposition.x, lefteyeposition.y + lefteyewidth / 2), CV_RGB(0, 0, 0), 2, 8, 0);
			    }
		    	if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
				((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
			    {
				    cvLine(frame, cvPoint(righteyeposition.x - righteyewidth, righteyeposition.y), cvPoint(righteyeposition.x + righteyewidth, righteyeposition.y), CV_RGB(0, 0, 0), 2, 8, 0);
				    cvLine(frame, cvPoint(righteyeposition.x, righteyeposition.y - righteyewidth / 2), cvPoint(righteyeposition.x, righteyeposition.y + righteyewidth / 2), CV_RGB(0, 0, 0), 2, 8, 0);
			    }
		    }
		} 
		else
		{
			if((lefteyeopen == 1) && (righteyeopen == 1))
			{
				if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
					((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
				{
					cvLine(frame, cvPoint(lefteyeposition.x - lefteyewidth, lefteyeposition.y), cvPoint(lefteyeposition.x + lefteyewidth, lefteyeposition.y), CV_RGB(0, 255, 0), 2, 8, 0);
					cvLine(frame, cvPoint(lefteyeposition.x, lefteyeposition.y - lefteyewidth / 2), cvPoint(lefteyeposition.x, lefteyeposition.y + lefteyewidth / 2), CV_RGB(0, 255, 0), 2, 8, 0);
				}

				if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
					((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
				{
					cvLine(frame, cvPoint(righteyeposition.x - righteyewidth, righteyeposition.y), cvPoint(righteyeposition.x + righteyewidth, righteyeposition.y), CV_RGB(0, 255, 0), 2, 8, 0);
					cvLine(frame, cvPoint(righteyeposition.x, righteyeposition.y - righteyewidth / 2), cvPoint(righteyeposition.x, righteyeposition.y + righteyewidth / 2), CV_RGB(0, 255, 0), 2, 8, 0);
				}
			}
			else
			{
				if(((lefteyeposition.x - lefteyewidth) > 0) && ((lefteyeposition.y - lefteyewidth / 2)> 0) && 
					((lefteyeposition.x + lefteyewidth) < frame->width) && ((lefteyeposition.y + lefteyewidth / 2) < frame->height))
				{
					cvLine(frame, cvPoint(lefteyeposition.x - lefteyewidth, lefteyeposition.y), cvPoint(lefteyeposition.x + lefteyewidth, lefteyeposition.y), CV_RGB(255, 0, 0), 2, 8, 0);
					cvLine(frame, cvPoint(lefteyeposition.x, lefteyeposition.y - lefteyewidth / 2), cvPoint(lefteyeposition.x, lefteyeposition.y + lefteyewidth / 2), CV_RGB(255, 0, 0), 2, 8, 0);
				}
				if(((righteyeposition.x - righteyewidth) > 0) && ((righteyeposition.y - righteyewidth / 2)> 0) && 
					((righteyeposition.x + righteyewidth) < frame->width) && ((righteyeposition.y + righteyewidth / 2) < frame->height))
				{
					cvLine(frame, cvPoint(righteyeposition.x - righteyewidth, righteyeposition.y), cvPoint(righteyeposition.x + righteyewidth, righteyeposition.y), CV_RGB(255, 0, 0), 2, 8, 0);
					cvLine(frame, cvPoint(righteyeposition.x, righteyeposition.y - righteyewidth / 2), cvPoint(righteyeposition.x, righteyeposition.y + righteyewidth / 2), CV_RGB(255, 0, 0), 2, 8, 0);
				}
			}

		}
		
 		
		//P80 method, compute the percentage of frames that the eye is closed to 20% in the recent 100 frames
 		if (fatigue>0.15)
 		{
 			alarm(frame);
 		}
 
 
 		
		//compute the times of closed eyes in the recent 100 frames
 		for (j = 0; j < 100; j ++)
 		{
 			lefteyeclosenum += lefteyeclose[j];
 			righteyeclosenum += righteyeclose[j];
 		}
 		
		//in the recent 100 frames has more than 35 frames of closed eyes, give alarm
 		if(i % 100 == 0)
 		{
 			if((lefteyeclosenum >= 25) || (righteyeclosenum >= 25))
 			{
 				alarm(frame);
 			}
 		}
 
 
 		
		//if the recent 10 frames has more than half frames of closed eyes, give alarm
 		for(j = 0; j < 10; j ++)
 		{
 			if(lefteyeclose[(i - j)%100] == 0)
 				lefteyealarm += 1;
 			if(righteyeclose[(i - j)%100] == 0)
 				righteyealarm += 1;
 		}
 		if((lefteyealarm >= 5) && (righteyealarm >= 5))
 		{
 			alarm(frame);
 		}
 
 		if((lefteyealarm >= 9) || (righteyealarm >= 9))
 		{
 			alarm(frame);
 		}
 
 	}
	cvShowImage( "camera", frame);//display the image

	d = cvWaitKey(1);
	if( d == 27 ) 
	{
		eyedetection = 0;
	}

	cvReleaseImage(&lastframe);
	cvReleaseImage(&frame2);
	cvReleaseImage(&smallframe);
	cvReleaseImage(&smallgrayscale);
	return eyedetection;
}