#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(SMITH, 2.0, [shiozaki@northwestern.edu])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_CHECK_LIB(boost_regex-gcc-mt, main, , [
    AC_CHECK_LIB(boost_regex-mt, main, , [
        AC_CHECK_LIB(boost_regex, main, , [
            AC_MSG_ERROR("Specify the path to libboost_regex using LDFLAGS")])
    ])
])
AC_CHECK_LIB(blas, main, , [AC_MSG_ERROR("Specify the path to BLAS using LDFLAGS")])
AC_CHECK_LIB(lapack, main, , [AC_MSG_ERROR("Specify the path to LAPACK using LDFLAGS")])

CXXFLAGS="$CXXFLAGS -std=c++0x"
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/dg/Makefile
                 src/ig/Makefile
                 src/storage/Makefile])
AC_OUTPUT
