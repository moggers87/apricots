bin_PROGRAMS = apricots

apricots_SOURCES = SDLfont.cpp shape.cpp setup.cpp sampleio.cpp init.cpp game.cpp finish.cpp fall.cpp drawall.cpp drak.cpp collide.cpp apricots.cpp all.cpp ai.cpp
apricots_LDADD   =

SUBDIRS = data docs

# set the include path found by configure
AM_CPPFLAGS= $(all_includes) -DAP_PATH="\"@prefix@/share/apricots/\"" -DSYSCONFIG_PATH="\"@sysconfdir@\""

# the library search path.
apricots_LDFLAGS = $(all_libraries)

APRICOTS_DATA_FILES = $(srcdir)/data

EXTRA_DIST = $(APRICOTS_DATA_FILES)

install-data-local:
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	find "$(APRICOTS_DATA_FILES)" -type f -exec $(INSTALL_DATA) {} ${DESTDIR}$(pkgdatadir) \;

uninstall-local:
	-rm -rf ${DESTDIR}$(pkgdatadir)
