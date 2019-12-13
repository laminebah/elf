#include "Elf_header.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc,char **argv) { 
	
	if(argc != 2)
	{
		fprintf(stderr, "Nombre d'argument incorrect \n");
		fprintf(stderr, "Usage: %s fichier \n", argv[0]);
		exit(1);
	}

	FILE *felf = fopen (argv[1],"rb");
	if (felf == NULL) {
		fprintf(stderr, "Erreur d'ouverture du fichier \n");
		exit(1);		
	}

	Elf32_Ehdr * elf_head;
	elf_head = lecture_entete (felf);
	if (elf_head == NULL)
	{
		fprintf(stderr, "Erreur d'initialisation de l'entete \n");
		exit(1);
	}

	print_header(elf_head);

	return EXIT_SUCCESS;
}

