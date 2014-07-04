#pragma once

#define DSGM_NO_PALETTE -1

#define DSGM_GetPalette(palette)\
DSGM_Palettes[palette]

#define DSGM_FORM_RAM_PALETTE(palette)\
{ (u8 *)&palette##_Pal_bin, (u32 *)&palette##_Pal_bin_size }

#define DSGM_FORM_NITRO_PALETTE(palette)\
{ NULL, 0, #palette "_Pal.bin" }

#define DSGM_LOAD_TOP_PALETTE_RAM(paletteName, paletteNumber)\
DSGM_LoadTopPaletteRAM(paletteName##_Pal_bin, paletteName##_Pal_bin_size, paletteNumber)

#define DSGM_LOAD_BOTTOM_PALETTE_RAM(paletteName, paletteNumber)\
DSGM_LoadBottomPaletteRAM(paletteName##_Pal_bin, paletteName##_Pal_bin_size, paletteNumber)

#define DSGM_LOAD_TOP_PALETTE_NITRO(paletteName, paletteNumber)\
DSGM_LoadTopPaletteNitro(#paletteName "_Pal.bin", paletteNumber)

#define DSGM_LOAD_BOTTOM_PALETTE_NITRO(paletteName, paletteNumber)\
DSGM_LoadBottomPaletteNitro(#paletteName "_Pal.bin", paletteNumber)

typedef struct {
	u8 *palette;
	u32 *paletteLength;
	char *nitroFilename;
	
	int paletteNumber[2];
} DSGM_Palette;

extern int DSGM_nextPalette[2];

void DSGM_UnlockSpritePalette(u8 screen);
void DSGM_LockSpritePalette(u8 screen);

inline int DSGM_NextFreePalette(u8 screen);

inline bool DSGM_PaletteLoaded(u8 screen, DSGM_Palette *palette);

//bool DSGM_PaletteTopLoaded(int palette);
//bool DSGM_PaletteBottomLoaded(int palette);

//u8 DSGM_GetNextTopPalette(void);
//u8 DSGM_GetNextBottomPalette(void);

//bool DSGM_PaletteIsNitro(int palette);

inline bool DSGM_PaletteIsNitro(DSGM_Palette *palette);

//void DSGM_ClearPalettes(void);

void DSGM_ClearPalettes(DSGM_Palette *palettes, int paletteCount);

//void DSGM_LoadTopPalette(int palette);
//void DSGM_LoadBottomPalette(int palette);

void DSGM_LoadPaletteFull(u8 screen, DSGM_Palette *Palette);

/*void DSGM_LoadTopPaletteRAM(u16 *palette, u32 paletteSize, DSGM_Palette *palette);
void DSGM_LoadBottomPaletteRAM(u16 *palette, u32 paletteSize, DSGM_Palette *palette);
void DSGM_LoadTopPaletteNitro(char *filename, DSGM_Palette *palette);
void DSGM_LoadBottomPaletteNitro(char *filename, DSGM_Palette *palette);*/
