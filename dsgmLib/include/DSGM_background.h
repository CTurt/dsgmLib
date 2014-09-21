#pragma once

#define DSGM_NO_BACKGROUND NULL
#define DSGM_DRAWABLE_BACKGROUND (void *)2

#define DSGM_FORM_RAM_BACKGROUND(background, size, type)\
{ NULL, NULL, NULL, (u8 *)&background##_Tiles_bin, (u32 *)&background##_Tiles_bin_size, (u8 *)&background##_Map_bin, (u32 *)&background##_Map_bin_size, (u8 *)&background##_Pal_bin, (u32 *)&background##_Pal_bin_size, size, type, 0 }

#define DSGM_FORM_NITRO_BACKGROUND(background, size, type)\
{ #background "_Tiles.bin", #background "_Map.bin", #background "_Pal.bin", NULL, NULL, NULL, NULL, NULL, NULL, size, type, 0 }

#define DSGM_FORM_FAT_BACKGROUND(background, size, type)\
{ "fat:/" #background "_Tiles.bin", "fat:/" #background "_Map.bin", "fat:/" #background "_Pal.bin", NULL, NULL, NULL, NULL, NULL, NULL, size, type, 0 }

#define DSGM_layers DSGM_Rooms[DSGM_currentRoom].layers

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
	
	unsigned short tilesCount;
} DSGM_Background;

typedef struct {
	DSGM_Background *background;
	
	u8 screen;
	int layerNumber;
	bool attachedToView;
	u8 mapBase;
	u8 tileBase;
	
	int x;
	int y;
	int vramId;
} DSGM_Layer;

inline void DSGM_LockBackgroundPalette(u8 screen);
inline void DSGM_UnlockBackgroundPalette(u8 screen);
inline unsigned short *DSGM_GetBackgroundPalette(u8 screen, int layerNumber);
inline void DSGM_SetScreenColor(u8 screen, u16 color);

inline bool DSGM_BackgroundIsNitroFull(DSGM_Background *background);

void DSGM_InitDrawableBackground(DSGM_Layer *layer);

void DSGM_LoadBackgroundFull(DSGM_Layer *layer);

void DSGM_LoadBackgroundNitroFull(DSGM_Layer *layer);
void DSGM_LoadBackgroundRAMFull(DSGM_Layer *layer);

void DSGM_ScrollBackgroundFull(DSGM_View *view, DSGM_Layer *layer);

#define DSGM_GetLayerPriority(screen, layerNumber) DSGM_GetLayerPriorityFull(&DSGM_layers[screen][layerNumber])
inline unsigned int DSGM_GetLayerPriorityFull(DSGM_Layer *layer);

#define DSGM_SetLayerPriority(screen, layerNumber, priority) DSGM_SetLayerPriorityFull(&DSGM_layers[screen][layerNumber], priority)
inline void DSGM_SetLayerPriorityFull(DSGM_Layer *layer, unsigned int priority);

inline BgSize DSGM_GetBGSize(u8 screen, int layerNumber);
int DSGM_GetBGWidth(u8 screen, int layerNumber);
int DSGM_GetBGHeight(u8 screen, int layerNumber);

#define DSGM_GetTile(screen, layerNumber, x, y) DSGM_GetTileFull(&DSGM_layers[screen][layerNumber], x, y)
inline u16 DSGM_GetTileFull(DSGM_Layer *layer, int x, int y);

#define DSGM_SetTile(screen, layerNumber, x, y, tile) DSGM_SetTileFull(&DSGM_layers[screen][layerNumber], x, y, tile)
inline void DSGM_SetTileFull(DSGM_Layer *layer, int x, int y, u16 tile);

#define DSGM_SetRotationCenter(screen, x, y) DSGM_SetRotationCenterFull(&DSGM_layers[screen][3], x, y)
inline void DSGM_SetRotationCenterFull(DSGM_Layer *layer, int x, int y);

#define DSGM_RotateBackground(screen, angle) DSGM_RotateBackgroundFull(&DSGM_layers[screen][3], angle)
inline void DSGM_RotateBackgroundFull(DSGM_Layer *layer, int angle);

#define DSGM_ScaleBackground(screen, x, y) DSGM_ScaleBackgroundFull(&DSGM_layers[screen][3], x, y)
inline void DSGM_ScaleBackgroundFull(DSGM_Layer *layer, int x, int y);