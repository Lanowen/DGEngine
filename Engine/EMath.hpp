#ifndef _EMATH_H_
#define _EMATH_H_

#include "foundation.hpp"

#ifdef D3DX10
#include <D3DX10math.h>

//TODO
//create base types that implement operator and functions for all types, and proxy time for renderer specific definitions
//expand types for Vec2,3,4 to a standard

typedef D3DXVECTOR2 Vec2;
typedef D3DXVECTOR3 Vec3;
typedef D3DXVECTOR4 Vec4;

class Quat : public D3DXQUATERNION {
public:
	FORCE_INLINE static Quat Identity(){
		return *reinterpret_cast<Quat*>( D3DXQuaternionIdentity(&Quat()) );
	}

	FORCE_INLINE static Quat fromYawPitchRoll(Real yaw, Real pitch, Real roll){
		return *reinterpret_cast<Quat*>( D3DXQuaternionRotationYawPitchRoll(&Quat(),yaw, pitch, roll) );
	}

	FORCE_INLINE Quat& operator+= (const Quat& other){
		return *this += other;
	}

	FORCE_INLINE Quat& operator+ (const Quat& other){
		return *this + other;
	}

	FORCE_INLINE Quat& operator* (const Quat& other){
		*this *= other;
		return *this;
	}

	//TODO
	//Add other functions
};

struct Mat44 : public D3DXMATRIX {
	FORCE_INLINE static Mat44 Identity(){
		return *reinterpret_cast<Mat44*>( D3DXMatrixIdentity(&Mat44()) );
	}

	//TODO
	//Add other functions
};

#endif

#endif