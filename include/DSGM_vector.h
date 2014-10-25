#pragma once

#define EPSINT32 2 //check that

#define min(a,b) (((a)>(b))?(b):(a))
#define max(a,b) (((a)>(b))?(a):(b))

#define vect(x,y,z) ((vect3D){(x),(y),(z)})
#define vect2(x,y) ((vect2D){(x),(y)})

typedef struct {
	int32 x, y, z;
} vect3D;

typedef struct {
	int32 x, y;
} vect2D;

static inline void transposeMatrix33(int32 *m1, int32 *m2) {
	int i, j;
	for(i = 0; i < 3; i++) for(j = 0; j < 3; j++) m2[j+i*3] = m1[i+j*3];
}

static inline vect3D evalVectMatrix33(int32 *m, vect3D v) {
	return vect((mulf32(v.x,m[0])+mulf32(v.y,m[1])+mulf32(v.z,m[2])),
				(mulf32(v.x,m[3])+mulf32(v.y,m[4])+mulf32(v.z,m[5])),
				(mulf32(v.x,m[6])+mulf32(v.y,m[7])+mulf32(v.z,m[8])));
}

static inline vect3D minVect(vect3D u, vect3D v) {
	return vect(min(u.x,v.x),min(u.y,v.y),min(u.z,v.z));
}

static inline vect3D maxVect(vect3D u, vect3D v) {
	return vect(max(u.x,v.x),max(u.y,v.y),max(u.z,v.z));
}

static inline vect3D addVect(vect3D p1, vect3D p2) {
	return vect(p1.x+p2.x,p1.y+p2.y,p1.z+p2.z);
}

static inline vect3D vectMult(vect3D v, int32 k) {
	return vect(mulf32(v.x,k), mulf32(v.y,k), mulf32(v.z,k));
}

static inline vect3D vectMultInt(vect3D v, int k) {
	return vect((v.x*k), (v.y*k), (v.z*k));
}

static inline vect3D vectDivInt(vect3D v, int k) {
	return vect((v.x/k), (v.y/k), (v.z/k));
}

static inline vect3D vectDifference(vect3D p1, vect3D p2) {
	return vect(p1.x-p2.x,p1.y-p2.y,p1.z-p2.z);
}

static inline int32 sqDistance(vect3D p1, vect3D p2) {
	return (mulf32((p1.x-p2.x),(p1.x-p2.x))+mulf32((p1.y-p2.y),(p1.y-p2.y))+mulf32((p1.z-p2.z),(p1.z-p2.z)));
}

static inline int32 distance(vect3D p1, vect3D p2) {
	return sqrtf32(mulf32((p1.x-p2.x),(p1.x-p2.x))+mulf32((p1.y-p2.y),(p1.y-p2.y))+mulf32((p1.z-p2.z),(p1.z-p2.z)));
}

static inline int32 magnitude(vect3D p1) {
	return sqrtf32(mulf32((p1.x),(p1.x))+mulf32((p1.y),(p1.y))+mulf32((p1.z),(p1.z)));
}

static inline int32 sqMagnitude(vect3D p1) {
	return (mulf32((p1.x),(p1.x))+mulf32((p1.y),(p1.y))+mulf32((p1.z),(p1.z)));
}

static inline vect3D divideVect(vect3D v, int32 d) {
	return vect(divf32(v.x,d),divf32(v.y,d),divf32(v.z,d));
}

static inline vect3D normalize(vect3D v) {
	return divideVect(v,sqrtf32(mulf32((v.x),(v.x))+mulf32((v.y),(v.y))+mulf32((v.z),(v.z))));
}

static inline int32 dotProduct(vect3D v1, vect3D v2) {
	return (mulf32(v1.x,v2.x)+mulf32(v1.y,v2.y)+mulf32(v1.z,v2.z));
}

static inline vect3D vectProduct(vect3D v1, vect3D v2) {
	return vect(mulf32(v1.y,v2.z)-mulf32(v1.z,v2.y),mulf32(v1.z,v2.x)-mulf32(v1.x,v2.z),mulf32(v1.x,v2.y)-mulf32(v1.y,v2.x));
}

static inline int32 fakeDotProduct(vect3D v1, vect3D v2) {
	return ((v1.x*v2.x)+(v1.y*v2.y)+(v1.z*v2.z));
}

static inline bool equals(int32 a, int32 b) {
	return abs(a-b) <= EPSINT32;
}

static inline void writeVect(vect3D v, FILE *f) {
	fwrite(&v, sizeof(vect3D), 1, f);
}

static inline void readVect(vect3D *v, FILE *f) {
	fread(v, sizeof(vect3D), 1, f);
}
