####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = apricots

## INCLUDES were found outside kdevelop specific part

apricots_SOURCES = SDLfont.cpp shape.cpp setup.cpp sampleio.cpp init.cpp game.cpp finish.cpp fall.cpp drawall.cpp drak.cpp collide.cpp apricots.cpp all.cpp ai.cpp 
apricots_LDADD   = 

SUBDIRS = docs 

EXTRA_DIST = ai.cpp all.cpp apricots.cpp apricots.h collide.cpp drak.cpp drawall.cpp fall.cpp finish.cpp game.cpp init.cpp linkedlist.h sampleio.cpp sampleio.h setup.cpp shape.cpp shape.h apricots.shapes SDLfont.cpp SDLfont.h alt-8x16.psf afterburner.wav bomb.wav engine.wav explode.wav finish.wav fuelexplode.wav groundhit.wav gunshot.wav gunshot2.wav jet.wav laser.wav shot.wav splash.wav stall.wav apricots.cfg 

install-data-local:
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/apricots.shapes $(pkgdatadir)/apricots.shapes
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/alt-8x16.psf $(pkgdatadir)/alt-8x16.psf
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/afterburner.wav $(pkgdatadir)/afterburner.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/bomb.wav $(pkgdatadir)/bomb.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/engine.wav $(pkgdatadir)/engine.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/explode.wav $(pkgdatadir)/explode.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/finish.wav $(pkgdatadir)/finish.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/fuelexplode.wav $(pkgdatadir)/fuelexplode.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/groundhit.wav $(pkgdatadir)/groundhit.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/gunshot.wav $(pkgdatadir)/gunshot.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/gunshot2.wav $(pkgdatadir)/gunshot2.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/jet.wav $(pkgdatadir)/jet.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/laser.wav $(pkgdatadir)/laser.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/shot.wav $(pkgdatadir)/shot.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/splash.wav $(pkgdatadir)/splash.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/stall.wav $(pkgdatadir)/stall.wav
	$(mkinstalldirs) $(pkgdatadir)/
	$(INSTALL_DATA) $(srcdir)/apricots.cfg $(pkgdatadir)/apricots.cfg

uninstall-local:
	-rm -f $(pkgdatadir)/apricots.shapes
	-rm -f $(pkgdatadir)/alt-8x16.psf
	-rm -f $(pkgdatadir)/afterburner.wav
	-rm -f $(pkgdatadir)/bomb.wav
	-rm -f $(pkgdatadir)/engine.wav
	-rm -f $(pkgdatadir)/explode.wav
	-rm -f $(pkgdatadir)/finish.wav
	-rm -f $(pkgdatadir)/fuelexplode.wav
	-rm -f $(pkgdatadir)/groundhit.wav
	-rm -f $(pkgdatadir)/gunshot.wav
	-rm -f $(pkgdatadir)/gunshot2.wav
	-rm -f $(pkgdatadir)/jet.wav
	-rm -f $(pkgdatadir)/laser.wav
	-rm -f $(pkgdatadir)/shot.wav
	-rm -f $(pkgdatadir)/splash.wav
	-rm -f $(pkgdatadir)/stall.wav
	-rm -f $(pkgdatadir)/apricots.cfg

####### kdevelop will overwrite this part!!! (end)############
# set the include path found by configure
INCLUDES= $(all_includes)
 
# the library search path.
apricots_LDFLAGS = $(all_libraries) 
                                                                                
