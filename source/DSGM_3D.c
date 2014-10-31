#include "DSGM.h"

void DSGM_InitStandard3D(void) {
	glInit();
	glEnable(GL_BLEND);
	glColor(RGB15(31, 31, 31));
	
	glEnable(GL_ANTIALIAS);
	glClearColor(0, 0, 0, 0);
	glClearPolyID(63);
	glClearDepth(0x7FFF);
	
	// When 3D mode is not enabled, VRAM bank B is used for top screen sprites, bank G is unused
	// When 3D mode is enabled, VRAM bank B is used for textures, and bank G is used for top screen sprites
	// Since bank G is smaller, we will have less VRAM available for sprites on the top screen
	glEnable(GL_TEXTURE_2D);
	
	glViewport(0, 0, 255, 191);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 256.0 / 192.0, 0.1, 512);
	
	glMaterialf(GL_AMBIENT, RGB15(4, 4, 4));
	glMaterialf(GL_DIFFUSE, RGB15(31, 31, 31));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8, 8, 8));
	glMaterialShinyness();
	
	glMatrixMode(GL_POSITION);
	glLoadIdentity();
}

inline void DSGM_UseCamera(DSGM_Camera *camera) {
	//glLoadIdentity();
	gluLookAtf32(camera->position.x, camera->position.y, camera->position.z, camera->lookAt.x, camera->lookAt.y, camera->lookAt.z, camera->up.x, camera->up.y, camera->up.z);
}

inline u64 DSGM_Distance3D(s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2) {
	s64 h = x1 - x2;
	s64 v = y1 - y2;
	s64 b = z1 - z2;
	return h * h + v * v + b * b;
}

void DSGM_GetPositionMatrix(struct m4x4 *m) {
	glGetFixed(GL_GET_MATRIX_POSITION, m->m);
	glMatrixMode(GL_POSITION);
}

vect3D DSGM_MatrixTranslationToVector(m4x4 initialMatrix, m4x4 resultantMatrix) {
	vect3D deltaPosition;
	deltaPosition.x = resultantMatrix.m[DSGM_MATRIX_X] - initialMatrix.m[DSGM_MATRIX_X];
	deltaPosition.y = resultantMatrix.m[DSGM_MATRIX_Y] - initialMatrix.m[DSGM_MATRIX_Y];
	deltaPosition.z = resultantMatrix.m[DSGM_MATRIX_Z] - initialMatrix.m[DSGM_MATRIX_Z];
	return deltaPosition;
}

void DSGM_ApplyQuaternion(Quaternion *q) {
	Quaternion_normalize(q);
	m4x4 m = Quaternion_toMatrix(*q);
	glMultMatrix4x4(&m);
}

void DSGM_ApplyInverseQuaternion(Quaternion *q) {
	Quaternion_normalize(q);
	Quaternion qi = Quaternion_inverted(*q);
	m4x4 m = Quaternion_toMatrix(qi);
	glMultMatrix4x4(&m);
}

void DSGM_DrawCube(float x, float y, float z, float width, float height, float depth) {
	glBegin(GL_QUADS);
	//z	face
	glColor3f(1,0,0);
	glVertex3f(x , y , z );
	glVertex3f(x + width, y , z );
	glVertex3f(x + width, y + height, z );
	glVertex3f(x , y + height, z );

	//z + depth face
	glColor3f(1,0,1);
	glVertex3f(x , y , z + depth);
	glVertex3f(x , y + height, z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y , z + depth);


	//x	face
	glColor3f(1,1,0);
	glVertex3f(x , y , z );
	glVertex3f(x , y + height, z );
	glVertex3f(x , y + height, z + depth);
	glVertex3f(x , y , z + depth);

	//x + width face
	glColor3f(1,1,1);
	glVertex3f(x + width, y , z );
	glVertex3f(x + width, y , z + depth);
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x + width, y + height, z );

	//y	face
	glColor3f(0,1,0);
	glVertex3f(x , y , z );
	glVertex3f(x , y , z + depth);
	glVertex3f(x + width, y , z + depth);
	glVertex3f(x + width, y , z );

	//y	+ height face
	glColor3f(0,1,1);
	glVertex3f(x , y + height, z );
	glVertex3f(x + width, y + height, z );
	glVertex3f(x + width, y + height, z + depth);
	glVertex3f(x , y + height, z + depth);
	
	glEnd();
}
