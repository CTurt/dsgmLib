#pragma once

// Vector
typedef float vec3_t[3];

// MD2 header
typedef struct {
	int ident;
	int version;
	
	int skinwidth;
	int skinheight;
	
	int framesize;
	
	int num_skins;
	int num_vertices;
	int num_st;
	int num_tris;
	int num_glcmds;
	int num_frames;
	
	int offset_skins;
	int offset_st;
	int offset_tris;
	int offset_frames;
	int offset_glcmds;
	int offset_end;
} md2_header_t;

// Texture name
typedef struct {
	char name[64];
} md2_skin_t;

// Texture coords
typedef struct {
	short s;
	short t;
} md2_texCoord_t;

// Triangle info
typedef struct {
	unsigned short vertex[3];
	unsigned short st[3];
} md2_triangle_t;

// Compressed vertex
typedef struct {
	unsigned char v[3];
	unsigned char normalIndex;
} md2_vertex_t;

// Model frame
typedef struct {
	vect3D scale;
	vect3D translate;
	char name[16];
	md2_vertex_t *verts;
	vect3D *packedVerts;
	u32 *packedv10;
	vect3D *faceNormals;
	vect3D min, max;
	u16 next;
	u32 *displayList[4];
} md2_frame_t;

typedef struct {
	u16 start, end;
} md2Anim_struct;

// MD2 model structure
typedef struct {
	md2_header_t header;
	
	u8 numAnim;
	md2Anim_struct *animations;
	
	//mtlImg_struct *texture;
	
	md2_skin_t *skins;
	md2_texCoord_t *texcoords;
	u32 *packedTexcoords;
	md2_triangle_t *triangles;
	md2_frame_t *frames;
} DSGM_Model;

typedef struct {
	u16 currentFrame, nextFrame, interpCounter;
	u8 currentAnim;
	u8 oldAnim;
	bool oneshot;
	u8 animationSpeed;
	bool fixTransformations;
	bool interpolate;
	u32 *palette;
	DSGM_Model *model;
} DSGM_ModelInstance;

int DSGM_LoadModel(const char *filename, DSGM_Model *mdl);
void DSGM_FreeModel(DSGM_Model *mdl);
void DSGM_RenderModelFrame(int n, const DSGM_Model *mdl);
void DSGM_RenderModelFrameInterp(int n, int n2, int m, bool t, const DSGM_Model *mdl);

void DSGM_InitModelInstance(DSGM_ModelInstance *mi, DSGM_Model *mdl);
void DSGM_SetModelInstanceAnimation(DSGM_ModelInstance *mi, u16 newAnim, bool oneshot);
void DSGM_UpdateModelInstanceAnimation(DSGM_ModelInstance *mi);
inline void DSGM_RenderModelInstance(DSGM_ModelInstance *mi);
