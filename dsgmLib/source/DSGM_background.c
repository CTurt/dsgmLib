#include "DSGM.h"

void DSGM_UnlockBackgroundPalette(u8 screen) {
	switch(screen) {
		case DSGM_TOP:
			vramSetBankE(VRAM_E_LCD);
			break;
			
		case DSGM_BOTTOM:
			vramSetBankH(VRAM_H_LCD);
			break;
	}
}

void DSGM_LockBackgroundPalette(u8 screen) {
	switch(screen) {
		case DSGM_TOP:
			vramSetBankE(VRAM_E_BG_EXT_PALETTE);
			break;
			
		case DSGM_BOTTOM:
			vramSetBankH(VRAM_H_SUB_BG_EXT_PALETTE);
			break;
	}
}

inline bool DSGM_BackgroundIsNitroFull(DSGM_Background *background) {
	return !background->tiles;
}

void DSGM_InitDrawableBackground(DSGM_BackgroundInstance *backgroundInstance) {
	DSGM_Debug("Drawable background screen %d, layer %d", backgroundInstance->screen, backgroundInstance->layer);
	switch(backgroundInstance->screen) {
		case DSGM_TOP:
			backgroundInstance->vramId = bgInit(backgroundInstance->layer, BgType_Bmp8, BgSize_B8_256x256, backgroundInstance->mapBase, backgroundInstance->tileBase);
			break;
			
		case DSGM_BOTTOM:
			backgroundInstance->vramId = bgInitSub(backgroundInstance->layer, BgType_Bmp8, BgSize_B8_256x256, backgroundInstance->mapBase, backgroundInstance->tileBase);
			break;
	}
}

void DSGM_LoadBackgroundFull(DSGM_BackgroundInstance *backgroundInstance) {
	switch(backgroundInstance->screen) {
		case DSGM_TOP:
			backgroundInstance->vramId = bgInit(backgroundInstance->layer, backgroundInstance->background->type, backgroundInstance->background->size, backgroundInstance->mapBase, backgroundInstance->tileBase);
			break;
			
		case DSGM_BOTTOM:
			backgroundInstance->vramId = bgInitSub(backgroundInstance->layer, backgroundInstance->background->type, backgroundInstance->background->size, backgroundInstance->mapBase, backgroundInstance->tileBase);
			break;
	}
	
	bgHide(backgroundInstance->vramId);
	
	if(DSGM_BackgroundIsNitroFull(backgroundInstance->background)) {
		DSGM_LoadBackgroundNitroFull(backgroundInstance);
	}
	else {
		DSGM_LoadBackgroundRAMFull(backgroundInstance);
	}
	
	//DSGM_ScrollBackgroundFull(backgroundInstance);
	
	bgUpdate();
	bgShow(backgroundInstance->vramId);
}

void DSGM_LoadBackgroundNitroFull(DSGM_BackgroundInstance *backgroundInstance) {
	backgroundInstance->tilesCount = DSGM_ReadFileManual(bgGetGfxPtr(backgroundInstance->vramId), 0, DSGM_AUTO_LENGTH, backgroundInstance->background->nitroTilesFilename) / 64;
	DSGM_ReadFileManual(bgGetMapPtr(backgroundInstance->vramId), 0, DSGM_AUTO_LENGTH, backgroundInstance->background->nitroMapFilename);
	DSGM_UnlockBackgroundPalette(backgroundInstance->screen);
	switch(backgroundInstance->screen) {
		case DSGM_TOP:
			DSGM_ReadFileManual(&VRAM_E_EXT_PALETTE[backgroundInstance->layer][0], 0, DSGM_AUTO_LENGTH, backgroundInstance->background->nitroPaletteFilename);
			break;
			
		case DSGM_BOTTOM:
			DSGM_ReadFileManual(&VRAM_H_EXT_PALETTE[backgroundInstance->layer][0], 0, DSGM_AUTO_LENGTH, backgroundInstance->background->nitroPaletteFilename);
			break;
	}
	DSGM_LockBackgroundPalette(backgroundInstance->screen);
}

void DSGM_LoadBackgroundRAMFull(DSGM_BackgroundInstance *backgroundInstance) {
	dmaCopy(backgroundInstance->background->tiles, bgGetGfxPtr(backgroundInstance->vramId), *backgroundInstance->background->tilesLength);
	backgroundInstance->tileCount = (*backgroundInstance->background->tilesLength) / 64;
	dmaCopy(backgroundInstance->background->map, bgGetMapPtr(backgroundInstance->vramId), *backgroundInstance->background->mapLength);
	DSGM_UnlockBackgroundPalette(backgroundInstance->screen);
	switch(backgroundInstance->screen) {
		case DSGM_TOP:
			dmaCopy(backgroundInstance->background->palette, &VRAM_E_EXT_PALETTE[backgroundInstance->layer][0], *backgroundInstance->background->paletteLength);
			break;
			
		case DSGM_BOTTOM:
			dmaCopy(backgroundInstance->background->palette, &VRAM_H_EXT_PALETTE[backgroundInstance->layer][0], *backgroundInstance->background->paletteLength);
			break;
	}
	DSGM_LockBackgroundPalette(backgroundInstance->screen);
}

void DSGM_ScrollBackgroundFull(DSGM_View *view, DSGM_BackgroundInstance *background) {
	//DSGM_Debug("Scrolling to Y: %d\n", background->y - background->attachedToView ? view->y : 0);
	//DSGM_Debug("DScrolling to Y: %d\n", view[DSGM_BOTTOM].y);
	//DSGM_Debug("Scrolling bg s %d, l %d\n", background->screen, background->layer);
	if(background != NULL/* && background != DSGM_TEXT_BACKGROUND*/) {
		if(background->background != NULL) {
			if(background->vramId != 0) {
				bgSetScroll(background->vramId, background->x - background->attachedToView ? view->x : 0, background->y - background->attachedToView ? view->y : 0);
				//DSGM_Debug("S s %d, l %d", background->screen, background->layer);
			}
		}
	}
}

inline BgSize DSGM_GetBGSize(u8 screen, int layer) {
	return bgState[layer + ((screen == DSGM_BOTTOM) * 4)].size;
}

int DSGM_GetBGWidth(u8 screen, int layer) {
	switch(DSGM_GetBGSize(screen, layer)) {
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

int DSGM_GetBGHeight(u8 screen, int layer) {
	switch(DSGM_GetBGSize(screen, layer)) {
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

inline u16 DSGM_GetTile(DSGM_BackgroundInstance *background, int x, int y) {
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
	//int t = y * DSGM_GetBGWidth(screen, layer) / 16 + x;
	//return bgGetMapPtr(background->vramId)[y * DSGM_GetBGWidth(screen, layer) / 16 + x];
	return bgGetMapPtr(background->vramId)[y * DSGM_GetBGWidth(background->screen, background->layer) / 16 + x];
}

inline void DSGM_SetTile(DSGM_BackgroundInstance *background, int x, int y, u16 tile) {
	while(y > 31) {
		y -= 32;
		x += 64;
	}
	while(x > 31) {
		x -= 32;
		y += 32;
	}
	bgGetMapPtr(background->vramId)[y * DSGM_GetBGWidth(background->screen, background->layer) / 16 + x] = tile;
}
