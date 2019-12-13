#include "Elf_header.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc,char **argv) { 
	FILE *felf = fopen (argv[1],"rb");
	if (felf == NULL) {
		exit(1);		
	}
	Elf32_Ehdr * elf_head;
	elf_head = lecture_entete (felf);
	if (elf_head == NULL)
		exit(1);
	return EXIT_SUCCESS;
}
