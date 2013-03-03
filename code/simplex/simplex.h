/*
 * simplex.h -- simplex algorithm implementation
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 * $Id$
 */
#ifndef _simplex_h
#define _simplex_h

#include <stdlib.h>
#include <stdio.h>
#include <linprog.h>

typedef struct simplex_tableau_s {
	int	m;
	int	n;
	double	*t;
	int	*flags;
} simplex_tableau_t;

extern int	simplex_debug;

simplex_tableau_t	*simplex_tableau_new(int m, int n);
void	simplex_tableau_free(simplex_tableau_t *t);
void	simplex_tableau_set(simplex_tableau_t *t, int row, int col, double a);
double	simplex_tableau_get(simplex_tableau_t *t, int row, int col);
void	simplex_tableau_show(FILE *out, simplex_tableau_t *t);
void	simplex_tableau_pivot(simplex_tableau_t *, int row, int col);
int	simplex_tableau_findpivot(simplex_tableau_t *t,
		int *pivoti, int *pivotj);
int	simplex_tableau_compute(simplex_tableau_t *t);

simplex_tableau_t	*simplex_tableau_linprog(linprog_t *linprog);

#endif /* _simplex_h */
