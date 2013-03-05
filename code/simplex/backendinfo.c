/*
 * backendinfo.c -- display info about included backends
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <backend.h>
#include <stdio.h>
#include <stdlib.h>
#include <simplexcpu.h>
#include <globals.h>
#include <getopt.h>

int	main(int argc, char *argv[]) {
	int	c;
	while (EOF != (c = getopt(argc, argv, "d")))
		switch (c) {
		case 'd':
			debug++;
			break;
		}

	if (debug) {
		fprintf(stderr, "%s:%d: CPU version: %d\n", __FILE__, __LINE__,
			simplexcpu_version);
	}
	printf("idx name             init release pivot\n");
	printf("--- ---------------- ---- ------- -----\n");
	for (int i = 0; i < backend_count(); i++) {
		backend_t	*backend = backend_backend(i);
		printf("[%1d] %-16.16s   %c     %c      %c\n",
			i, backend_name(i),
			(backend->init) ? 'x' : ' ',
			(backend->release) ? 'x' : ' ',
			(backend->pivot) ? 'x' : ' ');
	}

	exit(EXIT_FAILURE);
}
