#include "DSGM.h"

size_t DSGM_GetFileLength(char *filename) {
	FILE *f = NULL;
	size_t s = 0;
	
	f = fopen(filename, "rb");
	if(!f) {
		DSGM_Debug("Failed to open file!");
	}
	
	fseek(f, 0, SEEK_END);
	s = ftell(f);
	
	fclose(f);
	
	return s;
}

void DSGM_ReadFileManual(void *destination, int start, size_t length, char *filename) {
	FILE *f = NULL;
	
	f = fopen(filename, "rb");
	if(!f) {
		DSGM_Debug("Failed to open file!");
	}
	
	fseek(f, start, SEEK_SET);
	
	if(length != DSGM_AUTO_LENGTH) {
		fread(destination, length, 1, f);
	}
	else {
		fread(destination, DSGM_GetFileLength(filename), 1, f);
	}
	
	fclose(f);
}

char *DSGM_ReadFile(char *filename) {
	FILE *f = NULL;
	size_t s = 0;
	char *destination = NULL;
	
	f = fopen(filename, "rb");
	if(!f) {
		DSGM_Debug("Failed to open file!");
	}
	
	fseek(f, 0, SEEK_END);
	s = ftell(f);
	rewind(f);
	
	destination = DSGM_Alloc(s + 1);
	
	fread(destination, s, 1, f);
	fclose(f);
	
	destination[s] = '\0';
	
	return destination;
}
