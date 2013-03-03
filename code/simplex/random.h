/*
 * random.h -- random functions
 *
 * (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
 * $Id$
 */
#ifndef _random_h
#define _random_h

extern double	random_unit();
extern double	random_equi(double from, double to);
extern void	random_on_sphere(double *v);
extern void	random_on_octantsphere(double *v);

#endif /* _random_h */
