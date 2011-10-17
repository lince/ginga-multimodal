# set the include path found by configure
INCLUDES = -I/usr/local/include/ginga/ -I/usr/local/include/directfb/ \
        $(all_includes)


LIBS =

# the library search path.
localLibdir = /usr/local/lib/ginga

libmultimodaltestapp_so_LDFLAGS = -Wl,-soname,libmultimodaltestapp.so -shared \
	-L/usr/local/lib -L/usr/local/lib/ginga -L/usr/local/lib/ginga/cm
localLib_PROGRAMS = libmultimodaltestapp.so

libmultimodaltestapp_so_SOURCES = MultimodalTest.cpp

libmultimodaltestapp_so_LDADD = -lgingaccmmi -ltelemidiautil \
	-lgingaccsystemiocodemap \
	-lgingaccsystemio -lgingaccsystemiodfb -lxerces-c
