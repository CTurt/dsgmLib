#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_FONT(ComicSans),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// drawObject
	{
		DSGM_NO_SPRITE,
		(DSGM_Event)drawObject_create,
		(DSGM_Event)drawObject_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((drawObjectObjectInstance *)0)->variables)
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
				
				// Only layer 3 may be drawable
				// Layer 3
				{
					DSGM_DRAWABLE_BACKGROUND,	// Background
					DSGM_BOTTOM,				// Screen
					3,							// Layer
					false,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
			},
			
			// Top screen
			{
				// Layer 0
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_TOP,					// Screen
					0,							// Layer
					false,						// Attached to view system
					0,							// Map base
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
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[drawObject], DSGM_BOTTOM, 1,
		64, 64
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}

// Several different things for you to try!

void drawObject_create(drawObjectObjectInstance *me) {
	//DSGM_DrawTextToBackground(DSGM_BOTTOM, 40, 80, DSGM_DEFAULT_FONT, 2, DSGM_RED, "Draw on me!");
	DSGM_DrawTextToBackground(DSGM_BOTTOM, 40, 80, &DSGM_Backgrounds[ComicSans], 2, DSGM_RED, "Draw on me!");
	
	// Test 1:
	//int i;
	//for(i = 0; i < 5; i++) {
	//	DSGM_DrawTileToBackground(DSGM_BOTTOM, DSGM_Random(20, 180), DSGM_Random(30, 140), DSGM_DEFAULT_FONT, DSGM_Random('A', 'Z'), DSGM_Random(1, 6), DSGM_RED);
	//}
}

void drawObject_loop(drawObjectObjectInstance *me) {
	if(DSGM_held.Stylus) {
		// Test 2:
		//DSGM_DrawPixelToBackground(DSGM_BOTTOM, DSGM_stylus.x, DSGM_stylus.y, DSGM_Color(31, 0, 0));
		
		// Test 3:
		//DSGM_DrawRectangleToBackground(DSGM_BOTTOM, DSGM_stylus.x - 3, DSGM_stylus.y - 3, 6, 6, 2, DSGM_Color(31, 0, 0));
		
		// Test 4:
		//DSGM_DrawFilledRectangleToBackground(DSGM_BOTTOM, DSGM_stylus.x - 3, DSGM_stylus.y - 3, 6, 6, DSGM_Color(31, 0, 0));
		
		// Test 5:
		//DSGM_DrawCircleToBackground(DSGM_BOTTOM, DSGM_stylus.x, DSGM_stylus.y, 16, 2, DSGM_Color(31, 0, 0));
		
		// Test 6:
		//DSGM_DrawFilledCircleToBackground(DSGM_BOTTOM, DSGM_stylus.x, DSGM_stylus.y, 16, DSGM_Color(31, 0, 0));
		
		// Test 7:
		//DSGM_ClearDrawableBackground(DSGM_BOTTOM);
		//DSGM_DrawCircleToBackground(DSGM_BOTTOM, DSGM_stylus.x, DSGM_stylus.y, 16, 2, DSGM_Color(31, 0, 0));
		
		// Test 8:
		//DSGM_ClearDrawableBackground(DSGM_BOTTOM);
		//DSGM_DrawLineAtAngleToBackground(DSGM_BOTTOM, 127, 95, DSGM_GetAngle(127, 95, DSGM_stylus.x, DSGM_stylus.y), 16, 2, DSGM_Color(31, 0, 0));
	}
	
	// Test 9:
	if(DSGM_held.Stylus && !DSGM_newpress.Stylus) {
		DSGM_DrawLineToBackground(DSGM_BOTTOM, DSGM_stylus.x - DSGM_stylus.dx, DSGM_stylus.y - DSGM_stylus.dy, DSGM_stylus.x, DSGM_stylus.y, 6, DSGM_GREEN);
	}
}
