#include "DSGM.h"

#define DSGM_LegacySin(angle) (sinLerp(angle << 6) >> 4)
#define DSGM_LegacyCos(angle) (cosLerp(angle << 6) >> 4)

static inline u64 DSGM_Distance(s32 x1, s32 y1, s32 x2, s32 y2) {
   s64 h = x1 - x2;
   s64 v = y1 - y2;
   return h * h + v * v;
}

static u16 DSGM_AdjustAngle(u16 angle, s16 anglerot, s32 startx, s32 starty, s32 targetx, s32 targety) {
	u64 distances[3];
	startx = startx << 8;
	starty = starty << 8;
	targetx = targetx << 8;
	targety = targety << 8;
	u16 tempangle = (angle - anglerot) & 511;
	
	distances[0] = DSGM_Distance(startx + DSGM_LegacyCos(tempangle), starty - DSGM_LegacySin(tempangle), targetx, targety);
	tempangle += anglerot;
	tempangle &= 511;
	distances[1] = DSGM_Distance(startx + DSGM_LegacyCos(tempangle), starty - DSGM_LegacySin(tempangle), targetx, targety);
	tempangle += anglerot;
	tempangle &= 511;
	distances[2] = DSGM_Distance(startx + DSGM_LegacyCos(tempangle), starty - DSGM_LegacySin(tempangle), targetx, targety);
	
	if(distances[0] < distances[1])  angle -= anglerot;
	else if(distances[2] < distances[1])  angle += anglerot;
	
	return angle & 511;
}

inline u16 DSGM_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety) {
	u16 angle = 0;
	u16 anglerot = 180;
	
	while(anglerot > 5) {
		angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
		anglerot = (anglerot - ((3 * anglerot) >> 3));
	}
	
	anglerot = 4;
	angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = 2;
	angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = 1;
	angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	
	return angle << 6;
}

inline int DSGM_Random(int min, int max) {
	return (rand() % max) + min;
}

void DSGM_Delay(unsigned int time) {
	int i;
	for(i = 0; i < time; i++) {
		swiWaitForVBlank();
	}
}
