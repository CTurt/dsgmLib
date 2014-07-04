#pragma once

#define DSGM_NO_SPRITE NULL

#define DSGM_FORM_RAM_SPRITE(sprite, palette, size, frames)\
{ (u8 *)&sprite##_Sprite_bin, (u32 *)&sprite##_Sprite_bin_size, NULL, &DSGM_Palettes[palette], size, frames, NULL }

#define DSGM_FORM_NITRO_SPRITE(sprite, palette, size, frames)\
{ NULL, 0, #sprite "_Sprite.bin", &DSGM_Palettes[palette], size, frames, NULL }

typedef struct {
	u8 *tiles;
	u32 *tilesLength;
	char *nitroFilename;
	DSGM_Palette *palette;
	SpriteSize size;
	u8 frames;
	
	u16 **topTiles;
	u16 **bottomTiles;
} DSGM_Sprite;

typedef struct {
	u8 width;
	u8 height;
} DSGM_Size;

extern int DSGM_nextFreeSprite[2];
extern int DSGM_nextFreeRotset[2];

extern int DSGM_rotations[2][32];

extern const DSGM_Size DSGM_Sizes[3][4];

inline int DSGM_NextFreeSpriteNumber(u8 screen);
inline int DSGM_NextFreeRotset(u8 screen);

inline int DSGM_GetSpriteWidth(DSGM_Sprite *sprite);
inline int DSGM_GetSpriteHeight(DSGM_Sprite *sprite);

inline bool DSGM_SpriteIsNitro(DSGM_Sprite *sprite);
inline bool DSGM_SpriteLoaded(u8 screen, DSGM_Sprite *sprite);

void DSGM_ResetSprites(DSGM_Sprite *sprites, int spriteCount);

void DSGM_LoadSpriteFull(u8 screen, DSGM_Sprite *sprite);

void DSGM_CreateSprite(u8 screen, int spriteNumber, int x, int y, ObjPriority priority, int frame, bool hFlip, bool vFlip, DSGM_Sprite *sprite);
//void DSGM_CreateSprite(DSGM_ObjectInstance *instance);

void DSGM_SetSpriteXY(u8 screen, int spriteNumber, int x, int y);
void DSGM_SetSpriteFrame(u8 screen, int spriteNumber, DSGM_Sprite *sprite, u8 frame);
void DSGM_SetSpriteHFlip(u8 screen, int spriteNumber, bool flip);
void DSGM_SetSpriteVFlip(u8 screen, int spriteNumber, bool flip);
void DSGM_SetSpritePriority(u8 screen, int spriteNumber, ObjPriority priority);
void DSGM_SetRotsetRotation(u8 screen, int rotset, int angle);
