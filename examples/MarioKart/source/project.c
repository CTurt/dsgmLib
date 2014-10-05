#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
int rotation = 50;

int carBodyTextureID;
int carWheelsTextureID;
int kart_tire_L_TextureID;
int kart_tire_M_TextureID;
int kart_tire_S_TextureID;
int carEmblemTextureID;

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_RAM_BACKGROUND(MarioKart, BgSize_T_256x256, BgType_Text8bpp),
	DSGM_FORM_RAM_BACKGROUND(MarioKartLower, BgSize_T_256x256, BgType_Text8bpp),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
	DSGM_FORM_RAM_PALETTE(DSGMPal0),
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
	DSGM_FORM_RAM_SPRITE(itemBox, DSGMPal0, SpriteSize_64x64, 1),
	DSGM_FORM_RAM_SPRITE(banana, DSGMPal0, SpriteSize_32x32, 1),
};

DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT] = {
	// renderer
	{
		DSGM_NO_SPRITE,
		(DSGM_Event)renderer_create,
		(DSGM_Event)renderer_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((rendererObjectInstance *)0)->variables)
	},
	
	// itemBox
	{
		&DSGM_Sprites[itemBoxSprite],
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((itemBoxObjectInstance *)0)->variables)
	},
	
	// banana
	{
		&DSGM_Sprites[bananaSprite],
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((bananaObjectInstance *)0)->variables)
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
					&DSGM_Backgrounds[MarioKartLower],// Background
					DSGM_BOTTOM,				// Screen
					2,							// Layer
					true,						// Attached to view system
					3,							// Map base
					1,							// Tile base
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
				// Only top screen layer 0 can be a 3D renderer
				// Layer 0
				{
					DSGM_3D_RENDERER,			// Background
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
					&DSGM_Backgrounds[MarioKart],// Background
					DSGM_TOP,					// Screen
					2,							// Layer
					true,						// Attached to view system
					0,							// Map base
					1,							// Tile base
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
		
		NULL,
		
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
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_TOP, 3);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_TOP][0], &DSGM_Objects[renderer], DSGM_TOP, 1,
		0, 0
	);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_TOP][1], &DSGM_Objects[banana], DSGM_TOP, 1,
		8, 8
	);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_TOP][2], &DSGM_Objects[itemBox], DSGM_TOP, 1,
		5, 5
	);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 0);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void renderer_create(rendererObjectInstance *me) {
	DSGM_DrawText(DSGM_BOTTOM, 1, 1, "dsgmLib model rendering");
	DSGM_DrawText(DSGM_BOTTOM, 1, 4, "Hold A to use low poly wheels");
	
	DSGM_DrawText(DSGM_BOTTOM, 1, 22, "models & textures by Nintendo");
	
	glInit();
	glEnable(GL_ANTIALIAS);
	
	glClearColor(0, 0, 0, 0);
	glClearPolyID(63);
	glClearDepth(0x7FFF);
	
	glEnable(GL_BLEND);
	
	// When 3D mode is not enabled, VRAM bank B is used for top screen sprites, bank G is unused
	// When 3D mode is enabled, VRAM bank B is used for textures, and bank G is used for top screen sprites
	// Since bank G is smaller, we will have less VRAM available for sprites on the top screen
	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &carBodyTextureID);
	glBindTexture(0, carBodyTextureID);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_64, TEXTURE_SIZE_64, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, carBody_Texture_bin);
	
	glGenTextures(1, &carWheelsTextureID);
	glBindTexture(0, carWheelsTextureID);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_64, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, carWheels_Texture_bin);
	
	glGenTextures(1, &kart_tire_L_TextureID);
	glBindTexture(0, kart_tire_L_TextureID);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, TEXTURE_SIZE_64, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, kart_tire_L_Texture_bin);
	
	glGenTextures(1, &carEmblemTextureID);
	glBindTexture(0, carEmblemTextureID);
	glTexImage2D(0, 0, GL_RGBA, TEXTURE_SIZE_32, TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T, carEmblem_Texture_bin);
	
	glViewport(0, 0, 255, 191);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 40);
	
	gluLookAt(0.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void renderer_loop(rendererObjectInstance *me) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glMaterialf(GL_AMBIENT, RGB15(8,8,8));
	glMaterialf(GL_DIFFUSE, RGB15(24,24,24));
	glMaterialf(GL_SPECULAR, RGB15(0,0,0));
	glMaterialf(GL_EMISSION, RGB15(0,0,0));
	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_DECAL);
	
	glRotateY(rotation++);
	
	glBindTexture(0, carBodyTextureID);
	glCallList((u32 *)carBody_bin);
	
	glColor(RGB15(31, 31, 31));
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_MODULATION);
	
	glBindTexture(0, carEmblemTextureID);
	glCallList((u32 *)carEmblem_bin);
	
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_DECAL);
	
	if(!DSGM_held.A) {
		/*
		Some wheel position and size calculations
		
		UPPER Big wheel Y 0.458252
		LOWER Big wheel Y -0.010742
		CENTRE Big wheel Y (0.458252 - 0.010742) / 2
		SIZE Big wheel Y (0.458252 - 0.010742)
		
		UPPER Smaller wheel Y 0.341065
		LOWER Smaller wheel Y -0.010742
		CENTRE Smaller wheel Y (0.341065 - 0.010742) / 2
		SIZE Smaller wheel Y (0.341065 - 0.010742)
		
		Smaller wheel scale = (0.341065 - 0.010742) / (0.458252 - 0.010742)
		*/
		
		glBindTexture(0, kart_tire_L_TextureID);
		
		glPushMatrix();
		glTranslatef(0.458252, (0.458252 - 0.010742) / 2.0f, -0.534180);
		glRotateX(rotation * 4);
		glCallList((u32 *)kart_tire_L_bin);
		glPopMatrix(1);
		
		glPushMatrix();
		glTranslatef(-0.458252, (0.458252 - 0.010742) / 2.0f, -0.534180);
		glRotateX(rotation * 4);
		glCallList((u32 *)kart_tire_L_bin);
		glPopMatrix(1);
		
		glPushMatrix();
		glTranslatef(-0.458252, (0.341065 - 0.010742) / 2.0f, 0.403809);
		glScalef((0.341065 - 0.010742) / (0.458252 - 0.010742), (0.341065 - 0.010742) / (0.458252 - 0.010742), (0.341065 - 0.010742) / (0.458252 - 0.010742));
		glRotateX(rotation * 4);
		glCallList((u32 *)kart_tire_L_bin);
		glPopMatrix(1);
		
		glPushMatrix();
		glTranslatef(0.458252, (0.341065 - 0.010742) / 2.0f, 0.403809);
		glScalef((0.341065 - 0.010742) / (0.458252 - 0.010742), (0.341065 - 0.010742) / (0.458252 - 0.010742), (0.341065 - 0.010742) / (0.458252 - 0.010742));
		glRotateX(rotation * 4);
		glCallList((u32 *)kart_tire_L_bin);
		glPopMatrix(1);
	}
	else {
		glBindTexture(0, carWheelsTextureID);
		glCallList((u32 *)carWheels_bin);
	}
	
	glPopMatrix(1);
}
