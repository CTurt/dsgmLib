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
int DSGM_nextFreeRotset[2] = { 0, 0 };

int DSGM_rotations[2][32];

const DSGM_Size DSGM_Sizes[3][4] = {{{8, 8}, {16, 16}, {32, 32}, {64, 64}}, {{16, 8}, {32, 8}, {32, 16}, {64, 32}}, {{8, 16}, {8, 32}, {16, 32}, {32, 64}}};

inline int DSGM_NextFreeSpriteNumber(u8 screen) {
	return DSGM_nextFreeSprite[screen]++;
}

inline int DSGM_NextFreeRotset(u8 screen) {
	return DSGM_nextFreeRotset[screen]++;
}

inline int DSGM_GetSpriteWidth(DSGM_Sprite *sprite) {
	u16 atr0 = SPRITE_SIZE_SHAPE(sprite->size);
	u16 atr1 = SPRITE_SIZE_SIZE(sprite->size);
	return DSGM_Sizes[atr0][atr1].width;
}

inline int DSGM_GetSpriteHeight(DSGM_Sprite *sprite) {
	u16 atr0 = SPRITE_SIZE_SHAPE(sprite->size);
	u16 atr1 = SPRITE_SIZE_SIZE(sprite->size);
	return DSGM_Sizes[atr0][atr1].width;
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
	}
	
	DSGM_nextFreeSprite[DSGM_TOP] = 0;
	DSGM_nextFreeSprite[DSGM_BOTTOM] = 0;
	DSGM_nextFreeRotset[DSGM_TOP] = 0;
	DSGM_nextFreeRotset[DSGM_BOTTOM] = 0;
	
	for(i = 0; i < 128; i++) {	
		//oamSet(&oamMain, i, 0, 0, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, NULL, -1, false, true, false, false, false);
		//oamSet(&oamSub, i, 0, 0, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, NULL, -1, false, true, false, false, false);
	}
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

void DSGM_SetSpriteXY(u8 screen, int spriteNumber, int x, int y) {
	switch(screen) {
		case DSGM_TOP:
			oamMain.oamMemory[spriteNumber].x = x;
			oamMain.oamMemory[spriteNumber].y = y;
			break;
			
		case DSGM_BOTTOM:
			oamSub.oamMemory[spriteNumber].x = x;
			oamSub.oamMemory[spriteNumber].y = y;
			break;
	}
}

void DSGM_SetSpriteFrame(u8 screen, int spriteNumber, DSGM_Sprite *sprite, u8 frame) {
	if(screen == DSGM_TOP) oamMain.oamMemory[spriteNumber].gfxIndex = oamGfxPtrToOffset(&oamMain, sprite->topTiles[frame]);
	if(screen == DSGM_BOTTOM) oamSub.oamMemory[spriteNumber].gfxIndex = oamGfxPtrToOffset(&oamSub, sprite->bottomTiles[frame]);
}

void DSGM_SetSpriteHFlip(u8 screen, int spriteNumber, bool flip) {
	if(screen == DSGM_TOP) oamMain.oamMemory[spriteNumber].hFlip = flip;
	if(screen == DSGM_BOTTOM) oamSub.oamMemory[spriteNumber].hFlip = flip;
}

void DSGM_SetSpriteVFlip(u8 screen, int spriteNumber, bool flip) {
	if(screen == DSGM_TOP) oamMain.oamMemory[spriteNumber].vFlip = flip;
	if(screen == DSGM_BOTTOM) oamSub.oamMemory[spriteNumber].vFlip = flip;
}

void DSGM_SetSpritePriority(u8 screen, int spriteNumber, ObjPriority priority) {
	if(screen == DSGM_TOP) oamMain.oamMemory[spriteNumber].priority = priority;
	if(screen == DSGM_BOTTOM) oamSub.oamMemory[spriteNumber].priority = priority;
}

void DSGM_SetRotsetRotation(u8 screen, int rotset, int angle) {
	oamRotateScale(screen == DSGM_TOP ? &oamMain : &oamSub, rotset, angle, intToFixed(1, 8), intToFixed(1, 8));
}
