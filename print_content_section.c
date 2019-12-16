
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "Elf_header.h"
#include <assert.h>
#include <byteswap.h>
#include <elf.h>
#include "print_content_section.h"

int get_section_num(Elf32_Shdr* sectionHeader, Elf32_Ehdr* header, char * name){
	 
	
	// if(name[0]==46){
	// 	for(int i=0;i< header->e_shnum;i++){
	// 		if(!strcmp(name,sectionHeader[i].sh_name)){
	// 			return i;
	// 		}
	// 	}	
	// } else if(name[0]>='0' && name[0]<='9'){
	// 	return name[0]-'0';
	// }
	return -2;	
}



// void affiche_contentSection(Elf32_Shdr sectionHexa, Elf32_Shdr* section, Elf32_Ehdr* header, FILE *file, char* name){
		
// 		int n;
		
// 		n = get_section_num(&sectionHexa, section, header, name);
// 		if(n>=0){
// 			unsigned char *name= sectionHexa + (section[n].sh_name);
// 			printf("Vidange hexadécimale de la section « %s »:\n",name);

// 			int addr= section[n].sh_addr;
// 		    int size= section[n].sh_size;

// 			assert(fseek(file, section[n].sh_offset, SEEK_SET) != -1);
// 			myhexdump(file,addr,size);

// 		} else {
// 			fprintf(stderr,"Le nom de table non existant\n");
		
// 		}
		
// }


// void myhexdump(FILE *file,int addr,int size){

//   unsigned char buffer[N]; //Use unsigned char,prevent hex overflow.
//   int count,i;

// 	  while(size>sizeof(buffer)){
// 	    	count=fread(buffer,1,sizeof(buffer),file);
// 	    	assert(count == sizeof(buffer));
// 	        printf(" 0x%08x  ",addr);//number in hex.
// 	        addr+=16;
// 	        for(i=0;i<sizeof(buffer);i++){
// 	            if(i<count){
// 	                printf("%02x",buffer[i]);
// 	            }
// 	            else{
// 	                printf("   ");
// 	            }
// 		    if((i+1)%4==0){
// 			printf(" ");
// 		    } 
// 	         }

// 	    	for(i=0;i<sizeof(buffer);i++){
// 	            if(i<count){
// 	      		printf("%c",isprint(buffer[i])?buffer[i]:'.');
// 	            }
// 	            else{
// 	                printf(" ");
// 	            }
// 	    	}

// 	    	printf("\n");
// 	    	size-=16;
// 	  }
//     	assert(fread(buffer,1,size,file) == size);
//         printf(" 0x%08x  ",addr);//number in hex.
//         addr+=16;
//         for(i=0;i<N;i++){
//             if(i<size){
//                 printf("%02x",buffer[i]);
//             }
//             else{
//                 printf("  ");
//             }
// 	    if((i+1)%4==0){
// 		printf(" ");
// 	    } 
//          }

//     	for(i=0;i<N;i++){
//             if(i<size){
//       		printf("%c",isprint(buffer[i])?buffer[i]:'.');
//             }
//             else{
//                 printf(" ");
//             }
// 	}

//     	printf("\n");

// }



// void content_section(char* name, Elf32_Shdr* section ,Elf32_Ehdr* entete, FILE *fichier){

// 	Elf32_Shdr sectionHexa;
// 	int offset = entete->e_shoff + entete->e_shstrndx * entete->e_shentsize;

// 	//repositionner le cursseur au debut du fichier
// 	rewind (fichier);

// 	//se deplacer par le debut du des entetes de section + inde
// 	assert(fseek(fichier, offset , SEEK_SET) != -1);

// 	//charger la table string
//     assert(sizeof(Elf32_Shdr) ==fread(&sectionHexa, sizeof(char), sizeof(Elf32_Shdr), fichier));  

//     assert(fseek(fichier,bswap_32(sectionHexa.sh_offset), SEEK_SET) != -1);

//     //alocations de la table de string
//     Elf32_Shdr* sectionHex =  malloc(sizeof(unsigned char)*bswap_32(sectionHexa.sh_size));
// 	//elf->sectionHexa = malloc(sizeof(unsigned char)*bswap_32(sectionHexa.sh_size));

//     assert( bswap_32(sectionHexa.sh_size) == fread(sectionHex, sizeof(char),bswap_32(sectionHexa.sh_size), fichier) );	
    
//     affiche_contentSection(sectionHex, section, entete,name);
// }

		


// void initElf(Elf32_info *elf,FILE *file){
// 	assert(sizeof(elf->header) == fread(&elf->header,1,sizeof(elf->header),file));
	
// 	if(elf->header.e_ident[EI_DATA]==2){		
// 		setup_little_endian(elf);
// 	}
// 	elf->section = malloc(sizeof(Elf32_Shdr) * bswap_16(elf->header.e_shnum));
// 	assert(elf->section != NULL);
// 	lire_Section_table(elf,file);
// }	


// void setup_little_endian(Elf32_info *elf){

// 	elf->header.e_type = bswap_16(elf->header.e_type);
// 	elf->header.e_machine = bswap_16(elf->header.e_machine);
// 	elf->header.e_version = bswap_32(elf->header.e_version);
// 	elf->header.e_entry = bswap_32(elf->header.e_entry);
// 	elf->header.e_phoff = bswap_32(elf->header.e_phoff);
// 	elf->header.e_shoff = bswap_32(elf->header.e_shoff);
// 	elf->header.e_flags = bswap_32(elf->header.e_flags);
// 	elf->header.e_ehsize = bswap_16(elf->header.e_ehsize);
// 	elf->header.e_phentsize = bswap_16(elf->header.e_phentsize);
// 	elf->header.e_phnum = bswap_16(elf->header.e_phnum);
// 	elf->header.e_shentsize = bswap_16(elf->header.e_shentsize);
// 	elf->header.e_shnum = bswap_16(elf->header.e_shnum);
// 	elf->header.e_shstrndx = bswap_16(elf->header.e_shstrndx);
	



// void lire_Section_table(Elf32_info *elf,FILE *file){
// 		int i;
// 		int sechnum=elf->header.e_shnum;
// 		assert(fseek(file,elf->header.e_shoff,SEEK_SET) != -1);
// 		for(i=0;i<sechnum;i++){			
// 			assert(sizeof(Elf32_Shdr) == fread(&elf->section[i],sizeof(char),sizeof(Elf32_Shdr),file));
			
// 			if(elf->header.e_ident[EI_DATA]==2){	
// 				elf->section[i].sh_name = bswap_32(elf->section[i].sh_name);
// 				elf->section[i].sh_type = bswap_32(elf->section[i].sh_type);
// 				elf->section[i].sh_addr = bswap_32(elf->section[i].sh_addr);
// 				elf->section[i].sh_offset = bswap_32(elf->section[i].sh_offset);
// 				elf->section[i].sh_size = bswap_32(elf->section[i].sh_size);
// 				elf->section[i].sh_entsize = bswap_32(elf->section[i].sh_entsize);
// 				elf->section[i].sh_flags = bswap_32(elf->section[i].sh_flags);
// 				elf->section[i].sh_link = bswap_32(elf->section[i].sh_link);
// 				elf->section[i].sh_info = bswap_32(elf->section[i].sh_info);
// 				elf->section[i].sh_addralign = bswap_32(elf->section[i].sh_addralign);
// 			}
			
// 		}
// } 

