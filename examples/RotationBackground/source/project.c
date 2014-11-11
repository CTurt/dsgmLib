#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_BACKGROUND(Exorcist, BgSize_ER_256x256, BgType_ExRotation),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// zoomController
	{
		DSGM_NO_SPRITE,
		(DSGM_Event)zoomController_create,
		(DSGM_Event)zoomController_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((zoomControllerObjectInstance *)0)->variables)
	},
};

DSGM_Room DSGM_Rooms[DSGM_ROOM_COUNT] = {
	// Room_1
	{
		// Backgrounds
		{
			// Bottom screen (http://mtheall.com/vram.html#T0=3&NT0=128&MB0=21&TB0=3&T2=1&NT2=256&MB2=17&TB2=1&S2=3&T3=4&NT3=256&MB3=16&TB3=0&S3=1)
			{
				// Layer 0
				{
					DSGM_DEFAULT_FONT,			// Background
					DSGM_BOTTOM,				// Screen
					0,							// Layer
					false,						// Attached to view system
					21,							// Map base
					3,							// Tile base
					0, 0, 0
				},
				
				// Layer 1
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_BOTTOM,				// Screen
					1,							// Layer
					false,						// Attached to view system
					0,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Layer 2
				{
					DSGM_NO_BACKGROUND,			// Background
					DSGM_BOTTOM,				// Screen
					2,							// Layer
					false,						// Attached to view system
					17,							// Map base
					1,							// Tile base
					0, 0, 0
				},
				
				// Only layer 3 may be rotatable, to make it appear not at the back use
				// DSGM_SetLayerPriority(screen, layer, priority)
				
				// Layer 3
				{
					&DSGM_Backgrounds[odale],// Background
					DSGM_BOTTOM,				// Screen
					3,							// Layer
					false,						// Attached to view system
					16,							// Map base
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
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[zoomController], DSGM_BOTTOM, 1,
		0, 0
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void zoomController_create(zoomControllerObjectInstance *me) {
	DSGM_layers[DSGM_BOTTOM][3].x = 64;
	DSGM_layers[DSGM_BOTTOM][3].y = 64;
	DSGM_SetRotationCenter(DSGM_BOTTOM, 128, 96);
}

void zoomController_loop(zoomControllerObjectInstance *me) {
	me->variables->timer++;
	
	DSGM_RotateBackground(DSGM_BOTTOM, me->variables->timer << 7);
	DSGM_ScaleBackground(DSGM_BOTTOM, (sinLerp(me->variables->timer << 6) >> 3) + 580, (sinLerp(me->variables->timer << 6) >> 3) + 580);
}
