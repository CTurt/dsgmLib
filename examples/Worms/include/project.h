#pragma once

#define DSGM_SOUND_STREAM_COUNT 0
#define DSGM_SOUND_EFFECT_COUNT 0
#define DSGM_SOUND_COUNT (DSGM_SOUND_STREAM_COUNT + DSGM_SOUND_EFFECT_COUNT)
#define DSGM_BACKGROUND_COUNT 2
#define DSGM_PALETTE_COUNT 1
#define DSGM_SPRITE_COUNT 1
#define DSGM_OBJECT_COUNT 2
#define DSGM_ROOM_COUNT 1

// Include backgrounds, palettes and sprites to be loaded from RAM
#include "ground_Bitmap_bin.h"

// No sounds, no enum
//typedef enum {
//} DSGM_SoundNames;

typedef enum {
	ComicSans,
	worms,
} DSGM_BackgroundNames;

typedef enum {
	DSGMPal0,
} DSGM_PaletteNames;

typedef enum {
	grenadeSprite,
} DSGM_SpriteNames;

typedef enum {
	terrain,
	grenade,
} DSGM_ObjectNames;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
	} *variables;
} terrainObjectInstance;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
		int xVelocity;
		int yVelocity;
		unsigned char fuse;
	} *variables;
} grenadeObjectInstance;

typedef enum {
	Room_1,
} DSGM_RoomNames;

extern DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT];
extern DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT];
extern DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT];
extern DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT];
extern DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT];
extern DSGM_Room DSGM_Rooms[DSGM_ROOM_COUNT];

extern int DSGM_currentRoom;

void DSGM_SetupRooms(int room);

void terrain_create(terrainObjectInstance *me);
void terrain_loop(terrainObjectInstance *me);

void grenade_create(grenadeObjectInstance *me);
void grenade_loop(grenadeObjectInstance *me);
