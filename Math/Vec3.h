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


#ifndef VEC3_H
#define VEC3_H

/** \addtogroup foundation
@{
*/

#include "PMath.h"


/**
\brief 3 Element vector class.

This is a vector class with public data members.
This is not nice but it has become such a standard that hiding the xyz data members
makes it difficult to reuse external code that assumes that these are public in the library.
The vector class can be made to use float or double precision by appropriately defining Real.
This has been chosen as a cleaner alternative to a template class.
*/
class Vec3
{
public:

	/**
	\brief default constructor leaves data uninitialized.
	*/
	FORCE_INLINE Vec3() {}

	/**
	\brief Assigns scalar parameter to all elements.

	Useful to initialize to zero or one.

	\param[in] a Value to assign to elements.
	*/
	explicit FORCE_INLINE Vec3(Real a): x(a), y(a), z(a) {}

	/**
	\brief Initializes from 3 scalar parameters.

	\param[in] nx Value to initialize X component.
	\param[in] ny Value to initialize Y component.
	\param[in] nz Value to initialize Z component.
	*/
	FORCE_INLINE Vec3(Real nx, Real ny, Real nz): x(nx), y(ny), z(nz) {}

	/**
	\brief Copy ctor.
	*/
	FORCE_INLINE Vec3(const Vec3& v): x(v.x), y(v.y), z(v.z) {}

	//Operators

	/**
	\brief Assignment operator
	*/
	FORCE_INLINE	Vec3&	operator=(const Vec3& p)			{ x = p.x; y = p.y; z = p.z;	return *this;		}

	/**
	\brief element access
	*/
	FORCE_INLINE Real& operator[](int index)					{ ASSERT(index>=0 && index<=2); return (&x)[index]; }

	/**
	\brief element access
	*/
	FORCE_INLINE const Real& operator[](int index) const		{ ASSERT(index>=0 && index<=2); return (&x)[index]; }

	/**
	\brief returns true if the two vectors are exactly equal.
	*/
	FORCE_INLINE bool operator==(const Vec3&v) const	{ return x == v.x && y == v.y && z == v.z; }

	/**
	\brief returns true if the two vectors are not exactly equal.
	*/
	FORCE_INLINE bool operator!=(const Vec3&v) const	{ return x != v.x || y != v.y || z != v.z; }

	/**
	\brief tests for exact zero vector
	*/
	FORCE_INLINE bool isZero()	const					{ return x==0.0f && y==0.0f && z == 0.0f;			}

	/**
	\brief returns true if all 3 elems of the vector are finite (not NAN or INF, etc.)
	*/
	INLINE bool isFinite() const
	{
		return IsFinite(x) && IsFinite(y) && IsFinite(z);
	}

	/**
	\brief is normalized - used by API parameter validation
	*/
	FORCE_INLINE bool isNormalized() const
	{
		const float unitTolerance = Real(1e-4);
		return isFinite() && Abs(magnitude()-1)<unitTolerance;
	}

	/**
	\brief returns the squared magnitude

	Avoids calling Sqrt()!
	*/
	FORCE_INLINE Real magnitudeSquared() const		{	return x * x + y * y + z * z;					}

	/**
	\brief returns the magnitude
	*/
	FORCE_INLINE Real magnitude() const				{	return Sqrt(magnitudeSquared());		}

	/**
	\brief negation
	*/
	FORCE_INLINE Vec3 operator -() const
	{
		return Vec3(-x, -y, -z);
	}

	/**
	\brief vector addition
	*/
	FORCE_INLINE Vec3 operator +(const Vec3& v) const		{	return Vec3(x + v.x, y + v.y, z + v.z);	}

	/**
	\brief vector difference
	*/
	FORCE_INLINE Vec3 operator -(const Vec3& v) const		{	return Vec3(x - v.x, y - v.y, z - v.z);	}

	/**
	\brief scalar post-multiplication
	*/
	FORCE_INLINE Vec3 operator *(Real f) const				{	return Vec3(x * f, y * f, z * f);			}

	/**
	\brief scalar division
	*/
	FORCE_INLINE Vec3 operator /(Real f) const
	{
		f = Real(1) / f;	// PT: inconsistent notation with operator /=
		return Vec3(x * f, y * f, z * f);
	}

	/**
	\brief vector addition
	*/
	FORCE_INLINE Vec3& operator +=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	/**
	\brief vector difference
	*/
	FORCE_INLINE Vec3& operator -=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	/**
	\brief scalar multiplication
	*/
	FORCE_INLINE Vec3& operator *=(Real f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	/**
	\brief scalar division
	*/
	FORCE_INLINE Vec3& operator /=(Real f)
	{
		f = 1.0f/f;	// PT: inconsistent notation with operator /
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	/**
	\brief returns the scalar product of this and other.
	*/
	FORCE_INLINE Real dot(const Vec3& v) const		
	{	
		return x * v.x + y * v.y + z * v.z;				
	}

	/**
	\brief cross product
	*/
	FORCE_INLINE Vec3 cross(const Vec3& v) const
	{
		return Vec3(y * v.z - z * v.y, 
					  z * v.x - x * v.z, 
					  x * v.y - y * v.x);
	}

	/** return a unit vector */

	FORCE_INLINE Vec3 getNormalized() const
	{
		const Real m = magnitudeSquared();
		return m>0 ? *this * RecipSqrt(m) : Vec3(0,0,0);
	}

	/**
	\brief normalizes the vector in place
	*/
	FORCE_INLINE Real normalize()
	{
		const Real m = magnitude();
		if (m>0) 
			*this /= m;
		return m;
	}

	/**
	\brief normalizes the vector in place. Does nothing if vector magnitude is under NORMALIZATION_EPSILON.
	Returns vector magnitude if >= NORMALIZATION_EPSILON and 0.0f otherwise.
	*/
	FORCE_INLINE Real normalizeSafe()
	{
		const Real mag = magnitude();
		if (mag < NORMALIZATION_EPSILON)
			return 0.0f;
		*this *= Real(1) / mag;
		return mag;
	}

	/**
	\brief normalizes the vector in place. Asserts if vector magnitude is under NORMALIZATION_EPSILON.
	returns vector magnitude.
	*/
	FORCE_INLINE Real normalizeFast()
	{
		const Real mag = magnitude();
		ASSERT(mag >= NORMALIZATION_EPSILON);
		*this *= Real(1) / mag;
		return mag;
	}

	/**
	\brief a[i] * b[i], for all i.
	*/
	FORCE_INLINE Vec3 multiply(const Vec3& a) const
	{
		return Vec3(x*a.x, y*a.y, z*a.z);
	}

	/**
	\brief element-wise minimum
	*/
	FORCE_INLINE Vec3 minimum(const Vec3& v) const
	{ 
		return Vec3(Min(x, v.x), Min(y,v.y), Min(z,v.z));	
	}

	/**
	\brief returns MIN(x, y, z);
	*/
	FORCE_INLINE float minElement()	const
	{
		return Min(x, Min(y, z));
	}
	
	/**
	\brief element-wise maximum
	*/
	FORCE_INLINE Vec3 maximum(const Vec3& v) const
	{ 
		return Vec3(Max(x, v.x), Max(y,v.y), Max(z,v.z));	
	} 

	/**
	\brief returns MAX(x, y, z);
	*/
	FORCE_INLINE float maxElement()	const
	{
		return Max(x, Max(y, z));
	}

	Real x,y,z;
};

static FORCE_INLINE Vec3 operator *(Real f, const Vec3& v)
{
	return Vec3(f * v.x, f * v.y, f * v.z);
}

/** @} */
#endif // VEC3_H
