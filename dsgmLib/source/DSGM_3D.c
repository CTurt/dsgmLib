#include "DSGM.h"

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
