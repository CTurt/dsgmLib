#include "DSGM.h"

PrintConsole DSGM_text[2][4];
int DSGM_textLayer[2];

void DSGM_InitText(DSGM_Layer *layer) {
	consoleInit(&DSGM_text[layer->screen][layer->layerNumber], layer->layerNumber, BgType_Text4bpp, BgSize_T_256x256, layer->mapBase, layer->tileBase, layer->screen, 1);
	consoleSelect(&DSGM_text[layer->screen][layer->layerNumber]);
	consoleClear();
	layer->vramId = DSGM_text[layer->screen][layer->layerNumber].bgId;
	DSGM_textLayer[layer->screen] = layer->layerNumber;
	DSGM_Debug("Init text screen %d, layerNumber %d, vramId %d\n", layer->screen, layer->layerNumber, DSGM_text[layer->screen][layer->layerNumber].bgId);
}

inline void DSGM_SetTextLayer(u8 screen, u8 layerNumber) {
	DSGM_textLayer[screen] = layerNumber;
}

void DSGM_ClearText(u8 screen) {
	consoleSelect(&DSGM_text[screen][DSGM_textLayer[screen]]);
	consoleClear();
}

void DSGM_DrawText(u8 screen, int x, int y, const char *format, ...) {
	consoleSelect(&DSGM_text[screen][DSGM_textLayer[screen]]);
	DSGM_text[screen][DSGM_textLayer[screen]].cursorX = x;
	DSGM_text[screen][DSGM_textLayer[screen]].cursorY = y;
	
	va_list arg;
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}

void DSGM_BoxText(u8 screen, u8 x, u8 y, u8 width, u8 height, u8 delay, const char *format, ...) {
	char text[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(text, 1023, format, args);
	va_end(args);
	
    int i;
    int len = strlen(text);
	u8 dx;
	u8 dy;
	u8 jx = 0;
	u8 jy = 0;
    
	consoleSelect(&DSGM_text[screen][DSGM_textLayer[screen]]);
	
    for(i = 0; i < len; i++) {
		dx = x + ((i - jx) % width);
		dy = y + ((i - jx) / width) + jy;
		if(height && dy >= y + height) break;
		DSGM_text[screen][DSGM_textLayer[screen]].cursorX = dx;
		DSGM_text[screen][DSGM_textLayer[screen]].cursorY = dy;
        printf("%c", text[i]);
		if(text[i] == '\n' || (text[i] == ' ' && DSGM_GetWordLength(text + i + 1) >= width - (dx - x) && (dx - x) > 0)) {
			jy++;
			jx += dx;
		}
		DSGM_Delay(delay);
    }
}
