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


#ifndef TRANSFORM_H
#define TRANSFORM_H
/** \addtogroup foundation
  @{
*/

#include "Quat.h"
#include "Plane.h"

/*!
\brief class representing a rigid euclidean transform as a quaternion and a vector
*/

class Transform
{
public:
	Quat q;
	Vec3 p;

//#define TRANSFORM_DEFAULT_CONSTRUCT_NAN

	FORCE_INLINE Transform() 
#ifdef TRANSFORM_DEFAULT_CONSTRUCT_IDENTITY
		: q(0, 0, 0, 1), p(0, 0, 0)
#elif defined(TRANSFORM_DEFAULT_CONSTRUCT_NAN)
#define invalid Sqrt(-1.0f)
		: q(invalid, invalid, invalid, invalid), p(invalid, invalid, invalid)
#undef invalid
#endif
	{
	}

	FORCE_INLINE explicit Transform(const Vec3& position): q(0, 0, 0, 1), p(position)
	{
	}

	FORCE_INLINE explicit Transform(const Quat& orientation): q(orientation), p(0, 0, 0)
	{
		ASSERT(orientation.isSane());
	}

	FORCE_INLINE Transform(const Vec3& p0, const Quat& q0): q(q0), p(p0) 
	{
		ASSERT(q0.isSane());
	}

	FORCE_INLINE explicit Transform(const Mat44& m);	// defined in Mat44.h
	
	FORCE_INLINE Transform operator*(const Transform& x) const
	{
		ASSERT(x.isSane());
		return transform(x);
	}

	FORCE_INLINE Transform getInverse() const
	{
		ASSERT(isFinite());
		return Transform(q.rotateInv(-p),q.getConjugate());
	}


	FORCE_INLINE Vec3 transform(const Vec3& input) const
	{
		ASSERT(isFinite());
		return q.rotate(input) + p;
	}

	FORCE_INLINE Vec3 transformInv(const Vec3& input) const
	{
		ASSERT(isFinite());
		return q.rotateInv(input-p);
	}

	FORCE_INLINE Vec3 rotate(const Vec3& input) const
	{
		ASSERT(isFinite());
		return q.rotate(input);
	}

	FORCE_INLINE Vec3 rotateInv(const Vec3& input) const
	{
		ASSERT(isFinite());
		return q.rotateInv(input);
	}

	//! Transform transform to parent (returns compound transform: first src, then *this)
	FORCE_INLINE Transform transform(const Transform& src) const
	{
		ASSERT(src.isSane());
		ASSERT(isSane());
		// src = [srct, srcr] -> [r*srct + t, r*srcr]
		return Transform(q.rotate(src.p) + p, q*src.q);
	}

	/**
	\brief returns true if finite and q is a unit quaternion
	*/

	bool isValid() const
	{
		return p.isFinite() && q.isFinite() && q.isUnit();
	}

	/**
	\brief returns true if finite and quat magnitude is reasonably close to unit to allow for some accumulation of error vs isValid
	*/

	bool isSane() const
	{
	      return isFinite() && q.isSane();
	}


	/**
	\brief returns true if all elems are finite (not NAN or INF, etc.)
	*/
	FORCE_INLINE bool isFinite() const { return p.isFinite() && q.isFinite(); }

	//! Transform transform from parent (returns compound transform: first src, then this->inverse)
	FORCE_INLINE Transform transformInv(const Transform& src) const
	{
		ASSERT(src.isSane());
		ASSERT(isFinite());
		// src = [srct, srcr] -> [r^-1*(srct-t), r^-1*srcr]
		Quat qinv = q.getConjugate();
		return Transform(qinv.rotate(src.p - p), qinv*src.q);
	}

	static FORCE_INLINE Transform createIdentity() 
	{ 
		return Transform(Vec3(0)); 
	}

	/**
	\brief transform plane
	*/

	FORCE_INLINE Plane transform(const Plane& plane) const
	{
		Vec3 transformedNormal = rotate(plane.n);
		return Plane(transformedNormal, plane.d - p.dot(transformedNormal));
	}

	/**
	\brief inverse-transform plane
	*/

	FORCE_INLINE Plane inverseTransform(const Plane& plane) const
	{
		Vec3 transformedNormal = rotateInv(plane.n);
		return Plane(transformedNormal, plane.d + p.dot(plane.n));
	}


};

/** @} */
#endif // TRANSFORM_H
