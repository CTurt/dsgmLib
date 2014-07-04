#include "DSGM.h"

PrintConsole DSGM_text[2];

void DSGM_InitText(DSGM_BackgroundInstance *backgroundInstance) {
	consoleInit(&DSGM_text[backgroundInstance->screen], backgroundInstance->layer, BgType_Text4bpp, BgSize_T_256x256, backgroundInstance->layer + 28, 0, backgroundInstance->screen, 1);
	consoleSelect(&DSGM_text[backgroundInstance->screen]);
	consoleClear();
	backgroundInstance->vramId = DSGM_text[backgroundInstance->screen].bgId;
	DSGM_Debug("Init text screen %d, layer %d, vramId %d\n", backgroundInstance->screen, backgroundInstance->layer, DSGM_text[backgroundInstance->screen].bgId/*backgroundInstance->vramId*/);
}

void DSGM_ClearText(u8 screen) {
	consoleSelect(&DSGM_text[screen]);
	consoleClear();
}

void DSGM_DrawText(u8 screen, int x, int y, const char *format, ...) {
	consoleSelect(&DSGM_text[screen]);
	DSGM_text[screen].cursorX = x;
	DSGM_text[screen].cursorY = y;
	
	va_list arg;
	va_start(arg, format);
	vfprintf(stdout, format, arg);
	va_end(arg);
}
