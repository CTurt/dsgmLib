#pragma once

#define DSGM_SOUND_STREAM_COUNT 1
#define DSGM_SOUND_EFFECT_COUNT 2
#define DSGM_SOUND_COUNT (DSGM_SOUND_STREAM_COUNT + DSGM_SOUND_EFFECT_COUNT)
#define DSGM_BACKGROUND_COUNT 3
#define DSGM_PALETTE_COUNT 1
#define DSGM_SPRITE_COUNT 2
#define DSGM_OBJECT_COUNT 1
#define DSGM_ROOM_COUNT 2

// Include backgrounds, palettes and sprites to be loaded from RAM


typedef enum {
	FlatOutLies,
	Ambulance,
	Boom,
} DSGM_SoundNames;

typedef enum {
	odale,
	cloud,
	space,
} DSGM_BackgroundNames;

typedef enum {
	DSGMPal0,
} DSGM_PaletteNames;

typedef enum {
	taptapman,
} DSGM_SpriteNames;

typedef enum {
	player,
} DSGM_ObjectNames;

typedef enum {
	Room_1,
	Room_2,
} DSGM_RoomNames;

extern DSGM_Sound DSGM_Sounds[DSGM_SOUND_COUNT];
extern DSGM_Background DSGM_Backgrounds[DSGM_BACKGROUND_COUNT];
extern DSGM_Palette DSGM_Palettes[DSGM_PALETTE_COUNT];
extern DSGM_Sprite DSGM_Sprites[DSGM_SPRITE_COUNT];
extern DSGM_Object DSGM_Objects[DSGM_OBJECT_COUNT];
extern DSGM_Room DSGM_Rooms[DSGM_ROOM_COUNT];

extern int DSGM_currentRoom;

void DSGM_SetupRooms(int room);

void player_create(DSGM_ObjectInstance *me);
void player_loop(DSGM_ObjectInstance *me);
void player_touch(DSGM_ObjectInstance *me);
