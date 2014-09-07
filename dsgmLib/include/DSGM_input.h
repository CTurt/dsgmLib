#pragma once

#define DSGM_NOT_HOLDING_3(k1, k2, k3) (!(DSGM_held.k1 || DSGM_held.k2 || DSGM_held.k3))
#define DSGM_NOT_HOLDING_DPAD() (!(DSGM_held.Left || DSGM_held.Right || DSGM_held.Up || DSGM_held.Down))

typedef struct {
	bool A;
	bool B;
	bool Select;
	bool Start;
	bool Right;
	bool Left;
	bool Up;
	bool Down;
	bool R;
	bool L;
	bool X;
	bool Y;
	bool Stylus;
	bool Lid;
} DSGM_Input;

extern DSGM_Input DSGM_newpress, DSGM_held, DSGM_release;
extern touchPosition DSGM_stylus;

void DSGM_UpdateInput(void);
