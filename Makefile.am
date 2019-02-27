ACLOCAL_AMFLAGS = -I m4

nobase_pkginclude_HEADERS = config.h

GEN_DRV =
PLAT_DRV =

if PLATFORM_LINUX
  PLAT_DRV += src/tape_drivers/linux/ltotape
endif

if PLATFORM_MAC
  PLAT_DRV += src/tape_drivers/osx/ltotape
endif

if PLATFORM_NETBSD
  PLAT_DRV += src/tape_drivers/netbsd/ltotape
endif

if CHECKONLY
  SUBDIRS = messages src/tape_drivers/generic/dummy
else
  SUBDIRS = messages ${GEN_DRV} ${PLAT_DRV}
endif
