#
# Makefile.am -- build Makefile for the simplex algorithm project
#
# (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
# $Id$
#

noinst_LIBRARIES = libsimplex.a

libsimplex_a_SOURCES = linprog.c simplex.c random.c povray.c \
	clutils.c globals.c simplexocl.c backend.c simplexblas.c \
	simplexcpu.c

noinst_HEADERS = linprog.h simplex.h random.h povray.h \
	clutils.h globals.h simplexocl.h backend.h simplexblas.h \
	simplexcpu.h

noinst_PROGRAMS = test1 simplexanimation test3 test4 backendinfo sphere

AM_CFLAGS = -D PKGDATADIR=\"$(pkgdatadir)\"

backendinfo_SOURCES = backendinfo.c
backendinfo_LDADD = -L. -lsimplex -lblas -lm
backendinfo_DEPENDENCIES = libsimplex.a

simplexanimation_SOURCES = simplexanimation.c
simplexanimation_LDADD = -L. -lsimplex -lblas -lm
simplexanimation_DEPENDENCIES = libsimplex.a

test1_SOURCES = test1.c
test1_LDADD = -L. -lsimplex -lblas
test1_DEPENDENCIES = libsimplex.a

test3_SOURCES = test3.c
test3_LDADD = -L. -lsimplex -lblas
test3_DEPENDENCIES = libsimplex.a

test4_SOURCES = test4.c
test4_LDADD = -L. -lsimplex -lblas
test4_DEPENDENCIES = libsimplex.a

sphere_SOURCES = sphere.c
sphere_LDADD = -L. -lsimplex -lblas -lm
sphere_DEPENDENCIES = libsimplex.a

# build curves video
curves:	curves/curves.mpg

curves/curves.mpg:	Makefile makemovie
	if [ ! -d curves ]; then mkdir -p curves; fi
	./simplexanimation -d -b BLAS -m 10 -e 1 -r 1000 -P curves
	cd curves; for f in *.pov; povray -W1920 -H1080 $${f}; done
	./makemovie curves

multi:	multi/multi.mpg

multi/multi.mpg:	Makefile makemovie
	if [ ! -d multi ]; then mkdir -p multi; fi
	./simplexanimation -d -t 0.04 -b BLAS -m 10 -e 100 -r 10 -p multi2


#	(cd multi; ./render)
#	./makemovie multi
