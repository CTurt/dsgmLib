#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
int timer = 0;
bool light = true;

DSGM_Model archvile;
int archvileTexture;

DSGM_ModelInstance archvileInstance;
Quaternion q;

void updateLighting(void) {
	DSGM_DrawText(DSGM_BOTTOM, 1, 3, "Light: %s (A to toggle) ", light ? "Enabled" : "Disabled");
	if(light) {
		glMaterialf(GL_EMISSION, BIT(15) | RGB15(0, 0, 0));
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_FORMAT_LIGHT0);
	}
	else {
		glMaterialf(GL_EMISSION, BIT(15) | RGB15(31, 31, 31));
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT);
	}
}

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_BACKGROUND(Quake2, BgSize_T_256x256, BgType_Text8bpp),
};

DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT] = {
};

DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT] = {
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
					&DSGM_Backgrounds[Quake2],	// Background
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
					&DSGM_Backgrounds[Quake2],// Background
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
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_TOP, 1);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[Room_1].objectGroups[DSGM_TOP][0], &DSGM_Objects[renderer], DSGM_TOP, 1,
		0, 0
	);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[Room_1], DSGM_BOTTOM, 0);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void renderer_create(rendererObjectInstance *me) {
	DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Loading models and textures...");
	swiWaitForVBlank();
	
	if(!DSGM_LoadModel("Archvile.md2", &archvile)) {
		DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Failed to load model, does your card support NitroFS?");
		return;
	}
	
	DSGM_InitModelInstance(&archvileInstance, &archvile);
	DSGM_SetModelInstanceAnimation(&archvileInstance, 0, true);
	archvileInstance.animationSpeed = 1;
	archvileInstance.interpolate = true;
	
	DSGM_ClearText(DSGM_BOTTOM);
	
	glInit();
	glEnable(GL_BLEND);
	glColor(RGB15(31, 31, 31));
	
	glEnable(GL_ANTIALIAS);
	glClearColor(0, 0, 0, 0);
	glClearPolyID(63);
	glClearDepth(0x7FFF);
	
	// When 3D mode is not enabled, VRAM bank B is used for top screen sprites, bank G is unused
	// When 3D mode is enabled, VRAM bank B is used for textures, and bank G is used for top screen sprites
	// Since bank G is smaller, we will have less VRAM available for sprites on the top screen
	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(1, &archvileTexture);
	glBindTexture(0, archvileTexture);
	glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_256, TEXTURE_SIZE_256, 0, TEXGEN_TEXCOORD, archvile_Texture_bin);
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 2.0f);
	
	glViewport(0, 0, 255, 191);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(26, 256.0 / 192.0, 0.1, 2000);
	
	gluLookAt(0.0, 0.0, -250.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glMaterialf(GL_AMBIENT, RGB15(4, 4, 4));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialShinyness();
	
	glLight(0, RGB15(31, 31, 31), floattov10(-0.6f), floattov10(0.0f), floattov10(0.8f));
	updateLighting();
	
	DSGM_DrawText(DSGM_BOTTOM, 1, 1, "dsgmLib MD2 model rendering");
	DSGM_DrawText(DSGM_BOTTOM, 1, 5, "D-Pad + L/R to rotate");
	DSGM_DrawText(DSGM_BOTTOM, 1, 22, "models & textures from Quake 2");
	
	q = Quaternion_fromAxisAngle(vect(1 << 12, 0, 0), -degreesToAngle(90));
	Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(0, 0, 1 << 12), degreesToAngle(90)));
}

void renderer_loop(rendererObjectInstance *me) {
	glPushMatrix();
	
	Quaternion_normalize(&q);
	m4x4 m = Quaternion_toMatrix(q);
	glMultMatrix4x4(&m);
	
	// Center
	glTranslatef32(
		-(archvile.frames[0].min.x + (archvile.frames[0].max.x - archvile.frames[0].min.x) / 2.0f),
		-(archvile.frames[0].min.y + (archvile.frames[0].max.y - archvile.frames[0].min.y) / 2.0f),
		-(archvile.frames[0].min.z + (archvile.frames[0].max.z - archvile.frames[0].min.z) / 2.0f)
	);
	
	DSGM_RenderModelInstance(&archvileInstance);
	
	if(timer % 6 == 0) DSGM_UpdateModelInstanceAnimation(&archvileInstance);
	timer++;
	
	if(DSGM_held.Up) Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(0, 1 << 12, 0), -degreesToAngle(2)));
	if(DSGM_held.Down) Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(0, 1 << 12, 0), degreesToAngle(2)));
	if(DSGM_held.Left) Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(0, 0, 1 << 12), -degreesToAngle(2)));
	if(DSGM_held.Right) Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(0, 0, 1 << 12), degreesToAngle(2)));
	if(DSGM_held.L) Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(1 << 12, 0, 0), -degreesToAngle(2)));
	if(DSGM_held.R) Quaternion_multiply(&q, Quaternion_fromAxisAngle(vect(1 << 12, 0, 0), degreesToAngle(2)));
	
	if(DSGM_newpress.A) {
		light = !light;
		updateLighting();
	}
	
	glPopMatrix(1);
}
