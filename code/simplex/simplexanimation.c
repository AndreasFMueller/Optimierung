/*
 * test2.c -- simplex algorithm on a sphere
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <simplex.h>
#include <math.h>
#include <time.h>
#include <random.h>
#include <getopt.h>
#include <povray.h>
#include <sys/time.h>
#include <simplexcpu.h>
#include <backend.h>
#include <globals.h>

static int	n = 3;

/*
 * getcoordinates
 */
void	getcoordinates(simplex_tableau_t *t, double *v) {
	for (int i = 0; i < t->n; i++) {
		if (t->flags[i]) {
			v[i] = 0;
		} else {
			for (int j = 1; j <= t->m; j++) {
				if (simplex_tableau_get(t, j, i) == 1) {
					v[i] = simplex_tableau_get(t, j, t->n + t->m);
				}
			}
		}
	}
}

/*
 * perform the simplex computation
 */
simplex_image_t	*compute(simplex_image_t *parameters) {
	/* the container structure */
	simplex_image_t	*image = (simplex_image_t *)calloc(1,
		sizeof(simplex_image_t));
	memcpy(image, parameters, sizeof(simplex_image_t));

	/* create a table full of unit vectors */
	image->points = (double *)calloc(3 * image->m, sizeof(double));
	for (int i = 0; i < image->m; i++) {
		random_on_octantsphere(&image->points[3 * i]);
	}

	/* create a suitable simplex tableau */
	simplex_tableau_t	*tableau = simplex_tableau_new(image->m, n);

	/* target function */
	simplex_tableau_set(tableau, 0, 0, image->normal[0]);
	simplex_tableau_set(tableau, 0, 1, image->normal[1]);
	simplex_tableau_set(tableau, 0, 2, image->normal[2]);

	/* initialize the planes */
	for (int i = 1; i <= image->m; i++) {
		simplex_tableau_set(tableau, i, 0, image->points[3 * i + 0]);
		simplex_tableau_set(tableau, i, 1, image->points[3 * i + 1]);
		simplex_tableau_set(tableau, i, 2, image->points[3 * i + 2]);
		simplex_tableau_set(tableau, i, image->m + n, 1.);
	}

	struct timeval	start, end;
	gettimeofday(&start, NULL);

	/* build an array to store the points of the approximate solutions */
	image->nvertices = 1;
	image->vertices = (double *)calloc(3 * image->nvertices, sizeof(double));
	image->vertices[0] = image->vertices[1] = image->vertices[2] = 0.;

	double	v[3];
	getcoordinates(tableau, v);

	int	pivoti = 0, pivotj = 0;
	int	counter = 0;
	while (simplex_tableau_findpivot(tableau, &pivoti, &pivotj) > 0) {
		simplex_tableau_pivot(tableau, pivoti, pivotj);
		getcoordinates(tableau, v);
		/* add the point we just found to the list of points */
		image->vertices = (double *)realloc(image->vertices,
			3 * sizeof(double) * (image->nvertices + 1));
		image->vertices[3 * image->nvertices + 0] = v[0];
		image->vertices[3 * image->nvertices + 1] = v[1];
		image->vertices[3 * image->nvertices + 2] = v[2];
		image->nvertices++;
	}

	/* end time */
	gettimeofday(&end, NULL);
	image->time = (end.tv_sec + 0.0000001 * end.tv_usec)
		- (start.tv_sec + 0.0000001 * start.tv_usec);

	/* add the path length */
	image->length = povray_image_length(image);

	/* clean up the simplex tableau */
	simplex_tableau_free(tableau);

	/* return the image we just have computed */
	return image;
}

int	main(int argc, char *argv[]) {
	simplex_image_t	image = {
		.m = 100,
		.points = NULL,
		.nvertices = 0,
		.vertices = NULL,
		.normal = { 1/sqrt(3), 1/sqrt(3), 1/sqrt(3) },
		.flags = POVRAY_CURVE | POVRAY_PREAMBLE | POVRAY_DOMAIN | POVRAY_GOAL
	};

	int	c;
	char	*fileprefix = NULL;
	char	*allcurves = NULL;
	double	timestep = 1;
	int	repeat = 1;
	simplex_image_t	**solutions = NULL;
	int	R = 10;
	int	edgestep = 1;

	/* initialize the random number generator */
	srandom(time(NULL));

	/* parse the command line */
	while (EOF != (c = getopt(argc, argv, "dp:m:s:t:r:P:R:e:b:")))
		switch (c) {
		case 'b':
			if (backend_select(optarg) < 0) {
				fprintf(stderr, "cannot select backend %s\n",
					optarg);
				exit(EXIT_FAILURE);
			}
			break;
		case 'd':
			debug++;
			break;
		case 'p':
			fileprefix = optarg;
			break;
		case 'P':
			allcurves = optarg;
			break;
		case 'm':
			image.m = atoi(optarg);
			break;
		case 's':
			srandom(atoi(optarg));
			break;
		case 't':
			timestep = atof(optarg);
			break;
		case 'r':
			repeat = atoi(optarg);
			solutions = (simplex_image_t **)realloc(solutions,
				repeat * sizeof(simplex_image_t *));
			break;
		case 'R':
			R = atoi(optarg);
			break;
		case 'e':
			edgestep = atoi(optarg);
			break;
		}

	/* if there is no backend selected so far, we use the CPU backend
	   which supposedly is always present */
	if (NULL == backend_current()) {
		if (debug) {
			fprintf(stderr, "%s:%d: loading CPU backend vers %d\n",
				__FILE__, __LINE__, simplexcpu_version);
		}
		if (backend_select("CPU") < 0) {
			fprintf(stderr, "backend CPU failed\n");
			exit(EXIT_FAILURE);
		}
	}
	if (backend_init() < 0) {
		fprintf(stderr, "backend initialization failed\n");
		exit(EXIT_FAILURE);
	}

	int	counter = 0;
	for (int r = 0; r < repeat; r++) {
		/* perform the simplex computation */
		simplex_image_t	*simage = compute(&image);
		image.m += edgestep;

		/* produce a povray file */
		if (fileprefix) {
			double	t = 0;
			while (t <= simage->nvertices - 1) {
				char	filename[1024];
				snprintf(filename, sizeof(filename),
					"%s/%05d.pov", fileprefix, counter);
				FILE	*output = fopen(filename, "w");
				if (NULL == output) {
					fprintf(stderr, "cannot open file %s: %s\n",
						filename, strerror(errno));
					exit(EXIT_FAILURE);
				}
				povray_image(output, simage, t);
				fclose(output);
				counter++;
				t += timestep;
			}
		}

		/* report the results */
		printf("%d,%d,%f,%f\n", simage->m, simage->nvertices,
			simage->length, simage->time);
		fflush(stdout);

		/* cleanup the image */
		if ((solutions) && (allcurves)) {
			solutions[r] = simage;
		} else {
			povray_image_free(simage);
		}
	}

	if ((solutions) && (allcurves)) {
		for (int r = 1; r < repeat; r++) {
			char	allfilename[1024];
			snprintf(allfilename, sizeof(allfilename),
				"%s/%05d.pov", allcurves, r);

			/* create the file we want to write */
			FILE	*allcurvesfile = fopen(allfilename, "w");
			if (NULL == allcurvesfile) {
				fprintf(stderr, "cannot open file '%s' for "
					"writing: %s\n",
					allcurves, strerror(errno));
				exit(EXIT_FAILURE);
			}

			/* output the preamble from the parameter structure */
			image.flags = POVRAY_PREAMBLE | POVRAY_SPHERE;
			povray_image(allcurvesfile, &image, 0);

			/* output the curves from all other structures */
			int	rr = r - R;
			if (rr < 0) {
				rr = 0;
			}
			for (; rr < r; rr++) {
				solutions[rr]->flags = POVRAY_CURVE;
				povray_image(allcurvesfile, solutions[rr],
					100000);
			}
			fclose(allcurvesfile);
		}
	}

	/* cleanup the backend */
	backend_release();

	exit(EXIT_SUCCESS);
}
