#pragma once

#define DSGM_DEFAULT_FONT (void *)1

#define DSGM_FORM_RAM_FONT(background)\
{ NULL, NULL, NULL, (u8 *)&background##_Tiles_bin, (u32 *)&background##_Tiles_bin_size, NULL, NULL, (u8 *)&background##_Pal_bin, (u32 *)&background##_Pal_bin_size, BgSize_T_256x256, BgType_Text4bpp, 0 }

#define DSGM_FORM_NITRO_FONT(background)\
{ #background "_Tiles.bin", NULL, #background "_Pal.bin", NULL, NULL, NULL, NULL, NULL, NULL, BgSize_T_256x256, BgType_Text4bpp, 0 }

extern PrintConsole DSGM_text[2][4];
extern int DSGM_textLayer[2];

#define DSGM_DEFAULT_FONT_GFX ((u8 *)defaultConsole.font.gfx)
extern PrintConsole defaultConsole;

void DSGM_InitText(DSGM_Layer *layer);
inline void DSGM_SetTextLayer(u8 screen, u8 layerNumber);
void DSGM_ClearText(u8 screen);
void DSGM_DrawText(u8 screen, int x, int y, const char *format, ...);
void DSGM_BoxText(u8 screen, u8 x, u8 y, u8 width, u8 height, u8 delay, const char *format, ...);
