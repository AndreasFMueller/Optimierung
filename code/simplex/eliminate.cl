/*
 * eliminate.cl -- OpenCL implementation of the Simplex Algorithms Pivot
 *               Operation
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#define	A(i,j)	a[i * pivot[2] + j]

__kernel void	dividerow(__global double *a, __global int *pivot) {
	int	j = get_global_id(0);
	if (j != pivot[1]) {
		A(pivot[0], j) = A(pivot[0], j) / A(pivot[0], pivot[1]);
	}
}

__kernel void	eliminate(__global double *a, __global int *pivot) {
	int	i = get_global_id(0);
	int	j = get_global_id(1);
	if ((i != pivot[0]) && (j != pivot[1])) {
		A(i, j) -= A(pivot[0],j) * A(i,pivot[1]);
	}
}

