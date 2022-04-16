#include<elff.h>
#include<stdio.h>
#include<string.h>

ELF64_SectionHeaderEntryRef get_offset(ELF64_FileRef *file, int index) {
	ELF64_Off   offst; 
    	ELF64_Half      entrySize;
	ELF64_HeaderRef header;//= malloc(100);
	if(!ELF64_FileIsValid(*file)) return NULL;
	header = ELF64_FileGetHeader(*file);
	offst = ELF64_HeaderGetSectionHeaderOffset(header);
	entrySize = ELF64_HeaderGetSectionHeaderEntrySize(header);
    	return ( ELF64_SectionHeaderEntryRef )( ( void * )( ( char * )(*file) + offst + (  index ) ) );
}

char* get_offset_dif_type(ELF64_FileRef *file, int index) {
	ELF64_Off   offst; 
    	ELF64_Half      entrySize;
	if(!ELF64_FileIsValid(*file)) return NULL;
	ELF64_SectionHeaderEntryRef header = ELF64_FileGetSectionHeaderEntry(*file, index);

	ELF64_SymbolTableEntryRef sym = ELF64_FileGetSymbolTableEntryForSection(*file, header, 4);

	offst = ELF64_SymbolTableEntryGetNameOffset(sym);

	if(sym!=NULL) printf("%ld\n",offst );
    	return  ( char * )sym + offst +  index;
}


char* get_offset_h(ELF64_FileRef *file, int index) {
    	ELF64_Word                offst;
    	ELF64_Half      entrySize;
	ELF64_HeaderRef header;//= malloc(100);
	if(!ELF64_FileIsValid(*file)) return NULL;
	header = ELF64_FileGetHeader(*file);

	offst = ELF64_HeaderGetSectionHeaderOffset(header);
	return (char*)get_offset(file,index)+offst;

}

char* get_index(char *data, int index) {
	ELF64_FileRef  file;
	ELF64_HeaderRef header;
	file = ELF64_ReadFromData(data);
	if(ELF64_FileIsValid(file)) header = ELF64_FileGetHeader(file);
	//
	ELF64_SectionHeaderEntryRef section; //=  get_index(data, 4);

	section =get_offset(&file,index); //ERROR;

	char *nice  =get_offset_dif_type(&file,index); //ERROR;
			
	printf("%d", *(int*)(nice+50000));

	while((*(int*)(nice))!=0) {
		printf("nger \n");
		nice = nice +1;
	}
	

	if(section==NULL) return "fuck";
	const char *fsection = ELF64_FileGetNameOfSection(file, section);
	return fsection;
}

int main(int arg, char **argv) {
	char *data = malloc(40960);
	char *buf; //malloc(4000);
	buf = data;
	FILE *f = fopen(argv[1], "r")	;
	int offset=1;
	while(fread(buf, 1,1, f)) {
		offset+=1;
		data = realloc(data, offset);
		buf = data + (offset-1);
	}
	printf(get_index(data, 3));
	return 0;

}
