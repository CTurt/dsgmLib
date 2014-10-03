#pragma once

#define DSGM_AUTO_LENGTH 0

size_t DSGM_GetFileLength(char *filename);
size_t DSGM_ReadFileManual(void *destination, int start, size_t length, char *filename);
char *DSGM_ReadFile(char *filename);
