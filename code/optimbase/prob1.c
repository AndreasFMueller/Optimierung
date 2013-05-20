/*
 * prob1.c -- mock function for testing
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <optim.h>
#include <math.h>

static double	min[1] = { 0. };
static double	max[1] = { M_PI };

static double	prob1_function(optim_t *optim, double *x) {
	return sin(x[0]);
}

optim_t	prob1 = {
	.n = 1,
	.min = min,
	.max = max,
	.f = prob1_function,
	.needs_deallocation = 0
};
