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

/* function types for the backend methods */
typedef int (*backend_init_func)(struct backend_s *);
typedef int (*backend_release_func)(struct backend_s *);
typedef int (*backend_pivot_func)(struct backend_s *, simplex_tableau_t *, int, int);

typedef struct backend_s {
	void	*private_data;
	backend_init_func	init;
	backend_release_func	release;
	backend_pivot_func	pivot;
} backend_t;

/* backend selection and use */
extern int	backend_select(const char *name);
extern const char	*backend_current();
extern int	backend_init();
extern int	backend_release();
extern int	backend_pivot(simplex_tableau_t *st, int row, int col);

/* listing backends */
extern int	backend_count();
extern char	*backend_name(int backendindex);
extern backend_t	*backend_backend(int backendindex);

/* backend registration */
extern int	backend_register(const char *name, backend_t *backend);
extern int	backend_unregister(const char *name);

#define	BACKEND_REGISTER(backend, name)					\
static void	backend##_register() __attribute__ ((constructor));	\
static void	backend##_register() {					\
	if (backend_register(name, &backend##_backend) < 0) {	\
		fprintf(stderr, "cannot register %s backend", name);	\
		exit(EXIT_FAILURE);					\
	}								\
}

#endif /* _backend_h */
