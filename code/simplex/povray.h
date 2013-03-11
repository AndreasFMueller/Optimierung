/*
 * povray.h -- Povray interface
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _povray_h
#define _povray_h

#include <stdlib.h>
#include <stdio.h>

#define	POVRAY_SPHERE	(1 << 0)
#define POVRAY_DOMAIN	(1 << 1)
#define POVRAY_CURVE	(1 << 2)
#define POVRAY_GOAL	(1 << 3)
#define POVRAY_PREAMBLE	(1 << 4)
#define POVRAY_PLANES	(1 << 5)

typedef struct simplex_image_s {
	int	m;
	double	*points;
	double	normal[3];
	int	nvertices;
	double	*vertices;
	int	flags;
	double	length;
	double	time;
	double	scale;
	double	transparency;
} simplex_image_t;

extern simplex_image_t	*povray_image_new();
extern simplex_image_t	*povray_image_copy(simplex_image_t *parameters);
extern void	povray_image_free(simplex_image_t *image);
extern void	povray_image(FILE *out, simplex_image_t *image, double t);
extern double	povray_image_length(simplex_image_t *image);

#endif /* provray_h */
