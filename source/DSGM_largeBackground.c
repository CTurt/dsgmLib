#include "DSGM.h"

inline int DSGM_LargeBackgroundSizeToPixels(DSGM_LARGE_BACKGROUND_SIZES size) {
	return 1 << (size + 9);
}
