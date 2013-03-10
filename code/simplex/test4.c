/*
 * test4.c -- test program for system with origin not in admissible domain,
 *            dual problem
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

	/* first set up the linear Problem from 2.7 in the script:

	minimize Z = 12 m1 + 18 m2 + 28 m3

	   -  m1 +   m2 + 4 m3 -    m4       <=  2
	    3 m1 + 3 m2 +   m3         -  m5 <=  3
	      m1 -   m2 + 4 m3 +    m4       <= -2
	   -3 m1 - 3 m2 -   m3         +  m5 <= -3

	 */
	linprog_t	*linprog = linprog_new(4, 5);
	linprog->c[0] = -12; // negative sign for minimization
	linprog->c[1] = -18;
	linprog->c[2] = -28;

	linprog_a_set(linprog, 0, 0, -1);
	linprog_a_set(linprog, 0, 1,  1);
	linprog_a_set(linprog, 0, 2,  4);
	linprog_a_set(linprog, 0, 3, -1);
	linprog_a_set(linprog, 0, 4,  0);

	linprog_a_set(linprog, 1, 0,  3);
	linprog_a_set(linprog, 1, 1,  3);
	linprog_a_set(linprog, 1, 2,  1);
	linprog_a_set(linprog, 1, 3,  0);
	linprog_a_set(linprog, 1, 4, -1);

	linprog_a_set(linprog, 2, 0,  1);
	linprog_a_set(linprog, 2, 1, -1);
	linprog_a_set(linprog, 2, 2, -4);
	linprog_a_set(linprog, 2, 3,  1);
	linprog_a_set(linprog, 2, 4,  0);

	linprog_a_set(linprog, 3, 0, -3);
	linprog_a_set(linprog, 3, 1, -3);
	linprog_a_set(linprog, 3, 2, -1);
	linprog_a_set(linprog, 3, 3,  0);
	linprog_a_set(linprog, 3, 4,  1);

	linprog->b[0] =  2;	// b_1
	linprog->b[1] =  3;	// b_2
	linprog->b[2] = -2;	// b_3
	linprog->b[3] = -3;	// b_4

	printf("original simplex problem\n");
	linprog_show(stdout, linprog);

	/* build the linear program to find the initial point */
	linprog_t	*initial = linprog_initial(linprog);
	linprog_changesign(initial);

	printf("modified problem for initial vertex:\n");
	linprog_show(stdout, initial);

	/* create a simplex tableau from the linear problem */
	printf("simplex tableau with slack variables:\n");
	simplex_tableau_t	*inittableau = simplex_tableau_linprog(initial);
	simplex_tableau_show(stdout, inittableau);

	printf("solving the simplex tableau:\n");
	simplex_tableau_compute(inittableau);
	printf("final solution:\n");
	simplex_tableau_show(stdout, inittableau);

	/* now apply the initial vertex tableau to the tableau
	   of the original problem */
	printf("creating the simplex tableau for the original problem\n");
	simplex_tableau_t	*tableau = simplex_tableau_linprog(linprog);
	simplex_tableau_apply(tableau, inittableau);
	printf("original tablea with with initial vertex applied\n");
	simplex_tableau_show(stdout, tableau);

	/* now perform the ordinary simplex algorithm to find the optimum */
	simplex_tableau_compute(tableau);
	simplex_tableau_show(stdout, tableau);

	exit(EXIT_SUCCESS);
}
