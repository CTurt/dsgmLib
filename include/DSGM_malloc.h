#pragma once

#define DSGM_ALLOCATION_LIMIT 128

typedef struct {
	void **memory;
	bool tracked;
} DSGM_Heap;

extern DSGM_Heap DSGM_toFree[DSGM_ALLOCATION_LIMIT];
extern int DSGM_toFreeCount;

void *DSGM_TrackedAlloc(void **tracker, size_t length);
void *DSGM_Alloc(size_t length);
void DSGM_FreeAll(void);
