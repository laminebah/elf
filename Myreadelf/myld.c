#include "Elf_header.h"
#include <stdio.h>
#include <stdlib.h>
#include "printhead.h"
#include <unistd.h>
#include <string.h>
#include "section_header.h"
#include "Elf_symbole.h"
#include "print_symbole.h"
#include "fusion.h"
#include "reloc_table.h"


int s_types[8] = {SHT_NULL , SHT_PROGBITS, SHT_NOBITS, SHT_REL, SHT_RELA, SHT_SYMTAB, SHT_STRTAB, SHT_ARM_ATTRIBUTES};

int main (int argc, char** argv){
  	if (argc != 4){
          fprintf(stderr, "Nombre d'arguments incorrect\n");
          return EXIT_FAILURE;
        }
      
        FILE* file_in1 = fopen(argv[1], "rb");
     	if (file_in1 == NULL) {
  		fprintf(stdout, "Erreur d'ouverture du premier fichier \n");
  		return EXIT_FAILURE;		
  	}
       FILE* file_in2 = fopen(argv[2], "rb");
     	if (file_in2 == NULL) {
  		fprintf(stdout, "Erreur d'ouverture du second fichier \n");
  		return EXIT_FAILURE;		
  	}
  	FILE* file_out = fopen(argv[3], "wb");
     	if (file_out == NULL) {
  		fprintf(stdout, "Erreur d'ouverture du fichier de sorti\n");
  		return EXIT_FAILURE;		
  	}

    Elf32_Ehdr *elf_head1,*elf_head2;
	  Elf32_Shdr *sections_table1,*sections_table2;

    elf_head1 = lecture_entete(file_in1);
    elf_head2 = lecture_entete(file_in2);

    Elf32_Sym *symbTab1,*symbTab2;
    int index1,index2,nbsymb1,nbsymb2;
    SymbTab * sout;

    sections_table1=malloc(elf_head1->e_shnum*sizeof(Elf32_Shdr));
    get_sh_values(&sections_table1 , file_in1 , elf_head1);
    sections_table2=malloc(elf_head2->e_shnum*sizeof(Elf32_Shdr));
    get_sh_values(&sections_table2 , file_in2 , elf_head2);

    symbTab1 = lecture_symb(file_in1,sections_table1,*elf_head1,&nbsymb1,&index1);
    symbTab2 = lecture_symb(file_in2,sections_table2,*elf_head2,&nbsymb2,&index2);

    Donnees* d = malloc(sizeof(Donnees));

  /* inialisation de la structure de fusion */
  init_fusion(d, sections_table1, elf_head1,  sections_table2, elf_head2);

  /* fusion des sections de type : PROGBITS , REL,  ...*/
  for (int i = 0; i < 8; ++i)
      fusion_by_type(d,file_in1,file_in2, elf_head1 ,elf_head2,s_types[i]);

  /**fusion des symboles **/
 if ((sout=fusion_symbols (d,sections_table1,sections_table2,elf_head1,elf_head2, symbTab1,symbTab2,file_in1,file_in2,nbsymb2,index2,index1,nbsymb1)) == NULL) 
      exit (1);

  /* modification des attributs sh_link et sh_info */
  modification_indx_sections(d);
  /* ecriture de l'entete  */
  ecriture_entete(elf_head1,file_out, d);
  /* ecriture de la table des de section et la table des noms des sections   */
  ecriture_section_table(elf_head1, file_out , d);

  /*  Ecriture des symboles  */
  ecris_new_symb_in_file (file_out,d,sout);
  free (symbTab2);
  free (symbTab1);

   /* liberation de la memoire utilsee  */
  liberer_elf( elf_head1 , elf_head2 , sections_table1, sections_table2);
  liberer_fusion(d);



   /* fermer les fichiers overts  */
  free (sout->nomSymbs);
  free (sout->nom);
  free (sout);
  fermer_fichiers(file_in1,file_in2,file_out);

  return EXIT_SUCCESS;
}
