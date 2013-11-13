#include "threholdotsu.h"
#include <math.h>
#include <highgui.h>
#include <cv.h>



int ComputeValueOTSU (IplImage * img)
{ //use otsu method to compute the threshold
	int thresholdValue=1;//set the threshold
	int ihist [256] ; // histrogram, 256 point
	int i, j, k,n, n1, n2, Color=0;
	double m1, m2, sum, csum, fmax, sb;
	memset (ihist, 0, sizeof (ihist)) ; // initialize the histrogram...

	for (i=0;i< img->width;i++) // build the histrogram
	{ 
		for (j=0;j<img->height;j++)
		{ 
			Color=CV_IMAGE_ELEM(img, uchar, j, i);
			ihist [Color] += 1;
		}
	}
	sum=csum=0.0;
	n=0;
	for (k = 0; k <= 255; k++)
	{ 
		sum+= (double) k* (double) ihist [k] ; 
		n +=ihist [k]; 
	}
	// do the otsu global thresholding method
	fmax = - 1.0;
	n1 = 0;
	for (k=0;k<=255;k++) 
	{
		n1 += ihist[k];
		n2 = n - n1;
		csum += (double) k *ihist[k];
		m1 = csum / n1;
		m2 = (sum - csum) / n2;
		sb = (double) n1 *(double) n2 *(m1 - m2) * (m1 - m2);
		if (sb > fmax) 
		{
			fmax = sb;
			thresholdValue = k;
		}

	}
	return thresholdValue;


}

int thresholds (IplImage * img)
{ //compute the largest grey level the white part
	int thresholdValue=1;//阈值
	// 	int ihist [256] ; // 图像直方图, 256个点
	int i, j;
	int  mini =0, minj=0 , fmin=0 ;
	int s;
	IplImage *grayimage = cvCreateImage(cvGetSize(img),8, 1 );

	cvCvtColor(img, grayimage, CV_BGR2GRAY);
	
	for (i=0;i< grayimage->width;i++)
	{
		for (j=0;j<img->height;j++)
			s = CV_IMAGE_ELEM(grayimage,uchar,j,i);
		/*	cvGet2D(grayimage,i,j);*/
		// get the (j,i) pixel value, 注意cvGet2D与cvSet2D中坐标参数的顺序与其它opencv函数坐标参数顺序恰好相反.本函数中i代表y轴，即height;j代表x轴，即weight.
		//      	printf("intensity=%f\n",s.val[0]);
		//      	s.val[0]=111;
		//      	cvSet2D(img,i,j,s); // set the (j,i) pixel value
		{
			if ( s >= fmin)
			{
				fmin = s;
				mini=i;
				minj=j;
			}

		}
		thresholdValue =fmin;
	}

	return thresholdValue;
}

int thresholdsb(IplImage *img)

{//compute the smallest grey level, the black part
	int thresholdValue=1;//阈值

	int i, j;
	int  mini =0, minj=0 , fmin=255 ;
	int s;
	IplImage *grayimage = cvCreateImage(cvGetSize(img),8, 1 );

	cvCvtColor(img, grayimage, CV_BGR2GRAY);

	for (i=0;i< grayimage->width;i++)
	{
		for (j=0;j<img->height;j++)
			s = CV_IMAGE_ELEM(grayimage,uchar,j,i);
		/*	cvGet2D(grayimage,i,j);*/
		// get the (j,i) pixel value, 注意cvGet2D与cvSet2D中坐标参数的顺序与其它opencv函数坐标参数顺序恰好相反.本函数中i代表y轴，即height;j代表x轴，即weight.
		//      	printf("intensity=%f\n",s.val[0]);
		//      	s.val[0]=111;
		//      	cvSet2D(img,i,j,s); // set the (j,i) pixel value
		{
			if (s <= fmin)
			{
				fmin = s;
				mini=i;
				minj=j;
			}

		}
		thresholdValue =fmin;
	}

	return thresholdValue;

}