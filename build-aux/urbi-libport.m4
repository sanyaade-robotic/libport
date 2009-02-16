#
# urbi-libport.m4: This file is part of build-aux.
# Copyright (C) 2006-2009, Gostai S.A.S.
#
# This software is provided "as is" without warranty of any kind,
# either expressed or implied, including but not limited to the
# implied warranties of fitness for a particular purpose.
#
# See the LICENSE file for more information.
# For comments, bug reports and feedback: http://www.urbiforge.com
#


# _URBI_LIBPORT_SCHED
# -------------------
# Check for what we need to use lib/sched.
AC_DEFUN([_URBI_LIBPORT_SCHED],
[AC_CHECK_HEADERS([sched.h sys/resource.h sys/mman.h sys/times.h sys/param.h direct.h])
AC_CHECK_FUNCS([sched_setscheduler setpriority mlockall times])
])


# _URBI_LIBPORT_BOOST
# -------------------
# Check for libport dependencies on Boost.
AC_DEFUN([_URBI_LIBPORT_BOOST],
[# Check for Boost headers
BOOST_REQUIRE([1.35])
# Check for Boost.Thread
if test x$with_boost_thread_static = xyes; then
  boost_threads_flag=s
fi

URBI_ARG_ENABLE([enable-boost-threads],
		[Enable boost::threads support],
		[yes|no], [no])
if test x$enable_boost_threads = xyes; then
  AC_DEFINE([WITH_BOOST_THREADS], [1],
	    [Define to enable boost::thread support.])
  BOOST_THREADS([$boost_threads_flag])
fi
])




# _URBI_LIBPORT_COMMON
# --------------------
# Code common to the use of an installed or shipped libport.
AC_DEFUN([_URBI_LIBPORT_COMMON],
[
AC_ARG_WITH([boost-thread-static],
	[AC_HELP_STRING([--with-boost-thread-static]
			[link with the static version of boost thread library])]
	[], [with_boost_thread_static=yes])

AC_REQUIRE([URBI_PTHREAD])dnl
AC_REQUIRE([URBI_FLOAT_CHECK])dnl
AC_REQUIRE([_URBI_LIBPORT_BOOST])dnl
AC_REQUIRE([_URBI_LIBPORT_SCHED])dnl
AC_CHECK_FUNCS([semget])
])


# URBI_LIBPORT_INSTALLED
# ----------------------
# We use an installed libport, most probably that of the kernel we
# use.  We don't run URBI_UFLOAT since, of course, we use the same
# kind of ufloat as the kernel does.
#
# Cores use an installed libport (in kernelincludedir), *and* install it
# (in sdkincludedir).
AC_DEFUN([URBI_LIBPORT_INSTALLED],
[AC_REQUIRE([_URBI_LIBPORT_COMMON])dnl
AC_AFTER([$0], [URBI_DIRS])dnl

# Check that we can find libport files.
libport_config_hh=$(URBI_RESOLVE_DIR([$kernelincludedir/libport/config.h]))
if test ! -f $libport_config_h; then
  AC_ERROR([--with-urbi-kernel: cannot find $libport_config_h])
fi
])


# _URBI_LIBPORT_SUBST(SRC_PREFIX, BUILD_PREFIX)
# ---------------------------------------------
# Define Make macros to find the various libport componenents.
# BUILD_PREFIX must be empty, or end with a /.
#
# Avoid useless "./" in file names.  This is not only nicer, it is
# also needed for Make to match properly libraries being build, and
# libraries being used.  Without that, builds may fail, as the
# dependencies are incorrect.
m4_define([_URBI_LIBPORT_SUBST],
[# $(top_srcdir) to find sources, $(top_builddir) to find libport/config.h.
AC_SUBST([LIBPORT_CPPFLAGS], ['-I$1/include -I$2include'])
AC_SUBST([LIBPORT_LIBS],     ['$2lib/libport/libport.la'])

AC_SUBST([LTDL_CPPFLAGS], ['-I$1 -I$1/libltdl'])
AC_SUBST([LTDL_LIBS],     ['$2libltdl/libltdlc.la'])

AC_SUBST([SCHED_CPPFLAGS], ['$(LIBPORT_CPPFLAGS)'])
AC_SUBST([SCHED_LIBS],     ['$2lib/sched/libsched.la'])

AC_SUBST([SERIALIZE_CPPFLAGS], ['$(LIBPORT_CPPFLAGS)'])
AC_SUBST([SERIALIZE_LIBS],     ['$2lib/serialize/libserialize.la'])

AC_SUBST([TINYXML_CPPFLAGS], ['$(LIBPORT_CPPFLAGS)'])
AC_SUBST([TINYXML_LIBS],     ['$2lib/tinyxml/libtinyxml.la'])
])


# URBI_LIBPORT(DIRECTORY)
# -----------------------
# Invoke the macros needed by a Libport shipped in the DIRECTORY
# (i.e., a non-installed copy in this source tree).  Define
# convenience variables to find its various components.
AC_DEFUN([URBI_LIBPORT],
[AC_REQUIRE([_URBI_LIBPORT_COMMON])dnl
AC_REQUIRE([URBI_UFLOAT])dnl

_URBI_LIBPORT_SUBST(m4_if([$1], [], [$(top_srcdir)], [$(top_srcdir)/$1]),
                    m4_if([$1], [], [],              [$(top_builddir)/$1/]))

# Where we install the libport files (not including the /libport suffix).
URBI_PACKAGE_KIND_SWITCH(
  [kernel],     [AC_SUBST([libport_include_basedir], ['${kernelincludedir}'])],
  [sdk],	[AC_SUBST([libport_include_basedir], ['${includedir}'])],
  [core],	[AC_SUBST([libport_include_basedir], ['${sdkincludedir}'])])
])

## Local Variables:
## mode: autoconf
## End:
