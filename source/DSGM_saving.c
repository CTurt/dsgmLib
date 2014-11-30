#include "DSGM.h"

FILE *DSGM_save;
char DSGM_saveName[256];

// Saving applies to both saving and loading
inline bool DSGM_StartSaving(void) {
	DSGM_save = fopen(DSGM_saveName, "r+b");
	return DSGM_save ? true : false;
}

inline void DSGM_FinishSaving(void) {
	fclose(DSGM_save);
}
