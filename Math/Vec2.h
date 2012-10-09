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


#ifndef VEC2_H
#define VEC2_H

/** \addtogroup foundation
@{
*/

#include "Math.h"


/**
\brief 2 Element vector class.

This is a vector class with public data members.
This is not nice but it has become such a standard that hiding the xy data members
makes it difficult to reuse external code that assumes that these are public in the library.
The vector class can be made to use float or double precision by appropriately defining Real.
This has been chosen as a cleaner alternative to a template class.
*/
class Vec2
{
public:

	/**
	\brief default constructor leaves data uninitialized.
	*/
	FORCE_INLINE Vec2() {}

	/**
	\brief Assigns scalar parameter to all elements.

	Useful to initialize to zero or one.

	\param[in] a Value to assign to elements.
	*/
	explicit FORCE_INLINE Vec2(Real a): x(a), y(a) {}

	/**
	\brief Initializes from 2 scalar parameters.

	\param[in] nx Value to initialize X component.
	\param[in] ny Value to initialize Y component.
	*/
	FORCE_INLINE Vec2(Real nx, Real ny): x(nx), y(ny){}

	/**
	\brief Copy ctor.
	*/
	FORCE_INLINE Vec2(const Vec2& v): x(v.x), y(v.y) {}

	//Operators

	/**
	\brief Assignment operator
	*/
	FORCE_INLINE	Vec2&	operator=(const Vec2& p)			{ x = p.x; y = p.y;	return *this;		}

	/**
	\brief element access
	*/
	FORCE_INLINE Real& operator[](int index)					{ ASSERT(index>=0 && index<=1); return (&x)[index]; }

	/**
	\brief element access
	*/
	FORCE_INLINE const Real& operator[](int index) const		{ ASSERT(index>=0 && index<=1); return (&x)[index]; }

	/**
	\brief returns true if the two vectors are exactly equal.
	*/
	FORCE_INLINE bool operator==(const Vec2&v) const	{ return x == v.x && y == v.y; }

	/**
	\brief returns true if the two vectors are not exactly equal.
	*/
	FORCE_INLINE bool operator!=(const Vec2&v) const	{ return x != v.x || y != v.y; }

	/**
	\brief tests for exact zero vector
	*/
	FORCE_INLINE bool isZero()	const					{ return x==0.0f && y==0.0f;			}

	/**
	\brief returns true if all 2 elems of the vector are finite (not NAN or INF, etc.)
	*/
	INLINE bool isFinite() const
	{
		return IsFinite(x) && IsFinite(y);
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
	FORCE_INLINE Real magnitudeSquared() const		{	return x * x + y * y;					}

	/**
	\brief returns the magnitude
	*/
	FORCE_INLINE Real magnitude() const				{	return Sqrt(magnitudeSquared());		}

	/**
	\brief negation
	*/
	FORCE_INLINE Vec2 operator -() const
	{
		return Vec2(-x, -y);
	}

	/**
	\brief vector addition
	*/
	FORCE_INLINE Vec2 operator +(const Vec2& v) const		{	return Vec2(x + v.x, y + v.y);	}

	/**
	\brief vector difference
	*/
	FORCE_INLINE Vec2 operator -(const Vec2& v) const		{	return Vec2(x - v.x, y - v.y);	}

	/**
	\brief scalar post-multiplication
	*/
	FORCE_INLINE Vec2 operator *(Real f) const				{	return Vec2(x * f, y * f);			}

	/**
	\brief scalar division
	*/
	FORCE_INLINE Vec2 operator /(Real f) const
	{
		f = Real(1) / f;	// PT: inconsistent notation with operator /=
		return Vec2(x * f, y * f);
	}

	/**
	\brief vector addition
	*/
	FORCE_INLINE Vec2& operator +=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	
	/**
	\brief vector difference
	*/
	FORCE_INLINE Vec2& operator -=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	/**
	\brief scalar multiplication
	*/
	FORCE_INLINE Vec2& operator *=(Real f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	/**
	\brief scalar division
	*/
	FORCE_INLINE Vec2& operator /=(Real f)
	{
		f = 1.0f/f;	// PT: inconsistent notation with operator /
		x *= f;
		y *= f;
		return *this;
	}

	/**
	\brief returns the scalar product of this and other.
	*/
	FORCE_INLINE Real dot(const Vec2& v) const		
	{	
		return x * v.x + y * v.y;				
	}

	/** return a unit vector */

	FORCE_INLINE Vec2 getNormalized() const
	{
		const Real m = magnitudeSquared();
		return m>0 ? *this * RecipSqrt(m) : Vec2(0,0);
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
	\brief a[i] * b[i], for all i.
	*/
	FORCE_INLINE Vec2 multiply(const Vec2& a) const
	{
		return Vec2(x*a.x, y*a.y);
	}

	/**
	\brief element-wise minimum
	*/
	FORCE_INLINE Vec2 minimum(const Vec2& v) const
	{ 
		return Vec2(Min(x, v.x), Min(y,v.y));	
	}

	/**
	\brief returns MIN(x, y);
	*/
	FORCE_INLINE float minElement()	const
	{
		return Min(x, y);
	}
	
	/**
	\brief element-wise maximum
	*/
	FORCE_INLINE Vec2 maximum(const Vec2& v) const
	{ 
		return Vec2(Max(x, v.x), Max(y,v.y));	
	} 

	/**
	\brief returns MAX(x, y);
	*/
	FORCE_INLINE float maxElement()	const
	{
		return Max(x, y);
	}

	Real x,y;
};

static FORCE_INLINE Vec2 operator *(Real f, const Vec2& v)
{
	return Vec2(f * v.x, f * v.y);
}

/** @} */
#endif // VEC2_H
