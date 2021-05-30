#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apricots], [0.2.7], [moggers87+git@moggers87.co.uk])
AC_CONFIG_SRCDIR([apricots/game.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])
AX_IS_RELEASE([git-directory])
AX_CHECK_ENABLE_DEBUG()
AX_GENERATE_CHANGELOG

# set up compiler
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_17([noext])
AX_CXXFLAGS_WARN_ALL
AX_COMPILER_FLAGS_CXXFLAGS([CXXFLAGS])

# sdl
AC_PATH_PROG([SDL_CONFIG], sdl2-config)

CPPFLAGS="$CPPFLAGS `$SDL_CONFIG --cflags` `$SDL_CONFIG --libs`"
SDL_LIBS=`"$SDL_CONFIG" --libs`
SDL_CFLAGS=`"$SDL_CONFIG" --cflags`
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

# check for openal
AC_CHECK_LIB(openal, alGetError, [], [
  echo "Can't find OpenAL"
  exit 1
])
AC_CHECK_LIB(alure, alureInitDevice, [], [
  echo "Can't find ALURE"
  exit 1
])

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
