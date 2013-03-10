/*
 * test3.c -- test program for system with origin not in admissible domain
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <stdio.h>
#include <linprog.h>
#include <simplex.h>
#include <getopt.h>
#include <globals.h>
#include <backend.h>
#include <simplexcpu.h>

int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "dtb:")))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 't':
			simplex_debug = 1;
			break;
		case 'b':
			if (backend_select(optarg) < 0) {
				fprintf(stderr, "cannot select backend %s\n",
					optarg);
			}
			break;
		}

	/* if there is no backend selected, we select the CPU backend */
	if (NULL == backend_current()) {
		if (debug) {
			fprintf(stderr, "%s:%d: loading CPU backend vers %d\n",
				__FILE__, __LINE__, simplexcpu_version);
		}
		if (backend_select("CPU") < 0) {
			fprintf(stderr, "cannt select CPU backend");
			exit(EXIT_FAILURE);
		}
	}

	/* initialize the backend */
	if (debug) {
		fprintf(stderr, "%s:%d: initializing backend\n",
		__FILE__, __LINE__);
	}
	backend_init();

	/* first set up the linear Problem from the example 2.6.2 in 
	   the script:

		 x1        <=  2
		        x2 <=  1
		-x1 - 2 x2 <= -2
	 */
	linprog_t	*linprog = linprog_new(3, 2);
	linprog->c[0] = 2;
	linprog->c[1] = -1;

	linprog_a_set(linprog, 0, 0, 1); // a_11
	linprog_a_set(linprog, 0, 1, 0); // a_12
	linprog_a_set(linprog, 1, 0, 0); // a_21
	linprog_a_set(linprog, 1, 1, 1); // a_22
	linprog_a_set(linprog, 2, 0, -1); // a_31
	linprog_a_set(linprog, 2, 1, -2); // a_32

	linprog->b[0] = 2;	// b_1
	linprog->b[1] = 1;	// b_2
	linprog->b[2] = -2;	// b_3

	printf("original simplex problem\n");
	linprog_show(stdout, linprog);

	/* build the linear program to find the initial point */
	linprog_t	*initial = linprog_initial(linprog);
	linprog_changesign(initial);

	printf("modified problem for initial vertex:\n");
	linprog_show(stdout, initial);

	/* create a simplex tableau from the linear problem */
	printf("simplex tableau with slack variables:\n");
	simplex_tableau_t	*tableau = simplex_tableau_linprog(initial);
	simplex_tableau_show(stdout, tableau);

	printf("solving the simplex tableau:\n");
	simplex_tableau_compute(tableau);
	printf("final solution:\n");
	simplex_tableau_show(stdout, tableau);

	exit(EXIT_SUCCESS);
}
