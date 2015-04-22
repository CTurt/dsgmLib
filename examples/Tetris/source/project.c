#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
unsigned char flicker = 0;
unsigned int speed;
unsigned char started = 0;
int leadX, leadY, leadBY;
unsigned char type;
unsigned char rotate;

const signed char patterns[7][3][2] = {
	{
		{ 8, 0 },
		{ -8, 0 },
		{ 16, 0 }
	},
	{
		{ -8, 0 },
		{ 0, 8 },
		{ 8, 0 }
	},
	{
		{ -8, 0 },
		{ 0, 8 },
		{ 8, 8 }
	},
	{
		{ 8, 0 },
		{ -8, 8 },
		{ 0, 8 }
	},
	{
		{ -8, 0 },
		{ 8, 0 },
		{ 8, 8 }
	},
	{
		{ -8, 0 },
		{ 8, 0 },
		{ -8, 8 }
	},
	{
		{ 8, 0 },
		{ 0, 8 },
		{ 8, 8 }
	},
};

inline void *getPattern(unsigned char type, unsigned char rotation) {
	static signed char pattern[4][2];
	int i;
	
	pattern[0][0] = 0;
	pattern[0][1] = 0;
	
	for(i = 1; i < 4; i++) {
		pattern[i][0] = patterns[type][i - 1][0];
		pattern[i][1] = patterns[type][i - 1][1];
	}
	
	// Don't allow rotation of a square
	if(type == 6) return pattern; 
	
	while(rotation > 0) {
		signed char tempPattern[4][2];
		
		for(i = 0; i < 4; i++) {
			tempPattern[i][0] = pattern[i][1];
			tempPattern[i][1] = -pattern[i][0];
		}
		
		memcpy(&pattern, &tempPattern, sizeof(pattern));
		
		rotation--;
	}
	
	return pattern;
}

unsigned char collision(unsigned char leadX, unsigned char leadY, signed char (*pattern)[2]) {
	int i;
	for(i = 0; i < 4; i++) {
		if(DSGM_GetTile(DSGM_TOP, 1, (leadX + pattern[i][0]) / 8, (leadY + pattern[i][1]) / 8) != 0 || (leadY + pattern[i][1]) / 8 >= 192 / 8) {
			return 1;
		}
	}
	
	return 0;
}

void generateBlock(void) {
	rotate = 0;
	type = DSGM_Random(0, 6);
	
	signed char (*pattern)[2] = getPattern(type, rotate);
	
	leadX = 15 * 8;
	leadY = 0;
	leadBY = 0;
	
	DSGM_ObjectGroup *group = DSGM_GetObjectGroup(DSGM_TOP, &DSGM_Objects[block]);
	
	int i;
	for(i = 0; i < 4; i++) {
		blockObjectInstance *blockInstance = (blockObjectInstance *)&group->objectInstances[i];
		blockInstance->x = leadX + pattern[i][0];
		blockInstance->y = leadY + pattern[i][1];
		blockInstance->frame = type;
	}
}

void removeVBlankHandler(void) {
	void irqDummy(void);
	irqSet(IRQ_VBLANK, irqDummy);
}

int moveBlocksDownY;
void moveBlocksDown(void) {
	int x;
	while(moveBlocksDownY > 0) {
		for(x = 11; x < 21; x++) {
			DSGM_SetTile(DSGM_TOP, 1, x, moveBlocksDownY, DSGM_GetTile(DSGM_TOP, 1, x, moveBlocksDownY - 1));
		}
		
		moveBlocksDownY--;
	}
}

void gameOver(void) {
	// Remove all sprites from screen during animation
	oamClear(&oamMain, 0, 0);
	
	int x, y;
	for(y = 0; y < 192 / 8; y++) {
		for(x = 11; x < 21; x++) {
			DSGM_SetTile(DSGM_TOP, 1, x, y, 5);
		}
		
		DSGM_Update();
	}
	
	DSGM_Update();
	
	for(y = 0; y < 192 / 8; y++) {
		for(x = 11; x < 21; x++) {
			DSGM_SetTile(DSGM_TOP, 1, x, y, 0);
		}
		
		DSGM_Update();
	}
	
	flicker = 0;
	started = 0;
}

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_RAM_BACKGROUND(tetris, BgSize_T_256x256, BgType_Text8bpp),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_RAM_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_RAM_SPRITE(blocks, DSGMPal0, SpriteSize_8x8, 7),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// block
	{
		&DSGM_Sprites[blocks],
		DSGM_NO_EVENT,
		(DSGM_Event)block_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((blockObjectInstance *)0)->variables)
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
					7,							// Map base
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
					true,						// Attached to view system
					7,							// Map base
					0,							// Tile base
					-4, 0, 0
				},
				
				// Layer 1
				{
					&DSGM_Backgrounds[tetris],	// Background
					DSGM_TOP,					// Screen
					1,							// Layer
					false,						// Attached to view system
					15,							// Map base
					1,							// Tile base
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
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_TOP, 1);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_TOP][0], &DSGM_Objects[block], DSGM_TOP, 4,
		256, 0,
		256, 0,
		256, 0,
		256, 0
	);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 0);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void Room_1_handler(void) {
	if(!started) {
		DSGM_SetScreenColor(DSGM_BOTTOM, DSGM_Color(31, 31, 31));
		DSGM_SetTextColor(DSGM_BOTTOM, DSGM_Color(0, 0, 0));
		
		DSGM_DrawText(DSGM_BOTTOM, 5, 1, "dsgmLib Tetris example");
		
		flicker++;
		flicker %= 60;
		
		if(flicker < 30) {
			DSGM_SetTextColor(DSGM_TOP, DSGM_Color(0, 0, 0));
		}
		else {
			DSGM_SetTextColor(DSGM_TOP, DSGM_Color(31, 31, 31));
		}
		
		DSGM_DrawText(DSGM_TOP, 13, 10, "PRESS");
		DSGM_DrawText(DSGM_TOP, 13, 11, "START");
		
		if(DSGM_newpress.Start) {
			DSGM_ClearText(DSGM_TOP);
			
			speed = 64;
			started = 1;
			generateBlock();
		}
	}
	else {
		DSGM_ObjectGroup *blockGroup = DSGM_GetObjectGroup(DSGM_TOP, &DSGM_Objects[block]);
		signed char (*pattern)[2] = getPattern(type, rotate);
		
		if(!collision(leadX, leadY, pattern)) {
			leadBY += speed * (DSGM_held.Down ? 4 : 1);
			leadY = (leadBY >> 8) & ~7;
		}
		else {
			speed += 2;
			
			int i;
			for(i = 3; i >= 0; i--) {
				DSGM_SetTile(DSGM_TOP, 1, blockGroup->objectInstances[i].x / 8, blockGroup->objectInstances[i].y / 8 - 1, type + 4);
				blockGroup->objectInstances[i].x = 256;
			}
			
			if(leadY == 0) {
				gameOver();
			}
			else {
				int x, y;
				for(y = 192 / 8; y >= 0; y--) {
					for(x = 11; x < 21; x++) {
						if(DSGM_GetTile(DSGM_TOP, 1, x, y) == 0) break;
						
						// Full line!
						if(x == 20) {
							oamClear(&oamMain, 0, 0);
							oamUpdate(&oamMain);
							
							while(x >= 11) {
								DSGM_SetTile(DSGM_TOP, 1, x, y, 0);
								DSGM_Update();
								DSGM_Update();
								DSGM_Update();
								DSGM_Update();
								
								x--;
							}
							
							// Can't write this much data to VRAM now, do it in next VBlank
							moveBlocksDownY = y;
							irqSet(IRQ_VBLANK, moveBlocksDown);
							swiWaitForVBlank();
							removeVBlankHandler();
						}
					}
				}
				
				generateBlock();
			}
		}
		
		if(DSGM_newpress.L) {
			signed char (*pattern)[2] = getPattern(type, (rotate + 3) % 4);
			
			if(!collision(leadX, leadY, pattern)) {
				rotate += 3;
				rotate %= 4;
			}
		}
		
		if(DSGM_newpress.R) {
			signed char (*pattern)[2] = getPattern(type, (rotate + 1) % 4);
			
			if(!collision(leadX, leadY, pattern)) {
				rotate++;
				rotate %= 4;
			}
		}
		
		if(DSGM_newpress.Left) {
			if(!collision(leadX - 8, leadY, pattern)) {
				leadX -= 8;
			}
		}
		
		if(DSGM_newpress.Right) {
			if(!collision(leadX + 8, leadY, pattern)) {
				leadX += 8;
			}
		}
	}
}

void block_loop(blockObjectInstance *me) {
	if(!started) return;
	
	signed char (*pattern)[2] = getPattern(type, rotate);
	
	me->x = leadX + pattern[DSGM_GetObjectInstanceID(me)][0];
	me->y = leadY + pattern[DSGM_GetObjectInstanceID(me)][1];
}
