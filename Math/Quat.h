// This code contains NVIDIA Confidential Information and is disclosed to you 
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and 
// any modifications thereto. Any use, reproduction, disclosure, or 
// distribution of this software and related documentation without an express 
// license agreement from NVIDIA Corporation is strictly prohibited.
// 
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2012 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  



#ifndef QUAT_H
#define QUAT_H

/** \addtogroup foundation
@{
*/

#include "Vec3.h"

/**
\brief This is a quaternion class. For more information on quaternion mathematics
consult a mathematics source on complex numbers.

*/

class Quat
{
public:
	/**
	\brief Default constructor, does not do any initialization.
	*/
	FORCE_INLINE Quat()	{	}


	/**
	\brief Constructor.  Take note of the order of the elements!
	*/
	FORCE_INLINE Quat(Real nx, Real ny, Real nz, Real nw) : x(nx),y(ny),z(nz),w(nw) {}

	/**
	\brief Creates from angle-axis representation.

	Axis must be normalized!

	Angle is in radians!

	<b>Unit:</b> Radians
	*/
	INLINE Quat(Real angleRadians, const Vec3& unitAxis)
	{
		ASSERT(Abs(1.0f-unitAxis.magnitude())<1e-3f);
		const Real a = angleRadians * 0.5f;
		const Real s = Sin(a);
		w = Cos(a);
		x = unitAxis.x * s;
		y = unitAxis.y * s;
		z = unitAxis.z * s;
	}

	/**
	\brief Copy ctor.
	*/
	FORCE_INLINE Quat(const Quat& v): x(v.x), y(v.y), z(v.z), w(v.w) {}

	/**
	\brief Creates from orientation matrix.

	\param[in] m Rotation matrix to extract quaternion from.
	*/
	INLINE explicit Quat(const Mat33& m); /* defined in Mat33.h */

	/**
	\brief returns true if all elements are finite (not NAN or INF, etc.)
	*/
	bool isFinite() const
	{
		return IsFinite(x) 
			&& IsFinite(y) 
			&& IsFinite(z)
			&& IsFinite(w);
	}


	/**
	\brief returns true if finite and magnitude is close to unit
	*/

	bool isUnit() const
	{
		const Real unitTolerance = Real(1e-4);
		return isFinite() && Abs(magnitude()-1)<unitTolerance;
	}


	/**
	\brief returns true if finite and magnitude is reasonably close to unit to allow for some accumulation of error vs isValid
	*/

	bool isSane() const
	{
	      const Real unitTolerance = Real(1e-2);
	      return isFinite() && Abs(magnitude()-1)<unitTolerance;
	}

	/**
	\brief converts this quaternion to angle-axis representation
	*/

	INLINE void toRadiansAndUnitAxis(Real& angle, Vec3& axis) const
	{
		const Real quatEpsilon = (Real(1.0e-8f));
		const Real s2 = x*x+y*y+z*z;
		if(s2<quatEpsilon*quatEpsilon)  // can't extract a sensible axis
		{
			angle = 0;
			axis = Vec3(1,0,0);
		}
		else
		{
			const Real s = RecipSqrt(s2);
			axis = Vec3(x,y,z) * s; 
			angle = w<quatEpsilon ? Pi : Atan2(s2*s, w) * 2;
		}

	}

	/**
	\brief Gets the angle between this quat and the identity quaternion.

	<b>Unit:</b> Radians
	*/
	INLINE Real getAngle() const
	{
		return Acos(w) * Real(2);
	}


	/**
	\brief Gets the angle between this quat and the argument

	<b>Unit:</b> Radians
	*/
	INLINE Real getAngle(const Quat& q) const
	{
		return Acos(dot(q)) * Real(2);
	}


	/**
	\brief This is the squared 4D vector length, should be 1 for unit quaternions.
	*/
	FORCE_INLINE Real magnitudeSquared() const
	{
		return x*x + y*y + z*z + w*w;
	}

	/**
	\brief returns the scalar product of this and other.
	*/
	FORCE_INLINE Real dot(const Quat& v) const
	{
		return x * v.x + y * v.y + z * v.z  + w * v.w;
	}

	INLINE Quat getNormalized() const
	{
		const Real s = 1.0f/magnitude();
		return Quat(x*s, y*s, z*s, w*s);
	}


	INLINE float magnitude() const
	{
		return Sqrt(magnitudeSquared());
	}

	//modifiers:
	/**
	\brief maps to the closest unit quaternion.
	*/
	INLINE Real normalize()											// convert this Quat to a unit quaternion
	{
		const Real mag = magnitude();
		if (mag)
		{
			const Real imag = Real(1) / mag;

			x *= imag;
			y *= imag;
			z *= imag;
			w *= imag;
		}
		return mag;
	}

	/*
	\brief returns the conjugate.

	\note for unit quaternions, this is the inverse.
	*/
	INLINE Quat getConjugate() const
	{
		return Quat(-x,-y,-z,w);
	}

	/*
	\brief returns imaginary part.
	*/
	INLINE Vec3 getImaginaryPart() const
	{
		return Vec3(x,y,z);
	}

	/** brief computes rotation of x-axis */
	FORCE_INLINE Vec3 getBasisVector0()	const
	{	
//		return rotate(Vec3(1,0,0));
		const F32 x2 = x*2.0f;
		const F32 w2 = w*2.0f;
		return Vec3(	(w * w2) - 1.0f + x*x2,
						(z * w2)        + y*x2,
						(-y * w2)       + z*x2);
	}
	
	/** brief computes rotation of y-axis */
	FORCE_INLINE Vec3 getBasisVector1()	const 
	{	
//		return rotate(Vec3(0,1,0));
		const F32 y2 = y*2.0f;
		const F32 w2 = w*2.0f;
		return Vec3(	(-z * w2)       + x*y2,
						(w * w2) - 1.0f + y*y2,
						(x * w2)        + z*y2);
	}


	/** brief computes rotation of z-axis */
	FORCE_INLINE Vec3 getBasisVector2() const	
	{	
//		return rotate(Vec3(0,0,1));
		const F32 z2 = z*2.0f;
		const F32 w2 = w*2.0f;
		return Vec3(	(y * w2)        + x*z2,
						(-x * w2)       + y*z2,
						(w * w2) - 1.0f + z*z2);
	}

	/**
	rotates passed vec by this (assumed unitary)
	*/
	FORCE_INLINE const Vec3 rotate(const Vec3& v) const
//	INLINE const Vec3 rotate(const Vec3& v) const
	{
		const F32 vx = 2.0f*v.x;
		const F32 vy = 2.0f*v.y;
		const F32 vz = 2.0f*v.z;
		const F32 w2 = w*w-0.5f;
		const F32 dot2 = (x*vx + y*vy +z*vz);
		return Vec3
		(
			(vx*w2 + (y * vz - z * vy)*w + x*dot2), 
			(vy*w2 + (z * vx - x * vz)*w + y*dot2), 
			(vz*w2 + (x * vy - y * vx)*w + z*dot2)
		);
		/*
		const Vec3 qv(x,y,z);
		return (v*(w*w-0.5f) + (qv.cross(v))*w + qv*(qv.dot(v)))*2;
		*/
	}

	/**
	inverse rotates passed vec by this (assumed unitary)
	*/
	FORCE_INLINE const Vec3 rotateInv(const Vec3& v) const
//	INLINE const Vec3 rotateInv(const Vec3& v) const
	{
		const F32 vx = 2.0f*v.x;
		const F32 vy = 2.0f*v.y;
		const F32 vz = 2.0f*v.z;
		const F32 w2 = w*w-0.5f;
		const F32 dot2 = (x*vx + y*vy +z*vz);
		return Vec3
		(
			(vx*w2 - (y * vz - z * vy)*w + x*dot2), 
			(vy*w2 - (z * vx - x * vz)*w + y*dot2), 
			(vz*w2 - (x * vy - y * vx)*w + z*dot2)
		);
//		const Vec3 qv(x,y,z);
//		return (v*(w*w-0.5f) - (qv.cross(v))*w + qv*(qv.dot(v)))*2;
	}

	/**
	\brief Assignment operator
	*/
	FORCE_INLINE	Quat&	operator=(const Quat& p)			{ x = p.x; y = p.y; z = p.z; w = p.w;	return *this;		}

	FORCE_INLINE Quat& operator*= (const Quat& q)
	{
		const Real tx = w*q.x + q.w*x + y*q.z - q.y*z;
		const Real ty = w*q.y + q.w*y + z*q.x - q.z*x;
		const Real tz = w*q.z + q.w*z + x*q.y - q.x*y;

		w = w*q.w - q.x*x - y*q.y - q.z*z;
		x = tx;
		y = ty;
		z = tz;

		return *this;
	}

	FORCE_INLINE Quat& operator+= (const Quat& q)
	{
		x+=q.x;
		y+=q.y;
		z+=q.z;
		w+=q.w;
		return *this;
	}

	FORCE_INLINE Quat& operator-= (const Quat& q)
	{
		x-=q.x;
		y-=q.y;
		z-=q.z;
		w-=q.w;
		return *this;
	}

	FORCE_INLINE Quat& operator*= (const Real s)
	{
		x*=s;
		y*=s;
		z*=s;
		w*=s;
		return *this;
	}

	/** quaternion multiplication */
	INLINE Quat operator*(const Quat& q) const
	{
		return Quat(w*q.x + q.w*x + y*q.z - q.y*z,
					  w*q.y + q.w*y + z*q.x - q.z*x,
					  w*q.z + q.w*z + x*q.y - q.x*y,
					  w*q.w - x*q.x - y*q.y - z*q.z);
	}

	/** quaternion addition */
	FORCE_INLINE Quat operator+(const Quat& q) const
	{
		return Quat(x+q.x,y+q.y,z+q.z,w+q.w);
	}

	/** quaternion subtraction */
	FORCE_INLINE Quat operator-() const
	{
		return Quat(-x,-y,-z,-w);
	}


	FORCE_INLINE Quat operator-(const Quat& q) const
	{
		return Quat(x-q.x,y-q.y,z-q.z,w-q.w);
	}


	FORCE_INLINE Quat operator*(Real r) const
	{
		return Quat(x*r,y*r,z*r,w*r);
	}

	static INLINE Quat createIdentity() { return Quat(0,0,0,1); }

	/** the quaternion elements */
	Real x,y,z,w;
};

/** @} */
#endif // QUAT_H
