/*
 * sphere.c -- create a spherical problem an solve, track the path
 *
 * (c) 2013 Prof Dr Andreas, Hochschule Rapperswil
 */
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <globals.h>
#include <simplex.h>
#include <simplexcpu.h>
#include <backend.h>
#include <getopt.h>
#include <random.h>
#include <time.h>
#include <povray.h>
#include <math.h>

/*
 * getcoordinates
 */
void	getcoordinates(simplex_tableau_t *t, double *v) {
	double	vv[6];
	for (int i = 0; i < 6; i++) {
		if (t->flags[i]) {
			vv[i] = 0;
		} else {
			for (int j = 1; j <= t->m; j++) {
				if (simplex_tableau_get(t, j, i) == 1) {
					vv[i] = simplex_tableau_get(t, j,
						t->n + t->m);
				}
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		v[i] = vv[i] - vv[3 + i];
	}
	if (debug) {
		fprintf(stderr, "v = %f, %f, %f\n", vv[0], vv[1], vv[2]);
	}
}

int	main(int argc, char *argv[]) {
	simplex_image_t	image = {
		.m = 100,
		.points = NULL,
		.nvertices = 0,
		.vertices = NULL,
		.normal = { 1/sqrt(3), 1/sqrt(3), 1/sqrt(3) },
		.flags = POVRAY_CURVE | POVRAY_PREAMBLE | POVRAY_DOMAIN,
		.scale = 1,
		.transparency = 0.8
	};

	int	c;
	char	*filename = NULL;

	while (EOF != (c = getopt(argc, argv, "dtb:n:sp:S:T:")))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 't':
			simplex_debug = 1;
			break;
		case 'T':
			image.transparency = atof(optarg);
			break;
		case 'b':
			if (backend_select(optarg) < 0) {
				fprintf(stderr, "cannot select backend %s\n",
					optarg);
			}
			break;
		case 'n':
			image.m = atoi(optarg);
			break;
		case 's':
			srandom(time(NULL));
			break;
		case 'S':
			image.scale = atof(optarg);
			break;
		case 'p':
			filename = optarg;
			break;
		}

	/* if there is no backend selected, we select the CPU backend */
	if (NULL == backend_current()) {
		if (debug) {
			fprintf(stderr, "%s:%d: loading CPU backend vers %d\n",
				__FILE__, __LINE__, simplexcpu_version);
		}
		if (backend_select("CPU") < 0) {
			fprintf(stderr, "cannt select CPU backend");
			exit(EXIT_FAILURE);
		}
	}

	/* initialize the backend */
	if (debug) {
		fprintf(stderr, "%s:%d: initializing backend\n",
		__FILE__, __LINE__);
	}
	backend_init();

	/* create the set of normal vectors */
	image.points = (double *)calloc(3 * image.m, sizeof(double));
	for (int i = 0; i < image.m; i++) {
		random_on_sphere(&image.points[3 * i]);
	}

	/* create the simplex tableau */
	linprog_t	*linprog = linprog_new(image.m, 3);
	linprog->c[0] = linprog->c[1] = linprog->c[2] = 1; 
	for (int i = 0; i < image.m; i++) {
		linprog_a_set(linprog, i, 0, image.points[3 * i + 0]);
		linprog_a_set(linprog, i, 1, image.points[3 * i + 1]);
		linprog_a_set(linprog, i, 2, image.points[3 * i + 2]);
		linprog->b[i] = 1;
	}
	linprog_show(stdout, linprog);

	/* turn the system into something with only positive variables */
	fprintf(stdout, "program for positive variables:\n");
	linprog_t	*posprog = linprog_positive(linprog);
	linprog_show(stdout, posprog);

	/* create the linear program for the initial vertex */
	fprintf(stdout, "program for initial point:\n");
	linprog_t	*initprog = linprog_initial(posprog);
	linprog_show(stdout, initprog);

	/* solve using the simplex algorithm */
	simplex_tableau_t	*simplex = simplex_tableau_linprog(posprog);
	simplex_tableau_show(stdout, simplex);

	/* add zero */
	double	v[3] = { 0, 0, 0 };
	image.vertices = (double *)calloc(3, sizeof(double));
	image.vertices[0] = v[0];
	image.vertices[1] = v[1];
	image.vertices[2] = v[2];
	image.nvertices = 1;

	/* perform simplex algorithm */
	int	pivoti = 0, pivotj = 0;
	int	counter = 0;
	while (simplex_tableau_findpivot(simplex, &pivoti, &pivotj) > 0) {
		simplex_tableau_pivot(simplex, pivoti, pivotj);
		getcoordinates(simplex, v);
		image.vertices = (double *)realloc(image.vertices,
			3 * sizeof(double) * (image.nvertices + 1));
		image.vertices[3 * image.nvertices + 0] = v[0];
		image.vertices[3 * image.nvertices + 1] = v[1];
		image.vertices[3 * image.nvertices + 2] = v[2];
		image.nvertices++;
		counter++;
	}

	/* write the stuff into a file */
	if (filename) {
		FILE	*out = fopen(filename, "w");
		povray_image(out, &image, counter);
		fclose(out);
	}

	simplex_tableau_show(stdout, simplex);

	exit(EXIT_SUCCESS);
}
