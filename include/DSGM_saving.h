#pragma once

extern FILE *DSGM_save;
extern char DSGM_saveName[256];

inline bool DSGM_StartSaving(void);
inline void DSGM_FinishSaving(void);
