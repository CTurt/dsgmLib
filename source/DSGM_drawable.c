#include "DSGM.h"

void DSGM_InitDrawableBackground(DSGM_Layer *layer) {
	DSGM_Debug("Drawable background screen %d, layerNumber %d", layer->screen, layer->layerNumber);
	switch(layer->screen) {
		case DSGM_TOP:
			layer->vramId = bgInit(layer->layerNumber, BgType_Bmp16, BgSize_B16_256x256, layer->mapBase, layer->tileBase);
			break;
			
		case DSGM_BOTTOM:
			layer->vramId = bgInitSub(layer->layerNumber, BgType_Bmp16, BgSize_B16_256x256, layer->mapBase, layer->tileBase);
			break;
	}
}

inline void DSGM_ClearDrawableBackgroundFull(DSGM_Room *room, u8 screen) {
	// Some dma, swi function might be faster
	memset(bgGetGfxPtr(room->layers[screen][3].vramId), 0, 256 * 192 * sizeof(u16));
}

inline void DSGM_DrawPixelToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, u16 color) {
	bgGetGfxPtr(room->layers[screen][3].vramId)[x + (y << 8)] = color;
}

inline void DSGM_DrawRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, int thickness, u16 color) {
	DSGM_DrawLineToBackgroundFull(room, screen, x, y, x + width, y, thickness, color);
	DSGM_DrawLineToBackgroundFull(room, screen, x + width, y, x + width, y + height, thickness, color);
	DSGM_DrawLineToBackgroundFull(room, screen, x + width, y + height, x, y + height, thickness, color);
	DSGM_DrawLineToBackgroundFull(room, screen, x, y, x, y + height, thickness, color);
}

inline void DSGM_DrawFilledRectangleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int width, int height, u16 color) {
	u16 *gfx = bgGetGfxPtr(room->layers[screen][3].vramId);
	
	int xi, yi;
	for(xi = x; xi < x + width; xi++) {
		for(yi = y; yi < y + height; yi++) {
			if(xi >= 0 && yi >= 0 && xi < 256 && yi < 192) gfx[xi + (yi << 8)] = color;
		}
	}
}

// Adapted from http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
inline void DSGM_DrawLineToBackgroundFull(DSGM_Room *room, u8 screen, int x1, int y1, int x2, int y2, int thickness, u16 color) {
	int hthickness = thickness / 2;
	
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = abs(y2 - y1), sy = y1 < y2 ? 1 : -1; 
	int err = (dx > dy ? dx : -dy) / 2, e2;
	
	while(1) {
		DSGM_DrawFilledRectangleToBackgroundFull(room, screen, x1 - hthickness, y1 - hthickness, thickness, thickness, color);
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

inline void DSGM_DrawLineAtAngleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int angle, int length, int thickness, u16 color) {
	DSGM_DrawLineToBackgroundFull(room, screen, x, y, ((x << 12) + cosLerp(angle) * length) >> 12, ((y << 12) - sinLerp(angle) * length) >> 12, thickness, color);
}

inline void DSGM_DrawCircleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int radius, int thickness, u16 color) {
	int angle;
	int steps = thickness * 4096 / radius;
	for(angle = 0; angle < degreesToAngle(360); angle += steps) {
		DSGM_DrawFilledRectangleToBackgroundFull(room, screen, ((x << 12) + cosLerp(angle) * radius) >> 12, ((y << 12) - sinLerp(angle) * radius) >> 12, thickness, thickness, color);
		// circle would be more accurate than rectangle, but would be too slow
	}
}

inline void DSGM_DrawFilledCircleToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, int radius, u16 color) {
	int angle;
	int steps = 8192 / radius;
	for(angle = 0; angle < degreesToAngle(180); angle += steps) {
		int xb = x << 12;
		int yb = y << 12;
		int s = sinLerp(angle) * radius;
		int c = cosLerp(angle) * radius;
		DSGM_DrawLineToBackgroundFull(room, screen, (xb + c) >> 12, (yb - s) >> 12, (xb - c) >> 12, (yb + s) >> 12, 2, color);
	}
}

inline void DSGM_DrawTileToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, DSGM_Background *font, char tile, int size, u16 color) {
	unsigned char *fontGfx = NULL;
	bool freeGfx = false;
	
	if(font == DSGM_DEFAULT_FONT) fontGfx = DSGM_DEFAULT_FONT_GFX;
	else {
		//fontGfx = bgGetGfxPtr(font->vramId);
		if(DSGM_BackgroundIsNitroFull(font)) {
			size_t length = DSGM_GetFileLength(font->nitroTilesFilename);
			fontGfx = malloc(length);
			DSGM_ReadFileManual(fontGfx, 0, length, font->nitroTilesFilename);
			freeGfx = true;
		}
		else {
			fontGfx = font->tiles;
		}
	}
	
	int pixel;
	
	for(pixel = 0; pixel < 32; pixel++) {
		unsigned short word = fontGfx[(tile << 5) + pixel];
		
		int px = ((tile & 3) << 3) + ((pixel << 1) & 7);
		int py = ((tile >> 2) << 3) + (pixel >> 2);
		
		if((word & 0xF0) >> 4 != 0) DSGM_DrawFilledRectangleToBackgroundFull(room, screen, x + ((px + 1) % 8) * size, y + (py % 8) * size, size, size, color);
		if((word & 0x0F) != 0) DSGM_DrawFilledRectangleToBackgroundFull(room, screen, x + (px % 8) * size, y + (py % 8) * size, size, size, color);
	}
	
	if(freeGfx) free(fontGfx);
}

inline void DSGM_DrawTextToBackgroundFull(DSGM_Room *room, u8 screen, int x, int y, DSGM_Background *font, int size, u16 color, const char *format, ...) {
	char text[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(text, 1023, format, args);
	va_end(args);
	
	int i, len = strlen(text);
	for(i = 0; i < len; i++) {
		DSGM_DrawTileToBackgroundFull(room, screen, x + i * size * 8, y, font, text[i], size, color);
	}
}
