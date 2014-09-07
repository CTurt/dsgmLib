#include "DSGM.h"

void DSGM_InitDrawableBackground(DSGM_BackgroundInstance *backgroundInstance) {
	DSGM_Debug("Drawable background screen %d, layer %d", backgroundInstance->screen, backgroundInstance->layer);
	switch(backgroundInstance->screen) {
		case DSGM_TOP:
			backgroundInstance->vramId = bgInit(backgroundInstance->layer, BgType_Bmp16, BgSize_B16_256x256, backgroundInstance->mapBase, backgroundInstance->tileBase);
			break;
			
		case DSGM_BOTTOM:
			backgroundInstance->vramId = bgInitSub(backgroundInstance->layer, BgType_Bmp16, BgSize_B16_256x256, backgroundInstance->mapBase, backgroundInstance->tileBase);
			break;
	}
}

void DSGM_DrawPixelToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, u16 colour) {
	bgGetGfxPtr(room->backgroundInstances[screen][3].vramId)[x + (y << 8)] = colour;
}

void DSGM_DrawRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, u16 colour) {
	u16 *gfx = bgGetGfxPtr(room->backgroundInstances[screen][3].vramId);
	
	int xi, yi;
	for(xi = x; xi < x + width; xi++) {
		for(yi = y; yi < y + height; yi++) {
			if(xi >= 0 && yi >= 0 && xi < 256 && yi < 192) gfx[xi + (yi << 8)] = colour;
		}
	}
}

// Adapted from http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
void DSGM_DrawLineToBackgroundFull(DSGM_Room *room, u8 screen, int x1, int y1, int x2, int y2, int thickness, u16 colour) {
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
	int err = (dx > dy ? dx : -dy) / 2, e2;
	
	while(1) {
		DSGM_DrawRectangleToBackgroundFull(room, screen, x1 - thickness / 2, y1 - thickness / 2, thickness, thickness, colour);
		if(x1 == x2 && y1 == y2) break;
		
		e2 = err;
		if(e2 >-dx) {
			err -= dy;
			x1 += sx;
		}
		if(e2 < dy) {
			err += dx;
			y1 += sy;
		}
	}
}
