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
#include <globals.h>

int	simplex_debug = 0;

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
	if (debug) {
		fprintf(stderr, "%s:%d: allocating array of %d doubles\n",
			__FILE__, __LINE__, size);
	}
	st->t = (double *)calloc(size, sizeof(double));
	for (int i = 0; i < m; i++) {
		simplex_tableau_set(st, 1 + i, n + i, 1.);
	}
	st->flags = (int *)calloc(m + n + 1, sizeof(int));
	for (int i = 0; i < n; i++) {
		st->flags[i] = 1;
	}
	if (debug) {
		fprintf(stderr, "%s:%d: simplex_tableau_t allocated at %p\n",
			__FILE__, __LINE__, st);
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

double	simplex_tableau_b(simplex_tableau_t *st, int row) {
	return st->t[(row + 2) * (st->m + st->n + 1) - 1];
}

double	simplex_tableau_c(simplex_tableau_t *st, int col) {
	return st->t[col];
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
	if (debug) {
		fprintf(stderr, "%s:%d: creating new %d x %d simplex tableau\n",
			__FILE__, __LINE__, linprog->m, linprog->n);
	}
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
	for (int i = 0; i < linprog->m; i++) {
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
		result->flags[i] = (i < linprog->n) ? 1 : 0;
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
				printf("step %d with pivot %d,%d:\n",
					stepcounter, pivoti, pivotj);
				simplex_tableau_show(stdout, t);
			}
		}
	} while (rc > 0);
	return rc;
}

/**
 * \brief Apply flags from initial problem to original problem
 *
 * The function linprog_initial converted a linear program into a new one
 * that could be solved with the basic simplex algorithm and would return
 * an initial vertex. The vertex is expressed by setting a certain number
 * of variables of the simplex tableau of the original problem to zero.
 * This function translates this information into the simplex tableau
 * of the original problem, and ensures that the correct start situation
 * for the subsequent simplex exchange steps is established.
 */
void	simplex_tableau_apply(simplex_tableau_t *original,
		simplex_tableau_t *initial) {
	int	n = original->n;
	int	m = original->m;
	/* count the number of negative right hand sides */
	int	negative = 0;
	for (int i = 0; i < m; i++) {
printf("row[%d] = %f\n", i, simplex_tableau_b(original, i));
		if (simplex_tableau_b(original, i) < 0) {
			negative++;
		}
	}
	if (debug) {
		fprintf(stderr, "%s:%d: number of negative rhs: %d\n",
			__FILE__, __LINE__, negative);
	}

	/* now copy the flags to the original tableau: 
	   index 0 to n-1         to    0 to n-1
	   index n + negative     to    n to n + m	*/
	for (int i = 0; i < n; i++) {
		original->flags[i] = initial->flags[i];
	}
	for (int i = 0; i < m; i++) {
		original->flags[n + i] = initial->flags[n + negative + i];
	}

	/* eliminate all but the flagged columns */
	simplex_tableau_reduce(original);
}

void	simplex_tableau_rowexchange(simplex_tableau_t *st, int row1, int row2) {
	for (int j = 0; j < st->m + st->n + 1; j++) {
		double	v1 = simplex_tableau_get(st, row1, j);
		double	v2 = simplex_tableau_get(st, row2, j);
		simplex_tableau_set(st, row1, j, v2);
		simplex_tableau_set(st, row2, j, v1);
	}
}

void	simplex_tableau_reduce(simplex_tableau_t *st) {
simplex_tableau_show(stderr, st);
fprintf(stderr, "-------------------------------------------\n");
	/* forward reduction */
	int	*pivotcol = (int *)alloca(st->m * sizeof(int));
	int	currentcolumn = -1;
	for (int i = 0; i < st->m; i++) {
		/* find a pivot element, first determine the column */
		do {
			currentcolumn++;
		} while ((1 == st->flags[currentcolumn])
			&& (currentcolumn < (st->m + st->n)));
		pivotcol[i] = currentcolumn;

		/* now find a suitable row from rows i + 1 to m */
		int	k = i;
		double	pivotvalue;
		do {
			k++;
			pivotvalue = simplex_tableau_get(st, k, currentcolumn);
		} while ((k <= st->m) && (pivotvalue == 0));
		if (k > st->m) {
			fprintf(stderr, "reduction failed\n");
			simplex_tableau_show(stderr, st);
			exit(EXIT_FAILURE);
		}

		/* exchange the pivot row with the current row */
		if (k != i + 1) {
			simplex_tableau_rowexchange(st, i + 1, k);
		}

		/* perform a reduction */
		for (int j = 0; j < st->n + st->m + 1; j++) {
			simplex_tableau_set(st, k, j,
				simplex_tableau_get(st, k, j) / pivotvalue);
		}
		if (debug) {
			simplex_tableau_show(stderr, st);
		}
		for (int l = k + 1; l <= st->m; l++) {
			double	f = simplex_tableau_get(st, l, currentcolumn);
			for (int j = 0; j < st->n + st->m + 1; j++) {
				double	a = simplex_tableau_get(st, l, j);
				a -= f * simplex_tableau_get(st, k, j);
				simplex_tableau_set(st, l, j, a);
			}
		}
		if (debug) {
			simplex_tableau_show(stderr, st);
		}
	}

	/* backward insertion */
fprintf(stderr, "-------------------------------------------\n");
	for (int i = st->m - 1; i >= 0; i--) {
		int	k = pivotcol[i];
		if (debug) {
			fprintf(stderr, "%s:%d: backwards: i = %d, k = %d\n",
				__FILE__, __LINE__, i, k);
		}
		for (int l = 0; l <= i; l++) {
			double	f = simplex_tableau_get(st, l, k);
			for (int j = 0; j < st->n + st->m + 1; j++) {
				double	a = simplex_tableau_get(st, l, j);
				a -= f * simplex_tableau_get(st, i + 1, j);
				simplex_tableau_set(st, l, j, a);
			}
		}
		if (debug) {
			simplex_tableau_show(stderr, st);
		}
	}
}
