/*
 * povray.c -- Povray interface
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <povray.h>

#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <simplex.h>
#include <math.h>
#include <time.h>
#include <random.h>
#include <getopt.h>

/*
 * ship out set of planes
 */
#define	radius	0.01

static void	povray_plane(FILE *out, double *x, double d) {
	fprintf(out, "\tplane { <%f, %f, %f>, %f }\n", x[1], x[0], x[2], d);
}

static void	povray_preamble(FILE *out) {
	fprintf(out, "#include \"colors.inc\"\n");
	fprintf(out, "camera {\n");
	fprintf(out, "\tlocation <1.15, 1.15, 1.6>\n");
	fprintf(out, "\tlook_at <0.0, 0.5, 0.0>\n");
	fprintf(out, "\tright 0.6 * 16/9 * x\n");
	fprintf(out, "\tup 0.6 * y\n");
	fprintf(out, "}\n");
	fprintf(out, "light_source { <4, 10, 10> color White }\n");
	fprintf(out, "global_settings { ambient_light rgb<1, 1, 1> }\n");
	fprintf(out, "sky_sphere {\n");
	fprintf(out, "\tpigment {\n");
	fprintf(out, "\t\tcolor <1,1,1>\n");
	fprintf(out, "\t}\n");
	fprintf(out, "}\n");
}

static void	povray_point(FILE *out, double *p) {
	fprintf(out, "\tsphere { <%f, %f, %f>, %f }\n", p[1], p[0], p[2], radius * 1.5);
}

static double	sqr(double x) { return x * x; }
static double	distance(double *p1, double *p2) {
	return sqrt(sqr(p1[0] - p2[0]) + sqr(p1[1] - p2[1]) + sqr(p1[2] - p2[2]));
}

static void	povray_line(FILE *out, double *p1, double *p2) {
	fprintf(out, "\tsphere { <%f, %f %f>, %f }\n", p1[1], p1[0], p1[2], radius);
	if (distance(p1, p2) > 0.00001) {
		fprintf(out, "\tcylinder { <%f,%f,%f>,<%f,%f,%f>,%f }\n",
			p1[1], p1[0], p1[2],
			p2[1], p2[0], p2[2], radius);
	}
}

static void	povray_goal(FILE *out, double *normal, double *point) {
	double	d = 0;
	for (int i = 0; i < 3; i++) {
		d += normal[i] * point[i];
	}
	fprintf(out, "intersection {\n");
	fprintf(out, "\tplane { <%f, %f, %f>, %f }\n", normal[1], normal[0], normal[2], d);
	fprintf(out, "\tplane { <%f, %f, %f>, %f }\n", -normal[1], -normal[0], -normal[2], -d + 0.001);
	fprintf(out, "\tbox { <-0.1,-0.1,-0.1>, <1.1,1.1,1.1> }\n");
	fprintf(out, "\tpigment { color rgb <0.9, 0.9, 1, 0.99> }\n");
	fprintf(out, "}\n");
}

void	povray_image(FILE *out, simplex_image_t *image, double t) {
	if (POVRAY_PREAMBLE & image->flags) {
		povray_preamble(out);
	}

	/* the sphere inside the domain */
	if (POVRAY_SPHERE & image->flags) {
		fprintf(out, "intersection {\n");
		fprintf(out, "\tsphere { <0,0,0>,1 }\n");
		fprintf(out, "\tbox { <0,0,0>, <1,1,1> }\n");
		fprintf(out, "\tpigment { color rgb<0.9,0.9,0.9,0.4> }\n");
		fprintf(out, "}\n");
	}

	/* display the domain */
	if (POVRAY_DOMAIN & image->flags) {
		fprintf(out, "intersection {\n");
		double	v[3] = { -1, 0, 0 };
		povray_plane(out, v, 0);
		v[0] = 0; v[1] = -1;
		povray_plane(out, v, 0);
		v[1] = 0; v[2] = -1;
		povray_plane(out, v, 0);
		for (int i = 0; i < image->m; i++) {
			povray_plane(out, &image->points[3 * i], 1);
		}
		fprintf(out, "\tpigment {\n");
		fprintf(out, "\t\t color rgbf<0.8,0.8,0.8, 0.5>\n");
		fprintf(out, "\t}\n");
		fprintf(out, "}\n");
	}

	/* build the path */
	if (POVRAY_CURVE & image->flags) {
		fprintf(out, "union {\n");
		for (int i = 1; (i <= t) && (i < image->nvertices); i++) {
			povray_line(out, &image->vertices[3 * (i - 1)], &image->vertices[3 * i]);
		}
	}

	/* the initial point */
	double	intermediate[3] = { 0, 0, 0, };

	/* if t == 0, we should stop */
	if (t > 0) {
		/* compute the coordinates of the last point */
		int	last = image->nvertices - 1;
		int	previous = image->nvertices - 2;
		if (t >= image->nvertices) {
			memcpy(intermediate, &image->vertices[3 * last], 3 * sizeof(double));
		} else {
			previous = trunc(t);
			double	tau = t - previous;
			if (tau == 0) {
				memcpy(intermediate, &image->vertices[3 * previous],
					3 * sizeof(double));
			} else {
				intermediate[0] = (1 - tau) * image->vertices[3 * previous + 0]
					+ tau * image->vertices[3 * (previous + 1) + 0];
				intermediate[1] = (1 - tau) * image->vertices[3 * previous + 1]
					+ tau * image->vertices[3 * (previous + 1) + 1];
				intermediate[2] = (1 - tau) * image->vertices[3 * previous + 2]
					+ tau * image->vertices[3 * (previous + 1) + 2];
			}
		}
		if (POVRAY_CURVE & image->flags) {
			povray_line(out, &image->vertices[3 * previous], intermediate);
		}
	}

	if (POVRAY_CURVE & image->flags) {
		povray_point(out, intermediate);
		fprintf(out, "\tpigment { color rgb<1,0,0> }\n");
		fprintf(out, "}\n");
	}

	/* display the plane of the goal function */
	if (POVRAY_GOAL & image->flags) {
		povray_goal(out, image->normal, intermediate);
	}
}

void	povray_image_free(simplex_image_t *image) {
	if (image->points) {
		free(image->points);
	}
	if (image->vertices) {
		free(image->vertices);
	}
	memset(image, 0, sizeof(simplex_image_t));
}

simplex_image_t	*povray_image_new() {
	simplex_image_t	*result = (simplex_image_t *)calloc(1, sizeof(simplex_image_t));
	return result;
}

simplex_image_t	*povray_image_copy(simplex_image_t *parameters) {
	simplex_image_t	*result = (simplex_image_t *)calloc(1, sizeof(simplex_image_t));
	result->m = parameters->m;
	result->flags = parameters->flags;
	return result;
}

double	povray_image_length(simplex_image_t *image) {
	double	d = 0;
	for (int i = 1; i < image->nvertices; i++) {
		d += distance(&image->vertices[3 * (i - 1)], &image->vertices[3 * i]);
	}
	return d;
}
