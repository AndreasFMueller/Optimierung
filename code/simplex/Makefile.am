#
# Makefile.am -- build Makefile for the simplex algorithm project
#
# (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
# $Id$
#

noinst_LIBRARIES = libsimplex.a

libsimplex_a_SOURCES = linprog.c simplex.c random.c povray.c \
	clutils.c globals.c simplexocl.c backend.c simplexlapack.c \
	simplexcpu.c

noinst_HEADERS = linprog.h simplex.h random.h povray.h \
	clutils.h globals.h simplexocl.h backend.h simplexlapack.h \
	simplexcpu.h

noinst_PROGRAMS = test1 simplexanimation test3

AM_CFLAGS = -D PKGDATADIR=\"$(pkgdatadir)\"

simplexanimation_SOURCES = simplexanimation.c
simplexanimation_LDADD = -L. -lsimplex -lblas -lm
simplexanimation_DEPENDENCIES = libsimplex.a

test1_SOURCES = test1.c
test1_LDADD = -L. -lsimplex -lblas
test1_DEPENDENCIES = libsimplex.a

test3_SOURCES = test3.c
test3_LDADD = -L. -lsimplex -lblas
test3_DEPENDENCIES = libsimplex.a
