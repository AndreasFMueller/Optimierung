/*
 * linprog.h -- linear programming operations
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 * $Id$
 */
#ifndef _linprog_h
#define _linprog_h

#include <stdlib.h>
#include <stdio.h>

typedef struct linprog_s {
	int	m;	/* number of inequalities */
	int	n;	/* number of variables */
	double	*a;	/* mxn coefficients of matrix */
	double	*b;	/* m-vector of right hand sides */
	double	*c;	/* n-vector of target function coefficients */
} linprog_t;

linprog_t	*linprog_new(int m, int n);
void	linprog_free(linprog_t *linprog);
linprog_t	*linprog_dual(linprog_t *linprog);
linprog_t	*linprog_positive(linprog_t *linprog);
linprog_t	*linprog_initial(linprog_t *linprog);
double	linprog_a_get(linprog_t *linprog, int i, int j);
void	linprog_a_set(linprog_t *linprog, int i, int j, double a);
void	linprog_show(FILE *out, linprog_t *linprog);
void	linprog_changesign(linprog_t *linprog);

#endif /* _linprog_h */
