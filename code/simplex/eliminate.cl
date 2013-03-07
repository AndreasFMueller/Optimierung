/*
 * eliminate.cl -- OpenCL implementation of the Simplex Algorithms Pivot
 *                 Operation
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#pragma OPENCL EXTENSION cl_khr_fp64  : enable
#pragma OPENCL EXTENSION cl_amd_fp64  : enable

/* if USE_DOUBLE16 is defined, the kernel uses vector operations which
   hopefully are faster than scalar operations. However, this does not
   seem to give any advantage on CPU based OpenCL implementations */
#undef USE_DOUBLE16

#define	A(i,j)	a[i * pivot[2] + j]

__kernel void	dividerow(__global double *a, __global int *pivot) {
	int	j = get_global_id(0);
	if (j != pivot[1]) {
		A(pivot[0], j) = A(pivot[0], j) / A(pivot[0], pivot[1]);
	}
}

__kernel void	eliminate(__global double *a, __global int *pivot) {
	int	i = get_global_id(0);
	if (i == pivot[0]) {
		return;
	}

	/* we will go through one row using index j */
	int	j = 0;
	double	m = -A(i, pivot[1]); /* pivot element value */

	/* we compute a pointer to the beginning of both rows, because
	   this is faster than recomputing the offset every time */
	__global double	*row = &A(i, 0);
	__global double	*pivotrow = &A(pivot[0], 0);

#ifdef USE_DOUBLE16
	/* we need a vector version of the pivot element */
	double16	mv = m;
	double	s = 0;

	/* go through the pivot row at stride 16, using double16 vector
	   operations, which can in principle be parallelized */
	while ((j + 15) < pivot[2]) {
		double16	pivotrowv = vload16(0, pivotrow);
		double16	rowv = vload16(0, row);
		vstore16(fma(mv, pivotrowv, rowv), 0, row);
		row += 16;
		pivotrow += 16;
		j += 16;
	}
#endif /* USE_DOUBLE16 */

	/* there isn't enought data to do vector operations, so we
	   finish off the row with vector operations */
	while (j < pivot[2]) {
		*row = fma(m, *pivotrow, *row);
		row++;
		pivotrow++;
		j++;
	}

	/* we still have to fix the pivot column elements */
	A(i, pivot[1]) = 0;
}

