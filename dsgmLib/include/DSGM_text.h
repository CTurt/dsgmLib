#pragma once

extern PrintConsole DSGM_text[2];

void DSGM_InitText(DSGM_BackgroundInstance *backgroundInstance);
void DSGM_ClearText(u8 screen);
void DSGM_DrawText(u8 screen, int x, int y, const char *format, ...);
