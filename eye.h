# ifndef EYE_H
# define EYE_H

#include "stdafx.h"
#include "cv.h"

#include "cxcore.h"
#include "threholdotsu.h"

#include <math.h>

/*#include "iostream"*/
using namespace std;



int IsEyeOpen(IplImage * eyes);
/*
正常情况下
判别眼睛是否睁开
eyes 为输入的眼睛部分的图像
*/


void GetImageRect(IplImage* orgImage, CvRect rectInImage, IplImage* imgRect);
/*
从图像orgImage中提取一块（rectInImage）子图像imgRect
orgImage为原图像
rectInImage为所需要图像的快信息
imgRect为子图像
*/


int GetEyePosition(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition);
/*
正常情况下
找出眼睛精确位置，并且返回是否闭眼
返回true表示睁眼，返回false表示闭眼
faces为人脸图像
point1为人眼区域左上点
point2为人眼区域有下点
eyeposition为眼睛的位置
*/

int GetEyePositionwhite(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition);
/*
find the eye position based on the white part of the eye
*/

double howmucheye(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition );
/*
找出眼睛位置，并返回睁开的百分比
*/

void alarm(IplImage *img);
/*
在输出图像上显示警报信息
*/


/*bool IsEye(IplImage * frame, CvPoint eyeposition);*/
/*
判断当前位置是不是眼睛
是返回TRUE，否则返回FALSE
*/

int GetEyepositionInDarky(IplImage * faces,CvPoint point1, CvPoint point2,CvPoint *eyeposition);
/*
同样找出眼睛位置并返回是否闭眼
*/

/*bool GetEyePositionInDark(IplImage * faces, CvPoint point1, CvPoint point2, CvPoint *eyeposition);*/
/*
黑暗情况下
找出眼睛精确位置，并且返回是否闭眼
返回true表示睁眼，返回false表示闭眼
faces为人脸图像
point1为人眼区域左上点
point2为人眼区域有下点
eyeposition为眼睛的位置
*/

/*bool IsEyeOpenInDark(IplImage * eyes, int thre);*/
/*
黑暗情况下
判别眼睛是否睁开
eyes 为输入的眼睛部分的图像
eyeposition为眼睛在图像eyes中的相对位置
*/


/*bool DetectEyeCircle(IplImage * grayeyes, CvPoint * eyeposition, int eyethre);*/
/*
判断眼球亮的部分是否为圆形
grayeyes为为输入的眼睛部分的图像
eyeposition为眼睛在图像eyes中的相对位置
eyethre为灰度图眼睛的ostu方法计算出来的阈值
*/


int DetectIsDark(IplImage * frame);
/*
判断是否为黑夜
是黑夜返回TRUE
否则返回FALSE
*/
#endif