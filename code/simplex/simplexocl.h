/*
 * simplexocl.h -- OpenCL implementation of the Simplex step
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _simplexocl_h
#define _simplexocl_h

#include <simplex.h>
#include <clutils.h>

typedef struct simplexocl_s {
	clu_device_t	*device;
	clcompiler_t	*compiler;
	
} simplexocl_t;

extern simplexocl_t	*simplexocl_setup();

/**
 * \brief perform the elimination step
 *
 * This operation calls the eliminate kernel
 */
extern void	simplexcol_tableau_eliminate(simplexocl_t *simplexocl,
	simplex_tableau_t *st, int row, int col);

extern void	simplexocl_tableau_pivot(simplexocl_t *simplexocl, 
	simplex_tableau_t *st, int row, int col);

extern void	simplexocl_cleanup(simplexocl_t *simplexocl);

#endif /* _simplexocl_h */
