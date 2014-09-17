#pragma once

#define DSGM_LegacySin(angle) (sinLerp(angle << 6) >> 4)
#define DSGM_LegacyCos(angle) (cosLerp(angle << 6) >> 4)

inline u16 DSGM_GetAngle(s32 startx, s32 starty, s32 targetx, s32 targety);
