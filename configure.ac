dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])

m4_define(OLIGO_VERSION, m4_normalize(m4_include(VERSION)))

AC_INIT([oligo], [OLIGO_VERSION], [sandain@hotmail.com])


AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(src/oligo.c)

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_PROG_CC

AM_PROG_CC_C_O

AM_PROG_AR

PKG_CHECK_MODULES([CHECK], [check >= 0.9.10])

LT_INIT

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    tests/Makefile
])

AC_OUTPUT
