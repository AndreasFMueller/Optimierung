/*
 * optim.h -- common declarations for optimization problems
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _optim_h
#define _optim_h

struct optim_s;
typedef double	(*target_function_t)(struct optim_s *optim, double *x);

typedef struct optim_s {
	int	n;	/* dimension */
	double	*min;	/* minimum allowed coordinate values */
	double	*max;	/* maximum allowed coordinate values */
	target_function_t	f;
	int	needs_deallocation;
} optim_t;

extern optim_t	*optim_new(int n, target_function_t f);
extern void	optim_free(optim_t *optim);

/* open dynamically loadable library and get a symbol from it */
extern optim_t	*optim_open(const char *libraryname, const char *symbolname);

/* XXX we ave a small resource leak in this API: we cannot unload a library */

#endif /* _optim_h */
