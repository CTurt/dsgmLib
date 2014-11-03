#pragma once

#define DSGM_FORM_RAM_TEXTURE(type, width, height, data) { type, width, height, data, -1 }

typedef struct {
	GL_TEXTURE_TYPE_ENUM type;
	enum GL_TEXTURE_SIZE_ENUM width;
	enum GL_TEXTURE_SIZE_ENUM height;
	const unsigned char *data;
	int id;
} DSGM_Texture;

void DSGM_LoadTexture(DSGM_Texture *texture);
