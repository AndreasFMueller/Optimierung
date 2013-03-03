/*
 * linprog.c -- implement linprog algorithm
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 * $Id$
 */
#define _GNU_SOURCE

#include <linprog.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create a new linear program structure
 */
linprog_t	*linprog_new(int m, int n) {
	linprog_t	*result = calloc(1, sizeof(linprog_t));
	result->a = calloc(m * n, sizeof(double));
	result->b = calloc(m, sizeof(double));
	result->c = calloc(n, sizeof(double));
	result->m = m;
	result->n = n;
	return result;
}

/**
 * \brief Free the linear programm structure with all the attached arrays
 */
void	linprog_free(linprog_t *linprog) {
	if (linprog->a) {
		free(linprog->a);
	}
	if (linprog->b) {
		free(linprog->b);
	}
	if (linprog->c) {
		free(linprog->c);
	}
	memset(linprog, 0, sizeof(linprog_t));
	free(linprog);
}

/**
 * \brief get a matrix element from a Simplex problem
 */
double	linprog_a_get(linprog_t *linprog, int i, int j) {
	return linprog->a[i * linprog->n + j];
}

/**
 * \brief set a matrix element in a linprog problem
 */
void	linprog_a_set(linprog_t *linprog, int i, int j, double a) {
	int	offset = i * linprog->n + j;
	linprog->a[i * linprog->n + j] = a;
}

/**
 * \brief Compute the dual linear program associated with a linear program.
 *
 * construct the dual linprog problem from the original problem
 */
linprog_t	*linprog_dual(linprog_t *linprog) {
	/* create a new linprog problem with the dimensions of the
	   dual problem */
	linprog_t	*result = linprog_new(2 * linprog->n, linprog->m);

	/* copy the coefficients in A and c to A and b in the
	   dual problem */
	for (int i = 0; i < linprog->n; i++) {
		result->b[2 * i + 0] = linprog->c[i];
		result->b[2 * i + 1] = -linprog->c[i];
		/* for every column in the original problem, we copy
		   two inequalities making up the equation from the
		   dual problem */
		for (int j = 0; j < linprog->m; j++) {
			linprog_a_set(result, 2 * i + 0, j,
				linprog_a_get(linprog, j, i));
			linprog_a_set(result, 2 * i + 1, j,
				-linprog_a_get(linprog, j, i));
		}
	}
	/* copy the right hand side vector, as it is the new
	   target function */
	memcpy(result->c, linprog->b, linprog->m * sizeof(double));
	return result;
}

/**
 * \brief Convert a general linear program into one that only 
 *
 * Convert the original linear program into a linear program that
 * only uses positive values of the variables.
 */
linprog_t	*linprog_positive(linprog_t *linprog) {
	linprog_t	*result = linprog_new(linprog->m, 2 * linprog->n);

	/* fill the A array and the target function coefficients */
	for (int i = 0; i < linprog->n; i++) {
		result->c[i] = linprog->c[i];
		result->c[i + linprog->n] = -linprog->c[i];
		for (int j = 0; j < linprog->m; j++) {
			linprog_a_set(result, i, j,
				linprog_a_get(linprog, i, j));
			linprog_a_set(result, i, j + linprog->n,
				-linprog_a_get(linprog, i, j));
		}
	}

	/* copy the right hand side of the inequalities */
	for (int j = 0; j < linprog->m; j++) {
		result->b[j] = linprog->b[j];
	}
}

/**
 * \brief Create a linear program to find an initial program
 */
linprog_t	*linprog_initial(linprog_t *linprog) {
	/* count the number of negative right hand sides */
	int	negative = 0;
	for (int i = 0; i < linprog->m; i++) {
		if (linprog->b[i] < 0) {
			negative++;
		}
	}

	/* create the new linprog structure */
	linprog_t	*result = linprog_new(linprog->m,
		linprog->n + negative);

	/* fill in the target function data */
	for (int j = 0; j < linprog->n; j++) {
		result->c[j] = 0;
		for (int i = 0; i < linprog->m; i++) {
			if (linprog->b[i] < 0) {
				result->c[j] += linprog_a_get(linprog, i, j);
			}
		}
	}
	for (int j = 0; j < negative; j++) {
		result->c[linprog->n + j] = 1;
	}

	/* fill in the coefficients */
	for (int i = 0; i < linprog->m; i++) {
		if (linprog->b[i] < 0) {
			for (int j = 0; j < linprog->n; j++) {
				linprog_a_set(result, i, j,
					-linprog_a_get(linprog, i, j));
			}
			for (int j = 0; j < negative; j++) {
				linprog_a_set(result, i, linprog->n + j, 1);
			}
			result->b[i] = -linprog->b[i];
		} else {
			for (int j = 0; j < linprog->n; j++) {
				linprog_a_set(result, i, j,
					linprog_a_get(linprog, i, j));
			}
			for (int j = 0; j < negative; j++) {
				linprog_a_set(result, i, linprog->n + j, 0);
			}
			result->b[i] = linprog->b[i];
		}
	}
	return result;
}

/**
 * \brief display a linear optimization  problem
 */
void	linprog_show(FILE *out, linprog_t *linprog) {
	for (int j = 0; j < linprog->n; j++) {
		fprintf(out, "%12.6f", linprog->c[j]);
	}
	fprintf(out, "\n");
	for (int i = 0; i < linprog->m; i++) {
		for (int j = 0; j < linprog->n; j++) {
			fprintf(out, "%12.6f", linprog_a_get(linprog, i, j));
		}
		fprintf(out, "    <= %12.6f\n", linprog->b[i]);
	}
}

/**
 * \brief Convert a Maximum problem into a minimum problem.
 */
void	linprog_changesign(linprog_t *linprog) {
	for (int i = 0; i <= linprog->n; i++) {
		linprog->c[i] = -linprog->c[i];
	}
}
