#ifndef HAARFACES_H
#define HAARFACES_H

#include "stdafx.h"
#include <cv.h>
#include <cxcore.h>
#include "eye.h"


void detectfaces( IplImage* img, IplImage * small_img, CvHaarClassifierCascade *facecascade, 
				 CvHaarClassifierCascade *lefteyecascade, CvHaarClassifierCascade *righteyecascade, int IsDark);
/*
使用haar分类器检测图像上的人脸，并在人脸部分检测人眼
img为输入图像
small_img为输入图像缩小后的图像
facecascade为人脸检测器
lefteyecascade为左眼检测器
righteyecascade为右眼检测器
*/


#endif