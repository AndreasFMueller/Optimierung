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

/**
 * \brief perform the exchange step at pivot (row, col)
 */
static int	simplexcpu_pivot(backend_t *backend,
			simplex_tableau_t *st, int row, int col) {
	/* get the pivot element value */
	double	pivot = simplex_tableau_get(st, row, col);

	/* divide the pivot row by the pivot element */
	int	rowlength = st->n + st->m + 1;
	int	offset = row * rowlength;
	double	*pivotrow = &st->t[offset];
	for (int j = 0; j < rowlength; j++, pivotrow++) {
		*pivotrow /= pivot;
	}
	
	/* subtract suitable multiples of the pivot row from all other
	   rows. */
	double	*x = &st->t[offset]; /* pivot row */
	for (int i = 0; i <= st->m; i++) {
		/* skip the pivot row */
		if (i == row) {
			continue;
		}

		/* we use two pointers when doing the row operation,
		   this allows us to do without an address computation,
		   and is slightly faster */
		pivotrow = &st->t[offset];
		double	*row = &st->t[i * rowlength];
		double	a = row[col];
		int	j = 0;
		while (j < rowlength) {
			*row -= a * *pivotrow;
			row++;
			pivotrow++;
			j++;
		}
	}
	return 0;
}

static backend_t	simplexcpu_backend = {
	.private_data = NULL,
	.init = NULL,
	.release = NULL,
	.pivot = simplexcpu_pivot,
	.option = NULL
};

BACKEND_REGISTER(simplexcpu, "CPU")

