#include "Elf_header.h"
#include <stdio.h>
#include <stdlib.h>
#include "printhead.h"
#include <unistd.h>
#include <string.h>
#define A  0 //toutes les commandes
#define H  1 //entete
#define X  2 //une section avec numéro ou name
#define XS 3 //table section
#define R  4 //relocation
#define S  5 //symbole

void freemem (void *adr) {
	if (adr != NULL) free (adr);
}

int main (int argc,char **argv) { 
	int option,choix;
	while ((option=getopt(argc,argv,"ahxsrS:")) != -1) {
		switch (option) {
			case 'a':
					//all
					choix = 0;
				break;
			case 'h':
					//entete
					choix = 1;
				break;
			case 'x':
					//une section
					choix = 2;
				break;
			case 's':
					//symbole
					choix = 5;
				break;
			case 'r':
					//relocation
					choix = 4;
				break;
			case 'S':
					//table section
					choix = 3;
				break;
			default:
				return EXIT_FAILURE;
		}
	}
	FILE *felf;
	if (choix == X) {
		if(argc < 4) {
			fprintf(stderr, "Nombre d'argument incorrect \n");
			fprintf(stderr, "Usage: %s fichier \n", argv[0]);
			exit(1);
		}
		//printf ("%s\n",argv[3]);
		felf = fopen (argv[3],"rb");
	} else {
		if(argc < 3) {
			fprintf(stderr, "Nombre d'argument incorrect \n");
			fprintf(stderr, "Usage: %s fichier \n", argv[0]);
			exit(1);
		}
		//sprintf ("%s\n",argv[2]);
		felf = fopen (argv[2],"rb");
	}
	if (felf == NULL) {
		fprintf(stderr, "Erreur d'ouverture du fichier \n");
		return EXIT_FAILURE;		
	}
	int numero;
	char *namesection;
	//déclaration des tructures
	Elf32_Ehdr * elf_head;
	elf_head = lecture_entete (felf);
	if (elf_head == NULL)
	{
		fprintf(stderr, "Erreur d'initialisation de l'entete \n");
		exit(1);
	}
	//exécution commande option
	switch (choix) {
		case A:
			//all : A compléter
			break;
		case XS:
			//table section : A compléter
			break;
		case H:
			//entete :
			print_header(elf_head);
			break;
		case S:
			//symbole : A compléter
			break;
		case X:
			//une section : soit par N° section ou Name sections
			//si c'est entier on affiche à l'aide du numéro sinon à l'aide du nom
			if ((numero=atoi(argv[2]))) {
				//appel de la fonction en lui fournissant N°
				//*****A compléter******/
			} else {
				//par name;
				namesection=malloc(sizeof(char)*strlen(argv[2])+1);
				if (namesection == NULL) return EXIT_FAILURE;
				if (argv[2][0] != '.') strcpy(namesection,".");
				else strcpy(namesection,"");
				strcat(namesection,argv[2]);
				//appel de la fonction en lui fournissant name
				//******A compléter*******/
			}
			break;
		case R:
			//relocation
		/*******A compléter*******/
			break;

	}
	freemem (elf_head);
	fclose (felf);
	return EXIT_SUCCESS;
}

