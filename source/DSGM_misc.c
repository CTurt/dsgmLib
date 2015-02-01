#include "DSGM.h"

#define DSGM_LegacySin(angle) (sinLerp(angle << 6) >> 4)
#define DSGM_LegacyCos(angle) (cosLerp(angle << 6) >> 4)

inline int DSGM_GetBrightness(u8 screen) {
	int v = (screen == DSGM_TOP ? REG_MASTER_BRIGHT : REG_MASTER_BRIGHT_SUB);
	return (v - (v & 1 << 14)) < 16 ? (v - (v & 1 << 14)) : -(v - (v & 2 << 14));
}

void DSGM_FadeIn(u8 screen, u8 delay) {
	int brightness = DSGM_GetBrightness(DSGM_BOTTOM);
	if(brightness == -15) {
		// Fade in from black
		while(brightness < 0) {
			brightness += 1;
			DSGM_SetBrightness(screen, brightness);
			int i; for(i = 0; i < delay; i++) swiWaitForVBlank();
		}
	}
	else if(brightness == 15) {
		// Fade in from white
		while(brightness > 0) {
			brightness -= 1;
			DSGM_SetBrightness(screen, brightness);
			int i; for(i = 0; i < delay; i++) swiWaitForVBlank();
		}
	}
}

void DSGM_FadeOutToBlack(u8 screen, u8 delay) {
	int brightness = DSGM_GetBrightness(DSGM_BOTTOM);
	if(brightness == 0) {
		while(brightness > -15) {
			brightness -= 1;
			DSGM_SetBrightness(screen, brightness);
			int i; for(i = 0; i < delay; i++) swiWaitForVBlank();
		}
	}
}

void DSGM_FadeOutToWhite(u8 screen, u8 delay) {
	int brightness = DSGM_GetBrightness(DSGM_BOTTOM);
	if(brightness == 0) {
		while(brightness < 15) {
			brightness += 1;
			DSGM_SetBrightness(screen, brightness);
			int i; for(i = 0; i < delay; i++) swiWaitForVBlank();
		}
	}
}

inline u64 DSGM_SquareDistance(s32 x1, s32 y1, s32 x2, s32 y2) {
	s64 h = x1 - x2;
	s64 v = y1 - y2;
	return h * h + v * v;
}

inline u64 DSGM_Distance(s32 x1, s32 y1, s32 x2, s32 y2) {
	s64 h = x1 - x2;
	s64 v = y1 - y2;
	return sqrt32(h * h + v * v);
}

static u16 DSGM_AdjustAngle(u16 angle, s16 anglerot, s32 startx, s32 starty, s32 targetx, s32 targety) {
	u64 distances[3];
	startx = startx << 10; // 8
	starty = starty << 10; // 8
	targetx = targetx << 10; // 8
	targety = targety << 10; // 8
	u16 tempangle = (angle - anglerot) & (511 << 6);
	
	distances[0] = DSGM_SquareDistance(startx + cosLerp(tempangle), starty - sinLerp(tempangle), targetx, targety);
	tempangle += anglerot;
	tempangle &= 511 << 6;
	distances[1] = DSGM_SquareDistance(startx + cosLerp(tempangle), starty - sinLerp(tempangle), targetx, targety);
	tempangle += anglerot;
	tempangle &= 511 << 6;
	distances[2] = DSGM_SquareDistance(startx + cosLerp(tempangle), starty - sinLerp(tempangle), targetx, targety);
	
	if(distances[0] < distances[1])	angle -= anglerot;
	else if(distances[2] < distances[1]) angle += anglerot;
	
	return angle & (511 << 6);
}

inline u16 DSGM_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety) {
	u16 angle = 0;
	u16 anglerot = 180 << 6;
	
	while(anglerot > 5 << 6) {
		angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
		anglerot = (anglerot - ((3 * anglerot) >> 3));
	}
	
	anglerot = 4 << 6;
	angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = 2 << 6;
	angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = 1 << 6;
	angle = DSGM_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	
	return angle;
}

inline int DSGM_Random(int min, int max) {
	return (rand() % (max - min)) + min;
}

void DSGM_Delay(unsigned int time) {
	int i;
	for(i = 0; i < time; i++) {
		swiWaitForVBlank();
	}
}

unsigned int DSGM_GetWordLength(char *text) {
	int i;
	int len = strlen(text);
	
	for(i = 0; i < len; i++) {
		if(text[i] == ' ' || text[i] == '\n') break;
	}
	
	return i;
}

// Taken from: http://alienryderflex.com/intersect/
bool DSGM_Intersection(int Ax, int Ay, int Bx, int By, int Cx, int Cy, int Dx, int Dy) {
	int distAB, theCos, theSin, newX, ABpos;
	
	//	Fail if either line segment is zero-length
	if((Ax == Bx && Ay == By) || (Cx == Dx && Cy == Dy)) return false;
	
	//	Fail if the segments share an end-point
	if((Ax == Cx && Ay == Cy) || (Bx == Cx && By == Cy) || (Ax == Dx && Ay == Dy) || (Bx == Dx && By == Dy)) return false;
	
	// Translate the system so that point A is on the origin
	Bx -= Ax; By -= Ay;
	Cx -= Ax; Cy -= Ay;
	Dx -= Ax; Dy -= Ay;
	
	//	Discover the length of segment A-B
	distAB = sqrt32(Bx * Bx + By * By);
	
	// Rotate the system so that point B is on the positive X axis
	theCos = Bx / distAB;
	theSin = By / distAB;
	newX = Cx * theCos + Cy * theSin;
	Cy = Cy * theCos - Cx * theSin; Cx = newX;
	newX = Dx * theCos + Dy * theSin;
	Dy = Dy * theCos - Dx * theSin; Dx = newX;
	
	//	Fail if segment C-D doesn't cross line A-B
	if((Cy < 0 && Dy < 0) || (Cy >= 0 && Dy >= 0)) return false;
	
	// Discover the position of the intersection point along line A-B
	ABpos = Dx + (Cx - Dx) * Dy / (Dy - Cy);
	
	//	Fail if segment C-D crosses line A-B outside of segment A-B
	if(ABpos < 0 || ABpos > distAB) return false;
	
	// Apply the discovered position to line A-B in the original coordinate system
	// *X = Ax + ABpos * theCos;
	// *Y = Ay + ABpos * theSin;
	
	return true;
}
