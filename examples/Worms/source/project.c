#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
#define COLLISION_WIDTH 8

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_BACKGROUND(worms, BgSize_T_256x256, BgType_Text8bpp),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_NITRO_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_NITRO_SPRITE(grenade, DSGMPal0, SpriteSize_32x32, 1),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// terrain
	{
		DSGM_NO_SPRITE,
		(DSGM_Event)terrain_create,
		(DSGM_Event)terrain_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((terrainObjectInstance *)0)->variables)
	},
	
	// grenade
	{
		&DSGM_Sprites[grenadeSprite],
		(DSGM_Event)grenade_create,
		(DSGM_Event)grenade_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((grenadeObjectInstance *)0)->variables)
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
					&DSGM_Backgrounds[worms],	// Background
					DSGM_BOTTOM,				// Screen
					2,							// Layer
					true,						// Attached to view system
					4,							// Map base
					1,							// Tile base
					0, 0, 0
				},
				
				// Layer 3
				{
					DSGM_NO_BACKGROUND,			// Background
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
					&DSGM_Backgrounds[worms],	// Background
					DSGM_TOP,					// Screen
					2,							// Layer
					true,						// Attached to view system
					6,							// Map base
					0,							// Tile base
					0, 0, 0
				},
				
				// Only layer 3 may be drawable
				// Layer 3
				{
					DSGM_DRAWABLE_BACKGROUND,	// Background
					DSGM_TOP,					// Screen
					3,							// Layer
					true,						// Attached to view system
					1,							// Map base
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
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[terrain], DSGM_BOTTOM, 1,
		64, 64
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void terrain_create(terrainObjectInstance *me) {
	// Todo DSGM_TOP_S
	// Use main engine on bottom screen, we have to use main engine because it has more VRAM
	lcdMainOnBottom();
	
	DSGM_DrawText(DSGM_BOTTOM, 1, 1, "dsgmLib Worms terrain example");
	DSGM_DrawText(DSGM_BOTTOM, 1, 3, "Stylus - place grenades");
	
	// Put drawable background (layer 3) ontop of the layer 2 background
	DSGM_SetLayerPriority(DSGM_TOP, 3, 2);
	DSGM_SetLayerPriority(DSGM_TOP, 2, 3);
	
	// Draw some landscape
	int height = DSGM_Random(120, 150) << 12;
	int nangle = DSGM_Random(degreesToAngle(0), degreesToAngle(90)) - degreesToAngle(45);
	int angle = nangle;
	int x = 0;
	while(x < 256 << 12) {
		while(nangle > -(degreesToAngle(12)) && height < 120 << 12) nangle -= 1;
		while(nangle < degreesToAngle(12) && height > 170 << 12) nangle += 1;
		
		int y;
		for(y = height >> 12; y < 192; y++) {
			DSGM_DrawPixelToBackground(DSGM_TOP, x >> 12, y, (u16)ground_Bitmap_bin[(x >> 12) * 256 + y] | ((1) << 15));
			//DSGM_DrawPixelToBackground(DSGM_TOP, x >> 12, y, DSGM_BLACK);
		}
		
		x += cosLerp(angle);
		height -= sinLerp(angle);
		angle += (nangle - angle) / 24;
		
		if(angle / 48 == nangle / 48) {
			nangle = DSGM_Random(degreesToAngle(0), degreesToAngle(90)) - degreesToAngle(45);
		}
	}
}

void terrain_loop(terrainObjectInstance *me) {
	if(DSGM_newpress.Stylus) {
		DSGM_CreateObjectInstance(DSGM_TOP, DSGM_stylus.x - 16, DSGM_stylus.y - 16, &DSGM_Objects[grenade]);
	}
}

void grenade_create(grenadeObjectInstance *me) {
	DSGM_InitObjectInstanceRotScale(me);
	me->bitshift = 8;
	me->bx = me->x << me->bitshift;
	me->by = me->y << me->bitshift;
	me->variables->xVelocity = 0;
	me->variables->yVelocity = 0;
	me->variables->fuse = 3 * 60;
}

void grenade_loop(grenadeObjectInstance *me) {
	me->variables->fuse--;
	*me->angle -= me->variables->xVelocity * 2;
	
	if((me->bx >> me->bitshift <= 0 && me->variables->xVelocity < 0) || (me->bx >> me->bitshift >= 223 && me->variables->xVelocity > 0)) {
		me->variables->xVelocity = -me->variables->xVelocity;
	}
	
	if(DSGM_GetDrawablePixel(DSGM_TOP, (me->bx >> me->bitshift) + 16, (me->by >> me->bitshift) + 32) & 1 << 15) {
		me->variables->yVelocity = 0;
		
		int hl = 120, hr = 120;
		while(!(DSGM_GetDrawablePixel(DSGM_TOP, (me->bx >> me->bitshift) + (16 - COLLISION_WIDTH), hl) & 1 << 15) && hl < 192) {
			hl++;
		}
		
		while(!(DSGM_GetDrawablePixel(DSGM_TOP, (me->bx >> me->bitshift) + (16 + COLLISION_WIDTH), hr) & 1 << 15) && hr < 192) {
			hr++;
		}
		
		me->variables->xVelocity += hr - hl;
	}
	
	else {
		me->variables->yVelocity += 100;
	}
	
	if(me->variables->fuse == 0) {
		DSGM_DrawFilledCircleToBackground(DSGM_TOP, (me->bx >> me->bitshift) + 16, (me->by >> me->bitshift) + 16, 32, ARGB16(0, 0, 0, 0));
		DSGM_DeleteObjectInstance(me);
	}
	
	me->bx += me->variables->xVelocity;
	me->by += me->variables->yVelocity;
	
	while(DSGM_GetDrawablePixel(DSGM_TOP, (me->bx >> me->bitshift) + 16, (me->by >> me->bitshift) + 31) & 1 << 15) {
		me->by -= 1 << me->bitshift;
	}
	
	me->x = me->bx >> me->bitshift;
	me->y = me->by >> me->bitshift;
}
