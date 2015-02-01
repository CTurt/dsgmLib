#pragma once

#define DSGM_RealSin(a) fixedToFloat(sinLerp(a), 12)
#define DSGM_RealCos(a) fixedToFloat(cosLerp(a), 12)

inline int DSGM_GetBrightness(u8 screen);
#define DSGM_SetBrightness(screen, brightness) setBrightness((-screen) + 2, brightness)
void DSGM_FadeIn(u8 screen, u8 delay);
void DSGM_FadeOutToBlack(u8 screen, u8 delay);
void DSGM_FadeOutToWhite(u8 screen, u8 delay);

inline u64 DSGM_SquareDistance(s32 x1, s32 y1, s32 x2, s32 y2);
inline u64 DSGM_Distance(s32 x1, s32 y1, s32 x2, s32 y2);
inline u16 DSGM_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety);

inline int DSGM_Random(int min, int max);

void DSGM_Delay(unsigned int time);

size_t DSGM_GetWordLength(char *text);

bool DSGM_Intersection(int Ax, int Ay, int Bx, int By, int Cx, int Cy, int Dx, int Dy);
