/*
 * simplexocl.c -- simplex step implementation 
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <simplexocl.h>
#include <globals.h>
#include <simplex.h>

simplexocl_t	*simplexocl_setup() {
	simplexocl_t	*result = calloc(1, sizeof(simplexocl_t));
	// get platform, also creates the context
	clu_platform_t	*platform = clu_getplatform(0);
	// get device, also creates the command queue
	result->device = clu_getdevice(platform, 0);
	// create a new compiler
	result->compiler = clu_newcompiler(result->device->platform->context,
		result->device->device_id);
	result->compiler->path = ".";
	// compile the kernel
	clu_compile(result->compiler, "eliminate.cl");
	return result;
}

/**
 * \brief perform the elimination step
 *
 * This operation calls the eliminate kernel
 */
void	simplexocl_eliminate(simplexocl_t *simplexocl,
	simplex_tableau_t *st, int row, int col) {
	int	pivot[3] = { row, col, st->m + st->n + 1 };

	/* work unit dimensions */

	int	size = (st->m + 1) * (st->m + st->n + 1); // total data size

	/* create parameter buffers for arguments */
	cl_int	err = CL_SUCCESS;
	
	cl_mem	simplex_tableau = clCreateBuffer(
		simplexocl->device->platform->context,
		CL_MEM_READ_WRITE, sizeof(double) * size,
		NULL, &err);
	if (CL_SUCCESS != err) {
		fprintf(stderr, "cannot create matrix buffer\n");
		exit(EXIT_FAILURE);
	}
	cl_mem	simplex_pivot = clCreateBuffer(
		simplexocl->device->platform->context,
		CL_MEM_READ_ONLY, sizeof(int) * 3, NULL, &err);
	if (CL_SUCCESS != err) {
		fprintf(stderr, "cannot create pivot buffer\n");
		exit(EXIT_FAILURE);
	}
	if (debug) {
		fprintf(stderr, "%s:%d: parameters created\n",
			__FILE__, __LINE__);
	}

	/* set kernel arguments */
	err = clSetKernelArg(simplexocl->compiler->dividerow, 0, sizeof(cl_mem),
		&simplex_tableau);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot set kernel argument 0\n");
		exit(EXIT_FAILURE);
	}
	err = clSetKernelArg(simplexocl->compiler->dividerow, 1, sizeof(cl_mem),
		&simplex_pivot);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot set kernel argument 1\n");
		exit(EXIT_FAILURE);
	}

	err = clSetKernelArg(simplexocl->compiler->eliminate, 0, sizeof(cl_mem),
		&simplex_tableau);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot set kernel argument 0\n");
		exit(EXIT_FAILURE);
	}
	err = clSetKernelArg(simplexocl->compiler->eliminate, 1, sizeof(cl_mem),
		&simplex_pivot);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot set kernel argument 1\n");
		exit(EXIT_FAILURE);
	}

	/* enqueue parameter buffers */
	err = clEnqueueWriteBuffer(simplexocl->device->queue, simplex_tableau,
		CL_TRUE, 0, sizeof(double) * size, st->t, 0, NULL, NULL);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot send tableau buffer\n");
		exit(EXIT_FAILURE);
	}
	err = clEnqueueWriteBuffer(simplexocl->device->queue, simplex_pivot,
		CL_TRUE, 0, sizeof(int) * 3, pivot, 0, NULL, NULL);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot send pivot buffer\n");
		exit(EXIT_FAILURE);
	}

	/* launch the dividerow kernel */
	size_t	local_dividerow[1] = { 1 };
	size_t	global_dividerow[1] = { st->m + st->n + 1 };
	err = clEnqueueNDRangeKernel(simplexocl->device->queue,
		simplexocl->compiler->dividerow, 1, NULL,
		global_dividerow, local_dividerow,
		0, NULL, NULL);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot enqueue dividerow kernel\n");
		exit(EXIT_FAILURE);
	}

	/* launch the elminiate kernel */
	size_t	local_eliminate[2] = { 1, 1 };
	size_t	global_eliminate[2] = { st->m + 1, st->m + st->n + 1 };
	err = clEnqueueNDRangeKernel(simplexocl->device->queue,
		simplexocl->compiler->eliminate, 2, NULL,
		global_eliminate, local_eliminate,
		0, NULL, NULL);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot enqueue eliminate kernel\n");
		exit(EXIT_FAILURE);
	}

	/* await completion */
	err = clFinish(simplexocl->device->queue);
	if (CL_SUCCESS != err) {
		clu_perror(err, "kernel execution error\n");
		exit(EXIT_FAILURE);
	}

	/* read back the result buffer */
	err = clEnqueueReadBuffer(simplexocl->device->queue, simplex_tableau,
		CL_TRUE, 0, sizeof(double) * size, st->t, 0, NULL, NULL);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot read result buffer\n");
		exit(EXIT_FAILURE);
	}

	/* fix pivot column */
	for (int i = 0; i <= st->m; i++) {
		simplex_tableau_set(st, i, col, (i == row) ? 1 : 0);
	}
}

/**
 * \brief Simplex algorithm exchange step with pivot element (row,col)
 *
 */
void	simplexocl_tableau_pivot(simplexocl_t *simplexocl,
		simplex_tableau_t *st, int row, int col) {
	/* find out where the other column is by looking for 1 in the
	   the row */
	int	xi = simplex_tableau_find(st, row, col);
	if (xi < 0) {
		fprintf(stderr, "data inconsistency found\n");
		exit(EXIT_FAILURE);
		return;
	}
	st->flags[xi] = 1;
	st->flags[col] = 0;

	/* perform the exchange step */
	simplexocl_eliminate(simplexocl, st, row, col);
}

/**
 * \brief Run the Simplex algorithm
 */
int	simplexocl_tableau_compute(simplexocl_t *simplexocl,
		simplex_tableau_t *t) {
	int	pivoti, pivotj;
	int	rc = 0;
	int	stepcounter = 0;
	do {
		stepcounter++;
		rc = simplex_tableau_findpivot(t, &pivoti, &pivotj);
		if (rc > 0) {
			simplexocl_tableau_pivot(simplexocl, t, pivoti, pivotj);
			if (simplex_debug) {
				printf("step %d:\n", stepcounter);
				simplex_tableau_show(stdout, t);
			}
		}
	} while (rc > 0);
	return rc;
}

void	simplexocl_cleanup(simplexocl_t *simplexocl) {
	clu_freecompiler(simplexocl->compiler);
	simplexocl->compiler = NULL;
	clu_platform_t	*platform = simplexocl->device->platform;
	clu_releasedevice(simplexocl->device);
	simplexocl->device = NULL;
	clu_releaseplatform(platform);
}
