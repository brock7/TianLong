INCLUDES = $(STLPORT_CFLAGS) -I$(top_srcdir)/PlatformManagers/Win32/include -I$(top_srcdir)/RenderSystems/GL/include -I$(top_srcdir)/OgreMain/include $(PLATFORM_CFLAGS) -DSHAREDIR=\"$(pkgdatadir)\" 

platformdir = $(libdir)
platform_LTLIBRARIES = OgrePlatform.la

OgrePlatform_la_SOURCES =  OgreWin32ConfigDialog.cpp\
OgreWin32ErrorDialog.cpp \
OgreWin32Input.cpp \
OgreWin32Input8.cpp \
OgreWin32PlatformDll.cpp \
OgreWin32Timer.cpp

OgrePlatform_la_LDFLAGS = -module $(PLUGIN_FLAGS) -L$(top_builddir)/OgreMain/src
OgrePlatform_la_LIBADD = resource.lo $(PLATFORM_LIBS) $(GL_LIBS) -lOgreMain

RCFLAGS =  --use-temp-file --verbose -O COFF \
	--include-dir=$(top_srcdir)/PlatformManagers/Win32/include \
	--include-dir=$(top_srcdir)/PlatformManagers/Win32/misc

resource.lo: ../misc/OgreWin32Resources.rc
	$(RC) $(RCFLAGS) -o .libs/resource.o $<
# Generate fake libtool object
	echo "# $@" > $@
	echo "# Generated by OGRE, compatible with libtool" >> $@
	echo "pic_object='.libs/resource.o'" >> $@
	echo "non_pic_object=none" >> $@


