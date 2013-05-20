/*
 * optim.c -- common functions for optimization problems
 *
 * (c) 2013 Prof Dr Andreas Mueller
 */
#include <stdio.h>
#include <stdlib.h>
#include <optim.h>
#include <assert.h>
#include <string.h>
#include <dlfcn.h>

optim_t	*optim_new(int n, target_function_t f) {
	assert(n > 0);
	optim_t	*result = (optim_t *)calloc(1, sizeof(optim_t));
	result->n = n;
	result->min = (double *)calloc(n, sizeof(double));
	result->max = (double *)calloc(n, sizeof(double));
	result->f = f;
	result->needs_deallocation = 1;
	return result;
}

void	optim_free(optim_t *optim) {
	assert(NULL != optim);
	if (!optim->needs_deallocation) {
		return;
	}
	if (optim->max) { 
		free(optim->max);
		optim->max = NULL;
	}
	if (optim->min) { 
		free(optim->min);
		optim->min = NULL;
	}
	optim->f = NULL;
	optim->needs_deallocation = 0;
}

optim_t	*optim_open(const char *libraryname, const char *symbolname) {
	/* use dlopen to open the library */
	void	*handle = dlopen(libraryname, RTLD_NOW);
	if (NULL == handle) {
		fprintf(stderr, "cannot open library %s: %s\n",
			libraryname, dlerror());
		return NULL;
	}

	/* get the required symbol */
	optim_t	*result = (optim_t *)dlsym(handle, symbolname);
	if (NULL == result) {
		fprintf(stderr, "cannot find symbol %s: %s\n",
			symbolname, dlerror());
		dlclose(handle);
	}

	return result;
}
