#pragma once

extern PrintConsole DSGM_text[2][4];
extern int DSGM_textLayer[2];

#define DSGM_DEFAULT_FONT ((u8 *)defaultConsole.font.gfx)
extern PrintConsole defaultConsole;

void DSGM_InitText(DSGM_BackgroundInstance *backgroundInstance);
inline void DSGM_SetTextLayer(u8 screen, u8 layer);
void DSGM_ClearText(u8 screen);
void DSGM_DrawText(u8 screen, int x, int y, const char *format, ...);
void DSGM_BoxText(u8 screen, u8 x, u8 y, u8 width, u8 height, u8 delay, const char *format, ...);
