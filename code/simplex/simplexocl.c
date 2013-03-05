/*
 * simplexocl.c -- simplex step implementation 
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <simplexocl.h>
#include <globals.h>
#include <simplex.h>
#include <backend.h>

int	simplexocl_version = 1;

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
		CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(double) * size,
		st->t, &err);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot create matrix buffer\n");
		exit(EXIT_FAILURE);
	}
	cl_mem	simplex_pivot = clCreateBuffer(
		simplexocl->device->platform->context,
		CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * 3,
		pivot, &err);
	if (CL_SUCCESS != err) {
		clu_perror(err, "cannot create pivot buffer\n");
		exit(EXIT_FAILURE);
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
		simplexocl->compiler->eliminate, 1, NULL,
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

	/* fix the pivot element, we cannot do this in OpenCL because
	   it would introduce a race condition */
	simplex_tableau_set(st, row, col, 1);
}

/**
 * \brief Simplex algorithm exchange step with pivot element (row,col)
 *
 */
int	simplexocl_pivot(backend_t *backend,
		simplex_tableau_t *st, int row, int col) {
	simplexocl_t	*simplexocl = (simplexocl_t *)backend->private_data;

	/* perform the exchange step */
	simplexocl_eliminate(simplexocl, st, row, col);
	return 0;
}

/**
 * \brief Cleanup of the OpenCL backendCleanup of the OpenCL backend
 */
void	simplexocl_cleanup(simplexocl_t *simplexocl) {
	clu_freecompiler(simplexocl->compiler);
	simplexocl->compiler = NULL;
	clu_platform_t	*platform = simplexocl->device->platform;
	clu_releasedevice(simplexocl->device);
	simplexocl->device = NULL;
	clu_releaseplatform(platform);
}

/*
 * The functions below integrate the OpenCL backend into the backend
 * infrastructre
 */
int	simplexocl_init(backend_t *backend) {
	if (NULL != backend->private_data) {
		return -1;
	}
	backend->private_data = simplexocl_setup();
	if (NULL == backend->private_data) {
		return -1;
	}
	return 0;
}

int	simplexocl_release(backend_t *backend) {
	simplexocl_t	*simplexocl = (simplexocl_t *)backend->private_data;
	simplexocl_cleanup(simplexocl);
	backend->private_data = NULL;
}

backend_t	simplexocl_backend = {
	.private_data = NULL,
	.init = simplexocl_init,
	.release = simplexocl_release,
	.pivot = simplexocl_pivot
};

BACKEND_REGISTER(simplexocl, "OpenCL")

#if 0

static void	simplexocl_register() __attribute__ ((constructor));
static void	simplexocl_register() {
	if (backend_register("OpenCL", &simplexocl_backend)) {
		fprintf(stderr, "could not register OpenCL backend\n");
	}
	fprintf(stderr, "OpenCL backend registered\n");
}

#endif
