/*
 * simplexblas.c -- BLAS backend for the Simplex Algorithm
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <simplexblas.h>
#include <backend.h>
#include <stdio.h>
#include <stdlib.h>
#include <globals.h>

int simplexblas_version = 1;

extern void	dger_(int *m, int *n, double *alpha, double *x, int *incx,
		double *y, int *incy, double *a, int *lda);

static int	simplexblas_pivot(backend_t *backend,
			simplex_tableau_t *st, int row, int col) {
	/* get the pivot element value */
	double	pivot = simplex_tableau_get(st, row, col);

	/* divide the pivot row by the pivot element, there is no
	   BLAS operation to do this, so we do it ourselves */
	int	offset = row * (st->n + st->m + 1);
	for (int i = 0; i < st->n + st->m + 1; i++) {
		st->t[offset + i] /= pivot;
	}
	
	/* subtract suitable multiples of the pivot row from all other
	   rows. For this we first have to extract the pivot column */
	double	*x = &st->t[offset]; /* pivot row */
	int	incx = 1;
	double	*y = &st->t[col];
	int	incy = st->m + st->n + 1;

	/* we set the pivot element to zero. This has as a consequence
	   that the pivot row and pivot column are not modified by the
	   following operation. It is important to leave the pivot row
	   as it is, and for the pivot column there is no problem, as
	   we know what it should be after the operation, it does not
	   make sense to really compute the column */
	simplex_tableau_set(st, row, col, 0);

	/* use the BLAS function dger to perform the pivot operation */
	int	lda = st->m + st->n + 1;
	int	m = st->m + st->n + 1;
	int	n = st->m + 1;
	double	alpha = -1;
	dger_(&m, &n, &alpha, x, &incx, y, &incy, st->t, &lda);

	/* Set the pivot column, i.e. set all elements to 0 except
	   the pivot row, where we have to place a 1 */
	for (int i = 0; i <= st->m; i++) {
		simplex_tableau_set(st, i, col, 0);
	}
	simplex_tableau_set(st, row, col, 1);

	return 0;
}

static backend_t	simplexblas_backend = {
	.private_data = NULL,
	.init = NULL,
	.release = NULL,
	.pivot = simplexblas_pivot,
	.option = NULL
};

BACKEND_REGISTER(simplexblas, "BLAS")

