#include "DSGM.h"

inline int DSGM_LargeBackgroundSizeToPixels(DSGM_LARGE_BACKGROUND_SIZES size) {
	return (size + 1) * 256;
}

void DSGM_LargeBackgroundCopyScreen(DSGM_Layer *layer) {
	int tilesLength = DSGM_LargeBackgroundSizeToPixels(layer->background->size) / 8;
	
	int xl, yl;
	for(xl = 0; xl < 256 / 8; xl++) {
		for(yl = 0; yl < 192 / 8; yl++) {
			int x = xl;
			int y = yl;
			
			DSGM_SetTileForceVRAM(layer, x, y, layer->largeBackgroundMap[y * tilesLength + x]);
		}
	}
}

void DSGM_LargeBackgroundUpdateFull(DSGM_View *view, DSGM_Layer *layer) {
	int tilesLength = DSGM_LargeBackgroundSizeToPixels(layer->background->size) / 8;
	
	int minX = (layer->x + (layer->attachedToView ? view->x : 0) / 8) - 1;
	int minXt = minX;
	while(minXt < 0) minXt += tilesLength;
	
	int maxX = (layer->x + (layer->attachedToView ? view->x : 0) / 8) + (256 / 8);
	int maxXt = maxX;
	while(maxXt < 0) maxXt += tilesLength;
	
	int minY = (layer->y + (layer->attachedToView ? view->y : 0) / 8) - 1;
	int minYt = minY;
	while(minYt < 0) minYt += tilesLength;
	
	int maxY = (layer->y + (layer->attachedToView ? view->y : 0) / 8) + (192 / 8);
	int maxYt = maxY;
	while(maxYt < 0) maxYt += tilesLength;
	
	int y;
	for(y = minY; y < maxY; y++) {
		int ya = y;
		while(ya < 0) ya += (512 / 8);
		while(ya > tilesLength) ya -= (512 / 8);
		
		int xa = minX;
		while(xa < 0) xa += (512 / 8);
		while(xa > tilesLength) xa -= (512 / 8);
		DSGM_SetTileForceVRAM(layer, xa, ya, layer->largeBackgroundMap[(y % tilesLength) * tilesLength + (minXt % tilesLength)]);
		
		xa = maxX;
		while(xa < 0) xa += (512 / 8);
		while(xa > tilesLength) xa -= (512 / 8);
		DSGM_SetTileForceVRAM(layer, xa, ya, layer->largeBackgroundMap[(y % tilesLength) * tilesLength + (maxXt % tilesLength)]);
	}
	
	int x;
	for(x = minX; x < maxX; x++) {
		int xa = x;
		while(xa < 0) xa += (512 / 8);
		while(xa > tilesLength) xa -= (512 / 8);
		
		int ya = minY;
		while(ya < 0) ya += (512 / 8);
		while(ya > tilesLength) ya -= (512 / 8);
		DSGM_SetTileForceVRAM(layer, xa, minY, layer->largeBackgroundMap[(minYt % tilesLength) * tilesLength + (x % tilesLength)]);
		
		ya = maxY;
		while(ya < 0) ya += (512 / 8);
		while(ya > tilesLength) ya -= (512 / 8);
		DSGM_SetTileForceVRAM(layer, xa, maxY, layer->largeBackgroundMap[(maxYt % tilesLength) * tilesLength + (x % tilesLength)]);
	}
}
