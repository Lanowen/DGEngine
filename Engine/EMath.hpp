#ifndef _EMATH_H_
#define _EMATH_H_

#include "foundation.hpp"

#ifdef D3DX10
#include <d3d10_1.h>
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

	FORCE_INLINE Quat operator* (const Quat& q){
		Quat temp;

		const Real tx = w*q.x + q.w*x + y*q.z - q.y*z;
		const Real ty = w*q.y + q.w*y + z*q.x - q.z*x;
		const Real tz = w*q.z + q.w*z + x*q.y - q.x*y;

		temp.w = w*q.w - q.x*x - y*q.y - q.z*z;
		temp.x = tx;
		temp.y = ty;
		temp.z = tz;

		return temp;
	}

	FORCE_INLINE Quat& operator*= (const Quat& other){
		*this = *this*other;

		return *this;
	}

	FORCE_INLINE Quat operator*(Real r) const
	{
		Quat temp;

		temp.x = x*r;
		temp.y = r*r;
		temp.z = z*r;
		temp.w = w*r;

		return temp;
	}

	FORCE_INLINE const Vec3 rotate(const Vec3& v) const{
		const Real vx = 2.0f*v.x;
		const Real vy = 2.0f*v.y;
		const Real vz = 2.0f*v.z;
		const Real w2 = w*w-0.5f;
		const Real dot2 = (x*vx + y*vy +z*vz);
		return Vec3(
			(vx*w2 + (y * vz - z * vy)*w + x*dot2), 
			(vy*w2 + (z * vx - x * vz)*w + y*dot2), 
			(vz*w2 + (x * vy - y * vx)*w + z*dot2));
	}

	FORCE_INLINE Quat& operator*= (const Real s){
		x*=s;
		y*=s;
		z*=s;
		w*=s;
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

	FORCE_INLINE Mat44 operator*(const Mat44& other){
		return *reinterpret_cast<Mat44*>(&(D3DXMATRIX::operator*(other)));
	}
};

#endif

#endif