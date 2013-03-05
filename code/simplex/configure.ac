#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT
AM_INIT_AUTOMAKE(simplex, 1.0)
AC_CONFIG_SRCDIR([test1.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries. 

# if we find the BLAS library, we also force linking of the simplexblas_version
# symbol, which links the whole BLAS based backend
AC_CHECK_LIB([blas], [dger_], [
	LDFLAGS="${LDFLAGS} -lblas -Wl,--undefined,simplexblas_version"
])
# if we find the OpenCL library, we also force linking of the 
# simplexocl_version symbol, which links the OpenCL based backend
AC_CHECK_LIB([OpenCL], [clGetPlatformInfo], [
	LDFLAGS="${LDFLAGS} -lOpenCL -Wl,--undefined,simplexocl_version"
])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
