#include "DSGM.h"

inline void DSGM_UnlockBackgroundPalette(u8 screen) {
	switch(screen) {
		case DSGM_TOP:
			vramSetBankE(VRAM_E_LCD);
			break;
			
		case DSGM_BOTTOM:
			vramSetBankH(VRAM_H_LCD);
			break;
	}
}

inline void DSGM_LockBackgroundPalette(u8 screen) {
	switch(screen) {
		case DSGM_TOP:
			vramSetBankE(VRAM_E_BG_EXT_PALETTE);
			break;
			
		case DSGM_BOTTOM:
			vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
			break;
	}
}

inline unsigned short *DSGM_GetBackgroundPalette(u8 screen, int layerNumber) {
	return (screen == DSGM_TOP ? VRAM_E_EXT_PALETTE : VRAM_H_EXT_PALETTE)[layerNumber][0];
}

inline void DSGM_SetScreenColor(u8 screen, u16 color) {
	switch(screen) {
		case DSGM_TOP:
			BG_PALETTE[0] = color;
			break;
			
		case DSGM_BOTTOM:
			BG_PALETTE_SUB[0] = color;
			break;
	}
}

inline bool DSGM_BackgroundIsNitroFull(DSGM_Background *background) {
	return !background->tiles;
}

void DSGM_LoadBackgroundFull(DSGM_Layer *layer) {
	BgType type = (layer->background->type == DSGM_LARGE_BACKGROUND) ? BgType_Text8bpp : layer->background->type;
	BgSize size = (layer->background->type == DSGM_LARGE_BACKGROUND) ? BgSize_T_512x512 : layer->background->size;
	
	switch(layer->screen) {
		case DSGM_TOP:
			layer->vramId = bgInit(layer->layerNumber, type, size, layer->mapBase, layer->tileBase);
			break;
			
		case DSGM_BOTTOM:
			layer->vramId = bgInitSub(layer->layerNumber, type, size, layer->mapBase, layer->tileBase);
			break;
	}
	
	bgHide(layer->vramId);
	
	if(DSGM_BackgroundIsNitroFull(layer->background)) {
		DSGM_LoadBackgroundNitroFull(layer);
	}
	else {
		DSGM_LoadBackgroundRAMFull(layer);
	}
	
	if(layer->background->type == DSGM_LARGE_BACKGROUND) {
		DSGM_LargeBackgroundCopyScreen(layer);
	}
	
	//DSGM_ScrollBackgroundFull(layer);
	
	swiWaitForVBlank();
	
	bgUpdate();
	bgShow(layer->vramId);
}

void DSGM_LoadBackgroundNitroFull(DSGM_Layer *layer) {
	void *map;
	size_t mapLength = DSGM_GetFileLength(layer->background->nitroMapFilename);
	
	if(layer->background->type == DSGM_LARGE_BACKGROUND) {
		DSGM_Debug("Allocating %d bytes for large background map\n", mapLength);
		layer->largeBackgroundMap = malloc(mapLength);
		DSGM_Debug("Gave %p\n", layer->largeBackgroundMap);
		map = layer->largeBackgroundMap;
	}
	else {
		map = bgGetMapPtr(layer->vramId);
	}
	
	layer->background->tilesCount = DSGM_ReadFileManual(bgGetGfxPtr(layer->vramId), 0, DSGM_AUTO_LENGTH, layer->background->nitroTilesFilename) / 64;
	if(layer->background->nitroMapFilename) DSGM_ReadFileManual(map, 0, DSGM_AUTO_LENGTH, layer->background->nitroMapFilename);
	DSGM_UnlockBackgroundPalette(layer->screen);
	switch(layer->screen) {
		case DSGM_TOP:
			DSGM_ReadFileManual(&VRAM_E_EXT_PALETTE[layer->layerNumber][0], 0, DSGM_AUTO_LENGTH, layer->background->nitroPaletteFilename);
			break;
			
		case DSGM_BOTTOM:
			DSGM_ReadFileManual(&VRAM_H_EXT_PALETTE[layer->layerNumber][0], 0, DSGM_AUTO_LENGTH, layer->background->nitroPaletteFilename);
			break;
	}
	DSGM_LockBackgroundPalette(layer->screen);
}

void DSGM_LoadBackgroundRAMFull(DSGM_Layer *layer) {
	void *map;
	size_t mapLength = *layer->background->mapLength;
	
	if(layer->background->type == DSGM_LARGE_BACKGROUND) {
		DSGM_Debug("Allocating %d bytes for large background map\n", mapLength);
		layer->largeBackgroundMap = malloc(mapLength);
		DSGM_Debug("Gave %p\n", layer->largeBackgroundMap);
		map = layer->largeBackgroundMap;
	}
	else {
		map = bgGetMapPtr(layer->vramId);
	}
	
	dmaCopy(layer->background->tiles, bgGetGfxPtr(layer->vramId), *layer->background->tilesLength);
	layer->background->tilesCount = (*layer->background->tilesLength) / 64;
	if(layer->background->map != NULL) dmaCopy(layer->background->map, map, *layer->background->mapLength);
	DSGM_UnlockBackgroundPalette(layer->screen);
	switch(layer->screen) {
		case DSGM_TOP:
			dmaCopy(layer->background->palette, &VRAM_E_EXT_PALETTE[layer->layerNumber][0], *layer->background->paletteLength);
			break;
			
		case DSGM_BOTTOM:
			dmaCopy(layer->background->palette, &VRAM_H_EXT_PALETTE[layer->layerNumber][0], *layer->background->paletteLength);
			break;
	}
	DSGM_LockBackgroundPalette(layer->screen);
}

void DSGM_ScrollBackgroundFull(DSGM_View *view, DSGM_Layer *layer) {
	if(layer != NULL/* && background != DSGM_DEFAULT_FONT*/) {
		if(layer->background != NULL) {
			bgSetScroll(layer->vramId, layer->x + (layer->attachedToView ? view->x : 0), layer->y + (layer->attachedToView ? view->y : 0));
		}
	}
}

inline unsigned int DSGM_GetLayerPriorityFull(DSGM_Layer *layer) {
	return (unsigned int)bgGetPriority(layer->vramId);
}

inline void DSGM_SetLayerPriorityFull(DSGM_Layer *layer, unsigned int priority) {
	bgSetPriority(layer->vramId, priority);
}

inline BgSize DSGM_GetBGSize(u8 screen, int layerNumber) {
	return bgState[layerNumber + ((screen == DSGM_BOTTOM) * 4)].size;
}

int DSGM_GetBGWidth(u8 screen, int layerNumber) {
	switch(DSGM_GetBGSize(screen, layerNumber)) {
		case BgSize_R_128x128:
		case BgSize_ER_128x128:
		case BgSize_B8_128x128:
		case BgSize_B16_128x128:
			return 128;
			break;
			
		case BgSize_T_256x256:
		case BgSize_T_256x512:
		case BgSize_R_256x256:
		case BgSize_ER_256x256:
		case BgSize_B8_256x256:
		case BgSize_B16_256x256:
			return 256;
			break;
			
		case BgSize_T_512x256:
		case BgSize_T_512x512:
		case BgSize_R_512x512:
		case BgSize_ER_512x512:
		case BgSize_B8_512x512:
		case BgSize_B8_512x256:
		case BgSize_B8_512x1024:
		case BgSize_B16_512x512:
		case BgSize_B16_512x256:
			return 512;
			break;
			
		case BgSize_R_1024x1024:
		case BgSize_ER_1024x1024:
		case BgSize_B8_1024x512:
			return 1024;
			break;
			
		default:
			break;
	}
	
	return 256;
}

int DSGM_GetBGHeight(u8 screen, int layerNumber) {
	switch(DSGM_GetBGSize(screen, layerNumber)) {
		case BgSize_R_128x128:
		case BgSize_ER_128x128:
		case BgSize_B8_128x128:
		case BgSize_B16_128x128:
			return 128;
			break;
			
		case BgSize_T_256x256:
		case BgSize_T_512x256:
		case BgSize_R_256x256:
		case BgSize_ER_256x256:
		case BgSize_B8_256x256:
		case BgSize_B8_512x256:
		case BgSize_B16_256x256:
		case BgSize_B16_512x256:
			return 256;
			break;
			
		case BgSize_T_256x512:
		case BgSize_T_512x512:
		case BgSize_R_512x512:
		case BgSize_ER_512x512:
		case BgSize_B8_512x512:
		case BgSize_B8_1024x512:
		case BgSize_B16_512x512:
			return 512;
			break;
			
		case BgSize_R_1024x1024:
		case BgSize_ER_1024x1024:
		case BgSize_B8_512x1024:
			return 1024;
			break;
			
		default:
			break;
	}
	
	return 256;
}

inline u16 DSGM_GetTileFull(DSGM_Layer *layer, int x, int y) {
	u16 *map;
	if(layer->background->type == DSGM_LARGE_BACKGROUND) {
		map = layer->largeBackgroundMap;
	}
	else {
		map = bgGetMapPtr(layer->vramId);
	}
	
	// todo: optimise with div and modulus rather than loop
	while(y > 31) {
		y -= 32;
		x += 64;
	}
	while(x > 31) {
		x -= 32;
		y += 32;
	}
	
	// DSGM_GetBGWidth(layer->screen, layer->layerNumber)
	return map[y * 32 + x];
}

inline void DSGM_SetTileFull(DSGM_Layer *layer, int x, int y, u16 tile) {
	u16 *map;
	if(layer->background->type == DSGM_LARGE_BACKGROUND) {
		map = layer->largeBackgroundMap;
	}
	else {
		map = bgGetMapPtr(layer->vramId);
	}
	
	while(y > 31) {
		y -= 32;
		x += 64;
	}
	while(x > 31) {
		x -= 32;
		y += 32;
	}
	
	map[y * 32 + x] = tile;
}

inline void DSGM_SetTileForceVRAM(DSGM_Layer *layer, int x, int y, u16 tile) {
	u16 *map = bgGetMapPtr(layer->vramId);
	
	if(layer->background->type == DSGM_LARGE_BACKGROUND) {
		x %= 512 / 8;
		y %= 512 / 8;
	}
	
	while(y > 31) {
		y -= 32;
		x += 64;
	}
	while(x > 31) {
		x -= 32;
		y += 32;
	}
	
	map[y * DSGM_GetBGWidth(layer->screen, layer->layerNumber) / 16 + x] = tile;
}

inline void DSGM_SetRotationCenterFull(DSGM_Layer *layer, int x, int y) {
	bgSetCenter(layer->vramId, x, y);
}

inline void DSGM_RotateBackgroundFull(DSGM_Layer *layer, int angle) {
	bgSetRotate(layer->vramId, angle);
}

inline void DSGM_ScaleBackgroundFull(DSGM_Layer *layer, int x, int y) {
	bgSetScale(layer->vramId, x, y);
}
