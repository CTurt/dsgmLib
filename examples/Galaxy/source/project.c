#include "DSGM.h"

#include "DSGM_projectHelper.h"

// User variables / declarations
DSGM_Camera camera;

bool light = true;

DSGM_Model planetModel;
DSGM_Texture planetTexture = DSGM_FORM_RAM_TEXTURE(GL_RGB, TEXTURE_SIZE_32, TEXTURE_SIZE_32, grass_Texture_bin);

DSGM_Model marioModel;
DSGM_Texture marioTexture = DSGM_FORM_RAM_TEXTURE(GL_RGB, TEXTURE_SIZE_128, TEXTURE_SIZE_64, mario_Texture_bin);

DSGM_Model goombaModel;
DSGM_Texture goombaTexture = DSGM_FORM_RAM_TEXTURE(GL_RGB, TEXTURE_SIZE_32, TEXTURE_SIZE_32, goomba_Texture_bin);

enum marioModelAnimations {
	MARIO_ANIM_WAIT,
	MARIO_ANIM_WALK,
	MARIO_ANIM_RUN,
	MARIO_ANIM_JUMP,
	MARIO_ANIM_JUMP_END
};

enum goombaModelAnimations {
	GOOMBA_ANIM_WALK,
	GOOMBA_ANIM_RUN,
};

int yDistance = 0;
int yVelocity = 0;

#define PLANET_RADIUS 100
const int planetRadiusSqr = PLANET_RADIUS * PLANET_RADIUS;

void updateLighting(void) {
	if(light) {
		glMaterialf(GL_EMISSION, BIT(15) | RGB15(0, 0, 0));
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1);
	}
	else {
		glMaterialf(GL_EMISSION, BIT(15) | RGB15(31, 31, 31));
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_FRONT);
	}
}

void centerCamera(DSGM_Camera *camera, Quaternion q) {
	m4x4 initialMatrix, resultantMatrix;
	
	// camera->lookAt = vect(0, 0, 0)
	glPushMatrix();
	DSGM_GetPositionMatrix(&initialMatrix);
	DSGM_ApplyQuaternion(&q);
	glTranslatef32(0, (10 << 12),  ((((PLANET_RADIUS << 12)))));
	DSGM_GetPositionMatrix(&resultantMatrix);
	camera->lookAt = DSGM_MatrixTranslationToVector(initialMatrix, resultantMatrix);
	glPopMatrix(1);
	
	camera->up = Quaternion_toUpVector(&q);
	
	glPushMatrix();
	DSGM_GetPositionMatrix(&initialMatrix);
	DSGM_ApplyQuaternion(&q);
	glTranslatef32(0, -(10 << 12), ((((PLANET_RADIUS << 12) - (1 << 12))) * 2.0) + yDistance);
	DSGM_GetPositionMatrix(&resultantMatrix);
	camera->position = DSGM_MatrixTranslationToVector(initialMatrix, resultantMatrix);
	glPopMatrix(1);
}

// Resources
DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT] = {
	DSGM_FORM_SOUND_STREAM(GustyGarden),
};

DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT] = {
	DSGM_FORM_NITRO_BACKGROUND(galaxy, BgSize_T_256x256, BgType_Text8bpp),
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
	
	// mario
	{
		DSGM_NO_SPRITE,
		(DSGM_Event)mario_create,
		(DSGM_Event)mario_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((marioObjectInstance *)0)->variables)
	},
	
	// goomba
	{
		DSGM_NO_SPRITE,
		(DSGM_Event)goomba_create,
		(DSGM_Event)goomba_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((goombaObjectInstance *)0)->variables)
	},
	
	// planet
	{
		DSGM_NO_SPRITE,
		DSGM_NO_EVENT,
		(DSGM_Event)planet_loop,
		DSGM_NO_EVENT,
		DSGM_NO_EVENT,
		NULL, 0,
		
		sizeof(*((planetObjectInstance *)0)->variables)
	},
};

DSGM_Room DSGM_Rooms[DSGM_ROOM_COUNT] = {
	// menu
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
					&DSGM_Backgrounds[galaxy],	// Background
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
		
		menuHandler,
		
		// Object groups are dynamic, so must be set up at MARIO_ANIM_RUN time, see DSGM_SetupRooms.
		{
			NULL,
			NULL
		},
		{
			0,
			0
		}
	},
	
	// game
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
					&DSGM_Backgrounds[galaxy],// Background
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
					DSGM_NO_BACKGROUND,			// Background
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
		
		// Object groups are dynamic, so must be set up at MARIO_ANIM_RUN time, see DSGM_SetupRooms.
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

int DSGM_currentRoom = menu;

void DSGM_SetupRooms(int room) {
	if(room != DSGM_ALL_ROOMS) {
		switch(room) {
			case menu: goto menu; break;
			case game: goto game; break;
		}
	}
	
	menu:
	DSGM_Debug("menu reset\n");
	DSGM_LeaveRoom(&DSGM_Rooms[menu]);
	
	DSGM_SetupViews(&DSGM_Rooms[menu]);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[menu], DSGM_TOP, 0);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[game], DSGM_BOTTOM, 0);
	
	if(room != DSGM_ALL_ROOMS) return;
	
	game:
	DSGM_Debug("game reset\n");
	DSGM_LeaveRoom(&DSGM_Rooms[game]);
	
	DSGM_SetupViews(&DSGM_Rooms[game]);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[game], DSGM_TOP, 4);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[game].objectGroups[DSGM_TOP][0], &DSGM_Objects[renderer], DSGM_TOP, 1,
		0, 0
	);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[game].objectGroups[DSGM_TOP][1], &DSGM_Objects[mario], DSGM_TOP, 1,
		0, 0
	);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[game].objectGroups[DSGM_TOP][2], &DSGM_Objects[goomba], DSGM_TOP, 1,
		0, 0
	);
	
	DSGM_SetupObjectInstances(&DSGM_Rooms[game].objectGroups[DSGM_TOP][3], &DSGM_Objects[planet], DSGM_TOP, 1,
		0, 0
	);
	
	DSGM_SetupObjectGroups(&DSGM_Rooms[game], DSGM_BOTTOM, 0);
	
	if(room != DSGM_ALL_ROOMS) return;
}

void menuHandler(void) {
	DSGM_SetScreenColor(DSGM_BOTTOM, DSGM_WHITE);
	DSGM_SetTextColor(DSGM_BOTTOM, DSGM_BLACK);
	
	DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Super Mario Galaxy DS demo");
	DSGM_DrawText(DSGM_BOTTOM, 1, 2, "Models from Super Mario 64 DS");
	DSGM_DrawText(DSGM_BOTTOM, 1, 4, "D-Pad to move");
	DSGM_DrawText(DSGM_BOTTOM, 1, 6, "B to jump");
	DSGM_DrawText(DSGM_BOTTOM, 1, 8, "Y to run");
	DSGM_DrawText(DSGM_BOTTOM, 1, 10, "X to spawn goomba (max 5)");
	DSGM_DrawText(DSGM_BOTTOM, 1, 12, "A to toggle lighting");
	DSGM_DrawText(DSGM_BOTTOM, 1, 14, "Issues:");
	DSGM_DrawText(DSGM_BOTTOM, 1, 15, "- Poor music,");
	DSGM_DrawText(DSGM_BOTTOM, 1, 16, "- Camera always behind player,");
	DSGM_DrawText(DSGM_BOTTOM, 1, 17, "- Lighting flickers on NO$GBA,");
	DSGM_DrawText(DSGM_BOTTOM, 1, 18, "- Only 1 eye is textured,");
	DSGM_DrawText(DSGM_BOTTOM, 6, 22, "Press Start to enter");
	
	if(DSGM_newpress.Start) DSGM_SwitchRoom(game, false);
}

void renderer_create(rendererObjectInstance *me) {
	DSGM_SetTextColor(DSGM_BOTTOM, DSGM_BLACK);
	DSGM_DrawText(DSGM_BOTTOM, 1, 20, "Loading models and textures...");
	swiWaitForVBlank();
	
	DSGM_InitStandard3D();
	
	if(!DSGM_LoadModel("mario.md2", &marioModel)) {
		DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Failed to load model, does your card support NitroFS?");
		return;
	}
	
	if(!DSGM_LoadModel("goomba.md2", &goombaModel)) {
		DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Failed to load model, does your card support NitroFS?");
		return;
	}
	
	if(!DSGM_LoadModel("sphere.md2", &planetModel)) {
		DSGM_DrawText(DSGM_BOTTOM, 1, 1, "Failed to load model, does your card support NitroFS?");
		return;
	}
	
	DSGM_LoadTexture(&marioTexture);
	DSGM_LoadTexture(&goombaTexture);
	DSGM_LoadTexture(&planetTexture);
	
	vect3D light[2] = {
		normalize(vect(floattov10(-0.8f), floattov10(-0.9f), floattov10(0.4f))),
		normalize(vect(floattov10(0.8f), floattov10(0.9f), floattov10(-0.4f))),
	};
	
	glLight(0, RGB15(31, 31, 31), light[0].x, light[0].y, light[0].z);
	glLight(1, RGB15(31, 31, 31), light[1].x, light[1].y, light[1].z);
	
	updateLighting();
	
	DSGM_PlaySound(GustyGarden);
	
	DSGM_ClearText(DSGM_BOTTOM);
	
	glPushMatrix();
}

void renderer_loop(rendererObjectInstance *me) {
	if(DSGM_newpress.A) {
		light = !light;
		updateLighting();
	}
	
	if(DSGM_newpress.X) {
		if(DSGM_GetObjectGroup(DSGM_TOP, &DSGM_Objects[goomba])->objectInstanceCount < 5) {
			DSGM_CreateObjectInstance(DSGM_TOP, 0, 0, &DSGM_Objects[goomba]);
		}
	}
	
	glPopMatrix(1);
	glPushMatrix();
}

void mario_create(marioObjectInstance *me) {
	DSGM_InitModelInstance(&me->variables->modelInstance, &marioModel);
	DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_WAIT, false);
	me->variables->modelInstance.animationSpeed = 2;
	me->variables->modelInstance.interpolate = true;
	me->variables->modelInstance.fixTransformations = false;
	
	me->variables->q = Quaternion_identity();
	centerCamera(&camera, me->variables->q);
}

void mario_loop(marioObjectInstance *me) {
	// Move mario
	if(DSGM_held.Up) {
		Quaternion_multiply(&me->variables->q, Quaternion_fromAxisAngle(vect(1 << 12, 0, 0), -degreesToAngle(DSGM_held.Y + 1) / 2));
	}
	
	if(DSGM_held.Down) {
		Quaternion_multiply(&me->variables->q, Quaternion_fromAxisAngle(vect(1 << 12, 0, 0), degreesToAngle(DSGM_held.Y + 1) / 2));
	}
	
	if(DSGM_held.Left) {
		Quaternion_multiply(&me->variables->q, Quaternion_fromAxisAngle(vect(0, 0, 1 << 12), degreesToAngle(2)));
	}
	
	if(DSGM_held.Right) {
		Quaternion_multiply(&me->variables->q, Quaternion_fromAxisAngle(vect(0, 0, 1 << 12), -degreesToAngle(2)));
	}
	
	// Move camera
	DSGM_Camera absolute;
	centerCamera(&absolute, me->variables->q);
	
	camera.position.x += (absolute.position.x - camera.position.x) / 24;
	camera.position.y += (absolute.position.y - camera.position.y) / 24;
	camera.position.z += (absolute.position.z - camera.position.z) / 24;
	
	camera.lookAt.x += (absolute.lookAt.x - camera.lookAt.x) / 24;
	camera.lookAt.y += (absolute.lookAt.y - camera.lookAt.y) / 24;
	camera.lookAt.z += (absolute.lookAt.z - camera.lookAt.z) / 24;
	
	camera.up.x += (absolute.up.x - camera.up.x) / 24;
	camera.up.y += (absolute.up.y - camera.up.y) / 24;
	camera.up.z += (absolute.up.z - camera.up.z) / 24;
	
	DSGM_UseCamera(&camera);
	
	// Control mario physics and animation
	yDistance += yVelocity;
	
	if(yDistance >= 0) {
		yVelocity -= 650;
	}
	else {
		yDistance = 0;
		yVelocity = 0;
	}
	
	if(yDistance > 0) {
		if(!me->variables->modelInstance.oneshot) {
			me->variables->modelInstance.currentFrame = marioModel.animations[MARIO_ANIM_JUMP].end;
			me->variables->modelInstance.nextFrame = marioModel.animations[MARIO_ANIM_JUMP].end;
		}
	}
	else {
		if(me->variables->modelInstance.currentFrame == marioModel.animations[MARIO_ANIM_JUMP].end) {
			me->variables->modelInstance.oldAnim = MARIO_ANIM_WAIT;
			DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_JUMP_END, true);
		}
	}
	
	if(DSGM_newpress.B && yDistance == 0) {
		yVelocity = 15800;
		me->variables->modelInstance.oldAnim = MARIO_ANIM_JUMP;
		DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_JUMP, true);
	}
	
	if(yDistance == 0) {
		if(DSGM_NOT_HOLDING_DPAD()) {
			if(me->variables->modelInstance.currentAnim != MARIO_ANIM_JUMP_END) {
				DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_WAIT, false);
			}
		}
		else {
			if(me->variables->modelInstance.currentAnim == MARIO_ANIM_WAIT || me->variables->modelInstance.currentAnim == MARIO_ANIM_JUMP_END) {
				me->variables->modelInstance.oneshot = false;
				if(!DSGM_held.Y) DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_WALK, false);
				else DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_RUN, false);
			}
			
			if(DSGM_newpress.Y) {
				int n = me->variables->modelInstance.currentFrame - me->variables->modelInstance.model->animations[me->variables->modelInstance.currentAnim].start;
				DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_RUN, false);
				me->variables->modelInstance.nextFrame = n + me->variables->modelInstance.model->animations[me->variables->modelInstance.currentAnim].start;
				DSGM_UpdateModelInstanceAnimation(&me->variables->modelInstance);
			}
			
			if(DSGM_release.Y) {
				int n = me->variables->modelInstance.currentFrame - me->variables->modelInstance.model->animations[me->variables->modelInstance.currentAnim].start;
				DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, MARIO_ANIM_WALK, false);
				me->variables->modelInstance.nextFrame = n + me->variables->modelInstance.model->animations[me->variables->modelInstance.currentAnim].start;
				DSGM_UpdateModelInstanceAnimation(&me->variables->modelInstance);
			}
		}
	}
	
	// Draw mario
	glPushMatrix();
	DSGM_ApplyQuaternion(&me->variables->q);
	glTranslatef32(0, 0, ((PLANET_RADIUS << 12) - marioModel.frames[0].max.z) + yDistance);
	glRotatef32(-degreesToAngle(90), 1 << 12, 0, 0);
	glBindTexture(0, marioTexture.id);
	DSGM_ScaleTexture(1.0f / 16.0f, 1.0f / 16.0f);
	DSGM_RenderModelInstance(&me->variables->modelInstance);
	glPopMatrix(1);
	
	// Update animation
	DSGM_UpdateModelInstanceAnimation(&me->variables->modelInstance);
}

void goomba_create(goombaObjectInstance *me) {
	DSGM_InitModelInstance(&me->variables->modelInstance, &goombaModel);
	DSGM_SetModelInstanceAnimation(&me->variables->modelInstance, GOOMBA_ANIM_WALK, false);
	me->variables->modelInstance.animationSpeed = 2;
	me->variables->modelInstance.interpolate = false;
	
	me->variables->q = Quaternion_fromEuler(DSGM_Random(0, degreesToAngle(360)), DSGM_Random(0, degreesToAngle(360)), DSGM_Random(0, degreesToAngle(360)));
}

void goomba_loop(goombaObjectInstance *me) {
	// Move goomba
	Quaternion_multiply(&me->variables->q, Quaternion_fromAxisAngle(vect(1 << 12, 0, 0), -degreesToAngle(1) / 2));
	
	// Draw goomba
	glPushMatrix();
	DSGM_ApplyQuaternion(&me->variables->q);
	glTranslatef32(0, 0, ((PLANET_RADIUS << 12) - goombaModel.frames[0].max.z));
	glRotateXi(1 << 13);
	glBindTexture(0, goombaTexture.id);
	DSGM_ScaleTexture(1.0f / 32.0f, 1.0f / 32.0f);
	DSGM_RenderModelInstance(&me->variables->modelInstance);
	glPopMatrix(1);
	
	// Update animation
	DSGM_UpdateModelInstanceAnimation(&me->variables->modelInstance);
}

void planet_loop(planetObjectInstance *me) {
	// Draw planet
	glPushMatrix();
	glBindTexture(0, planetTexture.id);
	glScalef(PLANET_RADIUS * 2, PLANET_RADIUS * 2, PLANET_RADIUS * 2);
	glTranslatef32(-(1 << 11), 1 << 11, -(1 << 11));
	DSGM_RenderModelFrame(0, &planetModel);
	glPopMatrix(1);
}
