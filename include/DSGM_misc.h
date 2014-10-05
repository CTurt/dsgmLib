#pragma once

inline int DSGM_GetBrightness(u8 screen);
#define DSGM_SetBrightness(screen, brightness) setBrightness((-screen) + 2, brightness)
void DSGM_FadeIn(u8 screen, u8 delay);
void DSGM_FadeOutToBlack(u8 screen, u8 delay);
void DSGM_FadeOutToWhite(u8 screen, u8 delay);

inline u16 DSGM_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety);

inline int DSGM_Random(int min, int max);

void DSGM_Delay(unsigned int time);

size_t DSGM_GetWordLength(char *text);