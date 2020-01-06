#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fusion.h"

void freemem(void *adr){
	if (adr != NULL) free (adr);
}


Elf32_Shdr*  init_sections(Elf32_Shdr* sh, int* nbS, Elf32_Shdr* sections_table, Elf32_Ehdr* header){
	sh = malloc(sizeof(Elf32_Shdr)*header->e_shnum);
	assert (sh != NULL);
	int j=0;
    for(int i=0; i < header->e_shnum; i++){
            memcpy(&sh[j],&sections_table[i],sizeof(Elf32_Shdr));
            j++;
    }
    *nbS = j;
    return sh;
}

void init_fusion(Donnees* d, Elf32_Shdr* sections_table1, Elf32_Ehdr* header1, Elf32_Shdr* sections_table2, Elf32_Ehdr* header2){
	d->nbS1 = 0;
	d->nbS2 = 0;
	d->offset = 0;
	d->o_ecris = 1;
	d->sh1 = init_sections(d->sh1,&d->nbS1,sections_table1, header1);
	d->sh2 = init_sections(d->sh2,&d->nbS2,sections_table2, header2);
	d->f = (Donnees_fusion *) malloc(sizeof(Donnees_fusion) * d->nbS1);
	assert(d->f!=NULL);

}

char* get_section_name(Elf32_Ehdr* header, Elf32_Shdr* sections_table, int idx, FILE* fichier){
	char *name = malloc(SIZENAME*sizeof(char));
	fseek(fichier, sections_table[header->e_shstrndx].sh_offset + sections_table[idx].sh_name, SEEK_SET);
	char *s=fgets(name, SIZENAME, fichier);
	assert (s != NULL);
	return name;
}


int is_in(char * nameS,Elf32_Ehdr* h, Elf32_Shdr* t, FILE* f){
    for(int i=0; i < h->e_shnum; i++)
		if (strcmp(nameS, get_section_name(h,t,i,f))==0)
			return i;
	return -1;
}

void fusion_section1_in_section2(Donnees* d,FILE * file_in1, FILE* file_in2, Elf32_Ehdr* h1 ,Elf32_Ehdr* h2, Elf32_Word type){
	int idx;
	for (int i = 1; i < d->nbS1; ++i){
		if(d->sh1[i].sh_type == type){
			if(((idx=is_in(get_section_name(h1, d->sh1, i, file_in1), h2 , d->sh2, file_in2)) != -1) && d->f[i].newsh == NULL){
					 d->f[i].newsh = malloc(sizeof(Elf32_Shdr)*2);
					 assert(d->f[i].newsh != NULL);

					//  if (d->offset == 0)
					//  {	d->offset = d->sh1[i].sh_offset;}

						 memcpy(&d->f[i].newsh[0],&d->sh1[i], sizeof(Elf32_Shdr));
						 memcpy(&d->f[i].newsh[1],&d->sh2[idx], sizeof(Elf32_Shdr));
						 d->f[i].nbS = 2;
						 d->f[i].type = type;

					
						/*modificaion des offsets des deux sections a fusionnees*/
						 d->offset = d->f[i].newsh[0].sh_offset;
						 d->f[i].newsh[1].sh_offset = d->f[i].newsh[0].sh_offset + d->f[i].newsh[0].sh_size;

						/* modificaion du size global des deux sections a fusionnees */
						//d->f[i].size = d->sh1[i].sh_size + d->sh2[idx].sh_size;
						d->f[i].size = d->f[i].newsh[0].sh_size +  d->f[i].newsh[1].sh_size;

			}else{
				if (d->f[i].newsh == NULL){
					d->f[i].newsh = malloc(sizeof(Elf32_Shdr)*1);
					assert(d->f[i].newsh != NULL);
					memcpy(&d->f[i].newsh[0], &d->sh1[i],  sizeof(Elf32_Shdr));
					d->f[i].type = type;
					d->f[i].nbS = 1;

					/* modificaion du size global */
					d->f[i].size = d->sh1[i].sh_size;

					/*modificaion d'offset d'une section n'existe pas dans le deuxieme fichier  */
					d->f[i].newsh[0].sh_offset = d->offset;
					
				}
			}

			/* modification des offsets globals  */
			d->f[i].offset = d->f[i].newsh[0].sh_offset;
			d->offset = d->f[i].offset + d->f[i].size; 
		}
	}
}
void fusion_section2_in_section1(Donnees* d,FILE * file_in1, FILE* file_in2, Elf32_Ehdr* h1 ,Elf32_Ehdr* h2, Elf32_Word type){
	for (int i = 1; i < d->nbS2; ++i){
		if(d->sh2[i].sh_type == type){
				if(is_in(get_section_name(h2, d->sh2, i, file_in2), h1 , d->sh1, file_in1) == -1 ){
					d->nbS1++;
					d->f = (Donnees_fusion *)realloc(d->f,sizeof(Donnees_fusion) * d->nbS1);
					d->f[d->nbS1-1].newsh = malloc(sizeof(Elf32_Shdr)*1);
				 	assert(d->f[d->nbS1-1].newsh != NULL);
					memcpy(&d->f[d->nbS1-1].newsh[0],&d->sh2[i], sizeof(Elf32_Shdr));
					d->f[d->nbS1-1].type = type;
					d->f[d->nbS1-1].nbS = 1;

					/* modificaion du size global */
					d->f[d->nbS1-1].size =  d->sh2[i].sh_size;

					/*modificaion d'offset d'une section n'existe pas dans le premier fichier  */
					d->f[d->nbS1-1].newsh[0].sh_offset = d->offset;
					d->f[d->nbS1-1].offset = d->offset;
					d->offset += d->f[d->nbS1-1].size; 
				}
		}
	}
}
void fusion_by_type(Donnees* d,FILE * file_in1, FILE* file_in2, Elf32_Ehdr* h1 ,Elf32_Ehdr* h2, Elf32_Word type){
	// if(type == SHT_NULL){
	// 	d->f[0].newsh = malloc(sizeof(Elf32_Shdr)*1);
	// 	assert(d->f[0].newsh != NULL);
	// 	memcpy(&d->f[0].newsh[0], &d->sh1[0],  sizeof(Elf32_Shdr));
	// 	d->f[0].type = type;
	// 	d->f[0].nbS = 1;
	// 	d->f[0].size = 0;
	// 	d->f[0].offset = 0;

	// }
	fusion_section1_in_section2(d,file_in1,file_in2, h1 ,h2, type);
    fusion_section2_in_section1(d,file_in1,file_in2,  h1 ,h2, type);
}



void modification_indx_sections(Donnees * d){
	// for (int i = 1; i < d->nbS1; ++i){
 //    	   	d->f[i].newsh[0].sh_link = newsec[d->f[i].newsh[0].sh_link];
	// 		d->f[i].newsh[0].sh_info = newsec[d->f[i].newsh[0].sh_info];
 //    }
}

