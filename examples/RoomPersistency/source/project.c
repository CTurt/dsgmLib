#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

bool walkableTile(u16 tile) {
	switch(tile) {
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
		case 0x0D:
		case 0x0E:
		case 0x0F:
		case 0x10:
		case 0x2F:
		case 0x30:
		case 0x38:
		case 0x39:
		case 0x5C:
		case 0x5D:
		case 0x5E:
		case 0x5F:
		case 0x64:
		case 0x65:
		case 0x66:
		case 0x67:
		case 0x407:
		case 0x410:
		case 0x40D:
		case 0x464:
		case 0x80E:
		case 0x808:
		case 0x809:
		case 0x864:
		case 0xC0D:
			return true;
			break;
	}
	return false;
}

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
	DSGM_FORM_SOUND_STREAM(FlatOutLies),
	DSGM_FORM_SOUND_EFFECT(Ambulance),
	DSGM_FORM_SOUND_EFFECT(Boom),
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_BACKGROUND(odale, BgSize_T_512x512, BgType_Text8bpp),
	DSGM_FORM_NITRO_BACKGROUND(cloud, BgSize_T_256x256, BgType_Text8bpp),
	DSGM_FORM_NITRO_BACKGROUND(space, BgSize_T_256x256, BgType_Text8bpp),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_NITRO_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_NITRO_SPRITE(taptapman, DSGMPal0, SpriteSize_32x32, 9),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// player
	{
		&DSGM_Sprites[taptapman],
		(DSGM_Event)player_create,
		(DSGM_Event)player_loop,
		DSGM_NO_EVENT,
		(DSGM_Event)player_touch,
		NULL, 0,
		
		sizeof(*((playerObjectInstance *)0)->variables)
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
					3,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 1
				{
					&DSGM_Backgrounds[odale],	// Background
					DSGM_BOTTOM,				// Screen
					1,							// Layer
					true,						// Attached to view system
					4,							// Map base
					1,							// Tile base
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
				128, 128
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
				128, 128
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

	// Room_2
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
					3,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 1
				{
					&DSGM_Backgrounds[odale],	// Background
					DSGM_BOTTOM,				// Screen
					1,							// Layer
					true,						// Attached to view system
					4,							// Map base
					1,							// Tile base
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
				128, 128
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
				128, 128
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
			case Room_2: goto Room_2; break;
		}
	}
	
	Room_1:
	DSGM_Debug("Room_1 reset\n");
	DSGM_LeaveRoom(&DSGM_Rooms[Room_1]);
	
	DSGM_SetupViews(&DSGM_Rooms[Room_1]);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_TOP, 0);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 1);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[player], DSGM_BOTTOM, 1,
		239, 207
	);
	
	if(room != DSGM_ALL_ROOMS) return;
	
	Room_2:
	DSGM_Debug("Room 2 reset\n");
	DSGM_LeaveRoom(&DSGM_Rooms[Room_2]);
	
	DSGM_SetupViews(&DSGM_Rooms[Room_1]);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_2], DSGM_TOP, 0);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_2], DSGM_BOTTOM, 1);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_2].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[player], DSGM_BOTTOM, 1,
		239, 207
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void player_create(playerObjectInstance *me) {
	DSGM_DrawText(DSGM_BOTTOM, 18, 22, "DS Game Maker");
	DSGM_DrawText(DSGM_TOP, 1, 1, "Room persistency demo");
	DSGM_DrawText(DSGM_TOP, 1, 3, "Touch player to switch rooms");
	if(DSGM_currentRoom == Room_1) {
		DSGM_DrawText(DSGM_TOP, 1, 5, "Room_1");
	}
	else {
		DSGM_DrawText(DSGM_TOP, 1, 5, "Room_2");
	}
	DSGM_PlaySound(FlatOutLies);
}

void player_loop(playerObjectInstance *me) {
	// Position the player to be in the centre of the screen
	me->x = DSGM_view[DSGM_BOTTOM].x + 111;
	me->y = DSGM_view[DSGM_BOTTOM].y + 79;
	
	// Change direction
	if(DSGM_held.Left && DSGM_NOT_HOLDING_3(Right, Up, Down) && me->variables->direction != LEFT) {
		me->animationTimer = 11;
		me->variables->direction = LEFT;
		me->hFlip = true;
	}
	if(DSGM_held.Right && DSGM_NOT_HOLDING_3(Left, Up, Down) && me->variables->direction != RIGHT) {
		me->animationTimer = 11;
		me->variables->direction = RIGHT;
		me->hFlip = false;
	}
	if(DSGM_held.Up && DSGM_NOT_HOLDING_3(Left, Right, Down) && me->variables->direction != UP) {
		me->animationTimer = 11;
		me->variables->direction = UP;
		me->hFlip = false;
	}
	if(DSGM_held.Down && DSGM_NOT_HOLDING_3(Left, Right, Up) && me->variables->direction != DOWN) {
		me->animationTimer = 11;
		me->variables->direction = DOWN;
		me->hFlip = false;
	}
	
	// Move
	if(me->variables->direction == LEFT && DSGM_held.Left) {
		if(\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 8) / 8, (me->y + 16) / 8)) &&\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 8) / 8, (me->y + 31) / 8))\
		) DSGM_view[DSGM_BOTTOM].x--;
		DSGM_ReturnAnimateObjectInstance(me, 3, 4, 5, 12);
	}
	if(me->variables->direction == RIGHT && DSGM_held.Right) {
		if(\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 24) / 8, (me->y + 16) / 8)) &&\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 24) / 8, (me->y + 31) / 8))\
		) DSGM_view[DSGM_BOTTOM].x++;
		DSGM_ReturnAnimateObjectInstance(me, 3, 4, 5, 12);
	}
	if(me->variables->direction == UP && DSGM_held.Up) {
		if(\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 9) / 8, (me->y + 15) / 8)) &&\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 23) / 8, (me->y + 15) / 8))\
		) DSGM_view[DSGM_BOTTOM].y--;
		DSGM_ReturnAnimateObjectInstance(me, 6, 7, 8, 12);
	}
	if(me->variables->direction == DOWN && DSGM_held.Down) {
		if(\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 9) / 8, (me->y + 32) / 8)) &&\
			walkableTile(DSGM_GetTile(&DSGM_layers[DSGM_BOTTOM][1], (me->x + 23) / 8, (me->y + 32) / 8))\
		) DSGM_view[DSGM_BOTTOM].y++;
		DSGM_ReturnAnimateObjectInstance(me, 0, 1, 2, 12);
	}
	
	// Stand still
	if(DSGM_NOT_HOLDING_DPAD()) {
		if(me->variables->direction == LEFT || me->variables->direction == RIGHT) me->frame = 3;
		if(me->variables->direction == UP) me->frame = 6;
		if(me->variables->direction == DOWN) me->frame = 0;
	}
}

void player_touch(playerObjectInstance *me) {
	//mmStop();
	//DSGM_Sounds[FlatOutLies].loaded = false;
	//DSGM_PlaySound(FlatOutLies);
	//mmLoad(DSGM_Sounds[FlatOutLies].ID);
	//swiWaitForVBlank();
	//mmStart(DSGM_Sounds[FlatOutLies].ID, MM_PLAY_LOOP);
	
	DSGM_GotoNextRoom(false);
	/*if(DSGM_currentRoom == Room_1) DSGM_SwitchRoom(Room_2, false);
	else if(DSGM_currentRoom == Room_2) DSGM_SwitchRoom(Room_1, false);*/
}
