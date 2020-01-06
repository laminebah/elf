ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
AM_CFLAGS+=-D BIG_ENDIAN_SIMULATOR
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG

LDADD=-lpthread


SUBDIRS= 
SUBDIRS+=. Examples_loader

bin_PROGRAMS=
bin_PROGRAMS+=ARM_runner_example readelf myld


COMMON=debug.h debug.c util.h util.c
readelf=Myreadelf/Elf_header.h Myreadelf/Elf_header.c Myreadelf/printhead.h Myreadelf/printhead.c Myreadelf/section_header.h Myreadelf/section_header.c  Myreadelf/print_content_section.h Myreadelf/print_content_section.c Myreadelf/Elf_symbole.h Myreadelf/Elf_symbole.c Myreadelf/print_symbole.h Myreadelf/print_symbole.c Myreadelf/reloc_table.h Myreadelf/reloc_table.c 

myld=$(readelf) Myreadelf/fusion.h Myreadelf/fusion.c
#Myreadelf/Elf_header.h Myreadelf/Elf_header.c Myreadelf/printhead.h Myreadelf/printhead.c Myreadelf/section_header.h Myreadelf/section_header.c Myreadelf/fusion.h Myreadelf/fusion.c Myreadelf/Elf_symbole.h Myreadelf/Elf_symbole.c Myreadelf/print_symbole.h Myreadelf/print_symbole.c Myreadelf/reloc_table.h Myreadelf/reloc_table.c 

EXTRA_DIST=filtre.pl
SIMULATOR_INTERFACE=csapp.h csapp.c scanner.h scanner.l gdb_protocol.h gdb_protocol.c arm_simulator_interface.h arm_simulator_interface.c
ARM_runner_example_SOURCES=$(COMMON) $(SIMULATOR_INTERFACE) ARM_runner_example.c
readelf_SOURCES= $(COMMON) $(readelf) Myreadelf/readelf.c
myld_SOURCES= $(COMMON) $(myld) Myreadelf/myld.c
EXTRA_DIST+=sample_run.sh
