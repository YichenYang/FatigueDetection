# ifndef THREHOLDOTSU_H
# define THREHOLDOTSU_H

//#include "stdafx.h"
#include "cv.h"

#include "cxcore.h"


int ComputeValueOTSU (IplImage * img);
/*
用OTSU法计算给定图像的阈值
img为给定的图像
*/

int thresholds(IplImage * img); 
/*
自定义给定图像的阈值，以找到黑眼球为目标
img为给定的图像
*/
int thresholdsb(IplImage *img);

# endif