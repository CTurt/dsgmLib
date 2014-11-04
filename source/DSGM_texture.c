#include "DSGM.h"

void DSGM_LoadTexture(DSGM_Texture *texture) {
	// Doesn't support paletted textures
	if(!texture || (texture->type != GL_RGB && texture->type != GL_RGBA)) {
		return;
	}
	
	glGenTextures(1, &texture->id);
	glBindTexture(0, texture->id);
	glTexImage2D(0, 0, texture->type, texture->width, texture->height, 0, TEXGEN_TEXCOORD, texture->data);
}

void DSGM_ScaleTexture(float x, float y) {
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(x, y, 1);
	glMatrixMode(GL_POSITION);
}
