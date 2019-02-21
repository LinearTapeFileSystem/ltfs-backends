ACLOCAL_AMFLAGS = -I m4

nobase_pkginclude_HEADERS = config.h

if PLATFORM_LINUX
SUBDIRS = messages src/tape_drivers/linux/ltotape
endif

if PLATFORM_MAC
SUBDIRS = messages src/tape_drivers/osx/ltotape
endif

if PLATFORM_NETBSD
SUBDIRS = messages src/tape_drivers/netbsd/ltotape
endif
