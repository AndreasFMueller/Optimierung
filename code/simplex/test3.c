/*
 * test3.c -- 
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <stdio.h>
#include <linprog.h>
#include <simplex.h>

int	main(int argc, char *argv[]) {
	linprog_t	*linprog = linprog_new(3, 2);
	linprog->c[0] = 2;
	linprog->c[1] = -1;

	linprog_a_set(linprog, 0, 0, 1);
	linprog_a_set(linprog, 0, 1, 0);
	linprog_a_set(linprog, 1, 0, 0);
	linprog_a_set(linprog, 1, 1, 1);
	linprog_a_set(linprog, 2, 0, -1);
	linprog_a_set(linprog, 2, 1, -2);

	linprog->b[0] = 2;
	linprog->b[1] = 1;
	linprog->b[2] = -2;

	linprog_show(stdout, linprog);

	linprog_t	*initial = linprog_initial(linprog);
	linprog_changesign(initial);

	linprog_show(stdout, initial);

	simplex_tableau_t	*tableau = simplex_tableau_linprog(initial);
	simplex_tableau_show(stdout, tableau);

	simplex_debug = 1;

	simplex_tableau_compute(tableau);
	simplex_tableau_show(stdout, tableau);

	exit(EXIT_SUCCESS);
}
