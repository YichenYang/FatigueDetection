#include"eye.h"


extern CvMemStorage * storage;


int IsEyeOpen(IplImage * eyes)
{//Determine whether the eye is open
    int eyeopen;
	IplImage *temp;
	int thre;
	int minx, maxx, miny, maxy;
	double avg, avg2;
	int i, j;
	int rightpointy, leftpointy;


	rightpointy = 0;
	leftpointy = 0;
	temp = cvCreateImage(cvGetSize(eyes), eyes->depth, eyes->nChannels);
	cvEqualizeHist(eyes, eyes);

	thre = ComputeValueOTSU(eyes);
	cvThreshold(eyes, temp, thre - 100, 255, CV_THRESH_BINARY_INV);




	cvErode(temp, temp);
	cvDilate(temp, temp);
	cvDilate(temp, temp, NULL, 3);
	cvErode(temp, temp, NULL, 3);
	minx = eyes->width;
	miny = eyes->height;
	maxx = 0;
	maxy = 0;
	avg = 0;
	avg2 = 0;
	for (i = 0; i < eyes->width; i ++)
	{
		for (j = 0; j < eyes->height; j ++)
		{
			if (CV_IMAGE_ELEM(temp, uchar, j, i))
			{
				if(i < minx)
				{
					minx = i;
					leftpointy = j;
				}
				if(i > maxx)
				{
					maxx = i;
					rightpointy = j;
				}
				if(j < miny)
					miny = j;
				if(j > maxy)
					maxy = j;
				if((i < 30) || (i > 50))
					avg += (255 - CV_IMAGE_ELEM(eyes, uchar, j, i));
			}
		}
	}

	if((abs((maxx - minx) - (maxy - miny)) <= 15) && ((maxx - minx) < 40))
		return true;
	else
	{
		// 		avg /= (maxx - minx);
		if((minx < 30) && (maxx > 50))
			avg /= (maxx - minx - 20);
		if((minx >= 30) && (maxx > 50))
			avg /= (maxx - 50);
		if((minx < 30) && (maxx <= 50))
			avg /= (30 - minx);
		if((minx > 50) || (maxx < 30))
			avg /= (maxx - minx);
		if((minx >= 30) && (maxx <= 50))
			avg = 1;

		for(j = miny; j < maxy; j ++)
		{
			if(CV_IMAGE_ELEM(temp, uchar, j, 40))
			{
				avg2 += (255 - CV_IMAGE_ELEM(eyes, uchar, j, 40));
			}
		}
		if(avg2 * 1.0 / avg > 1.75)
		{
			eyeopen = 1;
		}
		else
		{
			eyeopen = 0;
		}
	}
	return eyeopen;
	cvReleaseImage(&temp);
}

/*double width_l=0, width_s=0;*/

double howmucheye(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition )
{//find how much is the eye open based on the height of the eyeball
	double avg2 = 0;
	CvRect rect, rect2;
	IplImage *eyes, *grayeyes;
	double m1, m2;
	int radium = 5;
	int i,j,xx,yy;
	IplImage *eye2, *grayeye2;
	IplImage *temp;
	int thre;
	int minx, maxx, miny, maxy;
	
	int rightpointy = 0, leftpointy = 0;



	if((point1.x < 0) || (point1.y < 0) || (point1.x >= faces->width) || (point1.y >= faces->height) ||
		(point2.x < 0) || (point2.y < 0) || (point2.x >= faces->width) || (point2.y >= faces->height))
	{
		eyeposition->x = 0;
		eyeposition->y = 0;
	}

	else
	{
		
		

		rect.x = point1.x;
		rect.y = point1.y;
		rect.width = point2.x - point1.x;
		rect.height = point2.y - point1.y;
		eyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
		grayeyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
		GetImageRect(faces, rect, eyes);
		cvCvtColor(eyes, grayeyes, CV_BGR2GRAY);
		m2 = 256 * 256;

		//find the position of pupil
		for (i = radium; i < grayeyes->width - radium; i ++)
		{
			for (j = radium; j < grayeyes->height - radium; j ++)
			{
				m1 = 0;
				for(xx = -radium; xx <= radium; xx ++)
				{
					for(yy = -radium; yy <= radium; yy ++)
					{
						if( xx * xx + yy * yy <= radium * radium)
							m1 += CV_IMAGE_ELEM(grayeyes, uchar, (j + yy), (i + xx));
					}
				}

				if(m1 < m2)
				{
					m2 = m1;
					eyeposition->x = i;
					eyeposition->y = j;
				}
			}
		}

		cvReleaseImage(&eyes);
		cvReleaseImage(&grayeyes);

		rect2.x = point1.x + eyeposition->x - 40;
		rect2.y = point1.y + eyeposition->y - 20;
		rect2.width = 81;
		rect2.height = 41;
		if((rect2.x > 0) && (rect2.y > 0) && ((rect2.x + rect2.width) < faces->width) && ((rect2.y + rect2.height) < faces->height))
		{
			
			eye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 3);
			grayeye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 1);
			GetImageRect(faces, rect2, eye2);
			cvCvtColor(eye2, grayeye2, CV_BGR2GRAY);
			cvEqualizeHist(grayeye2, grayeye2);

			

			temp = cvCreateImage(cvGetSize(grayeye2), 8,1);
			thre = ComputeValueOTSU(grayeye2);
			cvThreshold(grayeye2, temp, thre - 100, 255, CV_THRESH_BINARY_INV);
			cvErode(temp, temp);
			cvDilate(temp, temp);
			cvDilate(temp, temp, NULL, 3);
			cvErode(temp, temp, NULL, 3);
			minx = grayeye2->width;
			miny = grayeye2->height;
			maxx = 0;
			maxy = 0;

			for (i = 0; i < grayeye2->width; i ++)
			{
				for (j = 0; j < grayeye2->height; j ++)
				{
					if (CV_IMAGE_ELEM(temp, uchar, j, i))
					{
						if(i < minx)
						{
							minx = i;
							leftpointy = j;
						}
						if(i > maxx)
						{
							maxx = i;
							rightpointy = j;
						}
						if(j < miny)
							miny = j;
						if(j > maxy)
							maxy = j;
					}
				}
			}
			for(j = miny; j < maxy; j ++)
			{
				if(CV_IMAGE_ELEM(temp, uchar, j, 40))
				{
					avg2 += (255 - CV_IMAGE_ELEM(grayeye2, uchar, j, 40));
				}
			}
		}

		else
		{
			eyeposition->x = 0;
			eyeposition->y = 0;
			
		}
	}	
	
	return avg2;
}


void GetImageRect(IplImage* orgImage, CvRect rectInImage, IplImage* imgRect)
{//get the imgRect(recInImage) which is part of the orgImage 

	IplImage *result=imgRect;
	CvSize size;
	size.width= rectInImage.width;
	size.height= rectInImage.height;
	//result=cvCreateImage( size, orgImage->depth, orgImage->nChannels );
	//get the imgRect from orgImage
	cvSetImageROI(orgImage,rectInImage);
	cvCopy(orgImage,result);
	cvResetImageROI(orgImage);
}


int GetEyePosition(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition)
{//get the position of the eye
	int eyeopen;
	CvRect rect, rect2;
	IplImage *eyes, *grayeyes;
	double m1, m2;
	int radium = 5;
	int i,j,xx,yy;
	IplImage *eye2, *grayeye2;

	if((point1.x < 0) || (point1.y < 0) || (point1.x >= faces->width) || (point1.y >= faces->height) ||
		(point2.x < 0) || (point2.y < 0) || (point2.x >= faces->width) || (point2.y >= faces->height))
	{
		eyeposition->x = 0;
		eyeposition->y = 0;
		eyeopen = 1;
	}

	else
	{
		
		rect.x = point1.x;
		rect.y = point1.y;
		rect.width = point2.x - point1.x;
		rect.height = point2.y - point1.y;
		eyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
		grayeyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
		GetImageRect(faces, rect, eyes);
		cvCvtColor(eyes, grayeyes, CV_BGR2GRAY);
		m2 = 256 * 256;
// 		cvMinMaxLoc(grayeyes, &m1, &m2, eyeposition);

		//find the position
		for (i = radium; i < grayeyes->width - radium; i ++)
		{
			for (j = radium; j < grayeyes->height - radium; j ++)
			{
// 				m1 = CV_IMAGE_ELEM(grayeyes, uchar, j - 1, i) + CV_IMAGE_ELEM(grayeyes, uchar, j, i - 1) + CV_IMAGE_ELEM(grayeyes, uchar, j, i) + 
// 					CV_IMAGE_ELEM(grayeyes, uchar, j, i + 1) + CV_IMAGE_ELEM(grayeyes, uchar, j + 1, i) + CV_IMAGE_ELEM(grayeyes, uchar, j - 2, i)
// 					+ CV_IMAGE_ELEM(grayeyes, uchar, j + 2, i) + CV_IMAGE_ELEM(grayeyes, uchar, j, i + 2) + CV_IMAGE_ELEM(grayeyes, uchar, j, i - 2);

				m1 = 0;
				for(xx = -radium; xx <= radium; xx ++)
				{
					for( yy = -radium; yy <= radium; yy ++)
					{
						if( xx * xx + yy * yy <= radium * radium)
							m1 += CV_IMAGE_ELEM(grayeyes, uchar, (j + yy), (i + xx));
					}
				}

				if(m1 < m2)
				{
					m2 = m1;
					eyeposition->x = i;
					eyeposition->y = j;
				}
			}
		}
		
		cvReleaseImage(&eyes);
		cvReleaseImage(&grayeyes);

		
		rect2.x = point1.x + eyeposition->x - 40;
		rect2.y = point1.y + eyeposition->y - 20;
		rect2.width = 81;
		rect2.height = 41;
		if((rect2.x > 0) && (rect2.y > 0) && ((rect2.x + rect2.width) < faces->width) && ((rect2.y + rect2.height) < faces->height))
		{
			
			eye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 3);
			grayeye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 1);
			GetImageRect(faces, rect2, eye2);
			cvCvtColor(eye2, grayeye2, CV_BGR2GRAY);
			//determine whether it is closed
			eyeopen = IsEyeOpen(grayeye2);

			cvReleaseImage(&eye2);
			cvReleaseImage(&grayeye2);
			return eyeopen;
		}

		else
		{
			eyeposition->x = 0;
			eyeposition->y = 0;
			eyeopen = 1;
		}
	}
	return eyeopen;
}


int GetEyePositionwhite(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition)
{//get the position of the eye
	int eyeopen;
	CvRect rect, rect2;
	IplImage *eyes, *grayeyes;
	double m1, m2;
	int radium = 5;
	int i,j,xx,yy;
	IplImage *eye2, *grayeye2;
	int minx, miny, maxx, maxy;
	IplImage *temp;
	int whitepart = 0;

	if((point1.x < 0) || (point1.y < 0) || (point1.x >= faces->width) || (point1.y >= faces->height) ||
		(point2.x < 0) || (point2.y < 0) || (point2.x >= faces->width) || (point2.y >= faces->height))
	{
		eyeposition->x = 0;
		eyeposition->y = 0;
		eyeopen = 1;
	}

	else
	{
		
		rect.x = point1.x;
		rect.y = point1.y;
		rect.width = point2.x - point1.x;
		rect.height = point2.y - point1.y;
		eyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
		grayeyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 1);
		GetImageRect(faces, rect, eyes);
		cvCvtColor(eyes, grayeyes, CV_BGR2GRAY);
		m2 = 256 * 256;
// 		cvMinMaxLoc(grayeyes, &m1, &m2, eyeposition);

		//find the position
		for (i = radium; i < grayeyes->width - radium; i ++)
		{
			for (j = radium; j < grayeyes->height - radium; j ++)
			{
// 				m1 = CV_IMAGE_ELEM(grayeyes, uchar, j - 1, i) + CV_IMAGE_ELEM(grayeyes, uchar, j, i - 1) + CV_IMAGE_ELEM(grayeyes, uchar, j, i) + 
// 					CV_IMAGE_ELEM(grayeyes, uchar, j, i + 1) + CV_IMAGE_ELEM(grayeyes, uchar, j + 1, i) + CV_IMAGE_ELEM(grayeyes, uchar, j - 2, i)
// 					+ CV_IMAGE_ELEM(grayeyes, uchar, j + 2, i) + CV_IMAGE_ELEM(grayeyes, uchar, j, i + 2) + CV_IMAGE_ELEM(grayeyes, uchar, j, i - 2);

				m1 = 0;
				for(xx = -radium; xx <= radium; xx ++)
				{
					for( yy = -radium; yy <= radium; yy ++)
					{
						if( xx * xx + yy * yy <= radium * radium)
							m1 += CV_IMAGE_ELEM(grayeyes, uchar, (j + yy), (i + xx));
					}
				}

				if(m1 < m2)
				{
					m2 = m1;
					eyeposition->x = i;
					eyeposition->y = j;
				}
			}
		}
		
		cvReleaseImage(&eyes);
		cvReleaseImage(&grayeyes);

		
		rect2.x = point1.x + eyeposition->x - 40;
		rect2.y = point1.y + eyeposition->y - 20;
		rect2.width = 81;
		rect2.height = 41;
		if((rect2.x > 0) && (rect2.y > 0) && ((rect2.x + rect2.width) < faces->width) && ((rect2.y + rect2.height) < faces->height))
		{
			
			eye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 3);
			grayeye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 1);
			GetImageRect(faces, rect2, eye2);
			cvCvtColor(eye2, grayeye2, CV_BGR2GRAY);
			//determine whether it is closed


			temp = cvCreateImage(cvGetSize(grayeye2), 8,1);
			//thre = ComputeValueOTSU(grayeye2);
			cvThreshold(grayeye2, temp, 100, 255, CV_THRESH_BINARY);
			cvErode(temp, temp);
			cvDilate(temp, temp);
			cvDilate(temp, temp, NULL, 3);
			cvErode(temp, temp, NULL, 3);
			minx = grayeye2->width;
			miny = grayeye2->height;
			maxx = 0;
			maxy = 0;

			for (i = 0; i < grayeye2->width; i ++)
			{
				for (j = 0; j < grayeye2->height; j ++)
				{
					if (CV_IMAGE_ELEM(temp, uchar, j, i))
					{
						whitepart ++;
					}
				}
			}

			int area = (grayeye2->width) * (grayeye2->height);
			
			if(whitepart/area > 1/5)
				eyeopen = 1;
			else
				eyeopen = 0;

			cvReleaseImage(&eye2);
			cvReleaseImage(&grayeye2);
			return eyeopen;
		}

		else
		{
			eyeposition->x = 0;
			eyeposition->y = 0;
			eyeopen = 1;
		}
	}
	return eyeopen;
}

void alarm(IplImage *img)
{
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 2.f, 2.f, 0.0, 7, CV_AA);

	cvPutText(img, "EYE ALARM", cvPoint(0, 50), &font,CV_RGB(255, 0, 0));
}


// bool IsEye(IplImage * frame, CvPoint eyeposition)
// {
// 	if((eyeposition.x< 5) || (eyeposition.x > (frame->width - 5)) || (eyeposition.y < 5) || (eyeposition.y > (frame->height - 5)))
// 		return false;
// 	if((eyeposition.x>= 5) && (eyeposition.x <= (frame->width - 5)) && (eyeposition.y > 35) && (eyeposition.y < (frame->height - 35)))
// 	{
// 		IplImage * grayframe;
// 		grayframe = cvCreateImage(cvGetSize(frame), frame->depth, 1);
// 		cvCvtColor(frame, grayframe, CV_RGB2GRAY);
// 		int a = CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y, eyeposition.x);
// 		int temp1, temp2;
// 		temp1 = 0;
// 		temp2 = 0;
// 		for(int i = -5; i <= 5; i ++)
// 		{
// 			for (int j = -5; j <= 5; j ++)
// 			{
// 				temp1 += CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y - 30 + j, eyeposition.x + i);
// 				temp2 += CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y + 30 + j, eyeposition.x + i);
// 			}
// 		}
// 		temp1 /= 121;
// 		temp2 /= 121;
// 
// 		if((abs(a - temp1) < 50) && ((temp2 - a) > 50))
// 			return true;
// 		else
// 			return false;
// 	}
// 
// 	else if(eyeposition.y <= 35)
// 	{
// 		IplImage * grayframe;
// 		grayframe = cvCreateImage(cvGetSize(frame), frame->depth, 1);
// 		cvCvtColor(frame, grayframe, CV_RGB2GRAY);
// 		int a = CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y, eyeposition.x);
// 		int temp1;
// 		temp1 = 0;
// 		for(int i = -5; i <= 5; i ++)
// 		{
// 			for (int j = -5; j <= 5; j ++)
// 			{
// 				temp1 += CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y + 30 + j, eyeposition.x + i);
// 			}
// 		}
// 		temp1 /= 121;
// 
// 		if((temp1 -a) < 50)
// 			return true;
// 		else
// 			return false;
// 	}
// 	else if(eyeposition.y >= (frame->height - 35))
// 	{
// 		IplImage * grayframe;
// 		grayframe = cvCreateImage(cvGetSize(frame), frame->depth, 1);
// 		cvCvtColor(frame, grayframe, CV_RGB2GRAY);
// 		int a = CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y, eyeposition.x);
// 		int temp1;
// 		temp1 = 0;
// 		for(int i = -5; i <= 5; i ++)
// 		{
// 			for (int j = -5; j <= 5; j ++)
// 			{
// 				temp1 += CV_IMAGE_ELEM(grayframe, uchar, eyeposition.y - 30 + j, eyeposition.x + i);
// 			}
// 		}
// 		temp1 /= 121;
// 
// 		if(abs(temp1 -a) < 50)
// 			return true;
// 		else
// 			return false;
// 
// 	}
// 	return false;
// }


int GetEyepositionInDarky(IplImage * faces,CvPoint point1, CvPoint point2,CvPoint *eyeposition)
{//will work in this project
	int addwidth2 = 0;
	int eyeopen = 0;
	CvPoint temppoint;
	CvRect rect, rect2;
	double m1, m2;
	IplImage *eyes;
	IplImage *grayeyes;
	IplImage *temp;
	int thre;
	IplImage *temp1;
	int thre1;

	int i, j, a, b, xx, yy, x1, y1;
	int eyei = 0,eyej = 0;
	int maxi = 0, maxj = 0;
	int radium = 5,radium1 = 10,radium2 = 9;
	double n1 = 0, n2 = 0, maxn2 = 0;
	int eyenum = 0;


	if((point1.x < addwidth2) || (point1.y < addwidth2) || (point1.x >= faces->width - addwidth2) || (point1.y >= faces->height - addwidth2) ||
		(point2.x < addwidth2) || (point2.y < addwidth2) || (point2.x >= faces->width - addwidth2) || (point2.y >= faces->height - addwidth2))
	{
		eyeposition->x = 0;
		eyeposition->y = 0;
	
		eyeopen = 1;
	}

	else
	{
		
		temppoint.x = eyeposition->x;
		temppoint.y = eyeposition->y;


		
		

		rect.x = point1.x - addwidth2;
		rect.y = point1.y - addwidth2;
		rect.width = point2.x - point1.x + addwidth2 * 2;
		rect.height = point2.y - point1.y + addwidth2 * 2;
		
		eyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
	
		grayeyes = cvCreateImage(cvGetSize(eyes), 8, 1);
		GetImageRect(faces, rect, eyes);
		cvCvtColor(eyes, grayeyes, CV_BGR2GRAY);
		cvEqualizeHist(grayeyes, grayeyes);
		
		temp = cvCreateImage(cvGetSize(grayeyes), 8,1);
		thre =thresholds(eyes);
		if (thre > 240)
		{
			thre = 220;
		}
		cvThreshold(grayeyes, temp, thre, 255, CV_THRESH_BINARY);


		temp1 = cvCreateImage(cvGetSize(grayeyes), 8,1);
		thre1 = thresholdsb(eyes);
		if (thre1>100)
		{
			thre = 85;
		}
		cvThreshold(grayeyes, temp1, thre1 + 10, 255, CV_THRESH_BINARY_INV);
		
		m2 = 256*256;
	
		

		//ÕÒ³ö±ÕÑÛÑÛ¾¦Î»ÖÃ
		for ( a = radium; a < grayeyes->width - radium; a ++)
		{
			for ( b = radium; b < grayeyes->height - radium; b ++)
			{
				m1 = 0;
				for( xx = -radium; xx <= radium; xx ++)
				{
					for( yy = -radium; yy <= radium; yy ++)
					{
						if( xx * xx + yy * yy <= radium * radium)
							m1 += CV_IMAGE_ELEM(grayeyes, uchar, (b + yy), (a + xx));
					}
				}

				if(m1 < m2)
				{
					m2 = m1;
					maxi = a;
					maxj = b;
				}
			}
		}

		for (i = 0; i < grayeyes->width; i ++)
		{
			for (j = 0; j < grayeyes->height; j ++)
			{
				if (CV_IMAGE_ELEM(temp,uchar,j,i))
				{
					for( x1 = -radium1; x1 <= radium1; x1 ++)
					{
						for ( y1 = -radium1; y1<= radium1; y1 ++)
						{
							if (x1*x1 + y1*y1 <=  radium1*radium1 && x1*x1 +y1*y1 >= radium2 *radium2 && i + x1 >= 0 
								&& i + x1 <=grayeyes->width && j + y1 >= 0 && j + y1 <=grayeyes->height)
							{
								n1++;
								if ( CV_IMAGE_ELEM(temp1, uchar, (j + y1), (i + x1)))
								{
									n2 ++;

								}
							}
						}
					}

				}

				if (maxn2 < n2)
				{
					maxn2 = n2;
					if (maxn2 >= n1*0.3 )
					{

						eyei = i;
						eyej = j;
						eyenum = 1;
						n1 = 0;
						n2 = 0;

					} 
					else
					{
						eyenum = 0;
						n1 = 0;
						n2 = 0;
					}
				}
				else
				{
					n1 = 0;
					n2 = 0;
				}


			}
		}


		if (eyenum == 0)
		{
			eyeposition->x = maxi;
			eyeposition->y = maxj;
			eyeopen = 0;
		} 
		else
		{
			eyeposition->x =eyei;
			eyeposition->y = eyej;
			eyeopen = 1;
		}
		
		cvReleaseImage(&eyes);
		cvReleaseImage(&grayeyes);
		cvReleaseImage(&temp);
		cvReleaseImage(&temp1);
	
	}	
	return eyeopen;
	
}

// bool GetEyePositionInDark(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition)
// {//ºÚ°µÇé¿öÏÂÕÒ³öÑÛ¾¦¾«È·Î»ÖÃ£¬²¢ÇÒÅÐ¶ÏÊÇ·ñ±ÕÑÛ
// // 	cout << "eyes    ";
// 	int addwidth2 = 0;
// 	if((point1.x < addwidth2) || (point1.y < addwidth2) || (point1.x >= faces->width - addwidth2) || (point1.y >= faces->height - addwidth2) ||
// 		(point2.x < addwidth2) || (point2.y < addwidth2) || (point2.x >= faces->width - addwidth2) || (point2.y >= faces->height - addwidth2))
// 	{
// 		eyeposition->x = 0;
// 		eyeposition->y = 0;
// 		/*cout << "error" << endl;*/
// 		return true;
// 	}
// 
// 	else
// 	{
// 		CvPoint temppoint;
// 		temppoint.x = eyeposition->x;
// 		temppoint.y = eyeposition->y;
// 
// 		IplImage *temp;
// 		int thre;
// 
// 		CvRect rect, rect2;
// 		double m1, m2;
// 
// 		rect.x = point1.x - addwidth2;
// 		rect.y = point1.y - addwidth2;
// 		rect.width = point2.x - point1.x + addwidth2 * 2;
// 		rect.height = point2.y - point1.y + addwidth2 * 2;
// 		IplImage *eyes;
// 		eyes = cvCreateImage(cvSize(rect.width, rect.height), 8, 3);
// 		IplImage *grayeyes;
// 		grayeyes = cvCreateImage(cvGetSize(eyes), 8, 1);
// 		GetImageRect(faces, rect, eyes);
// 		cvCvtColor(eyes, grayeyes, CV_BGR2GRAY);
// 		int i, j;
// 
// 		thre = ComputeValueOTSU(grayeyes);
// 
// 		temp = cvCreateImage(cvGetSize(eyes), 8, 1);
// 		cvThreshold(grayeyes, temp, thre - 20, 255, CV_THRESH_BINARY);
// 
// 		cvDilate(temp, temp);
// 		cvErode(temp, temp);
// 		cvErode(temp, temp);
// 
// 		IplImage * grayfaces;
// 		grayfaces = cvCreateImage(cvGetSize(faces), 8, 1);
// 		cvCvtColor(faces, grayfaces, CV_BGR2GRAY);
// 		int thre2 = ComputeValueOTSU(grayfaces);
// 
// 
// 		
// 
// 
// 		if(DetectEyeCircle(grayeyes, eyeposition, thre))
// 		{
// 			cvReleaseImage(&temp);
// 			cvReleaseImage(&eyes);
// 			cvReleaseImage(&grayeyes);
// 			cvReleaseImage(&grayfaces);
// 			return true;
// 		}
// 
// 		else
// 		{
// 			m2 = 0;
// 			int blacknum;
// 
// 			int radium = 7;
// 			for (j = radium; j < rect.height - radium; j ++)
// 			{
// 				for (i = radium; i < rect.width - radium; i ++)
// 				{
// 					
// 					blacknum = 0;
// 
// 					if(CV_IMAGE_ELEM(temp, uchar, j, i) == 255)
// 					{
// 						double m3 = 0;
// 						double m4 = 0;
// 						for(int rr = 1; rr <= radium; rr ++)
// 						{
// 							for (int xx = -rr; xx <= rr; xx ++)
// 							{
// 								for (int yy = -rr; yy <= rr; yy ++)
// 								{
// 									if( (xx * xx + yy * yy <= rr * rr) && (xx * xx + yy * yy > ((rr - 1) * (rr - 1)) ))
// 									{
// 										m3 ++;
// 										if(CV_IMAGE_ELEM(temp, uchar, (j + yy), (i + xx)) == 0)
// 											m4 ++;
// 									}
// 
// 								}
// 							}
// 							if((m4 / m3) > 0.9)
// 								blacknum ++;
// 						}
// 
// 						if(blacknum > m2)
// 						{
// 							m2 = blacknum;
// 							eyeposition->x = i;
// 							eyeposition->y = j;
// 						}
// 					}
// 
// 				}
// 			}
// 
// 			if(m2 == 0)
// 			{
// 				eyeposition->x = temppoint.x - point1.x;
// 				eyeposition->y = temppoint.y - point1.y;
// 			}
// 
// 		
// 
// 			cvReleaseImage(&temp);
// 			cvReleaseImage(&eyes);
// 			cvReleaseImage(&grayeyes);
// 			cvReleaseImage(&grayfaces);
// 
// 
// 			bool eyeopen;
// 			rect2.x = point1.x + eyeposition->x - 40;
// 			rect2.y = point1.y + eyeposition->y - 20;
// 			rect2.width = 81;
// 			rect2.height = 41;
// 			if((rect2.x > 0) && (rect2.y > 0) && ((rect2.x + rect2.width) < faces->width) && ((rect2.y + rect2.height) < faces->height))
// 			{
// 				IplImage *eye2, *grayeye2;
// 				eye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 3);
// 				grayeye2 = cvCreateImage(cvSize(rect2.width, rect2.height), 8, 1);
// 				GetImageRect(faces, rect2, eye2);
// 				cvCvtColor(eye2, grayeye2, CV_BGR2GRAY);
// 				//ÅÐ¶ÏÊÇ·ñ±ÕÑÛ
// 				eyeopen = IsEyeOpenInDark(grayeye2, thre2);
// 
// 				cvReleaseImage(&eye2);
// 				cvReleaseImage(&grayeye2);
// 
// 				return eyeopen;
// 			}
// 
// 			else
// 			{
// 				return true;
// 			}
// 
// 		}
// 	}
//}

// bool IsEyeOpenInDark(IplImage * grayeyes, int thre)
// {//ºÚ°µÇé¿öÏÂÅÐ±ðÑÛ¾¦ÊÇ·ñÕö¿ª
// 	IplImage *temp;
// 	temp = cvCreateImage(cvGetSize(grayeyes), 8, 1);
// 
// 	cvThreshold(grayeyes, temp, thre + 75, 255, CV_THRESH_BINARY_INV);
// 
// 	int m = 0;
// 
// 	for (int i = 0; i < grayeyes->width; i ++)
// 	{
// 		for (int j = 0; j < grayeyes->height; j ++)
// 		{
// 			if(CV_IMAGE_ELEM(temp, uchar, j, i) == 255)
// 				m ++;
// 		}
// 	}
// 	cvReleaseImage(&temp);
// 
// 	if((m * 1.0 / grayeyes->width / grayeyes->height) > 0.3)
// 		return true;
// 	else
// 		return false;
// }
// 
// 
// bool DetectEyeCircle(IplImage * grayeyes, CvPoint * eyeposition, int eyethre)
// {
// 	int circlescale = 4;
// 	if( grayeyes != 0)
// 	{
// 		IplImage * temp = cvCreateImage( cvGetSize(grayeyes), 8, 1);
// 		IplImage * big = cvCreateImage( cvSize(grayeyes->width * circlescale, grayeyes->height * circlescale), 8, 1 );
// 
// 		cvThreshold(grayeyes, temp, eyethre - 20, 255, CV_THRESH_BINARY);
// 		cvErode(temp, temp);
// 
// 		cvResize(temp, big);
// 		cvSmooth(big, big, CV_GAUSSIAN, 15, 15); // smooth it, otherwise a lot of false circles may be detected
// 		CvSeq* circles = cvHoughCircles(big, storage, CV_HOUGH_GRADIENT, 1, 100, 200, 25,1,200 );
// 		int i;
// 		double tempradium = grayeyes->width * circlescale;
// 		int tempnum;
// 		for( i = 0; i < circles->total; i++ )
// 		{
// 			float* p = (float*)cvGetSeqElem( circles, i );
// 			if(p[2] < tempradium)
// 			{
// 				tempradium = p[2];
// 				tempnum = i;
// 			}
// 		}
// 
// 		if(circles->total)
// 		{
// 			float * p = (float*)cvGetSeqElem( circles, tempnum);
// 			eyeposition->x = cvRound(p[0] /circlescale);
// 			eyeposition->y = cvRound(p[1] /circlescale);
// 		}
// 
// 		cvReleaseImage(&big);
// // 		cvReleaseImage(&grayeyes);
// 		cvReleaseImage(&temp);
// 
// 		if(circles->total)
// 			return true;
// 		else
// 			return false;
// 	}
// 
// 	return false;
// 
// }


//int DetectIsDark(IplImage * frame)
//{//will not be used
//	int dark = 0;
//	int a[256] = {0};
//	int temp;
//	IplImage * grayframe;
//	int i, j, k;
//	grayframe = cvCreateImage(cvGetSize(frame), 8, 1);
//	cvCvtColor(frame, grayframe, CV_BGR2GRAY);
//
//	for (i = 0; i < frame->width; i ++)
//	{
//		for ( j = 0; j < frame->height; j ++)
//		{
//			temp = CV_IMAGE_ELEM(grayframe, uchar, j, i);
//			a[temp] ++;
//		}
//	}
//
//	temp = 0;
//
//	for ( k = 0; k < 10; k ++)
//	{
//		temp += a[k];
//	}
//
//	cvReleaseImage(& grayframe);
//
//
//	if((temp * 1.0 / (frame->width) / (frame->height)) > 0.3)
//	{
//		dark = 1;
//	}
//	else
//	{
//		dark = 0;
//	}
//	return dark;
//	
//}

// void Swap(char *ch1, char *ch2)
// {
// 	char tmp = *ch1;
// 	*ch1 = *ch2;
// 	*ch2 = tmp;
// }
// void Convert(int n, char str[])
// {
// 	int i, len;
// 	for(i = 0; n != 0; ++i)
// 	{
// 		str[i] = n % 10 + '0';
// 		n /= 10;
// 	}
// 	str[i] = '\0';
// 	len = i;
// 	/* ·­×ª */
// 	for(i = 0; i &lt; len/2; ++i)
// 		Swap(str+i, str+len-i-1);
// }