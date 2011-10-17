# set the include path found by configure
INCLUDES = -I/usr/local/include/directfb/ -I/usr/local/include/ginga/ \
        $(all_includes)


LIBS =

AM_CXXFLAGS = `pkg-config avahi-client --cflags --libs`

# the library search path.
localLibdir = /usr/local/lib/ginga

libgingaccmmi_so_LDFLAGS = -Wl,-soname,libgingaccmmi.so -shared \
	-L/usr/local/lib -L/usr/local/lib/ginga
localLib_PROGRAMS = libgingaccmmi.so

libgingaccmmi_so_SOURCES = InputModeModifier.cpp InputModeRedirecter.cpp \
	EnhancedInputManager.cpp  DuplicateException.cpp MultimodalInputEvent.cpp \
	DFBMultimodalInputEvent.cpp EventParser.cpp File.cpp \
	Channel.cpp Ink.cpp InkMLParser.cpp Trace.cpp TraceFormat.cpp Utility.cpp \
	CommunicationManager.cpp Acceleration.cpp

libgingaccmmi_so_LDADD = -ltelemidiautil -lgingaccsystemiocodemap \
	-lgingaccsystemio -lgingaccsystemiodfb -lxerces-c

