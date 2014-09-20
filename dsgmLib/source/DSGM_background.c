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
	switch(layer->screen) {
		case DSGM_TOP:
			layer->vramId = bgInit(layer->layerNumber, layer->background->type, layer->background->size, layer->mapBase, layer->tileBase);
			break;
			
		case DSGM_BOTTOM:
			layer->vramId = bgInitSub(layer->layerNumber, layer->background->type, layer->background->size, layer->mapBase, layer->tileBase);
			break;
	}
	
	bgHide(layer->vramId);
	
	if(DSGM_BackgroundIsNitroFull(layer->background)) {
		DSGM_LoadBackgroundNitroFull(layer);
	}
	else {
		DSGM_LoadBackgroundRAMFull(layer);
	}
	
	//DSGM_ScrollBackgroundFull(layer);
	
	bgUpdate();
	bgShow(layer->vramId);
}

void DSGM_LoadBackgroundNitroFull(DSGM_Layer *layer) {
	layer->background->tilesCount = DSGM_ReadFileManual(bgGetGfxPtr(layer->vramId), 0, DSGM_AUTO_LENGTH, layer->background->nitroTilesFilename) / 64;
	if(layer->background->nitroMapFilename) DSGM_ReadFileManual(bgGetMapPtr(layer->vramId), 0, DSGM_AUTO_LENGTH, layer->background->nitroMapFilename);
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
	dmaCopy(layer->background->tiles, bgGetGfxPtr(layer->vramId), *layer->background->tilesLength);
	layer->background->tilesCount = (*layer->background->tilesLength) / 64;
	if(layer->background->map != NULL) dmaCopy(layer->background->map, bgGetMapPtr(layer->vramId), *layer->background->mapLength);
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
			if(layer->vramId != 0) {
				bgSetScroll(layer->vramId, layer->x - layer->attachedToView ? view->x : 0, layer->y - layer->attachedToView ? view->y : 0);
			}
		}
	}
}

inline BgSize DSGM_GetBGSize(u8 screen, int layerNumber) {
	return bgState[layerNumber + ((screen == DSGM_BOTTOM) * 4)].size;
}

int DSGM_GetBGWidth(u8 screen, int layerNumber) {
	switch(DSGM_GetBGSize(screen, layerNumber)) {
		case BgSize_T_256x256:
		case BgSize_T_256x512:
			return 256;
			break;
			
		case BgSize_T_512x256:
		case BgSize_T_512x512:
			return 512;
			break;
			
		default:
			break;
	}
	return 512;
}

int DSGM_GetBGHeight(u8 screen, int layerNumber) {
	switch(DSGM_GetBGSize(screen, layerNumber)) {
		case BgSize_T_256x256:
		case BgSize_T_512x256:
			return 256;
			break;
			
		case BgSize_T_256x512:
		case BgSize_T_512x512:
			return 512;
			break;
			
		default:
			break;
	}
	return 512;
}

inline u16 DSGM_GetTile(DSGM_Layer *layer, int x, int y) {
	// todo: optimise with div and modulus rather than loop
	while(y > 31) {
		y -= 32;
		x += 64;
	}
	//x += 64 * ((y - 31) / 32);
	//y %= 32;
	while(x > 31) {
		x -= 32;
		y += 32;
	}
	//y += 64 * ((y - 31) / 32);
	//x %= 32;
	//int t = y * DSGM_GetBGWidth(screen, layerNumber) / 16 + x;
	//return bgGetMapPtr(background->vramId)[y * DSGM_GetBGWidth(screen, layerNumber) / 16 + x];
	return bgGetMapPtr(layer->vramId)[y * DSGM_GetBGWidth(layer->screen, layer->layerNumber) / 16 + x];
}

inline void DSGM_SetTile(DSGM_Layer *layer, int x, int y, u16 tile) {
	while(y > 31) {
		y -= 32;
		x += 64;
	}
	while(x > 31) {
		x -= 32;
		y += 32;
	}
	bgGetMapPtr(layer->vramId)[y * DSGM_GetBGWidth(layer->screen, layer->layerNumber) / 16 + x] = tile;
}
