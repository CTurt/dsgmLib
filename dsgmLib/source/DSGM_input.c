#include "DSGM.h"

DSGM_Input DSGM_newpress, DSGM_held, DSGM_release;
DSGM_TouchPosition DSGM_stylus;

void DSGM_UpdateInput(void) {
	scanKeys();
	
	int newpress = keysDown();
	int held = keysHeld();
	int release = keysUp();
	
  touchPosition tempStylus;
	touchRead(&tempStylus);
  DSGM_stylus.dx = tempStylus.px - DSGM_stylus.x;
  DSGM_stylus.dy = tempStylus.py - DSGM_stylus.y;
	DSGM_stylus.x = tempStylus.px;
	DSGM_stylus.y = tempStylus.py;
  
	#define MAP_KEY(DSGMKey, libndsKey)\
	DSGM_newpress.DSGMKey = (newpress & libndsKey);\
	DSGM_held.DSGMKey = (held & libndsKey);\
	DSGM_release.DSGMKey = (release & libndsKey)
	
	MAP_KEY(A,		KEY_A);
	MAP_KEY(B,		KEY_B);
	MAP_KEY(Select,	KEY_SELECT);
	MAP_KEY(Start,	KEY_START);
	MAP_KEY(Right,	KEY_RIGHT);
	MAP_KEY(Left,	KEY_LEFT);
	MAP_KEY(Up,		KEY_UP);
	MAP_KEY(Down,	KEY_DOWN);
	MAP_KEY(R,		KEY_R);
	MAP_KEY(L,		KEY_L);
	MAP_KEY(X,		KEY_X);
	MAP_KEY(Y,		KEY_Y);
	MAP_KEY(Stylus,	KEY_TOUCH);
	MAP_KEY(Lid,	KEY_LID);
	
	#undef MAP_KEY
}
