/*
 * clutils.h -- utilities to simplify using OpenCL 
 *
 * (c) 2011 Prof Dr Andreas Mueller, Hochschule Rapperswil 
 * $Id$
 */
#ifndef _clutils_h
#define _clutils_h

#ifdef __APPLE__
#include <opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <stdio.h>
#include <stdarg.h>

extern cl_program	cluCreateProgramWithFilename(cl_context context,
	const char *filename, cl_int *err);
extern void	clu_showplatforminfo(FILE *outputfile, cl_platform_id platform);
extern void	clu_showdeviceinfo(FILE *outputfile, cl_device_id device);
extern void	clu_perror(cl_int err, const char *fmt, ...);

typedef struct clcompiler_s {
	int		debug;
	cl_context	context;
	cl_device_id	device_id;
	cl_program	program;
	cl_kernel	eliminate;
	cl_kernel	dividerow;
	char		*path;
	void		*private;
} clcompiler_t;

extern clcompiler_t	*clu_newcompiler(cl_context context,
				cl_device_id device_id);
extern void	clu_define(clcompiler_t *compiler,
			const char *name, const char *value);
extern void	clu_vdefine(clcompiler_t *compiler,
			const char *name, const char *fmt, ...);
extern int	clu_compile(clcompiler_t *compiler, char *kernelfile);
extern void	clu_freecompiler(clcompiler_t *compiler);

#define MAXDEVICES	10

typedef struct clu_platform_s {
	cl_platform_id	platformid;
	unsigned int	ndeviceids;
	cl_device_id	device_id[MAXDEVICES];
	cl_context	context;
} clu_platform_t;

typedef struct clu_device_s {
	clu_platform_t	*platform;
	int		device;
	cl_device_id	device_id;
	cl_command_queue	queue;
} clu_device_t;

extern clu_platform_t	*clu_getplatform(int platform);
extern void		clu_releaseplatform(clu_platform_t *);

extern clu_device_t	*clu_getdevice(clu_platform_t *platform,
				int device);
extern void		clu_releasedevice(clu_device_t *);

#endif /* _clutils_h */
