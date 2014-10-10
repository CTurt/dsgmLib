#pragma once

u32 *glBeginListDL(void);
u32 glEndListDL(void);
u32 glBeginDL(u32 type);
void glEndDL(void);
u32 glNormalDL(uint32 normal);
u32 glVertexPackedDL(u32 packed);
void glVertex3v16DL(v16 x, v16 y, v16 z);
void glTexCoordPACKED(u32 uv);
void glColorDL(rgb color);
void glPolyFmtDL(u32 fmt);
