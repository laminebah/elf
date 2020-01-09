#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <byteswap.h>
#include "print_content_section.h"



								/////////////////////////////////////////////////////////
								//////////////////////// étape 3 ////////////////////////
								/////////////////////////////////////////////////////////



//retourner le nom d'une section a travers son indice  
char* get_section_name(Elf32_Ehdr* header, Elf32_Shdr* sections_table, int idx, FILE* fichier){
	char *name = malloc(SIZEN*sizeof(char));
	assert (name!= NULL);
	fseek(fichier, sections_table[header->e_shstrndx].sh_offset + sections_table[idx].sh_name, SEEK_SET);
	char *s=fgets(name, SIZEN, fichier);
	assert (s != NULL);
	return name;
}


//chercher si la section existe  
int existe_section(Elf32_Shdr* sectionHeader, Elf32_Ehdr* header, char * name, FILE* fichier){
	int res = -1;
	if(name[0]==46){
		for(int i=0;i< header->e_shnum;i++)
			if(!strcmp(name,get_section_name( header, sectionHeader, i , fichier)))
				res = i;	
	} sscanf(name, "%d", &res);
		return res;	
}

//affiche le contenu d'un tampon en format hexa
int hexdump(int addr,unsigned char* tampon,int size , int longeur){
	for(int i=0;i<longeur;i++){
	    if(i<size){
	        printf("%02x",tampon[i]);
	    }
	    else{
	        printf("  ");
	    }
	    if((i+1)%4==0){
			printf(" ");
	    } 
	}
	for(int i=0;i<longeur;i++){
	    if(i<size){
				printf("%c",isprint(tampon[i])?tampon[i]:'.');
	    }else{
	        printf("  ");
	    }
	}
	printf("\n");
	return size;
}

//afficher le contenu brute d'une section en hex 
void lire_contentsec(FILE *fichier,int addr,int size){
    unsigned char tampon[MAX]; 
  	int k;
	while(size>sizeof(tampon)){
	    	k=fread(tampon,1,sizeof(tampon),fichier);
	    	assert(k == sizeof(tampon));
	        printf(" 0x%08x  ",addr);
	        addr+=16;
	        size = hexdump(addr,tampon,size , sizeof(tampon));
	    	size-=16;
	}
	assert(fread(tampon,1,size,fichier) == size);
    printf(" 0x%08x  ",addr);
    addr+=16;
    size = hexdump(addr,tampon,size, MAX);
}

//fonction global d'affichage du contenu d'une section
void affiche_contentSection(unsigned char * sectionHexa, Elf32_Shdr* section, Elf32_Ehdr* header, FILE *fichier, char* nm){
	            	int k,n = existe_section(section, header, nm, fichier);
					unsigned char *name = (unsigned char *)get_section_name( header, section, n , fichier);
					char* nameVide = "";
					if (n == 0 ){
						printf(" La section « %s » n'a pas de données à vidanger.\n", nameVide);
					}
					else if(n < 0){
						printf(" readelf: AVERTISSEMENT: La section « %s » n'a pas été vidangée parce qu'inexistante!\n", nm);
					}else if(n>0 && n < header->e_shnum){
							int addr= section[n].sh_addr;
							int size= section[n].sh_size;
							if (size!= 0){
									printf("\n");
									printf("  Vidange hexadécimale de la section « %s » :\n",name);	
									k = fseek(fichier, section[n].sh_offset, SEEK_SET);
									assert(k != -1);
										lire_contentsec(fichier,addr,size);
										printf("\n");
							}else{
								printf(" La section « %s » n'a pas de données à vidanger.\n", name);
							}
					}else if(! (n>0 && n < header->e_shnum)){
						printf(" readelf: AVERTISSEMENT: La section %d n'a pas été vidangée parce qu'inexistante !\n", n);
					}else {
						if (section[n].sh_type == SHT_NOBITS || section[n].sh_type == SHT_NULL || section[n].sh_size == 0){
						printf(" La section « %s » n'a pas de données à vidanger.\n", name);
						}else{
						printf(" readelf: AVERTISSEMENT: La section %d n'a pas été vidangée parce qu'inexistante !\n", n);
					}
				}
}

//lecture d'une section aprtir d'un fichier ELF (+ l`affichage depend du boolean bool) 
unsigned char *print_content_section(char* name, Elf32_Shdr* section ,Elf32_Ehdr* entete, FILE *fichier, int bool){
	Elf32_Shdr table;
	int k;
	//calcule de l'offset du debut du contenu d'une section par rapport a l'entete  
	int offset = entete->e_shoff + entete->e_shstrndx * entete->e_shentsize;
	//se deplacer par le debut du des entetes de section + inde
	k=fseek(fichier, offset , SEEK_SET);
	assert(k != -1);
	//charger la table des strings
	k = fread(&table, sizeof(char), sizeof(Elf32_Shdr), fichier);
    assert(k ==sizeof(Elf32_Shdr));  
    k=fseek(fichier,bswap_32(table.sh_offset), SEEK_SET);
    assert(k != -1);
    //alocations de la table des string
    unsigned char * sectionHex = (unsigned char * )malloc( sizeof(unsigned char) * bswap_32(table.sh_size) );
    assert(sectionHex!=NULL);
    k=fread(sectionHex, sizeof(char),bswap_32(table.sh_size), fichier);
    //assert(k == bswap_32(table.sh_size) );	
    if(bool)
    	affiche_contentSection(sectionHex, section, entete, fichier, name);

    return sectionHex;
}
