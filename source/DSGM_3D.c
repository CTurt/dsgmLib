#include "DSGM.h"

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
