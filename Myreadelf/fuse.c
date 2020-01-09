#include "reloc_table.h"
#include "fusion_reloc.h"
#include "print_header"
#include "section_header"

typedef struct{
	int size, ind[];
} memo;



bool already_read(memo mem, int numSec){

	int i;

	for (i = 0; i < mem.size; i++){
		if (m.ind[i] == numSec)
			return true;
	}

	return false;
}


Elf32_Rel *read_rel (FILE *file, Elf32_Shdr sectionHeader){

	int i;
	Elf32_Rel *rel;
	Elf32_Ehdr e = lecture_entete(file);
	Elf32_Shdr s = read_section(file, s, e);
	Elf32_Sym sym = lecture_symb(file, s, e, &nbsymb, &i);

	// On parcours la section
	for (i = 0; i < sectionHeader.sh_size; i++){
		fseek(file, sectionHeader.sh_offset+i, SEEK_SET);

		// Récupération et correction de l'offset et de info
		//Elf32_Addr offset = read_offset(file);
		rel = read_REL_tab(e, s, file, i);
		


		// Récupération et correction de info
		//Elf32_Word info = read_info(file);

		// TODO : Ecriture







		Elf32_Sword addend = read_addend(file);

		// Correction de l'addend
		if (ELF32_ST_TYPE(sym[ELF32_R_SYM(info)].st_info) == STT_SECTION){
			switch(ELF32_R_TYPE(info)){
				case R_ARM_ABS32:
				case R_ARM_ABS16:
				case R_ARM_ABS8:
					addend += sectionHeader[i].sh_offset;
					break;

				case R_ARM_JUMP24:
				case R_ARM_CALL:
					addend += sectionHeader[i].sh_offset/4;
			}
			// TODO : Ecriture
		}

	}
	return rel;
}



int concat(Elf32_Rel *relOne, Elf32_Rel *relTwo, int RelSizeOne, int RelSizeTwo){

	int i,cpt=0;

	for (i = RelSizeOne; i < relSizeTwo; i++){
		relOne[i].r_offset = realloc(relOne[i].r_offset, sizeof(relOne.r_offset));
		relOne[i].r_info   = realloc(relOne[i].r_info, sizeof(relOne[i]r_info));

		relOne[i].r_offset = relTwo[i].r_offset;
		relOne[i].r_info   = relTwo[i].r_info;
	}

	// augmenter la taille de relOne
	return RelSizeOne+=RelSizeTwo;
}


Elf32_Rel merge_rel(Elf32_Ehdr e1, Elf32_Ehdr e2, Elf32_Shdr *s1, Elf32_Shdr *s2, Elf32_Sym *Sym1, Elf32_Sym *Sym2, FILE* f1, FILE* f2){

	int numSecOne, numSecTwo;
	int typeSecOne, typeSecTwo;
	char *name1=malloc(SIZENAME*sizeof(char));
	char *name2=malloc(SIZENAME*sizeof(char));

	memo mem;
	mem.size = 0;
	Elf32_Rel relOne, relTwo;
	int cpt = 0;

	// On parcourt les sections du 1er fichier
	for (numSecOne = 0; numSecOne < e1.e_shnum; numSecOne++){
		typeSecOne = s1[numSecOne]->sh_type;

		if (typeSecOne == SHT_REL){
			relOne = read_rel(f1, s1);
			int j = 0;

			int RelSizeOne=s1[numSecOne].sh_size/s1[numSecOne].sh_entsize;
			int	RelSizeTwo=s2[numSecOne].sh_size/s2[numSecOne].sh_entsize;
			cpt = RelSizeOne;
			// On parcours le 2ème fichier 
			for (numSecTwo = 0; numSecTwo < e2.e_shnum; numSecTwo++){
				typeSecTwo = s2[numSecTwo]->sh_type;
				


				//recuperation du nom de la premiere table
				int indice_name1 = ELF32_R_SYM(relOne[j].r_info);
				int indice_symbol1 = ELF32_R_SYM(relOne[j].r_info);
				char * Stype1 = get_sym_type(ELF32_ST_TYPE(Sym1[indice_symbol1].st_info));

				if(strcmp(Stype1,"NOTYPE")==0){
        			name1 = SymName(s1, Sym1[indice_name1].st_name, s1[indice_symbol1].sh_link, f1);
				}else if(strcmp(Stype1,"SECTION")==0){
        			name1 = SymName(s1, s1[Sym1[indice_name1].st_shndx].sh_name, e1.e_shstrndx, f1);
				}


				//recuperation du nom de la deuxieme table
				int indice_name2 = ELF32_R_SYM(relTwo[j].r_info);
				int indice_symbol2 = ELF32_R_SYM(relTwo[j].r_info);
				char * Stype2 = get_sym_type(ELF32_ST_TYPE(Sym2[indice_symbol2].st_info));

				if(strcmp(Stype2,"NOTYPE")==0){
        			name2 = SymName(s2, Sym2[indice_name2].st_name, s2[indice_symbol2].sh_link, f2);
				}else if(strcmp(Stype2,"SECTION")==0){
        			name2 = SymName(s2, s2[Sym2[indice_name2].st_shndx].sh_name, e2.e_shstrndx, f2);
				}


				if (typeSecTwo == SHT_REL && !already_read(mem, numSecTwo) && !strcmp(name1,name2)){
					relTwo = read_rel(f2, s2);
					
					// Concaténation 
					concat(relOne[numSecOne], relTwo[numSecTwo], RelSizeOne);

					// On mémorise dans ind[] le numéro de la section pour ne pas la relire 
					mem.ind[m.size] = numSecTwo;
					size++;
				}

				else if (typeSecTwo == SHT_REL && !already_read(mem, numSecTwo) && strcmp(name1,name2)){
					relOne[cpt].r_offset=relTwo[numSecTwo].r_offset;
					relOne[cpt].r_info=relTwo[numSecTwo].r_info;
					cpt++;
				}
				j++;
			}
		}
	}
	return relOne;
}



