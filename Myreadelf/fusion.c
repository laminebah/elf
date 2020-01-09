#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "fusion.h"
#include "print_content_section.h"
#include "Elf_symbole.h"


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
			if((idx=is_in(get_section_name(h1, d->sh1, i, file_in1), h2 , d->sh2, file_in2)) != -1){
					 d->f[i].newsh = malloc(sizeof(Elf32_Shdr)*2);
					 assert(d->f[i].newsh != NULL);

						 memcpy(&d->f[i].newsh[0],&d->sh1[i], sizeof(Elf32_Shdr));
						 memcpy(&d->f[i].newsh[1],&d->sh2[idx], sizeof(Elf32_Shdr));
						 d->f[i].nbS = 2;
						 d->f[i].type = type;

						 /* copie le nom de la section */
						 d->f[i].name = get_section_name(h1, d->sh1, i, file_in1);
					
						/*modificaion des offsets des deux sections a fusionnees*/
						 if (i==1){
						 	d->offset = d->f[i].newsh[0].sh_offset;
						 }
						 d->f[i].offset = d->offset;
						 d->f[i].newsh[0].sh_offset = d->offset;
						

						/* modificaion du size global des deux sections a fusionnees */
						d->f[i].size = d->sh1[i].sh_size +  d->sh2[idx].sh_size;	
						d->f[i].newsh[0].sh_size += d->sh2[idx].sh_size;

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
					d->f[i].size = d->sh1[0].sh_size;

					/*modificaion d'offset d'une section n'existe pas dans le deuxieme fichier  */
					d->f[i].newsh[0].sh_offset = d->offset;
					d->f[i].offset = d->offset;
				}
			}

			/* modification des offsets <de la section i> et de l'offset global  */
					d->f[i].newsh[0].sh_offset = d->offset;
					d->offset += d->f[i].size;
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

					/*  Modification du size et les offsets */
				 	d->f[d->nbS1-1].size = d->sh2[i].sh_size;
					d->f[d->nbS1-1].offset = d->offset;
					d->f[d->nbS1-1].newsh[0].sh_offset = d->offset;
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
		d->offset = 0;
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
  ptr = (char* ) malloc(sizeof(char) * SIZEN);
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
	elf_head->e_shstrndx = get_index_section(d,".shstrtab");
	size_t nmemb;

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


Elf32_Off ecriture_champs_section(int i, long set, Elf32_Shdr section, FILE* file, Donnees* d){
	Elf32_Off offset = 0;
	fseek(file, set, SEEK_SET);
	offset += fwrite(&d->f[i].newsh[0].sh_name,   sizeof(d->f[i].newsh[0].sh_name), 1, file);
	offset += fwrite(&d->f[i].type,      sizeof(d->f[i].type), 1, file);
	offset += fwrite(&d->f[i].newsh[0].sh_flags,     sizeof(d->f[i].newsh[0].sh_flags), 1, file);
	offset += fwrite(&d->f[i].newsh[0].sh_addr,      sizeof(d->f[i].newsh[0].sh_addr), 1, file);
	offset += fwrite(&d->f[i].newsh[0].sh_offset,    sizeof(d->f[i].offset), 1, file);
	offset += fwrite(&d->f[i].newsh[0].sh_size,      sizeof(d->f[i].size), 1, file);
	offset += fwrite(&d->f[i].sh_link,      sizeof(d->f[i].sh_link), 1, file);
	offset += fwrite(&d->f[i].sh_info,      sizeof(d->f[i].sh_info), 1, file);
	offset += fwrite(&d->f[i].newsh[0].sh_addralign, sizeof(d->f[i].newsh[0].sh_addralign), 1, file);
	offset += fwrite(&d->f[i].newsh[0].sh_entsize,   sizeof(d->f[i].newsh[0].sh_entsize), 1, file);

	return offset;
}

void ecriture_section_table(Elf32_Ehdr* elf_head, FILE* file, Donnees* d){
	Elf32_Off offset = 0;

	for(int i = 0; i < d->nbS1; i++){

		fseek(file, d->f[elf_head->e_shstrndx].offset + d->f[i].newsh[0].sh_name, SEEK_SET);
		fputs(d->f[i].name , file);
	}

	for(int i = 0; i < d->nbS1; i++){
		long set = d->offset + i * elf_head->e_shentsize;
		offset = ecriture_champs_section(i, set, d->f[i].newsh[0], file, d);
		if(d->f[i].nbS==2){
			offset = ecriture_champs_section(i, set, d->f[i].newsh[1], file, d);
		}

	}
	d->offset = offset;
}

//le nom d'un symbole à l'indice i
char *nom_symbole_ndx (Elf32_Sym *symb,FILE *fin,Elf32_Shdr *sectTab,int index,int i) {
	char *tampon ;
	int linksecsym,nmemb;

	linksecsym = sectTab[index].sh_link;
	tampon = malloc(sectTab[linksecsym].sh_size);  	         assert (tampon != NULL);
	nmemb  = fseek(fin,sectTab[linksecsym].sh_offset,SEEK_SET); assert (nmemb != -1);
	nmemb  = fread (tampon,sectTab[linksecsym].sh_size,1,fin);
	return tampon+symb[i].st_name;
	
}
char **recup_nom_tab (Elf32_Shdr *sectTab,int ndxsect,FILE *fin,Elf32_Ehdr *head,Elf32_Sym *symb,int nbs) {
	char **tab = malloc (sizeof(char*)*sectTab[ndxsect].sh_size);
	for (int i = 0; i < nbs;i ++) {
		tab [i] = malloc (sizeof(char)*strlen(nom_symbole_ndx (symb,fin,sectTab,ndxsect,i)));
		assert (tab[i] != NULL);
		strcpy (tab[i],"");
		strcat(tab[i],nom_symbole_ndx (symb,fin,sectTab,ndxsect,i));
	} 
	return tab;
}
SymbTab *lire_symb_tab_out (FILE *fin,Elf32_Shdr *sectTab,int shtype,Elf32_Ehdr *head,int ndx,int nbs,Elf32_Sym *tabSym) {
	SymbTab *symb;
	symb = malloc (sizeof(SymbTab));
	assert (symb != NULL);
	symb->symbTab = malloc(sizeof(Elf32_Sym) * nbs * 10);
	assert (ndx != -1 && symb -> symbTab!= NULL);
	symb -> symbTab = lecture_symb(fin,sectTab,*head,&nbs,&ndx);
	symb -> ndxStr = ndx ;
	symb -> nbsymb = nbs;
	symb -> nomSymbs = recup_nom_tab (sectTab,ndx,fin,head,symb->symbTab,nbs);
	symb -> tailnoms = head -> e_shnum;
	symb -> nom = get_section_name(head,sectTab,ndx,fin);
	return symb; 
}
int ndx_nom (SymbTab *sout,char * buff) {
	int i = 0;
	while (i < sout -> nbsymb && strcmp (sout->nomSymbs[i],buff)) i ++;
	return i < sout -> nbsymb ? i : -1;
}

static inline void swap_symbols(Elf32_Sym *sym1, Elf32_Sym *sym2)
{
	Elf32_Sym *tmp = malloc(sizeof(Elf32_Sym));
	memcpy(tmp, sym2, sizeof(Elf32_Sym));
	memcpy(sym2, sym1, sizeof(Elf32_Sym));
	memcpy(sym1, tmp, sizeof(Elf32_Sym));
	free(tmp);
}
 void sort_new_symbol_table(SymbTab *sout) {
	int j, cpt = 0;

	for(int i = 1; i < sout->nbsymb; i++)
	{
		if(ELF32_ST_TYPE(sout->symbTab[i].st_info) == STT_SECTION)
		{
			cpt++;
			continue;
		}

		for(j = i; (j < sout->nbsymb) && ELF32_ST_TYPE(sout->symbTab[j].st_info) != STT_SECTION; j++);
		if(j == sout->nbsymb)
			continue;

		swap_symbols(&sout->symbTab[i],&sout->symbTab[j]);
		cpt++;
	}

	for(int i = cpt; i < sout->nbsymb; i++)
	{
		if(ELF32_ST_BIND(sout->symbTab[i].st_info) != STB_GLOBAL)
			continue;

		for(j = sout->nbsymb - 1; (j >= i) && ELF32_ST_BIND(sout->symbTab[j].st_info) == STB_GLOBAL; j--);
		if(j < i)
			continue;

		swap_symbols(&sout->symbTab[i],&sout->symbTab[j]);
	}
}

SymbTab *fusion_symbols (Donnees* d,Elf32_Shdr *sec1,Elf32_Shdr *sec2,Elf32_Ehdr *h1,Elf32_Ehdr *h2,
	Elf32_Sym *sym1,Elf32_Sym *sym2,FILE *f1,FILE *f2,int nbs2,int ndxsym2,int ndxsym1,int nbs1) {
	SymbTab *sout = malloc (sizeof(SymbTab)*2);
	char *buff = malloc (sizeof(char)*50);
	SymbTab *st_s2 = malloc (sizeof (SymbTab)*2);
	int nb = 0;
	if (sout == NULL || st_s2 == NULL) return NULL;
	if (nbs1 >= nbs2) {
		sout = lire_symb_tab_out (f1,sec1,SHT_SYMTAB,h1,ndxsym1,nbs1,sym1);
		st_s2 = lire_symb_tab_out (f2,sec2,SHT_SYMTAB,h2,ndxsym2,nbs2,sym2);
	} 
	else  {
		sout = lire_symb_tab_out (f2,sec2,SHT_SYMTAB,h2,ndxsym2,nbs2,sym2);
		st_s2 = lire_symb_tab_out (f1,sec1,SHT_SYMTAB,h1,ndxsym1,nbs1,sym1);
	}
	nb = sout -> nbsymb;
	//on parcourt celui qui a le  plus petit nombre de symbole
	for (int i = 0; i < st_s2 -> nbsymb ; i ++) {
		strcpy (buff,"");
		strcat (buff,st_s2 -> nomSymbs [i]);
		int nd =  ndx_nom (sout,buff);
		if (nd > 0) {
			int is_gOut = ELF32_ST_BIND (sout -> symbTab[nd].st_info) == STB_GLOBAL;
			int is_gSt2 = ELF32_ST_BIND (st_s2 -> symbTab[i].st_info) == STB_GLOBAL;
			if (is_gOut && is_gSt2 && sout->symbTab[nd].st_shndx != SHN_UNDEF && st_s2->symbTab[i].st_shndx != SHN_UNDEF) {
				printf ("multiple définition de %s\n",buff);
				return NULL;
			} else if (is_gOut && is_gSt2 && sout->symbTab[nd].st_shndx == SHN_UNDEF && st_s2->symbTab[i].st_shndx != SHN_UNDEF) {
				sout->symbTab[nd]. st_value   = st_s2 -> symbTab [i].st_value; 
				sout->symbTab[nd]. st_size    += st_s2 -> symbTab [i].st_size;
				sout->symbTab[nd]. st_info = st_s2 -> symbTab [i].st_info;
				sout->symbTab[nd]. st_other   = st_s2 -> symbTab [i].st_other;
				sout->symbTab[nd]. st_shndx = st_s2 -> symbTab [i].st_shndx;

			} else if (is_gOut && is_gSt2) {
				sout->symbTab[nd]. st_value   = st_s2 -> symbTab [i].st_value; 
				sout->symbTab[nd]. st_size    += st_s2 -> symbTab [i].st_size;
				sout->symbTab[nd]. st_info = st_s2 -> symbTab [i].st_info;
				sout->symbTab[nd]. st_other   = st_s2 -> symbTab [i].st_other;
				sout->symbTab[nd]. st_shndx = st_s2 -> symbTab [i].st_shndx;

			} else if (!is_gOut && !is_gSt2) {
				//on ajoute ce  symbole
				int id = nb;
				sout->symbTab = realloc (sout->symbTab,10*(id + 1)*sizeof(Elf32_Sym));
				sout->symbTab [id].st_value = st_s2 -> symbTab [i].st_value;
				sout->symbTab [id].st_size = st_s2->symbTab [i].st_size;
				sout->symbTab [id].st_info = st_s2->symbTab [i].st_info;
				sout->symbTab [id].st_other = st_s2->symbTab [i].st_other;
				sout->symbTab [id].st_shndx = st_s2->symbTab [i].st_shndx;
				sout->nomSymbs = realloc (sout->nomSymbs,10*id*sizeof(char*));
				sout->nomSymbs [id] = malloc (sizeof(char)*strlen(st_s2->nomSymbs[i]));
				strcpy(sout->nomSymbs[id],"");
				strcat(sout->nomSymbs[id],st_s2->nomSymbs[i]);
				nb ++;
			}

		} else {
			if (ELF32_ST_BIND (st_s2 -> symbTab[i].st_info) == STB_GLOBAL) {
				//on ajoute le second;
				int id = nb;
				sout->symbTab = realloc (sout->symbTab,10*(id + 1)*sizeof(Elf32_Sym));
				sout->symbTab [id].st_value = st_s2 -> symbTab [i].st_value;
				sout->symbTab [id].st_size = st_s2->symbTab [i].st_size;
				sout->symbTab [id].st_info = st_s2->symbTab [i].st_info;
				sout->symbTab [id].st_other = st_s2->symbTab [i].st_other;
				sout->symbTab [id].st_shndx = st_s2->symbTab [i].st_shndx;
				sout->symbTab [id].st_name  = st_s2-> symbTab [i].st_name;
				sout->nomSymbs = realloc (sout->nomSymbs,10*id*sizeof(char*));
				sout->nomSymbs [id] = malloc (sizeof(char)*strlen(st_s2->nomSymbs[i]));
				strcpy(sout->nomSymbs[id],"");
				strcat(sout->nomSymbs[id],st_s2->nomSymbs[i]);
				nb ++;
			}
			if (ELF32_ST_BIND (st_s2 -> symbTab[i].st_info) == STB_LOCAL && 
				ELF32_ST_BIND (sout -> symbTab[i].st_info) == STB_LOCAL) {
				if (strlen(st_s2->nomSymbs[i]) > 0) {
					int id = nb;
					sout->symbTab = realloc (sout->symbTab,10*(id + 1)*sizeof(Elf32_Sym));
					sout->symbTab [id].st_value = st_s2 -> symbTab [i].st_value;
					sout->symbTab [id].st_size = st_s2->symbTab [i].st_size;
					sout->symbTab [id].st_info = st_s2->symbTab [i].st_info;
					sout->symbTab [id].st_other = st_s2->symbTab [i].st_other;
					sout->symbTab [id].st_shndx = st_s2->symbTab [i].st_shndx;
					sout->symbTab [id].st_name  = st_s2-> symbTab [i].st_name;
					sout->nomSymbs = realloc (sout->nomSymbs,10*id*sizeof(char*));
					sout->nomSymbs [id] = malloc (sizeof(char)*strlen(st_s2->nomSymbs[i]));
					strcpy(sout->nomSymbs[id],"");
					strcat(sout->nomSymbs[id],st_s2->nomSymbs[i]);
					nb ++;
				}
			}
		}
	}
	sout->nbsymb = nb;
	for (int i = 0; i < sout->nbsymb; i ++) {
		printf("%d:%06d:%s\n",i,sout->symbTab[i].st_value,sout->nomSymbs[i]);
	}
	return sout;	
}

void ecris_new_symb_in_file (FILE *fout,Donnees *d,SymbTab *sout) {
	int ind = get_index_section(d ,".symtab");
	int oldf = d -> f[ind].offset;
	int n_of = oldf;
	fseek (fout,oldf,SEEK_SET);
	for (int i = 0; i < sout->nbsymb ; i ++) {
		n_of += fwrite(&sout->symbTab[i].st_name,sizeof(sout->symbTab[i].st_name),1,fout);
		n_of += fwrite(&sout->symbTab[i].st_value,sizeof(sout->symbTab[i].st_value),1,fout);
		n_of += fwrite(&sout->symbTab[i].st_size,sizeof(sout->symbTab[i].st_size),1,fout);
		n_of += fwrite(&sout->symbTab[i].st_info,sizeof(sout->symbTab[i].st_info),1,fout);
		n_of += fwrite(&sout->symbTab[i].st_other, sizeof(sout->symbTab[i].st_other),1,fout);
		n_of += fwrite(&sout->symbTab[i].st_shndx,sizeof(sout->symbTab[i].st_shndx),1,fout);
		oldf = n_of;
	}
	d -> f[ind].newsh -> sh_size = sout->nbsymb *sizeof(Elf32_Sym);
	ind = get_index_section (d,".strtab");
	printf ("ecriture_entete de la tables des noms des symboles dans le fichier\n");
	fseek (fout,d->f[ind].offset,SEEK_SET);
	int nb = 0;
	d -> f [ind].newsh -> sh_size = nb; 
}

void freemem(void *adr){
	if (adr != NULL) free (adr);
}

void liberer_elf(Elf32_Ehdr * elf_head1 , Elf32_Ehdr * elf_head2 , Elf32_Shdr* sections_table1,Elf32_Shdr*  sections_table2){
  freemem (elf_head1);
  freemem (elf_head2);
  freemem (sections_table1);
  freemem (sections_table2);
}

void liberer_fusion(Donnees* d){
	freemem(d->sh1);
	freemem(d->sh2);
	for (int i = 0; i < d->nbS1; ++i){
		freemem(d->f[i].newsh);
	}
	freemem(d->f);
	freemem(d);
}

void fermer_fichiers(FILE* file_in1,FILE* file_in2, FILE* file_out){
  fclose (file_in1);
  fclose (file_in2);
  fclose (file_out);
}
