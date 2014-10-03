#include "DSGM.h"

DSGM_Input DSGM_newpress, DSGM_held, DSGM_release;
DSGM_TouchPosition DSGM_stylus;

void DSGM_UpdateInput(void) {
	scanKeys();
	
	int newpress = keysDown();
	int held = keysHeld();
	int release = keysUp();
	
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
	
	DSGM_newpress.Any = (DSGM_newpress.A || DSGM_newpress.B || DSGM_newpress.Select || DSGM_newpress.Start || DSGM_newpress.Right || DSGM_newpress.Left || DSGM_newpress.Up || DSGM_newpress.Down || DSGM_newpress.R || DSGM_newpress.L || DSGM_newpress.X || DSGM_newpress.Y || DSGM_newpress.Stylus || DSGM_newpress.Lid);
	DSGM_held.Any = (DSGM_held.A || DSGM_held.B || DSGM_held.Select || DSGM_held.Start || DSGM_held.Right || DSGM_held.Left || DSGM_held.Up || DSGM_held.Down || DSGM_held.R || DSGM_held.L || DSGM_held.X || DSGM_held.Y || DSGM_held.Stylus || DSGM_held.Lid);
	DSGM_release.Any = (DSGM_release.A || DSGM_release.B || DSGM_release.Select || DSGM_release.Start || DSGM_release.Right || DSGM_release.Left || DSGM_release.Up || DSGM_release.Down || DSGM_release.R || DSGM_release.L || DSGM_release.X || DSGM_release.Y || DSGM_release.Stylus || DSGM_release.Lid);
	
	if(DSGM_held.Stylus) {
		touchPosition tempStylus;
		touchRead(&tempStylus);
		DSGM_stylus.dx = tempStylus.px - DSGM_stylus.x;
		DSGM_stylus.dy = tempStylus.py - DSGM_stylus.y;
		DSGM_stylus.x = tempStylus.px;
		DSGM_stylus.y = tempStylus.py;
	}
	else {
		DSGM_stylus.dx = 0;
		DSGM_stylus.dy = 0;
	}
}
