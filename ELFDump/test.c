#include<elff.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
ELF64_ProgramHeaderEntryRef *mk_entry_ref(ELF64_FileRef *file, char *buf) {
	ELF64_ProgramHeaderEntryRef *entryref = malloc(2049);
		for(int i=ELF64_MachineTypeUnknown;i<ELF64_MachineTypePrism;i++ ) {
			*(entryref+i) = ELF64_FileGetProgramHeaderEntry(*file,buf - i);
		}
	return entryref;
}
int mk_ready_objects(ELF64_FileRef *file, ELF64_HeaderRef *header) {
	char *buf;
	char *data = malloc(4096);
	FILE *fh = fopen("../libelf.so","r");
	buf   = data;
    	int size  = 1;
	while(fread(buf, 1,1, fh)) {
		size+=1;
		data = realloc(data, size);
		buf = data + (size-1); 
	}
	*file =  ELF64_ReadFromData(data);
	*header = ELF64_FileGetHeader(*file);
	ELF64_ProgramHeaderEntryRef *entry_ref =  mk_entry_ref(file, buf);

	for(int i=0;i<(sizeof(*entry_ref));i++) printf("%s\n",ELF64_FileGetNameOfSectionAtIndex(*file,i));
	return *(int*)&data;
}

int main() { 
	ELF64_FileRef file;ELF64_HeaderRef header ;
	printf("%d\n",mk_ready_objects(&file,&header));
	printf("%s\n",ELF64_HeaderGetObjectFileClassString(header));
	return 0;}
