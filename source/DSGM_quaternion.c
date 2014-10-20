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

#include "DSGM.h"

void Quaternion_loadIdentity(Quaternion *quaternion) {
	quaternion->x = 0;
	quaternion->y = 0;
	quaternion->z = 0;
	quaternion->w = 1 << 12;
}

Quaternion Quaternion_identity() {
	Quaternion quaternion;
	
	Quaternion_loadIdentity(&quaternion);
	return quaternion;
}

Quaternion Quaternion_withValues(int32 x, int32 y, int32 z, int32 w) {
	Quaternion quaternion;
	
	quaternion.x = x;
	quaternion.y = y;
	quaternion.z = z;
	quaternion.w = w;
	return quaternion;
}

Quaternion Quaternion_fromVector(vect3D vector) {
	Quaternion quaternion;
	
	quaternion.x = vector.x;
	quaternion.y = vector.y;
	quaternion.z = vector.z;
	quaternion.w = 0;
	return quaternion;
}

vect3D Quaternion_toVector(Quaternion quaternion) {
	vect3D vector;
	
	vector.x = quaternion.x;
	vector.y = quaternion.y;
	vector.z = quaternion.z;
	
	return vector;
}

Quaternion Quaternion_fromAxisAngle(vect3D axis, int angle) {
	Quaternion quaternion;
	int32 sinAngle;
	
	angle /= 2;
	axis = normalize(axis);
	sinAngle = sinLerp(angle);
	
	quaternion.x = mulf32(axis.x, sinAngle);
	quaternion.y = mulf32(axis.y, sinAngle);
	quaternion.z = mulf32(axis.z, sinAngle);
	quaternion.w = cosLerp(angle);
	
	return quaternion;
}

/*void Quaternion_toAxisAngle(Quaternion quaternion, vect3D *axis, float *angle) {
	float sinAngle;
	
	Quaternion_normalize(&quaternion);
	sinAngle = sqrt(1.0f - (quaternion.w * quaternion.w));
	if (fabs(sinAngle) < 0.0005f) sinAngle = 1.0f;
	
	if (axis != NULL) {
		axis->x = (quaternion.x / sinAngle);
		axis->y = (quaternion.y / sinAngle);
		axis->z = (quaternion.z / sinAngle);
	}
	
	if (angle != NULL) {
		*angle = (acos(quaternion.w) * 2.0f);
	}
}*/

inline Quaternion Quaternion_fromRollPitchYaw(int32 roll, int32 pitch, int32 yaw) {
	//http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q60
	/*Quaternion q = Quaternion_identity();
	Quaternion qt = Quaternion_identity();
	
	Quaternion qx = { cosLerp(pitch/2), sinLerp(pitch/2), 0, 0 };
	Quaternion qy = { cosLerp(yaw/2), 0, sinLerp(yaw/2), 0 };
	Quaternion qz = { cosLerp(roll/2), 0, 0, sinLerp(roll/2) };
	qt = Quaternion_multiplied(qx, qy);
	q = Quaternion_multiplied(qt, qz);
	
	return q;*/
	
	//http://content.gpwiki.org/index.php/OpenGL%3aTutorials%3aUsing_Quaternions_to_represent_rotation#Quaternion_from_Euler_angles
	Quaternion this;
	
	int32 p = pitch / 2;
	int32 y = yaw / 2;
	int32 r = roll / 2;
	
	int32 sinp = sinLerp(p);
	int32 siny = sinLerp(y);
	int32 sinr = sinLerp(r);
	int32 cosp = cosLerp(p);
	int32 cosy = cosLerp(y);
	int32 cosr = cosLerp(r);
	
	this.x = mulf32(mulf32(sinr, cosp), cosy) - mulf32(mulf32(cosr, sinp), siny);
	this.y = mulf32(mulf32(cosr, sinp), cosy) + mulf32(mulf32(sinr, cosp), siny);
	this.z = mulf32(mulf32(cosr, cosp), siny) - mulf32(mulf32(sinr, sinp), cosy);
	this.w = mulf32(mulf32(cosr, cosp), cosy) + mulf32(mulf32(sinr, sinp), siny);
	
	return this;
}

m4x4 Quaternion_toMatrix(Quaternion quaternion) {
	m4x4 matrix;
	
	matrix.m[0]  = ((1 << 12) - (2 * (mulf32(quaternion.y,quaternion.y) + mulf32(quaternion.z,quaternion.z))));
	matrix.m[1]  =				(2 * (mulf32(quaternion.x,quaternion.y) + mulf32(quaternion.z,quaternion.w)));
	matrix.m[2]  =				(2 * (mulf32(quaternion.x,quaternion.z) - mulf32(quaternion.y,quaternion.w)));
	matrix.m[3]  = 0;
	matrix.m[4]  =				(2 * (mulf32(quaternion.x,quaternion.y) - mulf32(quaternion.z,quaternion.w)));
	matrix.m[5]  = ((1 << 12) - (2 * (mulf32(quaternion.x,quaternion.x) + mulf32(quaternion.z,quaternion.z))));
	matrix.m[6]  =				(2 * (mulf32(quaternion.y,quaternion.z) + mulf32(quaternion.x,quaternion.w)));
	matrix.m[7]  = 0;
	matrix.m[8]  =				(2 * (mulf32(quaternion.x,quaternion.z) + mulf32(quaternion.y,quaternion.w)));
	matrix.m[9]  =				(2 * (mulf32(quaternion.y,quaternion.z) - mulf32(quaternion.x,quaternion.w)));
	matrix.m[10] = ((1 << 12) - (2 * (mulf32(quaternion.x,quaternion.x) + mulf32(quaternion.y,quaternion.y))));
	matrix.m[11] = 0;
	matrix.m[12] = 0;
	matrix.m[13] = 0;
	matrix.m[14] = 0;
	matrix.m[15] = 1 << 12;
	
	return matrix;
}

void Quaternion_multiply(Quaternion * quaternion1, Quaternion quaternion2) {
	vect3D vector1, vector2, cross;
	int32 angle;
	
	vector1 = Quaternion_toVector(*quaternion1);
	vector2 = Quaternion_toVector(quaternion2);
	angle = mulf32(quaternion1->w, quaternion2.w) - dotProduct(vector1, vector2);
	
	cross = vectProduct(vector1, vector2);
	vector1.x = mulf32(vector1.x, quaternion2.w);
	vector1.y = mulf32(vector1.y, quaternion2.w);
	vector1.z = mulf32(vector1.z, quaternion2.w);
	vector2.x = mulf32(vector2.x, quaternion1->w);
	vector2.y = mulf32(vector2.y, quaternion1->w);
	vector2.z = mulf32(vector2.z, quaternion1->w);
	
	quaternion1->x = vector1.x + vector2.x + cross.x;
	quaternion1->y = vector1.y + vector2.y + cross.y;
	quaternion1->z = vector1.z + vector2.z + cross.z;
	quaternion1->w = angle;
}

Quaternion Quaternion_multiplied(Quaternion quaternion1, Quaternion quaternion2) {
	Quaternion_multiply(&quaternion1, quaternion2);
	return quaternion1;
}

inline void DSGM_RotateRollPitchYaw(int32 roll, int32 pitch, int32 yaw) {
	Quaternion q = Quaternion_fromRollPitchYaw(roll, pitch, yaw);
	m4x4 m = Quaternion_toMatrix(q);
	glMultMatrix4x4(&m);
}


void Quaternion_normalize(Quaternion *quaternion) {
	int32 magnitude;
	
	magnitude = sqrtf32(mulf32(quaternion->x, quaternion->x) +
	                 mulf32(quaternion->y, quaternion->y) +
	                 mulf32(quaternion->z, quaternion->z) +
	                 mulf32(quaternion->w, quaternion->w));
	quaternion->x = divf32(quaternion->x, magnitude);
	quaternion->y = divf32(quaternion->y, magnitude);
	quaternion->z = divf32(quaternion->z, magnitude);
	quaternion->w = divf32(quaternion->w, magnitude);
}

Quaternion Quaternion_normalized(Quaternion quaternion) {
	Quaternion_normalize(&quaternion);
	return quaternion;
}

/*#define SLERP_TO_LERP_SWITCH_THRESHOLD 0.01f

Quaternion Quaternion_slerp(Quaternion start, Quaternion end, float alpha) {
	float startWeight, endWeight, difference;
	Quaternion result;
	
	difference = (start.x * end.x) + (start.y * end.y) + (start.z * end.z) + (start.w * end.w);
	if ((1.0f - fabs(difference)) > SLERP_TO_LERP_SWITCH_THRESHOLD) {
		float theta, oneOverSinTheta;
		
		theta = acos(fabs(difference));
		oneOverSinTheta = 1.0f / sin(theta);
		startWeight = sin(theta * (1.0f - alpha)) * oneOverSinTheta;
		endWeight = sin(theta * alpha) * oneOverSinTheta;
		if (difference < 0.0f) {
			startWeight = -startWeight;
		}
	} else {
		startWeight = 1.0f - alpha;
		endWeight = alpha;
	}
	result.x = (start.x * startWeight) + (end.x * endWeight);
	result.y = (start.y * startWeight) + (end.y * endWeight);
	result.z = (start.z * startWeight) + (end.z * endWeight);
	result.w = (start.w * startWeight) + (end.w * endWeight);
	Quaternion_normalize(&result);
	
	return result;
}

void Quaternion_rotate(Quaternion * quaternion, vect3D axis, float angle) {
	Quaternion rotationQuaternion;
	
	rotationQuaternion = Quaternion_fromAxisAngle(axis, angle);
	Quaternion_multiply(quaternion, rotationQuaternion);
}

Quaternion Quaternion_rotated(Quaternion quaternion, vect3D axis, float angle) {
	Quaternion_rotate(&quaternion, axis, angle);
	return quaternion;
}

void Quaternion_invert(Quaternion * quaternion) {
	float length;
	
	length = 1.0f / ((quaternion->x * quaternion->x) +
	                 (quaternion->y * quaternion->y) +
	                 (quaternion->z * quaternion->z) +
	                 (quaternion->w * quaternion->w));
	quaternion->x *= -length;
	quaternion->y *= -length;
	quaternion->z *= -length;
	quaternion->w *= length;
}

Quaternion Quaternion_inverted(Quaternion quaternion) {
	Quaternion_invert(&quaternion);
	return quaternion;
}

vect3D Quaternion_multiplyVector(Quaternion quaternion, vect3D vector) {
	Quaternion vectorQuaternion, inverseQuaternion, result;
	
	vectorQuaternion = Quaternion_fromvect3D(vector);
	inverseQuaternion = Quaternion_inverted(quaternion);
	result = Quaternion_multiplied(quaternion, Quaternion_multiplied(vectorQuaternion, inverseQuaternion));
	return Quaternion_toVector(result);
}
*/
