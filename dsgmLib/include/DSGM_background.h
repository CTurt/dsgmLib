#pragma once

#define DSGM_NO_BACKGROUND NULL
#define DSGM_TEXT_BACKGROUND (void *)1

#define DSGM_FORM_RAM_BACKGROUND(background, size, type)\
{ NULL, NULL, NULL, (u8 *)&background##_Tiles_bin, (u32 *)&background##_Tiles_bin_size, (u8 *)&background##_Map_bin, (u32 *)&background##_Map_bin_size, (u8 *)&background##_Pal_bin, (u32 *)&background##_Pal_bin_size, size, type }

#define DSGM_FORM_NITRO_BACKGROUND(background, size, type)\
{ #background "_Tiles.bin", #background "_Map.bin", #background "_Pal.bin", NULL, NULL, NULL, NULL, NULL, NULL, size, type }

typedef struct {
	char *nitroTilesFilename;
	char *nitroMapFilename;
	char *nitroPaletteFilename;
	
	u8 *tiles;
	u32 *tilesLength;
	
	u8 *map;
	u32 *mapLength;
	
	u8 *palette;
	u32 *paletteLength;
	
	BgSize size;
	BgType type;
} DSGM_Background;

typedef struct {
	DSGM_Background *background;
	
	u8 screen;
	int layer;
	bool attachedToView;
	int x;
	int y;
	int vramId;
} DSGM_BackgroundInstance;

void DSGM_LockBackgroundPalette(u8 screen);
void DSGM_UnlockBackgroundPalette(u8 screen);

inline bool DSGM_BackgroundIsNitroFull(DSGM_Background *background);

void DSGM_LoadBackgroundFull(DSGM_BackgroundInstance *backgroundInstance);

void DSGM_LoadBackgroundNitroFull(DSGM_BackgroundInstance *background);
void DSGM_LoadBackgroundRAMFull(DSGM_BackgroundInstance *background);

void DSGM_ScrollBackgroundFull(DSGM_View *view, DSGM_BackgroundInstance *background);

inline BgSize DSGM_GetBGSize(u8 screen, int layer);
int DSGM_GetBGWidth(u8 screen, int layer);
int DSGM_GetBGHeight(u8 screen, int layer);

inline u16 DSGM_GetTile(DSGM_BackgroundInstance *background, int x, int y);
inline void DSGM_SetTile(DSGM_BackgroundInstance *background, int x, int y, u16 tile);
