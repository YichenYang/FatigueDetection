#include "stdafx.h"
#include "eyetrack.h"



/********************* Globals ************************/
           
int num_particles = PARTICLES;    /* number of particles */

int exported = FALSE;               /* TRUE to exported tracking sequence */


typedef struct params {
	CvPoint loc1[MAX_OBJECTS];
	CvPoint loc2[MAX_OBJECTS];
	IplImage* objects[MAX_OBJECTS];
	char* win_name;
	IplImage* orig_img;
	IplImage* cur_img;
	int n;
} params;

extern double scale;



int eyetrack(int num_objects, IplImage * hsv_frame, IplImage *lasthsv_frame, CvRect regions, gsl_rng* rng, CvPoint * eyeposition, IplImage * frame, int IsDark)
{
	CvPoint point1, point2;
	histogram** ref_histos;
	particle* particles, * new_particles;
	int w, h;
	float s;
	int j;

	w = hsv_frame->width;
	h = hsv_frame->height;

	num_objects = 1;
	ref_histos = compute_ref_histos( lasthsv_frame, &regions, num_objects );
	if( exported )
		export_ref_histos( ref_histos, num_objects );
	particles = init_distribution( &regions, ref_histos,num_objects, num_particles );


	

	for(j = 0; j < num_particles; j++ )
	{
		particles[j] = transition( particles[j], w, h, rng );
		s = particles[j].s;
		particles[j].w = likelihood( hsv_frame, cvRound(particles[j].y),cvRound( particles[j].x ),
			cvRound( particles[j].width * s ),cvRound( particles[j].height * s ),	particles[j].histo );
	}


	normalize_weights( particles, num_particles );
	new_particles = resample( particles, num_particles );
	free( particles );
	particles = new_particles;


	qsort( particles, num_particles, sizeof( particle ), &particle_cmp );

	display_particle(particles[0], &point1, &point2);

	point1.x = cvRound(scale * point1.x);
	point1.y = cvRound(scale * point1.y);
	point2.x = cvRound(scale * point2.x);
	point2.y = cvRound(scale * point2.y);


	int eyeopen;

	if(IsDark == 1)
		eyeopen = GetEyePosition(frame, point1, point2, eyeposition);
	else
		eyeopen = GetEyePosition(frame, point1, point2, eyeposition);

	eyeposition->x += point1.x;
	eyeposition->y += point1.y;

	return eyeopen;
}

double eyetrackmuch(int num_objects, IplImage * hsv_frame, IplImage *lasthsv_frame, CvRect regions, gsl_rng* rng, CvPoint * eyeposition, IplImage * frame, int IsDark)
{
	CvPoint point1, point2;
	histogram** ref_histos;
	particle* particles, * new_particles;
	int w, h;
	float s;
	int j;
	double eyeopen;

	w = hsv_frame->width;
	h = hsv_frame->height;

	num_objects = 1;
	ref_histos = compute_ref_histos( lasthsv_frame, &regions, num_objects );
	if( exported )
		export_ref_histos( ref_histos, num_objects );
	particles = init_distribution( &regions, ref_histos,num_objects, num_particles );


	

	for(j = 0; j < num_particles; j++ )
	{
		particles[j] = transition( particles[j], w, h, rng );
		s = particles[j].s;
		particles[j].w = likelihood( hsv_frame, cvRound(particles[j].y),cvRound( particles[j].x ),
			cvRound( particles[j].width * s ),cvRound( particles[j].height * s ),	particles[j].histo );
	}


	normalize_weights( particles, num_particles );
	new_particles = resample( particles, num_particles );
	free( particles );
	particles = new_particles;


	qsort( particles, num_particles, sizeof( particle ), &particle_cmp );

	display_particle(particles[0], &point1, &point2);

	point1.x = cvRound(scale * point1.x);
	point1.y = cvRound(scale * point1.y);
	point2.x = cvRound(scale * point2.x);
	point2.y = cvRound(scale * point2.y);


	

	eyeopen = howmucheye(frame,point1,point2,eyeposition);

	eyeposition->x += point1.x;
	eyeposition->y += point1.y;


	return eyeopen;

}