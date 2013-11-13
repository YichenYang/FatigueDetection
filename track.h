#ifndef TRACK_H
#define TRACK_H


#include "defs.h"
#include "utils.h"
#include "particles.h"
#include "observation.h"
//---gslµÄ¿âÎÄ¼þ-----------
// #pragma comment (lib, "libgsl.a")


histogram** compute_ref_histos( IplImage* frame, CvRect* regions, int n );
int export_ref_histos( histogram** ref_histos, int n );


#endif