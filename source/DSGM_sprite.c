#include "DSGM.h"

/*
	oam->oamMemory[id].shape = SPRITE_SIZE_SHAPE(size);
	oam->oamMemory[id].size = SPRITE_SIZE_SIZE(size);
	oam->oamMemory[id].x = x;
	oam->oamMemory[id].y = y;
	oam->oamMemory[id].palette = palette_alpha;
	oam->oamMemory[id].priority = priority;
	oam->oamMemory[id].hFlip = hflip;
	oam->oamMemory[id].vFlip = vflip;
	oam->oamMemory[id].isMosaic = mosaic;
		oam->oamMemory[id].gfxIndex = oamGfxPtrToOffset(oam, gfxOffset);
*/

int DSGM_nextFreeSprite[2] = { 0, 0 };
int DSGM_rotsetTracker[2][32];

int DSGM_rotations[2][32] = { { 0 }, { 0 } };
DSGM_Scale DSGM_scales[2][32] = { { [0 ... 31] = { 1 << 8, 1 << 8 } }, { [0 ... 31] = { 1 << 8, 1 << 8 } } };

const DSGM_Size DSGM_Sizes[3][4] = {{{8, 8}, {16, 16}, {32, 32}, {64, 64}}, {{16, 8}, {32, 8}, {32, 16}, {64, 32}}, {{8, 16}, {8, 32}, {16, 32}, {32, 64}}};

inline int DSGM_NextFreeSpriteNumber(u8 screen) {
	return DSGM_nextFreeSprite[screen]++;
}

inline int DSGM_NextFreeRotset(u8 screen) {
	int i;
	for(i = 0; i < 32; i++) {
		if(DSGM_rotsetTracker[screen][i] == 0) {
			return i;
		}
	}
	return 0;
}

inline int DSGM_GetSpriteWidth(DSGM_Sprite *sprite) {
	switch(sprite->size) {
		case SpriteSize_8x8: return 8;
		case SpriteSize_16x16: return 16;
		case SpriteSize_32x32: return 32;
		case SpriteSize_64x64: return 64;
		case SpriteSize_16x8: return 16;
		case SpriteSize_32x8: return 32;
		case SpriteSize_32x16: return 32;
		case SpriteSize_64x32: return 64;
		case SpriteSize_8x16: return 8;
		case SpriteSize_8x32: return 8;
		case SpriteSize_16x32: return 16;
		case SpriteSize_32x64: return 32;
		default: return 0;
	}
	//u16 atr0 = SPRITE_SIZE_SHAPE(sprite->size);
	//u16 atr1 = SPRITE_SIZE_SIZE(sprite->size);
	//return DSGM_Sizes[atr0][atr1].width;
}

inline int DSGM_GetSpriteHeight(DSGM_Sprite *sprite) {
	switch(sprite->size) {
		case SpriteSize_8x8: return 8;
		case SpriteSize_16x16: return 16;
		case SpriteSize_32x32: return 32;
		case SpriteSize_64x64: return 64;
		case SpriteSize_16x8: return 8;
		case SpriteSize_32x8: return 8;
		case SpriteSize_32x16: return 16;
		case SpriteSize_64x32: return 32;
		case SpriteSize_8x16: return 16;
		case SpriteSize_8x32: return 32;
		case SpriteSize_16x32: return 32;
		case SpriteSize_32x64: return 64;
		default: return 0;
	}
	//u16 atr0 = SPRITE_SIZE_SHAPE(sprite->size);
	//u16 atr1 = SPRITE_SIZE_SIZE(sprite->size);
	//return DSGM_Sizes[atr0][atr1].height;
}

inline bool DSGM_SpriteIsNitro(DSGM_Sprite *sprite) {
	return !sprite->tiles;
}

inline bool DSGM_SpriteLoaded(u8 screen, DSGM_Sprite *sprite) {
	if(screen == DSGM_TOP) return sprite->topTiles != NULL;
	else return sprite->bottomTiles != NULL;
}

void DSGM_ResetSprites(DSGM_Sprite *sprites, int spriteCount) {
	int i, j;
	for(i = 0; i < spriteCount; i++) {
		if(sprites[i].topTiles) {
			for(j = 0; j < sprites[i].frames; j++) {
				oamFreeGfx(&oamMain, sprites[i].topTiles[j]);
				sprites[i].topTiles[j] = NULL;
			}
			free(sprites[i].topTiles);
			sprites[i].topTiles = NULL;
		}
		
		if(sprites[i].bottomTiles) {
			for(j = 0; j < sprites[i].frames; j++) {
				oamFreeGfx(&oamSub, sprites[i].bottomTiles[j]);
				sprites[i].bottomTiles[j] = NULL;
			}
			free(sprites[i].bottomTiles);
			sprites[i].bottomTiles = NULL;
		}
	}
	
	for(i = 0; i < 32; i++) {
		DSGM_rotations[DSGM_TOP][i] = 0;
		DSGM_rotations[DSGM_BOTTOM][i] = 0;
		DSGM_scales[DSGM_TOP][i].x = 1 << 8;
		DSGM_scales[DSGM_TOP][i].y = 1 << 8;
		DSGM_scales[DSGM_BOTTOM][i].x = 1 << 8;
		DSGM_scales[DSGM_BOTTOM][i].y = 1 << 8;
	}
	
	DSGM_nextFreeSprite[DSGM_TOP] = 0;
	DSGM_nextFreeSprite[DSGM_BOTTOM] = 0;
	memset(&DSGM_rotsetTracker, 0, sizeof(DSGM_rotsetTracker));
	//DSGM_nextFreeRotset[DSGM_TOP] = 0;
	//DSGM_nextFreeRotset[DSGM_BOTTOM] = 0;
	
	oamClear(&oamMain, 0, 0);
	oamClear(&oamSub, 0, 0);
	//oamUpdate(&oamMain);
	//oamUpdate(&oamSub);
}

void DSGM_LoadSpriteFull(u8 screen, DSGM_Sprite *sprite) {
	int i;
	switch(screen) {
		case DSGM_TOP:
			//sprite->topTiles = DSGM_TrackedAlloc((void **)&sprite->topTiles, sizeof(u16 *) * sprite->frames);
			sprite->topTiles = malloc(sizeof(u16 *) * sprite->frames);
			for(i = 0; i < sprite->frames; i++) {
				sprite->topTiles[i] = oamAllocateGfx(&oamMain, sprite->size, SpriteColorFormat_256Color);
				if(DSGM_SpriteIsNitro(sprite)) {
					DSGM_ReadFileManual(sprite->topTiles[i], i * (DSGM_GetSpriteWidth(sprite) * DSGM_GetSpriteHeight(sprite)), DSGM_GetSpriteWidth(sprite) * DSGM_GetSpriteHeight(sprite)/*32 * 32*//*DSGM_AUTO_LENGTH*/, sprite->nitroFilename);
				}
				else {
					dmaCopy(sprite->tiles + (i * DSGM_GetSpriteWidth(sprite) * DSGM_GetSpriteHeight(sprite)), sprite->topTiles[i], *sprite->tilesLength);
				}
			}
			break;
		
		case DSGM_BOTTOM:
			//sprite->bottomTiles = DSGM_TrackedAlloc((void **)&sprite->bottomTiles, sizeof(u16 *) * sprite->frames);
			sprite->bottomTiles = malloc(sizeof(u16 *) * sprite->frames);
			for(i = 0; i < sprite->frames; i++) {
				sprite->bottomTiles[i] = oamAllocateGfx(&oamSub, sprite->size, SpriteColorFormat_256Color);
				if(DSGM_SpriteIsNitro(sprite)) {
					DSGM_ReadFileManual(sprite->bottomTiles[i], i * (DSGM_GetSpriteWidth(sprite) * DSGM_GetSpriteHeight(sprite)), DSGM_GetSpriteWidth(sprite) * DSGM_GetSpriteHeight(sprite)/*DSGM_AUTO_LENGTH*/, sprite->nitroFilename);
				}
				else {
					dmaCopy(sprite->tiles + (i * DSGM_GetSpriteWidth(sprite) * DSGM_GetSpriteHeight(sprite)), sprite->bottomTiles[i], *sprite->tilesLength);
				}
			}
			break;
	}
}

void DSGM_CreateSprite(u8 screen, int spriteNumber, int x, int y, ObjPriority priority, int frame, bool hFlip, bool vFlip, DSGM_Sprite *sprite) {
	//oamSet(screen == DSGM_TOP ? &oamMain : &oamSub, spriteNumber, 255, 191, 0, sprite->palette->paletteNumber[screen], sprite->size, SpriteColorFormat_256Color, screen == DSGM_TOP ? sprite->topTiles[0] : sprite->bottomTiles[0], -1, false, false, false, false, false);
	//swiWaitForVBlank();
	//oamUpdate(screen == DSGM_TOP ? &oamMain : &oamSub);
	//DSGM_SetSpriteXY(screen, spriteNumber, x, y);
	oamSet(screen == DSGM_TOP ? &oamMain : &oamSub, spriteNumber, x, y, priority, sprite->palette->paletteNumber[screen], sprite->size, SpriteColorFormat_256Color, screen == DSGM_TOP ? sprite->topTiles[frame] : sprite->bottomTiles[frame], -1, false, false, hFlip, vFlip, false);
	oamUpdate(screen == DSGM_TOP ? &oamMain : &oamSub);
}

void DSGM_SetRotset(u8 screen, int rotset, int angle, int scaleX, int scaleY) {
	oamRotateScale(screen == DSGM_TOP ? &oamMain : &oamSub, rotset, angle, scaleX, scaleY);
}

/*u16 DSGM_GetSpriteColor(DSGM_Sprite *sprite, int frame, int x, int y) {
	int tileX = x / 8;
	int tileY = y / 8;
	int pmx = x - tileX * 8;
	int pmy = y - tileY * 8;
	if(sprite->topTiles) {
		u16 *gfx = sprite->topTiles[frame];
		
		int pixel = 0;
		for(pixel = 0; pixel < 32; pixel++) {
			unsigned short word = gfx[((tileX + tileY * DSGM_GetSpriteWidth(sprite) / 8) << 5) + pixel];
			
			int px = ((tile & 3) << 3) + ((pixel << 1) & 7);
			int py = ((tile >> 2) << 3) + (pixel >> 2);
			
			if((word & 0xF0) >> 4 != 0) {
				//if(px == pmx && py == pmy) DSGM_DrawFilledRectangleToBackgroundFull(room, screen, x + ((px + 1) % 8) * size, y + (py % 8) * size, size, size, color);
			}
			
			if((word & 0x0F) != 0) {
				//if(px == pmx && pmy) DSGM_DrawFilledRectangleToBackgroundFull(room, screen, x + (px % 8) * size, y + (py % 8) * size, size, size, color);
			}
		}
	}
	else if(sprite->bottomTiles) {
		u16 *gfx = &sprite->bottomTiles[frame][tileX + tileY * DSGM_GetSpriteWidth(sprite) / 8];
	}
	else {
		DSGM_LoadSpriteFull(DSGM_TOP, sprite);
		return DSGM_GetSpriteColor(sprite, frame, x, y);
	}
}*/
