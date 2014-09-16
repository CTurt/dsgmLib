#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_NITRO_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_NITRO_SPRITE(player, DSGMPal0, SpriteSize_32x32, 1),
	DSGM_FORM_NITRO_SPRITE(bullet, DSGMPal0, SpriteSize_16x16, 1),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// player
	{
		&DSGM_Sprites[playerSprite],
		(DSGM_Event)player_create,
		(DSGM_Event)player_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((playerObjectInstance *)0)->variables)
	},
	
	// bullet
	{
		&DSGM_Sprites[bulletSprite],
		(DSGM_Event)bullet_create,
		(DSGM_Event)bullet_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((bulletObjectInstance *)0)->variables)
	},
};

DSGM_Room DSGM_Rooms[DSGM_ROOM_COUNT] = {
	// Room_1
	{
		// Backgrounds
		{
			// Bottom screen
			{
				// Layer 0
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_BOTTOM,				// Screen
					0,							// Layer
					false,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 1
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_BOTTOM,				// Screen
					1,							// Layer
					true,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 2
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_BOTTOM,				// Screen
					2,							// Layer
					true,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 3
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_BOTTOM,				// Screen
					3,							// Layer
					true,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
			},
			
			// Top screen
			{
				// Layer 0
				{
					DSGM_TEXT_BACKGROUND,		// Background
					DSGM_TOP,					// Screen
					0,							// Layer
					false,						// Attached to view system
					7,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 1
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_TOP,					// Screen
					1,							// Layer
					true,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 2
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_TOP,					// Screen
					2,							// Layer
					true,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 3
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_TOP,					// Screen
					3,							// Layer
					true,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
			}
		},
		
		// Initial views
		{
			// Bottom screen
			{
				0, 0
			},
			
			// Top screen
			{
				0, 0
			}
		},
		
		// Views
		{
			// Bottom screen
			{
				0, 0
			},
			
			// Top screen
			{
				0, 0
			}
		},
		
		// Object groups are dynamic, so must be set up at run time, see DSGM_SetupRooms.
		{
			NULL,
			NULL
		},
		{
			0,
			0
		}
	},
};

int DSGM_currentRoom = Room_1;

void DSGM_SetupRooms(int room) {
	if(room != DSGM_ALL_ROOMS) {
		switch(room) {
			case Room_1: goto Room_1; break;
		}
	}
	
	Room_1:
	DSGM_Debug("Room_1 reset\n");
	DSGM_LeaveRoom(&DSGM_Rooms[Room_1]);
	
	DSGM_SetupViews(&DSGM_Rooms[Room_1]);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_TOP, 0);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 1);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[player], DSGM_BOTTOM, 1,
		111, 79
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}


/*static inline u64 DSGM_Distance(s32 x1, s32 y1, s32 x2, s32 y2) {
   s64 h = x1 - x2;
   s64 v = y1 - y2;
   return(h*h + v*v);
}

static u16 DSGM_AdjustAngle(u16 angle, s16 anglerot, s32 x1, s32 y1, s32 x2, s32 y2) {
	u64 distances[3];
	x1 = x1 << 8;
	y1 = y1 << 8;
	x2 = x2 << 8;
	y2 = y2 << 8;
	u16 tempangle = (angle - anglerot) & 511;
	
	distances[0] = DSGM_Distance(x1 + cosLerp(tempangle), y1 - sinLerp(tempangle), x2, y2);
	tempangle += anglerot;
	tempangle &= 511;
	distances[1] = DSGM_Distance(x1 + cosLerp(tempangle), y1 - sinLerp(tempangle), x2, y2);
	tempangle += anglerot;
	tempangle &= 511;
	distances[2] = DSGM_Distance(x1 + cosLerp(tempangle), y1 - sinLerp(tempangle), x2, y2);
	
	if(distances[0] < distances[1])  angle -= anglerot;
	else if(distances[2] < distances[1])  angle += anglerot;
	
	return (angle & 511);
}

inline u16 DSGM_GetAngle(s32 x1, s32 y1, s32 x2, s32 y2) {
	u16 angle = 0;
	u16 anglerot = 180;
	
	while(anglerot > 5) {
		angle = DSGM_AdjustAngle(angle, anglerot, x1, y1, x2, y2);
		anglerot = (anglerot - ((3 * anglerot) >> 3));
	}
	
	anglerot = 4;
	angle = DSGM_AdjustAngle(angle, anglerot, x1, y1, x2, y2);
	anglerot = 2;
	angle = DSGM_AdjustAngle(angle, anglerot, x1, y1, x2, y2);
	anglerot = 1;
	angle = DSGM_AdjustAngle(angle, anglerot, x1, y1, x2, y2);
	
	return degreesToAngle(angle);
}*/


static inline u64 PA_Distance(s32 x1, s32 y1, s32 x2, s32 y2) {
   s64 h = x1 - x2;
   s64 v = y1 - y2;
   return(h*h + v*v);
}

u16 PA_AdjustAngle(u16 angle, s16 anglerot, s32 startx, s32 starty, s32 targetx, s32 targety) {
	u64 distances[3];
	startx = startx << 8; // Fixed point...
	starty = starty << 8; // Fixed point...
	targetx = targetx << 8; // Fixed point...
	targety = targety << 8; // Fixed point...
	u16 tempangle = (angle - anglerot) & 511;
	// Calcul des distances en fonction des angles
	//distances[0] = PA_Distance(startx + PA_Cos(tempangle), starty - PA_Sin(tempangle), targetx, targety);
	distances[0] = PA_Distance(startx + cosLerp(degreesToAngle(tempangle)), starty - sinLerp(degreesToAngle(tempangle)), targetx, targety);
	tempangle += anglerot;
	tempangle &= 511;
	//distances[1] = PA_Distance(startx + PA_Cos(tempangle), starty - PA_Sin(tempangle), targetx, targety);
	distances[1] = PA_Distance(startx + cosLerp(degreesToAngle(tempangle)), starty - sinLerp(degreesToAngle(tempangle)), targetx, targety);
	tempangle += anglerot;
	tempangle &= 511;
	//distances[2] = PA_Distance(startx + PA_Cos(tempangle), starty - PA_Sin(tempangle), targetx, targety);
	distances[2] = PA_Distance(startx + cosLerp(degreesToAngle(tempangle)), starty - sinLerp(degreesToAngle(tempangle)), targetx, targety);

	// On regarde si l'angle est optimal. Si ce n'est pas le cas,
	// on fait tourner toujours dans le meme sens...
	if (distances[0] < distances[1])  angle -= anglerot;
	else if (distances[2] < distances[1])  angle += anglerot;

	return angle&511;
}

static inline u16 DSGM_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety) {
	u16 angle = 0;
	u16 anglerot = 180;

	while(anglerot > 5) {
		angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
		anglerot = (anglerot - ((3 * anglerot) >> 3)); // On diminue petit ? petit la rotation...
	}

	// Ajustement encore plus précis...
	anglerot = 4;
	angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = 2;
	angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);
	anglerot = 1;
	angle = PA_AdjustAngle(angle, anglerot, startx, starty, targetx, targety);

	return angle;
}


void player_create(playerObjectInstance *me) {
	DSGM_DrawText(DSGM_TOP, 1, 1, "D-Pad to move, A to shoot");
	DSGM_InitObjectInstanceRotScale(me);
	me->bitshift = 11;
}

void player_loop(playerObjectInstance *me) {
	if(DSGM_held.Stylus) {
		DSGM_DrawText(DSGM_TOP, 5, 5, "Angle      ", DSGM_GetAngle(DSGM_stylus.px, DSGM_stylus.py, me->x, me->y));
		DSGM_DrawText(DSGM_TOP, 5, 5, "Angle %d", DSGM_GetAngle(DSGM_stylus.px, DSGM_stylus.py, me->x, me->y));
		(*me->angle) = DSGM_GetAngle(DSGM_stylus.px, DSGM_stylus.py, me->x, me->y);
	}
	if(DSGM_held.Left) {
		(*me->angle) += degreesToAngle(4);
	}
	if(DSGM_held.Right) {
		(*me->angle) -= degreesToAngle(4);
	}
	if(DSGM_held.Up) {
		DSGM_MoveObjectInstanceAtRotatedAngle(me);
	}
	if(DSGM_held.Down) {
		DSGM_MoveObjectInstanceAtAngle(me, (*me->angle) + degreesToAngle(180));
	}
	
	if(DSGM_newpress.A) {
		bulletObjectInstance *bulletInstance = (bulletObjectInstance *)DSGM_CreateObjectInstance(DSGM_BOTTOM, me->x + 8, me->y + 8, &DSGM_Objects[bullet]);
		*bulletInstance->angle = *me->angle;
		
		DSGM_UpdateInput();
	}
}

void bullet_create(bulletObjectInstance *me) {
	DSGM_InitObjectInstanceRotScale(me);
	me->bitshift = 9;
}

void bullet_loop(bulletObjectInstance *me) {
	DSGM_MoveObjectInstanceAtRotatedAngle(me);
	
	if(!DSGM_ObjectInstanceOnScreen(me)) {
		DSGM_DeleteObjectInstance(me);
	}
}
