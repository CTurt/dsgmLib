#pragma once

#define DSGM_NO_PALETTE -1

#define DSGM_GetPalette(palette)\
DSGM_Palettes[palette]

#define DSGM_FORM_RAM_PALETTE(palette)\
{ (u8 *)&palette##_Pal_bin, (u32 *)&palette##_Pal_bin_size, NULL, { DSGM_NO_PALETTE, DSGM_NO_PALETTE } }

#define DSGM_FORM_NITRO_PALETTE(palette)\
{ NULL, 0, #palette "_Pal.bin", { DSGM_NO_PALETTE, DSGM_NO_PALETTE } }

#define DSGM_FORM_FAT_PALETTE(palette)\
{ NULL, 0, "fat:/" #palette "_Pal.bin", { DSGM_NO_PALETTE, DSGM_NO_PALETTE } }

typedef struct {
	u8 *palette;
	u32 *paletteLength;
	char *nitroFilename;
	
	int paletteNumber[2];
} DSGM_Palette;

extern int DSGM_nextPalette[2];

#define DSGM_UnlockSpritePalette(screen) DSGM_UnlockSpritePaletteFull(&DSGM_Rooms[DSGM_currentRoom], screen)
inline void DSGM_UnlockSpritePaletteFull(DSGM_Room *room, u8 screen);

#define DSGM_LockSpritePalette(screen) DSGM_LockSpritePaletteFull(&DSGM_Rooms[DSGM_currentRoom], screen)
inline void DSGM_LockSpritePaletteFull(DSGM_Room *room, u8 screen);

inline u16 *DSGM_GetPaletteData(u8 screen, DSGM_Palette *palette);

inline int DSGM_NextFreePalette(u8 screen);

inline bool DSGM_PaletteLoaded(u8 screen, DSGM_Palette *palette);

inline bool DSGM_PaletteIsNitro(DSGM_Palette *palette);

void DSGM_ClearPalettes(DSGM_Palette *palettes, int paletteCount);

void DSGM_LoadPaletteFull(u8 screen, DSGM_Palette *Palette);
