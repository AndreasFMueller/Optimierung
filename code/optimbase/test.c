/*
 * test.c -- auxiliary program to test optimization function framework
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <optim.h>

int	main(int argc, char *argv[]) {
	int	c;
	char	*libraryname = NULL;
	char	*symbolname = NULL;

	/* parse command line */
	while (EOF != (c = getopt(argc, argv, "l:s:")))
		switch (c) {
		case 'l':
			libraryname = optarg;
			break;
		case 's':
			symbolname = optarg;
			break;
		}

	/* verify that we have library and symbol name */
	if (NULL == libraryname) {
		fprintf(stderr, "no library specified, use -l flag\n");
		exit(EXIT_FAILURE);
	}
	if (NULL == symbolname) {
		fprintf(stderr, "no symbol name specified, use -s flag\n");
		exit(EXIT_FAILURE);
	}

	/* open the library */
	optim_t	*o = optim_open(libraryname, symbolname);
	if (NULL == o) {
		fprintf(stderr, "cannot get symbol %s in library %s\n",
			symbolname, libraryname);
		exit(EXIT_FAILURE);
	}
	printf("opened library %s, symbol %s @ %p\n", libraryname,
		symbolname, o);

	/* get a point in the middle of the domain */
	double	*x = (double *)calloc(o->n, sizeof(double));
	int	i;
	if ((o->min != NULL) && (o->max != NULL)) {
		for (i = 0; i < o->n; i++) {
			x[i] = (o->min[i] + o->max[i]) / 2;
		}
	} else {
		for (i = 0; i < o->n; i++) {
			x[i] = 0;
		}
	}

	/* evaluate the function at that point */
	printf("f(");
	for (i = 0; i < o->n; i++) {
		if (i > 0) { printf (", "); }
		printf("%f", x[i]);
	}
	printf(") = %f\n", o->f(o, x));

	/* that's it */
	exit(EXIT_SUCCESS);
}
