#pragma once

#define DSGM_3D_RENDERER (void *)3

#define glVertex3fv(v) glVertex3f(v[0], v[1], v[2])
#define glNormal3fv(n) glNormal3f(n[0], n[1], n[2])

void DSGM_DrawCube(float x, float y, float z, float width, float height, float depth);
