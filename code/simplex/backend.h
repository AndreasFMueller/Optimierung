/*
 * backend.h -- common definitions for the computational backend to use in
 *              simplex algorithm computations
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _backend_h
#define _backend_h

#include <simplex.h>

struct backend_s;

typedef int (*backend_init_func)(struct backend_s *);
typedef int (*backend_release_func)(struct backend_s *);
typedef int (*backend_pivot_func)(struct backend_s *, simplex_tableau_t *, int, int);

typedef struct backend_s {
	void	*private_data;
	backend_init_func	init;
	backend_release_func	release;
	backend_pivot_func	pivot;
} backend_t;

int	backend_select(const char *name);
const char	*backend_current();
int	backend_init();
int	backend_release();
int	backend_pivot(simplex_tableau_t *st, int row, int col);

/* backend registration */
int	backend_register(const char *name, backend_t *backend);
int	backend_unregister(const char *name);

#endif /* _backend_h */
