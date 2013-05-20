/*
 * prob2.c -- mock function for testing
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <optim.h>
#include <math.h>

static double	min[2] = { -1, -1 };
static double	max[2] = { 2, 3 };

static inline double	sqr(double x) {
	return x * x;
}

static double	prob2_function(optim_t *optim, double *x) {
	return sqr(x[0]) + sqr(x[1]);
}

optim_t	prob2 = {
	.n = 2,
	.min = min,
	.max = max,
	.f = prob2_function,
	.needs_deallocation = 0,
};
