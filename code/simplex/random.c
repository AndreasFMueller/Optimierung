/*
 * random.h -- random functions
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 * $Id$
 */
#define _GNU_SOURCE

#include <random.h>
#include <stdlib.h>
#include <math.h>

double	random_unit() {
	return random() / (double)RAND_MAX;
}

double	random_equi(double from, double to) {
	return from + (to - from) * random_unit();
}

void	random_on_sphere(double *v) {
	double	phi = random_equi(0, 2 * M_PI);
	double	theta = acos(1 - 2 * random_unit());
	double	s = sin(theta);
	v[0] = s * cos(phi);
	v[1] = s * sin(phi);
	v[2] = cos(theta);
}

void	random_on_octantsphere(double *v) {
	random_on_sphere(v);
	v[0] = fabs(v[0]);
	v[1] = fabs(v[1]);
	v[2] = fabs(v[2]);
}
