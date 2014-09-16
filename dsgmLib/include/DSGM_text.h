#pragma once

extern PrintConsole DSGM_text[2][4];
extern int DSGM_textLayer[2];

void DSGM_InitText(DSGM_BackgroundInstance *backgroundInstance);
inline void DSGM_SetTextLayer(u8 screen, u8 layer);
void DSGM_ClearText(u8 screen);
void DSGM_DrawText(u8 screen, int x, int y, const char *format, ...);
