#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	// Width and height must be the same for a large background, in this case it is 3584x3584 pixels
	DSGM_FORM_NITRO_BACKGROUND(marioMap, DSGM_LARGE_BACKGROUND_3584, DSGM_LARGE_BACKGROUND),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
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
					2,							// Map base
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
					&DSGM_Backgrounds[marioMap],// Background
					DSGM_BOTTOM,				// Screen
					3,							// Layer
					true,						// Attached to view system
					3,							// Map base
					1,							// Tile base
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
					2,							// Map base
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
					&DSGM_Backgrounds[marioMap],// Background
					DSGM_TOP,					// Screen
					3,							// Layer
					true,						// Attached to view system
					3,							// Map base
					1,							// Tile base
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
		
		Room_1_handler,
		
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
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 0);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void Room_1_handler(void) {	
	if(DSGM_held.Left && DSGM_view[DSGM_BOTTOM].x > 0) {
		DSGM_view[DSGM_BOTTOM].x--;
	}
	
	if(DSGM_held.Right && DSGM_view[DSGM_BOTTOM].x < DSGM_LargeBackgroundSizeToPixels(DSGM_Backgrounds[marioMap].size) - 256) {
		DSGM_view[DSGM_BOTTOM].x++;
	}
	
	if(DSGM_held.Up && DSGM_view[DSGM_BOTTOM].y > 0) {
		DSGM_view[DSGM_BOTTOM].y--;
	}
	
	if(DSGM_held.Down && DSGM_view[DSGM_BOTTOM].y < 256 - 192) {
		DSGM_view[DSGM_BOTTOM].y++;
	}
	
	DSGM_view[DSGM_TOP].x = DSGM_view[DSGM_BOTTOM].x;
	DSGM_view[DSGM_TOP].y = DSGM_view[DSGM_BOTTOM].y;
}
