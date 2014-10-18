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
			
			/*while(y > 31) {
				y -= 32;
				x += 64;
			}
			while(x > 31) {
				x -= 32;
				y += 32;
			}*/
			
			DSGM_SetTileForceVRAM(layer, x, y, layer->largeBackgroundMap[y * tilesLength + x]);
		}
	}
}

void DSGM_LargeBackgroundUpdateFull(DSGM_View *view, DSGM_Layer *layer) {
	int tilesLength = DSGM_LargeBackgroundSizeToPixels(layer->background->size) / 8;
	
	int minX = (layer->x + (layer->attachedToView ? view->x : 0) / 8) - 1;
	if(minX < 0) minX = 0;
	int maxX = (layer->x + (layer->attachedToView ? view->x : 0) / 8) + (256 / 8);
	if(maxX > tilesLength) maxX = tilesLength;
	
	int minY = (layer->y + (layer->attachedToView ? view->y : 0) / 8) - 1;
	if(minY < 0) minY = 0;
	int maxY = (layer->y + (layer->attachedToView ? view->y : 0) / 8) + (192 / 8);
	if(maxY > tilesLength) maxY = tilesLength;
	
	int y;
	for(y = minY; y < maxY; y++) {
		DSGM_SetTileForceVRAM(layer, minX, y, layer->largeBackgroundMap[y * tilesLength + minX]);
		DSGM_SetTileForceVRAM(layer, maxX, y, layer->largeBackgroundMap[y * tilesLength + maxX]);
	}
	
	int x;
	for(x = minX; x < maxX; x++) {
		DSGM_SetTileForceVRAM(layer, x, minY, layer->largeBackgroundMap[minY * tilesLength + x]);
		DSGM_SetTileForceVRAM(layer, x, maxY, layer->largeBackgroundMap[maxY * tilesLength + x]);
	}
}
