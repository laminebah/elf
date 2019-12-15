readelf:readelf.o Elf_header.o util.o printhead.o
	gcc -Wall -Werror -o $@ $^
%.o:%.c
	gcc -Wall -Werror -c $<
readelf.o:readelf.c Elf_header.h printhead.h
Elf_header.o:Elf_header.c Elf_header.h
util.o:util.c util.h
printhead.o:printhead.c printhead.h

clean:
	rm *.o readelf
