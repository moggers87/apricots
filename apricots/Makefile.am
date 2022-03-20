bin_PROGRAMS = apricots

apricots_SOURCES = SDLfont.cpp shape.cpp setup.cpp sampleio.cpp init.cpp game.cpp finish.cpp fall.cpp drawall.cpp drak.cpp collide.cpp apricots.cpp all.cpp ai.cpp
apricots_LDADD   =

SUBDIRS = docs

EXTRA_DIST = ai.cpp all.cpp apricots.cpp apricots.h collide.cpp drak.cpp drawall.cpp fall.cpp finish.cpp game.cpp init.cpp linkedlist.h sampleio.cpp sampleio.h setup.cpp shape.cpp shape.h apricots.shapes SDLfont.cpp SDLfont.h alt-8x16.psf afterburner.wav bomb.wav engine.wav explode.wav finish.wav fuelexplode.wav groundhit.wav gunshot.wav gunshot2.wav jet.wav laser.wav shot.wav splash.wav stall.wav apricots.cfg

install-data-local:
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/apricots.shapes ${DESTDIR}$(pkgdatadir)/apricots.shapes
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/alt-8x16.psf ${DESTDIR}$(pkgdatadir)/alt-8x16.psf
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/afterburner.wav ${DESTDIR}$(pkgdatadir)/afterburner.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/bomb.wav ${DESTDIR}$(pkgdatadir)/bomb.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/engine.wav ${DESTDIR}$(pkgdatadir)/engine.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/explode.wav ${DESTDIR}$(pkgdatadir)/explode.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/finish.wav ${DESTDIR}$(pkgdatadir)/finish.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/fuelexplode.wav ${DESTDIR}$(pkgdatadir)/fuelexplode.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/groundhit.wav ${DESTDIR}$(pkgdatadir)/groundhit.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/gunshot.wav ${DESTDIR}$(pkgdatadir)/gunshot.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/gunshot2.wav ${DESTDIR}$(pkgdatadir)/gunshot2.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/jet.wav ${DESTDIR}$(pkgdatadir)/jet.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/laser.wav ${DESTDIR}$(pkgdatadir)/laser.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/shot.wav ${DESTDIR}$(pkgdatadir)/shot.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/splash.wav ${DESTDIR}$(pkgdatadir)/splash.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/stall.wav ${DESTDIR}$(pkgdatadir)/stall.wav
	$(mkinstalldirs) ${DESTDIR}$(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/apricots.cfg ${DESTDIR}$(pkgdatadir)/apricots.cfg

uninstall-local:
	-rm -f ${DESTDIR}$(pkgdatadir)/apricots.shapes
	-rm -f ${DESTDIR}$(pkgdatadir)/alt-8x16.psf
	-rm -f ${DESTDIR}$(pkgdatadir)/afterburner.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/bomb.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/engine.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/explode.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/finish.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/fuelexplode.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/groundhit.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/gunshot.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/gunshot2.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/jet.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/laser.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/shot.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/splash.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/stall.wav
	-rm -f ${DESTDIR}$(pkgdatadir)/apricots.cfg

# set the include path found by configure
AM_CPPFLAGS= $(all_includes) -DAP_PATH="\"@prefix@/share/apricots/\"" -DSYSCONFIG_PATH="\"@sysconfdir@\""

# the library search path.
apricots_LDFLAGS = $(all_libraries)
