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

// adapted from libnds
typedef struct {
	u16	rawx; //!< Raw x value from the A2D
	u16	rawy; //!< Raw y value from the A2D
	u16	x;    //!< Processed pixel X value
	u16	y;    //!< Processed pixel Y value
	u16	z1;   //!< Raw cross panel resistance
	u16	z2;   //!< Raw cross panel resistance
} DSGM_TouchPosition;

extern DSGM_Input DSGM_newpress, DSGM_held, DSGM_release;
extern DSGM_TouchPosition DSGM_stylus;

void DSGM_UpdateInput(void);
