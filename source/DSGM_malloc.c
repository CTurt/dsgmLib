#include "DSGM.h"

DSGM_Heap DSGM_toFree[DSGM_ALLOCATION_LIMIT];
int DSGM_toFreeCount = 0;

void *DSGM_TrackedAlloc(void **tracker, size_t length) {
	if(DSGM_toFreeCount == DSGM_ALLOCATION_LIMIT) {
		DSGM_Debug("Too many allocations!");
		return NULL;
	}
	
	void *data = malloc(length);
	*tracker = data;
	DSGM_toFree[DSGM_toFreeCount].memory = tracker;
	DSGM_toFree[DSGM_toFreeCount].tracked = true;
	DSGM_toFreeCount++;

	return data;
}

void *DSGM_Alloc(size_t length) {
	if(DSGM_toFreeCount == DSGM_ALLOCATION_LIMIT) {
		DSGM_Debug("Too many allocations!");
		return NULL;
	}
	
	void *data = malloc(length);
	DSGM_toFree[DSGM_toFreeCount].memory = data;
	DSGM_toFree[DSGM_toFreeCount].tracked = false;
	DSGM_toFreeCount++;
	
	return data;
}

void DSGM_FreeAll(void) {
	int i;
	for(i = 0; i < DSGM_toFreeCount; i++) {
		if(DSGM_toFree[i].tracked) {
			free(*DSGM_toFree[i].memory);
			*DSGM_toFree[i].memory = NULL;
		}
		else {
			free(DSGM_toFree[i].memory);
			DSGM_toFree[i].memory = NULL;
		}
	}
	DSGM_toFreeCount = 0;
}
