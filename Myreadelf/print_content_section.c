#include <string.h>
#include <stdlib.h>
#include "Elf_header.h"
#include <assert.h>
#include <byteswap.h>
#include <elf.h>
#include "print_content_section.h"



/////////////////////////////////////////////////////////
//////////////////////// étape 3 ////////////////////////
/////////////////////////////////////////////////////////



//chercher si la section existe  
int existe_section(unsigned char * sectionHexa, Elf32_Shdr* sectionHeader, Elf32_Ehdr* header, char * name){
	if(name[0]==46){
		for(int i=0;i< header->e_shnum;i++){
			if(!strcmp(name,(char *)sectionHexa + sectionHeader[i].sh_name)){
				return i;
			}
		}	
	} else if(name[0]>='0' && name[0]<='9'){
		return name[0]-'0';
	}
	return -2;	
}

//afficher le contenu brute d'une section en hex 
void hexdump(FILE *fichier,int addr,int size){
  unsigned char tampon[N]; 
  int k,i;
	  while(size>sizeof(tampon)){
	    	k=fread(tampon,1,sizeof(tampon),fichier);
	        printf("  0x%08x ",addr);
	        addr+=16;
	        for(i=0;i<sizeof(tampon);i++){
	            if(i<k){
	                printf("%02x",tampon[i]);
	            }
	            else{
	                printf(" ");
	            }
		    if((i+1)%4==0){
			printf(" ");
		    } 
	         }

	    	for(i=0;i<sizeof(tampon);i++){
	            if(i<k){
	      		printf("%c",isprint(tampon[i])?tampon[i]:'.');
	            }
	            else{
	                printf(" ");
	            }
	    	}

	    	printf("\n");
	    	size-=16;
	  }
    	assert(fread(tampon,1,size,fichier) == size);
        printf("  0x%08x ",addr);
        addr+=16;
        for(i=0;i<N;i++){
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

    	for(i=0;i<N;i++){
            if(i<size){
      			printf("%c",isprint(tampon[i])?tampon[i]:'.');
            }
            
		}
    	printf("\n");
}

//fonction global d'affichage du contenu d'une section
void affiche_contentSection(unsigned char * sectionHexa, Elf32_Shdr* section, Elf32_Ehdr* header, FILE *fichier, char* nm){
		int n = existe_section(sectionHexa, section, header, nm);
		unsigned char *name = (unsigned char *)sectionHexa + (section[n].sh_name);
		//printf("size: %d\n",section[n].sh_size);
		if(section[n].sh_type == SHT_NOBITS || section[n].sh_type == SHT_NULL || section[n].sh_size == 0){
				char* namevide;
				if (strcmp((const char *)name, "0") == 0){
					namevide = "";
					printf("  La section « %s » n'a pas de données à vidanger.\n", namevide);
				}else{
					printf("  La section « %s » n'a pas de données à vidanger.\n", name);
				}
		}else{
			if(n>=0){

				printf("\n");
				printf("  Vidange hexadécimale de la section « %s » : \n",name);
				//fprintf(stdout,"\n");
			
				int addr= section[n].sh_addr;
				int size= section[n].sh_size;

				assert(fseek(fichier, section[n].sh_offset, SEEK_SET) != -1);
				
				hexdump(fichier,addr,size);
				printf("\n");
			} else {
				printf("readelf: AVERTISSEMENT: La section « %s » n'a pas été vidangée parce qu'inexistante !", name);
			
			}
		}	
}

//lecture d'une section aprtir d'un fichier ELF
void print_content_section(char* name, Elf32_Shdr* section ,Elf32_Ehdr* entete, FILE *fichier){
	Elf32_Shdr table;
	//calcule de l'offset du debut du contenu d'une section par rapport a l'entete  
	int offset = entete->e_shoff + entete->e_shstrndx * entete->e_shentsize;
	//se deplacer par le debut du des entetes de section + inde
	assert(fseek(fichier, offset , SEEK_SET) != -1);
	//charger la table des strings
    assert(sizeof(Elf32_Shdr) ==fread(&table, sizeof(char), sizeof(Elf32_Shdr), fichier));  
    assert(fseek(fichier,bswap_32(table.sh_offset), SEEK_SET) != -1);
    //alocations de la table des string
    unsigned char * sectionHex = (unsigned char * )malloc( sizeof(unsigned char) * bswap_32(table.sh_size) );
    assert(sectionHex!=NULL);
    assert( bswap_32(table.sh_size) == fread(sectionHex, sizeof(char),bswap_32(table.sh_size), fichier) );	
    affiche_contentSection(sectionHex, section, entete, fichier, name);

}


