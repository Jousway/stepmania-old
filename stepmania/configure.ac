AC_PREREQ(2.59)
AC_INIT(StepMania, CVS, [stepmania-devs@lists.sf.net], StepMania)
AC_CONFIG_SRCDIR([src/StepMania.cpp])
AC_CONFIG_AUX_DIR(autoconf)
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([no-define])
AC_CONFIG_HEADERS([src/config.h])
AM_MAINTAINER_MODE

# We don't want PROG_CC/CXX default settings, but don't ignore explicit settings.
test -z "$CFLAGS"    && DEFAULT_CFLAGS=yes
test -z "$CXXFLAGS"  && DEFAULT_CXXFLAGS=yes
test -z "$LDFLAGS"   && DEFAULT_LDFLAGS=yes

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_CC_C_O

test "$DEFAULT_CFLAGS" = "yes" && CFLAGS="-Wall -W -Wno-unused -Wno-switch"
test "$DEFAULT_CXXFLAGS" = "yes" && CXXFLAGS="-Wall -W -Wno-unused -Wno-switch"
test "$DEFAULT_LDFLAGS" = "yes" && LDFLAGS=""

compile=release
AC_ARG_WITH(debug, AS_HELP_STRING([--with-debug],[Enable debug mode]), with_debug=$withval, with_debug=no)
AC_ARG_WITH(fast-compile, AS_HELP_STRING([--with-fast-compile],[Enable fast compile]), with_fast_compile=$withval, with_fast_compile=no)
if test "$with_debug" = "yes"; then
	compile=debug
fi
if test "$with_fast_compile" = "yes"; then
	compile=fast
fi

case $compile in
	release)
	test "$DEFAULT_CFLAGS" = "yes" && CFLAGS="$CFLAGS -O3"
	test "$DEFAULT_CXXFLAGS" = "yes" && CXXFLAGS="$CXXFLAGS -O3"
	;;
	debug)
	test "$DEFAULT_CFLAGS" = "yes" && CFLAGS="$CFLAGS -g"
	test "$DEFAULT_CXXFLAGS" = "yes" && CXXFLAGS="$CXXFLAGS -g"
	;;
	fast)
	test "$DEFAULT_CFLAGS" = "yes" && CFLAGS="$CFLAGS -O2 -fno-inline"
	test "$DEFAULT_CXXFLAGS" = "yes" && CXXFLAGS="$CXXFLAGS -O2 -fno-inline"
	;;
esac


# Some (broken?) versions of gcc treat char as unsigned, check for that.
AC_LANG_PUSH([C])
AC_MSG_CHECKING([for gcc option to use signed chars])
AC_RUN_IFELSE([AC_LANG_SOURCE([[int main(){return (char)-1==-1?0:1;}]])],
	   [sc_option=],[sc_option=-fsigned-char])
AC_MSG_RESULT([${sc_option-none}])
CFLAGS="$CFLAGS $sc_option"
CXXFLAGS="$CXXFLAGS $sc_option"
AC_LANG_POP([C])

AC_CHECK_HEADERS([sys/param.h])

if test x$with_debug = xyes; then
	AC_LANG_PUSH([C++])
	AC_CHECK_HEADER([debug/debug.h],[
		AC_DEFINE([_GLIBCXX_DEBUG],1,[STL debug])
		AC_DEFINE([_GLIBCXX_DEBUG_PEDANTIC],1,[STL pedantic debug])])
	AC_LANG_POP([C++])
fi

# Define UNIX for all Unix-like systems.  Don't define it for cross-compiling to
# non-Unix-like systems.  (-DUNIX selects the archutils and ArchHooks to use; if
# your platform doesn't use the Unix ones, you probably don't want to define UNIX.)
case $host_os in
*linux*)
	AC_DEFINE(LINUX, 1, [Linux])
	AC_DEFINE(UNIX, 1, [Unix])
	linux=yes
	unix=yes
	;;

*bsd*)
	AC_CHECK_DECL(BSD,,AC_DEFINE(BSD, 1, [BSD]),[
#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif])
	AC_DEFINE(UNIX, 1, [Unix])
	bsd=yes
	unix=yes
	;;
*)
	AC_DEFINE(UNIX, 1, [Unix])
	unix=yes
	;;
esac
AC_DEFINE(LUA_USE_LINUX, 1, [Linux for Lua])
AM_CONDITIONAL(UNIX, test "$unix" = "yes" )
AM_CONDITIONAL(LINUX, test "$linux" = "yes" )
AM_CONDITIONAL(BSD, test "$bsd" = "yes" )

# Define macros for individual CPU types, for a few bits of inline assembly.
# This is for major, compatible CPU classes--"CPU_X86" includes P2, P3, P4,
# AMD, etc.  If you need CPU-specific assembly, check at runtime--don't create
# separate binaries for each CPU if the binaries themselves are otherwise
# compatible.
case $host_cpu in
i?86*)
	AC_DEFINE(CPU_X86, 1, [x86])
	;;
x86_64*)
	AC_DEFINE(CPU_X86_64, 1, [x86-64])
	;;
powerpc*)
	AC_DEFINE(CPU_PPC, 1, [PPC])
	have_parallel_port=no
	;;
esac
AM_CONDITIONAL(HAVE_PARALLEL_PORT, test "${have_parallel_port-yes}" = "yes")

AC_C_BIGENDIAN( 
	AC_DEFINE(ENDIAN_BIG, 1, [Big endian]),
	AC_DEFINE(ENDIAN_LITTLE, 1, [Little endian]),
	AC_MSG_ERROR([Can't determine endianness]) )

AC_ARG_WITH(prof, AS_HELP_STRING([--with-prof],[Enable profiling]), with_prof=$withval, with_prof=no)
if test "$with_prof" = "yes"; then
	test "$DEFAULT_CFLAGS" = "yes" && CFLAGS="$CFLAGS -pg"
	test "$DEFAULT_CXXFLAGS" = "yes" && CXXFLAGS="$CXXFLAGS -pg"
	test "$DEFAULT_LDFLAGS" = "yes" && LDFLAGS="$LDFLAGS -pg"
fi

SM_X_WITH_OPENGL

have_libpng=yes
SM_STATIC
AC_CHECK_LIB(png, png_create_read_struct, [x=y], have_libpng=no, [-lz -lm]) # x=y to stop autoconf from messing with LIBS
AC_CHECK_HEADER(png.h, , have_libpng=no)
AC_ARG_WITH(static-png, AS_HELP_STRING([--with-static-png],[Statically link libpng]), with_static_png=$withval, with_static_png=no)
if test "$with_static_png" = "yes"; then
	LIB_PRE=$START_STATIC
	LIB_POST=$END_STATIC
fi
if test "$have_libpng" = "no"; then
  echo
  echo "*** libpng is required to build StepMania; please make sure that"
  echo "*** it is installed to continue the build process."
  exit 1
fi
LIBS="$LIBS $LIB_PRE -lpng -lz -lm $LIB_POST"
LIB_PRE=
LIB_POST=

AC_ARG_WITH(jpeg, AS_HELP_STRING([--without-jpeg],[Disable JPEG support]), with_jpeg=$withval, with_jpeg=yes)
AC_ARG_WITH(static-jpeg, AS_HELP_STRING([--with-static-jpeg],[Statically link libjpeg]), with_static_jpeg=$withval, with_static_jpeg=no)

if test "$with_static_png" = "yes"; then
	LIB_PRE=$START_STATIC
	LIB_POST=$END_STATIC
fi
if test "$with_jpeg" = "yes"; then
	have_libjpeg=yes
	AC_CHECK_LIB(jpeg, jpeg_read_scanlines, [x=y], have_libjpeg=no) # x=y to stop autoconf from messing with LIBS
	AC_CHECK_HEADER(jpeglib.h, , have_libjpeg=no)

	if test "$have_libjpeg" = "no"; then
		# Require JPEG by default, so people don't compile without it
		# by accident and then come asking us why files won't load.
		AC_MSG_ERROR(
[libjpeg is required to build StepMania; please make sure that it is installed
to continue the build process.  If you really want to compile without JPEG
support, pass the "--without-jpeg" flag to configure.])
	fi
	have_libjpeg=

	LIBS="$LIBS $LIB_PRE -ljpeg $LIB_POST"
else
	AC_DEFINE(NO_JPEG_SUPPORT, 1, [JPEG support not available])
fi
LIB_PRE=
LIB_POST=

AC_ARG_WITH(network, AS_HELP_STRING([--without-network],[Disable networking]), with_network=$withval, with_network=yes)
if test "$with_network" = "no"; then
	AC_DEFINE(WITHOUT_NETWORKING, 1, [Networking support not available])
fi
AM_CONDITIONAL(WITHOUT_NETWORKING, test "$with_network" = "no")

SM_ZLIB
SM_AUDIO
SM_VIDEO
SM_TLS

SM_CHECK_CRASH_HANDLER

AM_PATH_ALSA(0.9.0,AC_DEFINE([HAVE_ALSA],1,[Define presence of ALSA]),alsa=false)

AM_ICONV

AC_ARG_ENABLE(gtk2, AS_HELP_STRING([--disable-gtk2],[Disable GTK support]), enable_gtk2=$enableval, enable_gtk2=yes)

if test x$enable_gtk2 = xyes; then
  AM_PATH_GTK_2_0(2.0.0,AC_DEFINE([HAVE_GTK],1,[Define presence of GTK]),enable_gtk2=no)
fi

AC_CHECK_HEADER(sys/soundcard.h, [AC_DEFINE(HAVE_OSS, 1, [OSS support available])])
AC_CHECK_DECL(OSS_GETVERSION, AC_DEFINE([HAVE_OSS_GETVERSION],1,[OSS_GETVERSION is defined]), , [#include <sys/soundcard.h>])
AC_ARG_ENABLE(force-oss, AS_HELP_STRING([--enable-force-oss],[Force OSS]), force_oss=$enableval, force_oss=no)
AC_CHECK_HEADER(stdint.h, , [AC_DEFINE(MISSING_STDINT_H, 1, [stdint.h is missing])])
AC_CHECK_HEADERS([inttypes.h endian.h machine/endian.h alloca.h])

have_pulse=no
AC_CHECK_LIB(pulse, pa_stream_new, have_pulse=yes)
if test x$have_pulse = xyes; then
  AC_DEFINE(HAVE_PULSE, 1, [pulseaudio support available])
  LIBS="$LIBS -lpulse"
fi

AC_MSG_CHECKING(if cstdlib breaks llabs)
AC_LANG_PUSH(C++)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>
		 #include <cstdlib> 
		 using namespace std;]], [[llabs(1)]])],[AC_MSG_RESULT(no)],[AC_MSG_RESULT(yes)
		 AC_DEFINE([NEED_CSTDLIB_WORKAROUND], 1, [cstdlib breaks llabs])
	      ])
AC_LANG_POP(C++)
AC_CHECK_LIB( pthread, pthread_create, AC_DEFINE([HAVE_LIBPTHREAD],1,[libpthread is available]) )
AC_CHECK_LIB( pthread, pthread_mutex_timedlock, AC_DEFINE([HAVE_PTHREAD_MUTEX_TIMEDLOCK],1,[pthreads has pthread_mutex_timedlock()]) )
AC_CHECK_LIB( pthread, pthread_cond_timedwait, AC_DEFINE([HAVE_PTHREAD_COND_TIMEDWAIT],1,[pthreads has pthread_cond_timedwait()]) )

# Always:
AC_DEFINE(_GNU_SOURCE, 1, [Use GNU extensions])
AC_DEFINE(__STDC_FORMAT_MACROS, 1, [Use PRId64 and similar])

AM_CONDITIONAL(HAVE_ALSA, test x$alsa != xfalse )
AM_CONDITIONAL(HAVE_GTK, test "$enable_gtk2" != "no" )
AM_CONDITIONAL(HAVE_OSS, test x$ac_cv_header_sys_soundcard_h = xyes )
AM_CONDITIONAL(HAVE_PULSE, test x$have_pulse = xyes)
AM_CONDITIONAL(USE_CRASH_HANDLER, test "$use_crash_handler" = "yes" )

if test x$force_oss = xyes && test x$ac_cv_header_sys_soundcard_h = xyes; then
	AC_DEFINE([FORCE_OSS], 1, [Force OSS Usage])
fi

AC_CHECK_DECL(powf, , AC_DEFINE([NEED_POWF],1,[Need powf]), [#include <math.h>])
AC_CHECK_DECL(sqrtf, , AC_DEFINE([NEED_SQRTF],1,[Need sqrtf]), [#include <math.h>])
AC_CHECK_DECL(sinf, , AC_DEFINE([NEED_SINF],1,[Need sinf]), [#include <math.h>])
AC_CHECK_DECL(tanf, , AC_DEFINE([NEED_COSF],1,[Need tanf]), [#include <math.h>])
AC_CHECK_DECL(cosf, , AC_DEFINE([NEED_TANF],1,[Need cosf]), [#include <math.h>])
AC_CHECK_DECL(acosf, , AC_DEFINE([NEED_ACOSF],1,[Need acosf]), [#include <math.h>])
AC_CHECK_DECL(roundf, , AC_DEFINE([NEED_ROUNDF],1,[Need roundf]), [#include <math.h>])
AC_CHECK_DECL(truncf, , AC_DEFINE([NEED_TRUNCF],1,[Need truncf]), [#include <math.h>])
AC_CHECK_DECL(strtof, , AC_DEFINE([NEED_STRTOF],1,[Need strtof]), [#include <stdlib.h>])

# This doesn't work on glibc math functions:
# AC_CHECK_FUNCS([sqrtf sinf tanf cosf acosf roundf truncf])

AC_CHECK_DECLS([SIGPWR, SIGUSR1],,,[#include <signal.h>])

AC_ARG_ENABLE(tests, AS_HELP_STRING([--enable-tests],[Build test programs]), enable_tests=$enableval, enable_tests=no)
AM_CONDITIONAL(BUILD_TESTS, test "$enable_tests" = "yes" )

AC_ARG_ENABLE(lua-binaries, AS_HELP_STRING([--enable-lua-binaries],[Build lua and luac]), enable_lua_binaries=$enableval, enable_lua_binaries=no)
AM_CONDITIONAL(BUILD_LUA_BINARIES, test "$enable_lua_binaries" = "yes" )

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(src/Makefile)
AC_OUTPUT
