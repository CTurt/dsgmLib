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
			
			DSGM_SetTileFull(layer, x, y, layer->largeBackgroundMap[y * tilesLength + x]);
		}
	}
}

void DSGM_LargeBackgroundUpdate(DSGM_Layer *layer) {
	
}
