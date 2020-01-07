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
	for (int i = 0; i < d->nbS1; ++i){
		d->f[i].type = -1;
		d->f[i].sh_link = 0;
		d->f[i].sh_info = 0;
	}

}

char* get_section_name(Elf32_Ehdr* header, Elf32_Shdr* sections_table, int idx, FILE* fichier){
	char *name = malloc(SIZENAME*sizeof(char));
	assert (name!= NULL);
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

						 memcpy(&d->f[i].newsh[0],&d->sh1[i], sizeof(Elf32_Shdr));
						 memcpy(&d->f[i].newsh[1],&d->sh2[idx], sizeof(Elf32_Shdr));
						 d->f[i].nbS = 2;
						 d->f[i].type = type;

						 /* copie le nom de la section */
						 d->f[i].name = get_section_name(h1, d->sh1, i, file_in1);
					
						/*modificaion des offsets des deux sections a fusionnees*/
						 d->offset = d->f[i].newsh[0].sh_offset;
						 d->f[i].newsh[1].sh_offset = d->f[i].newsh[0].sh_offset + d->f[i].newsh[0].sh_size;

						/* modificaion du size global des deux sections a fusionnees */
						d->f[i].size = d->f[i].newsh[0].sh_size +  d->f[i].newsh[1].sh_size;

			}else{
				if (d->f[i].newsh == NULL){
					d->f[i].newsh = malloc(sizeof(Elf32_Shdr)*1);
					assert(d->f[i].newsh != NULL);
					memcpy(&d->f[i].newsh[0], &d->sh1[i],  sizeof(Elf32_Shdr));
					d->f[i].type = type;
					d->f[i].nbS = 1;

					/* copie le nom de la section */
					d->f[i].name = get_section_name(h1, d->sh1, i, file_in1);

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

					/* copie le nom de la section */
					d->f[d->nbS1-1].name = get_section_name(h2, d->sh2, i, file_in2);

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
	if(type == SHT_NULL){
		d->f[0].newsh = malloc(sizeof(Elf32_Shdr)*1);
		assert(d->f[0].newsh != NULL);
		memcpy(&d->f[0].newsh[0], &d->sh1[0],  sizeof(Elf32_Shdr));
		d->f[0].type = type;
		d->f[0].nbS = 1;
		d->f[0].size = 0;
		d->f[0].offset = 0;
		/* copie le nom de la section */
		d->f[0].name = get_section_name(h1, d->sh1, 0, file_in1);
	}else{
			fusion_section1_in_section2(d,file_in1,file_in2, h1 ,h2, type);
    		fusion_section2_in_section1(d,file_in1,file_in2,  h1 ,h2, type);
	}
}

char* split_name(char * name){
  char* ptr = NULL;
  int i=1,k=0;
  ptr = (char* ) malloc(sizeof(char) * SIZENAME);
  if (ptr == NULL)
      printf("erreur d'allocation pour ptr\n");

  while(name[i+1]!= '.'){
    	i++;
  }
  for (int j = i+1; j < strlen(name); ++j){
  		ptr[k] = name[j];
  		k++;
  }
  ptr[k] = '\0';
  return ptr;
}


int get_index_section(Donnees * d , char* name){
	int indx = -1;
	for (int j = 0; j < d->nbS1; ++j){
		if (strcmp( name , d->f[j].name) == 0)
			indx = j;
	}
	return indx;
}

void modification_indx_sections(Donnees * d){
	for (int i = 1; i < d->nbS1; ++i){
				switch(d->f[i].type){
					case SHT_REL:
							d->f[i].sh_link  = get_index_section(d,".symtab");
							d->f[i].sh_info = get_index_section(d,split_name(d->f[i].name));
						break;

					case SHT_RELA:
							d->f[i].sh_link  = get_index_section(d,".symtab");
							d->f[i].sh_info = get_index_section(d,split_name(d->f[i].name));
						break;

					case SHT_SYMTAB:
							d->f[i].sh_link  = get_index_section(d,".strtab");	
							d->f[i].sh_info = get_index_section(d,".shstrtab");
						break;

					case SHT_DYNSYM:
							d->f[i].sh_link  = get_index_section(d,".strtab");	
							d->f[i].sh_info = get_index_section(d,".shstrtab");
						break;

					case SHT_HASH:
							d->f[i].sh_link  = get_index_section(d,".symtab");
						break;

					case SHT_DYNAMIC:
							d->f[i].sh_link  = get_index_section(d,".strtab");	
						break;

					case SHT_GROUP:
							d->f[i].sh_link  = get_index_section(d,".symtab");
						break;
					default:
						break;

				}
    }
}


void ecriture_entete(Elf32_Ehdr* elf_head, FILE* file, Donnees* d){

	elf_head->e_shoff = d->offset;
	elf_head->e_shnum = d->nbS1;
	size_t nmemb;

	//debug("Il y a %u sections dans le nouveau fichier ELF créé.\n", elf_head->e_shnum);
	fseek(file, 0, SEEK_SET);
	nmemb = fwrite(elf_head->e_ident,   EI_NIDENT,   1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_type, sizeof(elf_head->e_type), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_machine, sizeof(elf_head->e_machine), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_version,   sizeof(elf_head->e_version), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_entry,     sizeof(elf_head->e_entry), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_phoff,     sizeof(elf_head->e_phoff), 1,file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_shoff,     sizeof(elf_head->e_shoff), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_flags,     sizeof(elf_head->e_flags), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_ehsize,    sizeof(elf_head->e_ehsize), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_phentsize, sizeof(elf_head->e_phentsize), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_phnum,     sizeof(elf_head->e_phnum), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_shentsize, sizeof(elf_head->e_shentsize), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_shnum,     sizeof(elf_head->e_shnum), 1, file); assert(nmemb == 1);
	nmemb = fwrite(&elf_head->e_shstrndx,  sizeof(elf_head->e_shstrndx), 1, file); assert(nmemb == 1);

}


void ecriture_section_table(Elf32_Ehdr* elf_head, FILE* file, Donnees* d){
	Elf32_Off offset = 0;
	
	//int ind = 0;
	//Elf32_Off written = 0;
	

	
	// for(ind = 0; strcmp(d->f[ind].name, ".shstrtab")  ; ind++);
	// elf_head->e_shstrndx = ind;
	
	// // printf("Écriture de la table des noms de section dans le fichier à l'offset %#x\n", d->f[ind].offset);
	// fseek(file, d->f[ind].offset, SEEK_SET);
	// for(int i = 0; i < d->nbS1; i++)
	// {	
	// 		d->f[i].newsh->sh_name = written;
	// 		printf("%u\n", d->f[i].newsh->sh_name);
	// 		written += fwrite(d->f[i].name, 1, strlen(d->f[i].name) + 1, file);
	// }
	// d->f[ind].newsh[0].sh_size = written;

	//debug("Il y a %d en-têtes de section, débutant à l'adresse de décalage 0x%x dans le nouveau fichier créé:\n\n",elf_head->e_shnum, elf_head->e_shoff);
	fseek(file, elf_head->e_shoff, SEEK_SET);
	for(int i = 0; i < d->nbS1; i++)
	{
		//debug("Écriture de l'en-tête de section n°%2i  dans le fichier à l'offset %#x ", i, d->offset + i * elf_head->e_shentsize);
		fseek(file, d->offset + i * elf_head->e_shentsize, SEEK_SET);

		offset += fwrite(&d->sh1[i].sh_name,      sizeof(d->sh1[i].sh_name), 1, file);
		offset += fwrite(&d->sh1[i].sh_type,      sizeof(d->sh1[i].sh_type), 1, file);
		offset += fwrite(&d->sh1[i].sh_flags,     sizeof(d->sh1[i].sh_flags), 1, file);
		offset += fwrite(&d->sh1[i].sh_addr,      sizeof(d->sh1[i].sh_addr), 1, file);
		offset += fwrite(&d->sh1[i].sh_offset,    sizeof(d->sh1[i].sh_offset), 1, file);
		offset += fwrite(&d->sh1[i].sh_size,      sizeof(d->sh1[i].sh_size), 1, file);
		offset += fwrite(&d->sh1[i].sh_link,      sizeof(d->sh1[i].sh_link), 1, file);
		offset += fwrite(&d->sh1[i].sh_info,      sizeof(d->sh1[i].sh_info), 1, file);
		offset += fwrite(&d->sh1[i].sh_addralign, sizeof(d->sh1[i].sh_addralign), 1, file);
		offset += fwrite(&d->sh1[i].sh_entsize,   sizeof(d->sh1[i].sh_entsize), 1, file);
	}
	d->offset = offset;
}


