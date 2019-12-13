readelf:readelf.o Elf_header.o
	gcc -Wall -Werror -o $@ $^
%.o:%.c
	gcc -Wall -Werror -c $<
readelf.o:readelf.c Elf_header.h
Elf_header.o:Elf_header.c Elf_header.h
util.o: util.h util.c
clean:
	rm *.o readelf
