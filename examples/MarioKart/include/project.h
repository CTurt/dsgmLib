#pragma once

#define DSGM_SOUND_STREAM_COUNT 0
#define DSGM_SOUND_EFFECT_COUNT 0
#define DSGM_SOUND_COUNT (DSGM_SOUND_STREAM_COUNT + DSGM_SOUND_EFFECT_COUNT)
#define DSGM_BACKGROUND_COUNT 2
#define DSGM_PALETTE_COUNT 1
#define DSGM_SPRITE_COUNT 2
#define DSGM_OBJECT_COUNT 3
#define DSGM_ROOM_COUNT 1

// Include backgrounds, palettes and sprites to be loaded from RAM
#include "MarioKart_Map_bin.h"
#include "MarioKart_Pal_bin.h"
#include "MarioKart_Tiles_bin.h"

#include "MarioKartLower_Map_bin.h"
#include "MarioKartLower_Pal_bin.h"
#include "MarioKartLower_Tiles_bin.h"

#include "DSGMPal0_Pal_bin.h"

#include "itemBox_Sprite_bin.h"
#include "banana_Sprite_bin.h"

#include "carBody_bin.h"
#include "carBody_Texture_bin.h"

#include "carWheels_bin.h"
#include "carWheels_Texture_bin.h"

#include "kart_tire_L_bin.h"
#include "kart_tire_L_Texture_bin.h"
#include "kart_tire_M_bin.h"
#include "kart_tire_M_Texture_bin.h"
#include "kart_tire_S_bin.h"
#include "kart_tire_S_Texture_bin.h"

#include "carEmblem_bin.h"
#include "carEmblem_Texture_bin.h"

// No sounds, no enum
//typedef enum {
//} DSGM_SoundNames;

typedef enum {
	MarioKart,
	MarioKartLower,
} DSGM_BackgroundNames;

typedef enum {
	DSGMPal0,
} DSGM_PaletteNames;

typedef enum {
	itemBoxSprite,
	bananaSprite,
} DSGM_SpriteNames;

typedef enum {
	renderer,
	itemBox,
	banana,
} DSGM_ObjectNames;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
	} *variables;
} rendererObjectInstance;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
	} *variables;
} itemBoxObjectInstance;

typedef struct {
	DSGM_ObjectInstanceBase;
	struct {
	} *variables;
} bananaObjectInstance;

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

void renderer_create(rendererObjectInstance *me);
void renderer_loop(rendererObjectInstance *me);
