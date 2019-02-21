#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ltfs-plugins], [0.1], [manu@netbsd.org])
AC_CONFIG_SRCDIR([src/tape_drivers/netbsd/ltotape/ltotape.c])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_MACRO_DIR([m4]) 
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AM_PROG_AR
AC_PROG_LIBTOOL


# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h stdlib.h string.h sys/file.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset strerror strstr])

dnl  
dnl Check for pkg-config
dnl 
if test -z "$PKG_CONFIG" 
then
    AC_PATH_PROG(PKG_CONFIG, pkg-config, [no]) 
fi  
if test "x${PKG_CONFIG}" = "xno"
then   
    AC_MSG_ERROR([pkg-config was not found])
fi

dnl
dnl Check for genrb
dnl
AC_PATH_PROG(GENRB, genrb, no)
if test "x${GENRB}" = "xno"
then
    AC_MSG_ERROR([genrb was not found])
fi
  
dnl 
dnl Check for pkgdata
dnl 
AC_PATH_PROG(PKGDATA, pkgdata, no)
if test "x${PKGDATA}" = "xno"
then          
    AC_MSG_ERROR([pkgdata was not found])
fi

dnl
dnl Check for ICU
dnl
ICU_MODULE_CFLAGS="`icu-config --cppflags 2> /dev/null`";
ICU_MODULE_LIBS="`icu-config --ldflags 2> /dev/null`";
if test -z "$ICU_MODULE_LIBS"
then
    PKG_CHECK_MODULES([ICU_MODULE], [icu >= 0.21])
fi

AC_MSG_CHECKING([use latest ICU])
AC_ARG_ENABLE([icu_6x],
    [AS_HELP_STRING([--enable-icu-6x],[Support handling of ICU 6x functions])],
    [icu_6x=$enableval],
    [icu_6x=no]
)
AC_MSG_RESULT([$icu_6x])

if test "x${icu_6x}" = "xyes"
then
    AC_MSG_CHECKING(for ICU version)
    ICU_MODULE_VERSION="`icu-config --version 2> /dev/null`";
    if test "${ICU_MODULE_VERSION%%.*}" -ge "60"
    then
        AM_EXTRA_CPPFLAGS="${AM_EXTRA_CPPFLAGS} -D ICU6x"
    fi
    AC_MSG_RESULT([$ICU_MODULE_VERSION])
fi

dnl
dnl test for ENOMEDIUM
dnl
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <errno.h>],[
#ifndef ENOMEDIUM
#error no ENOMEDIUM
#endif])],[],[AM_CPPFLAGS="${AM_CPPFLAGS} -DENOMEDIUM=EAGAIN"])


AM_CPPFLAGS="${AM_CPPFLAGS} `${PKG_CONFIG} --cflags ltfs`"
AM_CPPFLAGS="${AM_CPPFLAGS} `${PKG_CONFIG} --cflags fuse`"
AM_CPPFLAGS="${AM_CPPFLAGS} -DIBM_LTFS_BUILD -DGENERIC_OEM_BUILD"
AM_CPPFLAGS="${AM_CPPFLAGS} ${ICU_MODULE_CFLAGS}"

case "${target_os}" in
    linux*)
	target_linux=yes
        ;;
    darwin*)
        host_mac=yes
        ;;
    netbsd*)
        target_netbsd=yes
        ;;
    *)
        AC_MSG_ERROR(["OS $target_os is not supported"])
        ;;
esac

AM_CONDITIONAL([PLATFORM_LINUX], [test "x${target_linux}" = "xyes"])
AM_CONDITIONAL([PLATFORM_MAC], [test "x${target_mac}" = "xyes"])
AM_CONDITIONAL([PLATFORM_NETBSD], [test "x${target_netbsd}" = "xyes"])
AM_CONDITIONAL([CHK_MESSAGE], [test "x${use_msg_check}" = "xyes"])

AC_SUBST(CFLAGS)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_EXTRA_CPPFLAGS)
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_LDFLAGS) 

AC_CONFIG_FILES([Makefile messages/Makefile 
                 src/tape_drivers/linux/ltotape/Makefile
                 src/tape_drivers/osx/ltotape/Makefile
                 src/tape_drivers/netbsd/ltotape/Makefile])
AC_OUTPUT
