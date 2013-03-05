/*
 * simplexcpu.c -- CPU backend for the Simplex Algorithm
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <simplexcpu.h>
#include <backend.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>

int simplexcpu_version = 1;

static int	simplexcpu_pivot(backend_t *backend,
			simplex_tableau_t *st, int row, int col) {
	/* get the pivot element value */
	double	pivot = simplex_tableau_get(st, row, col);

	/* divide the pivot row by the pivot element */
	int	offset = row * (st->n + st->m + 1);
	for (int i = 0; i < st->n + st->m + 1; i++) {
		st->t[offset + i] /= pivot;
	}
	
	/* subtract suitable multiples of the pivot row from all other
	   rows. */
	double	*x = &st->t[offset]; /* pivot row */
	for (int i = 0; i <= st->m; i++) {
		/* skip the pivot row */
		if (i == row) {
			continue;
		}
		double	a = simplex_tableau_get(st, i, col);
		for (int j = 0; j < st->m + st->n + 1; j++) {
			simplex_tableau_set(st, i, j,
				simplex_tableau_get(st, i, j) - x[j] * a);
		}
	}
	return 0;
}

static backend_t	simplexcpu_backend = {
	.private_data = NULL,
	.init = NULL,
	.release = NULL,
	.pivot = simplexcpu_pivot
};

static void	simplexcpu_register() __attribute__ ((constructor));
static void	simplexcpu_register() {
	if (backend_register("CPU", &simplexcpu_backend) < 0) {
		fprintf(stderr, "cannot register CPU backend\n");
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "CPU backend registered\n");
}
