#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
#define COORD_COUNT 24

struct {
	u8 x, y;
} drawCoords[COORD_COUNT] = { { 0, 0 } };

struct {
	s16 x[2], y[2];
} perpendicularStylus = { { 0, 0} };

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_NITRO_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_NITRO_SPRITE(orange, DSGMPal0, SpriteSize_32x32, 3),
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
	
	// orange
	{
		&DSGM_Sprites[orangeSprite],
		(DSGM_Event)orange_create,
		(DSGM_Event)orange_loop,
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
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_BOTTOM][0], &DSGM_Objects[drawObject], DSGM_BOTTOM, 1,
		0, 0
	);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void drawObject_create(drawObjectObjectInstance *me) {
	DSGM_DrawText(DSGM_TOP, 1, 1, "dsgmLib Fruit Ninja example");
	DSGM_DrawText(DSGM_TOP, 1, 3, "Press A to spawn fruit");
	DSGM_DrawText(DSGM_TOP, 1, 4, "Use Stylus to swipe");
}

void drawObject_loop(drawObjectObjectInstance *me) {
	int i;
	
	if(DSGM_newpress.Stylus) {
		DSGM_ClearDrawableBackground(DSGM_BOTTOM);
		
		for(i = 0; i < COORD_COUNT; i++) {
			drawCoords[i].x = DSGM_stylus.x;
			drawCoords[i].y = DSGM_stylus.y;
		}
	}
	
	for(i = COORD_COUNT - 1; i >= 0; i--) {
		DSGM_DrawLineToBackground(DSGM_BOTTOM, drawCoords[i].x, drawCoords[i].y, drawCoords[i + 1].x, drawCoords[i + 1].y, 2, DSGM_Color(31 - (i * 32 / COORD_COUNT) < 0 ? 0 : 31 - (i * 32 / COORD_COUNT), 0, 0));
	}
	
	if(!DSGM_held.Stylus) {
		DSGM_DrawFilledRectangleToBackground(DSGM_BOTTOM, drawCoords[0].x - 2, drawCoords[0].y - 2, 4, 4, DSGM_Color(0, 0, 0));
	}
	
	for(i = COORD_COUNT - 1; i > 0; i--) {
		drawCoords[i].x = drawCoords[i - 1].x;
		drawCoords[i].y = drawCoords[i - 1].y;
	}
	
	drawCoords[0].x = DSGM_stylus.x;
	drawCoords[0].y = DSGM_stylus.y;
	
	if(DSGM_newpress.A) {
		DSGM_CreateObjectInstance(DSGM_BOTTOM, DSGM_Random(0, 224), 192, &DSGM_Objects[orange]);
	}
}

void orange_create(orangeObjectInstance *me) {
	me->bitshift = 8;
	
	me->bx = me->x << me->bitshift;
	me->by = me->y << me->bitshift;
	
	if(me->y == 192) {
		me->variables->vx = ((256 / 2) - me->x) * 3 + DSGM_Random(-64, 64);
		me->variables->vy = -DSGM_Random(800, 1200);
	}
}

void orange_loop(orangeObjectInstance *me) {	
	me->variables->vy += 16;
	
	me->bx += me->variables->vx;
	me->by += me->variables->vy;
	
	me->x = me->bx >> me->bitshift;
	me->y = me->by >> me->bitshift;
	
	if(me->y > 192) {
		DSGM_DeleteObjectInstance(me);
	}
	
	if(me->frame == 0) {
		int x = drawCoords[0].x, y = drawCoords[0].y, x2 = drawCoords[1].x, y2 = drawCoords[1].y, length = 32;
		
		int angle = DSGM_GetAngle(x, y, x2, y2) + degreesToAngle(90);
		
		x = (cosLerp(angle) * length / 2) >> 12;
		y = (-sinLerp(angle) * length / 2) >> 12;
		
		angle += degreesToAngle(180);
		
		x2 = (cosLerp(angle) * length / 2) >> 12;
		y2 = (-sinLerp(angle) * length / 2) >> 12;
		
		if(DSGM_Intersection(drawCoords[0].x, drawCoords[0].y, drawCoords[1].x, drawCoords[1].y, me->x + 16 + x, me->y + 16 + y, me->x + 16 + x2, me->y + 16 + y2)) {
			me->frame = 1;
			
			orangeObjectInstance *o = (orangeObjectInstance *)DSGM_CreateObjectInstance(DSGM_BOTTOM, me->x, me->y, me->object);
			o->variables->vx = me->variables->vx / 2;
			o->variables->vy = me->variables->vy / 2;
			o->frame = 2;
		}
	}
}
