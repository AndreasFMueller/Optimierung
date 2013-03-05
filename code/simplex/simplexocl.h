/*
 * simplexocl.h -- OpenCL implementation of the Simplex step
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _simplexocl_h
#define _simplexocl_h

#include <simplex.h>
#include <clutils.h>

extern int	simplexocl_version;

typedef struct simplexocl_s {
	clu_device_t	*device;
	clcompiler_t	*compiler;
} simplexocl_t;

#endif /* _simplexocl_h */
