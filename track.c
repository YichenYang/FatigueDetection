#include "track.h"
#include "defs.h"
#include "utils.h"
#include "particles.h"
#include "observation.h"



histogram** compute_ref_histos( IplImage* frame, CvRect* regions, int n )
{
	histogram** histos = malloc( n * sizeof( histogram* ) );
	IplImage* tmp;
	int i;

	/* extract each region from frame and compute its histogram */
	for( i = 0; i < n; i++ )
	{
		cvSetImageROI( frame, regions[i] );
		tmp = cvCreateImage( cvGetSize( frame ), IPL_DEPTH_32F, 3 );
		cvCopy( frame, tmp, NULL );
		cvResetImageROI( frame );
		histos[i] = calc_histogram( &tmp, 1 );
		normalize_histogram( histos[i] );
		cvReleaseImage( &tmp );
	}

	return histos;
}

int export_ref_histos( histogram** ref_histos, int n )
{
	char name[32];
	char num[3];
	FILE* file;
	int i;

	for( i = 0; i < n; i++ )
	{
		sprintf( num, 3, "%02d", i );
		strcpy( name, "hist_" );
		strcat( name, num );
		strcat( name, ".dat" );
		if( ! export_histogram( ref_histos[i], name ) )
			return 0;
	}

	return 1;
}