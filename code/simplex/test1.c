/*
 * test1.c -- test program running basic Simplex algorithm
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <simplex.h>
#include <simplexcpu.h>
#include <globals.h>
#include <backend.h>
#include <getopt.h>

int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "db:t")))
		switch (c) {
		case 'd':
			debug++;
			break;
		case 'b':
			if (backend_select(optarg) < 0) {
				fprintf(stderr, "cannot select backend %s\n",
					optarg);
			}
			break;
		case 't':
			simplex_debug = 1;
			break;
		}

	/* if there is no backend selected, we select the CPU backend */
	if (NULL == backend_current()) {
		if (debug) {
			fprintf(stderr, "%s:%d: loading CPU backend vers %d\n",
				__FILE__, __LINE__, simplexcpu_version);
		}
		backend_select("CPU");
	}

	/* initialize the backend */
	if (debug) {
		fprintf(stderr, "%s:%d: initializing backend\n",
		__FILE__, __LINE__);
	}
	backend_init();

	/* prepare the simplex tableau */
	if (debug) {
		fprintf(stderr, "%s:%d: setting up sample simplex tableau\n",
			__FILE__, __LINE__);
	}
	simplex_tableau_t	*tableau = simplex_tableau_new(3, 2);
	simplex_tableau_set(tableau, 0, 0, 2);
	simplex_tableau_set(tableau, 0, 1, 3);
	simplex_tableau_set(tableau, 0, 5, 0);

	simplex_tableau_set(tableau, 1, 0, -1);
	simplex_tableau_set(tableau, 1, 1, 3);
	simplex_tableau_set(tableau, 1, 5, 12);

	simplex_tableau_set(tableau, 2, 0, 1);
	simplex_tableau_set(tableau, 2, 1, 3);
	simplex_tableau_set(tableau, 2, 5, 18);

	simplex_tableau_set(tableau, 3, 0, 4);
	simplex_tableau_set(tableau, 3, 1, 1);
	simplex_tableau_set(tableau, 3, 5, 28);

	simplex_tableau_show(stdout, tableau);

	/* perform the simplex computation */
	simplex_tableau_compute(tableau);

	simplex_tableau_show(stdout, tableau);

	/* release the backend */
	backend_release();

	exit(EXIT_SUCCESS);
}
