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


#ifndef VEC4_H
#define VEC4_H
/** \addtogroup foundation
@{
*/
#include "Math.h"
#include "Vec3.h"
#include "Assert.h"


/**
\brief 4 Element vector class.

This is a vector class with public data members.
This is not nice but it has become such a standard that hiding the xyz data members
makes it difficult to reuse external code that assumes that these are public in the library.
The vector class can be made to use float or double precision by appropriately defining Real.
This has been chosen as a cleaner alternative to a template class.
*/

class Vec4
{
public:

	/**
	\brief default constructor leaves data uninitialized.
	*/
	INLINE Vec4() {}

	/**
	\brief Assigns scalar parameter to all elements.

	Useful to initialize to zero or one.

	\param[in] a Value to assign to elements.
	*/
	explicit INLINE Vec4(Real a): x(a), y(a), z(a), w(a) {}

	/**
	\brief Initializes from 3 scalar parameters.

	\param[in] nx Value to initialize X component.
	\param[in] ny Value to initialize Y component.
	\param[in] nz Value to initialize Z component.
	\param[in] nw Value to initialize W component.
	*/
	INLINE Vec4(Real nx, Real ny, Real nz, Real nw): x(nx), y(ny), z(nz), w(nw) {}


	/**
	\brief Initializes from 3 scalar parameters.

	\param[in] v Value to initialize the X, Y, and Z components.
	\param[in] nw Value to initialize W component.
	*/
	INLINE Vec4(const Vec3& v, Real nw): x(v.x), y(v.y), z(v.z), w(nw) {}


	/**
	\brief Initializes from an array of scalar parameters.

	\param[in] v Value to initialize with.
	*/
	explicit INLINE Vec4(const Real v[]): x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

	/**
	\brief Copy ctor.
	*/
	INLINE Vec4(const Vec4& v): x(v.x), y(v.y), z(v.z), w(v.w) {}

	//Operators

	/**
	\brief Assignment operator
	*/
	INLINE	Vec4&	operator=(const Vec4& p)			{ x = p.x; y = p.y; z = p.z; w = p.w;	return *this;		}

	/**
	\brief element access
	*/
	INLINE Real& operator[](int index)				{ ASSERT(index>=0 && index<=3); return (&x)[index]; }

	/**
	\brief element access
	*/
	INLINE const Real& operator[](int index) const	{ ASSERT(index>=0 && index<=3); return (&x)[index]; }

	/**
	\brief returns true if the two vectors are exactly equal.
	*/
	INLINE bool operator==(const Vec4&v) const	{ return x == v.x && y == v.y && z == v.z && w == v.w; }

	/**
	\brief returns true if the two vectors are not exactly equal.
	*/
	INLINE bool operator!=(const Vec4&v) const	{ return x != v.x || y != v.y || z != v.z || w!= v.w; }

	/**
	\brief tests for exact zero vector
	*/
	INLINE bool isZero()	const					{ return x==0 && y==0 && z == 0 && w == 0;			}

	/**
	\brief returns true if all 3 elems of the vector are finite (not NAN or INF, etc.)
	*/
	INLINE bool isFinite() const
	{
		return IsFinite(x) && IsFinite(y) && IsFinite(z) && IsFinite(w);
	}

	/**
	\brief is normalized - used by API parameter validation
	*/
	INLINE bool isNormalized() const
	{
		const float unitTolerance = Real(1e-4);
		return isFinite() && Abs(magnitude()-1)<unitTolerance;
	}


	/**
	\brief returns the squared magnitude

	Avoids calling Sqrt()!
	*/
	INLINE Real magnitudeSquared() const		{	return x * x + y * y + z * z + w * w;		}

	/**
	\brief returns the magnitude
	*/
	INLINE Real magnitude() const				{	return Sqrt(magnitudeSquared());		}

	/**
	\brief negation
	*/
	INLINE Vec4 operator -() const
	{
		return Vec4(-x, -y, -z, -w);
	}

	/**
	\brief vector addition
	*/
	INLINE Vec4 operator +(const Vec4& v) const		{	return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);	}

	/**
	\brief vector difference
	*/
	INLINE Vec4 operator -(const Vec4& v) const		{	return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);	}

	/**
	\brief scalar post-multiplication
	*/

	INLINE Vec4 operator *(Real f) const				{	return Vec4(x * f, y * f, z * f, w * f);		}

	/**
	\brief scalar division
	*/
	INLINE Vec4 operator /(Real f) const
	{
		f = Real(1) / f; 
		return Vec4(x * f, y * f, z * f, w * f);
	}

	/**
	\brief vector addition
	*/
	INLINE Vec4& operator +=(const Vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	
	/**
	\brief vector difference
	*/
	INLINE Vec4& operator -=(const Vec4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	/**
	\brief scalar multiplication
	*/
	INLINE Vec4& operator *=(Real f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	/**
	\brief scalar division
	*/
	INLINE Vec4& operator /=(Real f)
	{
		f = 1.0f/f;
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}

	/**
	\brief returns the scalar product of this and other.
	*/
	INLINE Real dot(const Vec4& v) const		
	{	
		return x * v.x + y * v.y + z * v.z + w * v.w;				
	}

	/** return a unit vector */

	INLINE Vec4 getNormalized() const
	{
		Real m = magnitudeSquared();
		return m>0 ? *this * RecipSqrt(m) : Vec4(0,0,0,0);
	}


	/**
	\brief normalizes the vector in place
	*/
	INLINE Real normalize()
	{
		Real m = magnitude();
		if (m>0) 
			*this /= m;
		return m;
	}

	/**
	\brief a[i] * b[i], for all i.
	*/
	INLINE Vec4 multiply(const Vec4& a) const
	{
		return Vec4(x*a.x, y*a.y, z*a.z, w*a.w);
	}

	/**
	\brief element-wise minimum
	*/
	INLINE Vec4 minimum(const Vec4& v) const
	{ 
		return Vec4(Min(x, v.x), Min(y,v.y), Min(z,v.z), Min(w,v.w));	
	}

	/**
	\brief element-wise maximum
	*/
	INLINE Vec4 maximum(const Vec4& v) const
	{ 
		return Vec4(Max(x, v.x), Max(y,v.y), Max(z,v.z), Max(w,v.w));	
	} 

	INLINE Vec3 getXYZ() const
	{
		return Vec3(x,y,z);
	}

	/**
	\brief set vector elements to zero
	*/
	INLINE void setZero() {	x = y = z = w = Real(0); }

	Real x,y,z,w;
};


static INLINE Vec4 operator *(Real f, const Vec4& v)
{
	return Vec4(f * v.x, f * v.y, f * v.z, f * v.w);
}

/** @} */
#endif // VEC4_H
