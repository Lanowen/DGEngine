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


#ifndef PLANE_H
#define PLANE_H

/** \addtogroup foundation
@{
*/

#include "Math.h"
#include "Vec3.h"

/**
\brief Representation of a plane.

 Plane equation used: n.dot(v) + d = 0
*/
class Plane
{
public:
	/**
	\brief Constructor
	*/
	 FORCE_INLINE Plane()
	{
	}

	/**
	\brief Constructor from a normal and a distance
	*/
	 FORCE_INLINE Plane(Real nx, Real ny, Real nz, Real distance)
		: n(nx, ny, nz)
		, d(distance)
	{
	}

	/**
	\brief Constructor from a normal and a distance
	*/
	 FORCE_INLINE Plane(const Vec3& normal, Real distance) 
		: n(normal)
		, d(distance)
	{
	}


	/**
	\brief Constructor from a point on the plane and a normal
	*/
	 FORCE_INLINE Plane(const Vec3& point, const Vec3& normal)
		: n(normal)		
		, d(-point.dot(n))		// p satisfies normal.dot(p) + d = 0
	{
	}

	/**
	\brief Constructor from three points
	*/
	 FORCE_INLINE Plane(const Vec3& p0, const Vec3& p1, const Vec3& p2)
	{
		n = (p1 - p0).cross(p2 - p0).getNormalized();
		d = -p0.dot(n);
	}

	 FORCE_INLINE Real distance(const Vec3& p) const
	{
		return p.dot(n) + d;
	}

	 FORCE_INLINE bool contains(const Vec3& p) const
	{
		return Abs(distance(p)) < (1.0e-7f);
	}

	/**
	\brief projects p into the plane
	*/
	 FORCE_INLINE Vec3 project(const Vec3 & p) const
	{
		return p - n * distance(p);
	}

	/**
	\brief find an arbitrary point in the plane
	*/
	 FORCE_INLINE Vec3 pointInPlane() const
	{
		return -n*d;
	}

	/**
	\brief equivalent plane with unit normal
	*/

	 FORCE_INLINE void normalize()
	{
		Real denom = 1.0f / n.magnitude();
		n *= denom;
		d *= denom;
	}


	Vec3	n;			//!< The normal to the plane
	Real	d;			//!< The distance from the origin
};

/** @} */
#endif

