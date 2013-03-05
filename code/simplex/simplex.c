/*
 * simplex.c -- implement simplex algorithm
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 * $Id$
 */
#define _GNU_SOURCE

#include <simplex.h>
#include <stdlib.h>
#include <string.h>
#include <backend.h>

extern int	simplex_debug = 0;

/**
 * \brief Create a new simplex tableau structure
 *
 * The simplex tableau structure contains all the data in a single array.
 * The first line is the target function, lines 1 to m are inequalities,
 * a <= sign is implied. The rightmost column is the constant.
 *
 * \param m	the number of inequalities
 * \param n	the number of unknowns
 */
simplex_tableau_t	*simplex_tableau_new(int m, int n) {
	simplex_tableau_t	*st
		= (simplex_tableau_t *)calloc(1, sizeof(simplex_tableau_t));
	st->m = m;
	st->n = n;
	int	size = (m + 1) * (n + m + 1);
	st->t = (double *)calloc(size, sizeof(double));
	for (int i = 0; i < m; i++) {
		simplex_tableau_set(st, 1 + i, n + i, 1.);
	}
	st->flags = (int *)calloc(m + n + 1, sizeof(int));
	for (int i = 0; i < n; i++) {
		st->flags[i] = 1;
	}
	return st;
}

/**
 * \brief Free the simplex tableau structore
 */
void	simplex_tableau_free(simplex_tableau_t *st) {
	if (st->flags) {
		free(st->flags);
	}
	if (st->t) {
		free(st->t);
	}
	memset(st, 0, sizeof(simplex_tableau_t));
	free(st);
}

/**
 * \brief Get a value from the simplex tableau
 */
double	simplex_tableau_get(simplex_tableau_t *st, int row, int col) {
	return st->t[col + row * (st->m + st->n + 1)];
}

/**
 * \brief Set a value in the simplex tableau
 */
void	simplex_tableau_set(simplex_tableau_t *st, int row, int col, double a) {
	st->t[col + row * (st->m + st->n + 1)] = a;
}

void	dger_(int *m, int *n, double *alpha, double *x, int *incx, double *y, int *incy, double *a, int *lda);

/**
 * \brief Given a pivot element, find the 
 */
int	simplex_tableau_find(simplex_tableau_t *st, int row, int col) {
	for (int i = 0; i < st->n + st->m; i++) {
		if ((st->flags[i] == 0)
			&& (simplex_tableau_get(st, row, i) == 1)) {
			return i;
		}
	}
	return -1;
}

/**
 * \brief Simplex algorithm exchange step with pivot element (row,col)
 *
 */
void	simplex_tableau_pivot(simplex_tableau_t *st, int row, int col) {
	/* find out where the other column is by looking for 1 in the
	   the row, and also set the markers for the variables that
	   are to be set to 0 */
	int	xi = simplex_tableau_find(st, row, col);
	if (xi < 0) {
		fprintf(stderr, "data inconsistency found\n");
		exit(EXIT_FAILURE);
		return;
	}
	st->flags[xi] = 1;
	st->flags[col] = 0;

	/* for the rest operation, use the backend pivot operation */
	backend_pivot(st, row, col);
}

/**
 * \brief Find the pivot for the standard simplex algorithm
 */
int	simplex_tableau_findpivot(simplex_tableau_t *st,
		int *pivotrow, int *pivotcol) {
	/* find out where we could best increase the target function */
	*pivotcol = -1;
	double	max = 0;
	for (int i = 0; i < st->n + st->m; i++) {
		if (st->flags[i]) {
			if (st->t[i] > max) {
				*pivotcol = i;
				max = st->t[i];
			}
		}
	}

	/* we have not found a column pivot, so we return 0 indicating
	   the problem */
	if (*pivotcol < 0) {
		return 0;
	}

	/* now find the limiting pivot */
	*pivotrow = 0;
	double	limit = -1;
	for (int j = 1; j <= st->m; j++) {
		/* we need the coefficient and the constant */
		double	a = simplex_tableau_get(st, j, *pivotcol);
		double	b = simplex_tableau_get(st, j, st->n + st->m);
		if (a != 0) {
			/* we are looking for the smallest positive quotient */
			double	q = b/a;
			if (q >= 0) {
				/* if limit is not set yet, then we have not
				   computed any positive quotients so far, so
				   we just use q for the limit. In other cases
				   we test whether the new quotient is smaller.
				 */
				if (limit < 0) {
					limit = q;
					*pivotrow = j;
				} else {
					if (q < limit) {
						limit = q;
						*pivotrow = j;
					}
				}
			}
		}
	}

	/* We have not find a matching row, so we are in serious trouble */
	if (*pivotrow < 0) {
		fprintf(stderr, "could not find a pivot\n");
		return -1;
	}

	/* we have found a nice pivot, so tell that */
	return 1;
}

/**
 * \brief Show the simplex tableau 
 *
 * Display a simplex tableau in human readable form.
 */
void	simplex_tableau_show(FILE *out, simplex_tableau_t *st) {
	/* display the numbers in the tableau */
	for (int row = 0; row <= st->m; row++) {
		for (int col = 0; col < st->n + st->m + 1; col++) {
			fprintf(out, "%8.4f ",
				simplex_tableau_get(st, row, col));
		}
		fprintf(out, "\n");
	}

	/* display the line of stars of selected columns */
	for (int col = 0; col < st->n + st->m; col++) {
		fprintf(out, (st->flags[col]) ? "       * " : "         ");
	}
	fprintf(out, "\n");
}

/**
 * \brief convert an optimization problem into a simplex tableau
 *
 * convert a linear optimization problem into a simplex tableau
 */
simplex_tableau_t	*simplex_tableau_linprog(linprog_t *linprog) {
	simplex_tableau_t	*result = simplex_tableau_new(linprog->m,
		linprog->n);

	/* copy the target function */
	for (int j = 0; j < linprog->n; j++) {
		simplex_tableau_set(result, 0, j, linprog->c[j]);
	}
	for (int j = 0; j < linprog->m; j++) {
		simplex_tableau_set(result, 0, linprog->n + j, 0);
	}
	simplex_tableau_set(result, 0, linprog->n + linprog->m, 0);

	/* copy the A matrix */
	for (int i = 0; i < linprog->m; i++) {
		for (int j = 0; j < linprog->n; j++) {
			simplex_tableau_set(result, i + 1, j,
				linprog_a_get(linprog, i, j));
		}
	}

	/* add the identity matrix */
	for (int i = 0; i <= linprog->m; i++) {
		for (int j = 0; j < linprog->m; j++) {
			simplex_tableau_set(result, i + 1, linprog->n + j,
				(j == i) ? 1 : 0);
		}
	}

	/* add the right hand side */
	for (int i = 1; i <= linprog->m; i++) {
		simplex_tableau_set(result, i, linprog->n + linprog->m,
			linprog->b[i - 1]);
	}

	/* set the flags row */
	for (int i = 0; i <= result->n; i++) {
		result->flags[i] = (i < linprog->m) ? 1 : 0;
	}

	/* return the simplex tableau */
	return result;
}

/**
 * \brief Run the Simplex algorithm
 */
int	simplex_tableau_compute(simplex_tableau_t *t) {
	int	pivoti, pivotj;
	int	rc = 0;
	int	stepcounter = 0;
	do {
		stepcounter++;
		rc = simplex_tableau_findpivot(t, &pivoti, &pivotj);
		if (rc > 0) {
			simplex_tableau_pivot(t, pivoti, pivotj);
			if (simplex_debug) {
				printf("step %d:\n", stepcounter);
				simplex_tableau_show(stdout, t);
			}
		}
	} while (rc > 0);
	return rc;
}
