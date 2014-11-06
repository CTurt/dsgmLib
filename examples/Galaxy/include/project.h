#pragma once

#define DSGM_SOUND_STREAM_COUNT 1
#define DSGM_SOUND_EFFECT_COUNT 0
#define DSGM_SOUND_COUNT (DSGM_SOUND_STREAM_COUNT + DSGM_SOUND_EFFECT_COUNT)
#define DSGM_BACKGROUND_COUNT 3
#define DSGM_PALETTE_COUNT 1
#define DSGM_SPRITE_COUNT 0
#define DSGM_OBJECT_COUNT 4
#define DSGM_ROOM_COUNT 2

// Include backgrounds, palettes and sprites to be loaded from RAM
#include "mario_Texture_bin.h"
#include "goomba_Texture_bin.h"
#include "grass_Texture_bin.h"

typedef enum {
	GustyGarden,
} DSGM_SoundNames;

typedef enum {
	galaxy,
} DSGM_BackgroundNames;

// No palettes, no enum
//typedef enum {
//} DSGM_PaletteNames;

// No sprites, no enum
//typedef enum {
//} DSGM_SpriteNames;

typedef enum {
	renderer,
	mario,
	goomba,
	planet,
} DSGM_ObjectNames;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
	} *variables;
} rendererObjectInstance;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
		Quaternion q;
		DSGM_ModelInstance modelInstance;
	} *variables;
} marioObjectInstance;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
		Quaternion q;
		DSGM_ModelInstance modelInstance;
	} *variables;
} goombaObjectInstance;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
	} *variables;
} planetObjectInstance;

typedef enum {
	menu,
	game,
} DSGM_RoomNames;

extern DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT];
extern DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT];
extern DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT];
extern DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT];
extern DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT];
extern DSGM_Room DSGM_Rooms[DSGM_ROOM_COUNT];

extern int DSGM_currentRoom;

void DSGM_SetupRooms(int room);

void menuHandler(void);

void renderer_create(rendererObjectInstance *me);
void renderer_loop(rendererObjectInstance *me);

void mario_create(marioObjectInstance *me);
void mario_loop(marioObjectInstance *me);

void goomba_create(goombaObjectInstance *me);
void goomba_loop(goombaObjectInstance *me);

void planet_loop(planetObjectInstance *me);
