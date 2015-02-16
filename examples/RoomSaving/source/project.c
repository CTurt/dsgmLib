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
	DSGM_FORM_NITRO_SPRITE(taptapman, DSGMPal0, SpriteSize_32x32, 9),
	DSGM_FORM_NITRO_SPRITE(ball, DSGMPal0, SpriteSize_32x32, 1),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// ball
	{
		&DSGM_Sprites[ballSprite],
		(DSGM_Event)ball_create,
		(DSGM_Event)ball_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((ballObjectInstance *)0)->variables)
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
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 1);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[ball], DSGM_BOTTOM, 1,
		64, 64
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void ball_create(ballObjectInstance *me) {
	DSGM_DrawText(DSGM_TOP, 1, 1, "Stylus to create balls");
	DSGM_DrawText(DSGM_TOP, 1, 2, "X to move balls");
	DSGM_DrawText(DSGM_TOP, 1, 3, "R to save");
	DSGM_DrawText(DSGM_TOP, 1, 4, "L to load");
}

void ball_loop(ballObjectInstance *me) {
	if(DSGM_newpress.Stylus) {
		DSGM_CreateObjectInstance(DSGM_BOTTOM, DSGM_stylus.x - 16, DSGM_stylus.y - 16, me->object);
		DSGM_UpdateInput();
	}
	
	if(DSGM_newpress.X) {
		me->x = DSGM_Random(0, 224);
		me->y = DSGM_Random(0, 160);
	}
	
	if(DSGM_newpress.R) {
		if(DSGM_StartSaving()) {
			DSGM_WriteRoom();
			DSGM_FinishSaving();
		}
		else {
			DSGM_DrawText(DSGM_TOP, 1, 6, "Can't save! DLDI patch, or use hbmenu.");
		}
	}
	
	if(DSGM_newpress.L) {
		// DSGM_StartSaving and DSGM_FinishSaving also apply to loading
		if(DSGM_StartSaving()) {
			DSGM_ReadRoom();
			DSGM_FinishSaving();
			
			// Reload room
			DSGM_SwitchRoom(DSGM_currentRoom, false);
		}
		else {
			DSGM_DrawText(DSGM_TOP, 1, 6, "Can't load! DLDI patch, or use hbmenu.");
		}
	}
}
