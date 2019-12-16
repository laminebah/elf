#include "section_header.h"

/////////////////////////////////////////////////////////
//////////////////////// étape 2 ////////////////////////
/////////////////////////////////////////////////////////


int32_t val_32(Elf32_Ehdr h, int32_t value){
	if(h.e_ident[EI_DATA]==ELFDATA2MSB)
		return __bswap_32(value);
	return value;
}


int16_t val_16(Elf32_Ehdr h, int16_t value){
	if(h.e_ident[EI_DATA]==ELFDATA2MSB)
		return __bswap_16(value);
	return value;
}




void read_section(FILE *fichier, Elf32_Shdr *s, Elf32_Ehdr h){
	fread(s,1,sizeof(Elf32_Shdr),fichier);
	s->sh_name        = val_32(h, s->sh_name);
	s->sh_type        = val_32(h, s->sh_type);
	s->sh_flags       = val_32(h, s->sh_flags);
	s->sh_addr        = val_32(h, s->sh_addr);
	s->sh_offset      = val_32(h, s->sh_offset);
	s->sh_size        = val_32(h, s->sh_size);
	s->sh_link        = val_32(h, s->sh_link);
	s->sh_info        = val_32(h, s->sh_info);
	s->sh_addralign   = val_32(h, s->sh_addralign);
	s->sh_entsize     = val_32(h, s->sh_entsize);

}


char *read_type (Elf32_Shdr s){
	char *type = "";
	switch(s.sh_type){
		case SHT_NULL:
			type = "NULL";
			break;
		case SHT_PROGBITS:
			type = "PROGBITS";
			break;
		case SHT_SYMTAB:
			type = "SYMTAB";
			break;
		case SHT_STRTAB:
			type = "STRTAB";
			break;
		case SHT_RELA:
			type = "RELA";
			break;
		case SHT_HASH:
			type = "HASH";
			break;
		case SHT_DYNAMIC:
			type = "DYNAMIC";
			break;
		case SHT_NOTE:
			type = "NOTE";
			break;
		case SHT_NOBITS:
			type = "NOBITS";
			break;
		case SHT_REL:
			type = "REL";
			break;
		case SHT_SHLIB:
			type = "SHLIB";
			break;
		case SHT_DYNSYM:
			type = "DYNSYM";
			break;
		case SHT_LOPROC:
			type = "LOPROC";
			break;
		case SHT_HIPROC:
			type = "HIPROC";
			break;
		case SHT_LOUSER:
			type = "LOUSER";
			break;
		case SHT_HIUSER:
			type = "HIUSER";
			break;
		default:
			break;
	}
	return type;
}


char *read_flags (char * flags, Elf32_Word flag){	
	
	if (SHF_WRITE & flag)
		strcat(flags, "W");
	if (SHF_ALLOC & flag)
		strcat(flags, "A");
	if (SHF_EXECINSTR & flag)
		strcat(flags, "X");
	if (SHF_INFO_LINK & flag)
		strcat(flags, "I");
	if (SHF_TLS & flag)
		strcat(flags, "T");
	if (SHF_GROUP & flag)
		strcat(flags, "G");
	if (SHF_OS_NONCONFORMING & flag)
		strcat(flags, "O");
	if (SHF_LINK_ORDER & flag)
		strcat(flags, "L");
	if (SHF_MERGE & flag)
		strcat(flags, "M");
	if (SHF_STRINGS & flag)
		strcat(flags, "S");
	return flags;
}


void print_section (FILE *file, Elf32_Shdr *T, Elf32_Ehdr *e){
	
	Elf32_Shdr S;
	char *name = malloc(SIZENAME*sizeof(char)), *type, *flag;
	int i;
	

	printf("Il y a %d en-têtes de section, débutant à l'adresse de décalage %d :\n",e->e_shnum, e->e_entry);
	printf("En-têtes de section : \n");
	printf("[Nr]\t%-15s\t\t%-15s\t%-15s\t%-15s\t%s\t %2s\t %s\t\t%s\t%s\t%s\n","Nom","Type","Adr","Decala.","Taille","ES","Fan","LN","Inf","Al");	
	for (i=0; i<e->e_shnum; i++){	 //tant qu'on a des entrées à lire
		S = T[i];

		// Récupération du type 
		type = read_type(S);
		printf("[%d]\t", i);
		
		//Récupération du flag
		flag = malloc(sizeof(char*));
		flag = read_flags(flag, S.sh_flags);

		// Récupération du nom
		fseek(file, T[e->e_shstrndx].sh_offset+S.sh_name, SEEK_SET);
		fgets(name, SIZENAME, file);

		// Affichage de tous les éléments de la section
		printf("%-16s\t", name);
		printf("%15s\t", type);
		printf("%08x\t", S.sh_addr);
		printf("%06x\t", S.sh_offset);
		printf("%06x\t", S.sh_size);
		printf("%02x\t", S.sh_entsize);
		printf("%-15s\t",flag);
		free(flag);
		printf("%02d\t", S.sh_link);
		printf("%03d\t", S.sh_info);
		printf("%02d\t", S.sh_addralign);
		printf("\n");
	}
	printf("\nClé des fanions : \n");
	printf("W (écriture), A (allocation), X (exécution), M (fusion), S (chaînes)\n");
	printf("I (info), L (ordre des liens), G (groupe), T (TLS), E (exclu), x (inconnu)\n");
	printf("O (traitement additionnel requis pour l'OS), o (spécifique à l'OS), p (spécifique au processeur)\n");
	free(name);
}

void get_sh_values(Elf32_Shdr **sectionHeader, FILE* f,Elf32_Ehdr *h){
	fseek(f,h->e_shoff,SEEK_SET);
	int i;
    for(i=0;i<h->e_shnum;i++)
    	read_section(f,&((*sectionHeader)[i]),*h);
}
/*
int main(int argc, char *argv[]){


	FILE *f;
	Elf32_Ehdr *h;
	Elf32_Shdr *sectionHeader;
	f=fopen(argv[1],"r");
    h=lecture_entete(f);
	sectionHeader=malloc(h->e_shnum*sizeof(Elf32_Shdr));
    get_sh_values(&sectionHeader,f,h);
	print_section(f,sectionHeader,h);
	free(sectionHeader);
	fclose(f);
	
	
	return 0;
}




*/




