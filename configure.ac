#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apricots], [0.2.7], [moggers87+git@moggers87.co.uk])
AC_CONFIG_SRCDIR([apricots/game.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign dist-bzip2 dist-zip subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# sdl
AC_PATH_PROG([SDL_CONFIG], sdl2-config)

CPPFLAGS="$CPPFLAGS `$SDL_CONFIG --cflags` `$SDL_CONFIG --libs`"
SDL_LIBS=`"$SDL_CONFIG" --libs`
SDL_CFLAGS=`"$SDL_CONFIG" --cflags`
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

# check for openal
AC_CHECK_HEADER(AL/al.h,[CXXFLAGS="$CXXFLAGS -DAP_AUDIO_OPENAL"])
AC_CHECK_LIB( openal, alGetError)
AC_CHECK_LIB( alut, alutInit)


# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit sqrt strtol])

AC_CONFIG_FILES([Makefile
                 apricots/Makefile
                 apricots/docs/Makefile
                 apricots/docs/en/Makefile])
AC_OUTPUT
