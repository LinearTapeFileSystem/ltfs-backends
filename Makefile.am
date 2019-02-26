ACLOCAL_AMFLAGS = -I m4

nobase_pkginclude_HEADERS = config.h

PLAT_DRV =

if PLATFORM_NETBSD
PLAT_DRV += src/tape_drivers/netbsd/ltotape
endif

GEN_DRV = src/tape_drivers/generic/dummy

SUBDIRS = messages $(GEN_DRV) $(PLAT_DRV)
