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

void player_create(playerObjectInstance *me) {
	DSGM_DrawText(DSGM_TOP, 1, 1, "D-Pad or Stylus to move");
	DSGM_DrawText(DSGM_TOP, 1, 2, "A to shoot");
	DSGM_InitObjectInstanceRotScale(me);
	me->bitshift = 11;
}

void player_loop(playerObjectInstance *me) {
	if(DSGM_held.Stylus) {
		(*me->angle) = DSGM_GetAngle(me->x + 16, me->y + 16, DSGM_stylus.x, DSGM_stylus.y);
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
