/*
  Copyright (C) 2008 Alex Diener

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Alex Diener adiener@sacredsoftware.net
*/

/*
	Modified for dsgmLib integration, 2014
	
	Additional help from: 
	elhobbs
	http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q60
	http://content.gpwiki.org/index.php/OpenGL%3aTutorials%3aUsing_Quaternions_to_represent_rotation#Quaternion_from_Euler_angles
	
	Not all quaternion functionality has been implemented, only what was needed
*/

#pragma once

typedef struct Quaternion Quaternion;

struct Quaternion {
	int32 x;
	int32 y;
	int32 z;
	int32 w;
};

void Quaternion_loadIdentity(Quaternion *quaternion);
Quaternion Quaternion_identity();
Quaternion Quaternion_withValues(int32 x, int32 y, int32 z, int32 w);

Quaternion Quaternion_fromVector(vect3D vector);
vect3D Quaternion_toVector(Quaternion quaternion);
Quaternion Quaternion_fromAxisAngle(vect3D axis, int angle);
//void Quaternion_toAxisAngle(Quaternion quaternion, vect3D *axis, float * angle);
inline Quaternion Quaternion_fromRollPitchYaw(int32 roll, int32 pitch, int32 yaw);
m4x4 Quaternion_toMatrix(Quaternion quaternion);

void Quaternion_multiply(Quaternion * quaternion1, Quaternion quaternion2);
Quaternion Quaternion_multiplied(Quaternion quaternion1, Quaternion quaternion2);

inline void DSGM_RotateRollPitchYaw(int32 roll, int32 pitch, int32 yaw);


void Quaternion_normalize(Quaternion * quaternion);
Quaternion Quaternion_normalized(Quaternion quaternion);

/*Quaternion Quaternion_slerp(Quaternion start, Quaternion end, float alpha);

void Quaternion_rotate(Quaternion * quaternion, vect3D axis, float angle);
Quaternion Quaternion_rotated(Quaternion quaternion, vect3D axis, float angle);

void Quaternion_invert(Quaternion * quaternion);
Quaternion Quaternion_inverted(Quaternion quaternion);

vect3D Quaternion_multiplyVector(Quaternion quaternion, vect3D vector);
*/
