#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
int score;

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_BACKGROUND(background, BgSize_T_256x256, BgType_Text8bpp),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_NITRO_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_NITRO_SPRITE(block, DSGMPal0, SpriteSize_32x32, 1),
	DSGM_FORM_NITRO_SPRITE(spike, DSGMPal0, SpriteSize_16x16, 1),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// player
	{
		&DSGM_Sprites[blockSprite],
		(DSGM_Event)player_create,
		(DSGM_Event)player_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((playerObjectInstance *)0)->variables)
	},
	
	// spike
	{
		&DSGM_Sprites[spikeSprite],
		DSGM_NO_EVENT,
		(DSGM_Event)spike_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((spikeObjectInstance *)0)->variables)
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
					DSGM_DEFAULT_FONT,			// Background
					DSGM_BOTTOM,				// Screen
					0,							// Layer
					false,						// Attached to view system
					8,							// Map base
					2,							// Tile base
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
					&DSGM_Backgrounds[background],// Background
					DSGM_BOTTOM,				// Screen
					2,							// Layer
					true,						// Attached to view system
					7,							// Map base
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
					DSGM_DEFAULT_FONT,			// Background
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
		
		NULL,	// Room handler
		
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
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 2);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[player], DSGM_BOTTOM, 1,
		32, 129
	);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][1], &DSGM_Objects[spike], DSGM_BOTTOM, 1,
		255, 145
	);
	
	DSGM_AddCollisionEvent(&DSGM_Objects[player], &DSGM_Objects[spike], player_collide_spike);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void player_create(playerObjectInstance *me) {
	// Reset score
	score = 0;
	
	u16 backgroundColor = DSGM_RandomColor();
	u16 lineColor = DSGM_RandomColor();
	
	DSGM_SetScreenColor(DSGM_TOP, backgroundColor);
	
	// We won't see this color because there is a background on top of it
	// however, it will briefly flicker to this color when changing rooms
	// try changing it to DSGM_RED and see what happens
	DSGM_SetScreenColor(DSGM_BOTTOM, backgroundColor);
	
	// This is how to change the color of an actual background (layer 2)
	DSGM_UnlockBackgroundPalette(DSGM_BOTTOM);
	DSGM_GetBackgroundPalette(DSGM_BOTTOM, 2)[1] = backgroundColor;
	DSGM_GetBackgroundPalette(DSGM_BOTTOM, 2)[2] = lineColor;
	DSGM_LockBackgroundPalette(DSGM_BOTTOM);
	
	// Change the player sprite colors
	DSGM_UnlockSpritePalette(DSGM_BOTTOM);
	DSGM_GetPaletteData(DSGM_BOTTOM, me->object->sprite->palette)[2] = backgroundColor;
	DSGM_GetPaletteData(DSGM_BOTTOM, me->object->sprite->palette)[1] = lineColor;
	DSGM_LockSpritePalette(DSGM_BOTTOM);
	
	// Change the font color
	DSGM_SetTextColor(DSGM_BOTTOM, lineColor);
	
	DSGM_InitObjectInstanceRotScale(me);
	me->by = me->y << me->bitshift;
}

void player_loop(playerObjectInstance *me) {
	me->variables->vy += 72;
	me->by += me->variables->vy;
	(*me->angle) -= 480;
	
	if(me->by >= 129 << me->bitshift) {
		me->by = 129 << me->bitshift;
		me->variables->vy = 0;
		(*me->angle) = 0;
		if(DSGM_held.Stylus || DSGM_held.A) me->variables->vy = -1296;
	}
	
	me->y = me->by >> me->bitshift;
	
	DSGM_view[me->screen].x += 2;
	me->x = DSGM_view[me->screen].x + 32;
	
	DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Score: %d", score);
}

void spike_loop(spikeObjectInstance *me) {
	if(me->x - DSGM_view[me->screen].x <= -16) {
		score++;
		me->x = DSGM_view[me->screen].x + 256;
	}
}

void player_collide_spike(playerObjectInstance *me, spikeObjectInstance *collider) {
	DSGM_ResetRoom();
}
