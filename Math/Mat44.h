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


#ifndef MAT44_H
#define MAT44_H
/** \addtogroup foundation
@{
*/

#include "Quat.h"
#include "Vec4.h"
#include "Mat33.h"
#include "Transform.h"

#ifndef DOXYGEN

/*!
\brief 4x4 matrix class

This class is layout-compatible with D3D and OpenGL matrices. More notes on layout are given in the Mat33

@see Mat33 Transform
*/

class Mat44
{
public:
	//! Default constructor
	INLINE Mat44()
	{}

	//! Construct from four 4-vectors
	Mat44(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4 &col3)
		: column0(col0), column1(col1), column2(col2), column3(col3)
	{}

	//! Construct from three base vectors and a translation
	Mat44(const Vec3& column0, const Vec3& column1, const Vec3& column2, const Vec3& column3)
		: column0(column0,0), column1(column1,0), column2(column2,0), column3(column3,1)
	{}

	//! Construct from float[16]
	explicit INLINE Mat44(Real values[]):
		column0(values[0],values[1],values[2], values[3]),
		column1(values[4],values[5],values[6], values[7]),
		column2(values[8],values[9],values[10], values[11]),
		column3(values[12], values[13], values[14], values[15])
	{
	}

	//! Construct from a quaternion
	explicit INLINE Mat44(const Quat& q)
	{
		const Real x = q.x;
		const Real y = q.y;
		const Real z = q.z;
		const Real w = q.w;

		const Real x2 = x + x;
		const Real y2 = y + y;
		const Real z2 = z + z;

		const Real xx = x2*x;
		const Real yy = y2*y;
		const Real zz = z2*z;

		const Real xy = x2*y;
		const Real xz = x2*z;
		const Real xw = x2*w;

		const Real yz = y2*z;
		const Real yw = y2*w;
		const Real zw = z2*w;

		column0 = Vec4(1.0f - yy - zz, xy + zw, xz - yw, 0.0f);
		column1 = Vec4(xy - zw, 1.0f - xx - zz, yz + xw, 0.0f);
		column2 = Vec4(xz + yw, yz - xw, 1.0f - xx - yy, 0.0f);
		column3 = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	//! Construct from a diagonal vector
	explicit INLINE Mat44(const Vec4& diagonal):
		column0(diagonal.x,0.0f,0.0f,0.0f),
		column1(0.0f,diagonal.y,0.0f,0.0f),
		column2(0.0f,0.0f,diagonal.z,0.0f),
		column3(0.0f,0.0f,0.0f,diagonal.w)
	{
	}

	Mat44(const Mat33& orientation, const Vec3& position):
		column0(orientation.column0,0.0f),
		column1(orientation.column1,0.0f),
		column2(orientation.column2,0.0f),
		column3(position,1)
	{
	}
		
	Mat44(const Transform& t)
	{
		*this = Mat44(Mat33(t.q), t.p);
	}

	//! Copy constructor
	INLINE Mat44(const Mat44& other)
		: column0(other.column0), column1(other.column1), column2(other.column2), column3(other.column3)
	{}

	//! Assignment operator
	INLINE const Mat44& operator=(const Mat44& other)
	{
		column0 = other.column0;
		column1 = other.column1;
		column2 = other.column2;
		column3 = other.column3;
		return *this;
	}

	INLINE static Mat44 createIdentity()
	{
		return Mat44(
			Vec4(1.0f,0.0f,0.0f,0.0f),
			Vec4(0.0f,1.0f,0.0f,0.0f),
			Vec4(0.0f,0.0f,1.0f,0.0f),
			Vec4(0.0f,0.0f,0.0f,1.0f));
	}


	INLINE static Mat44 createZero()
	{
		return Mat44(Vec4(0.0f), Vec4(0.0f), Vec4(0.0f), Vec4(0.0f));
	}

	//! Get transposed matrix
	INLINE Mat44 getTranspose() const
	{
		return Mat44(Vec4(column0.x, column1.x, column2.x, column3.x),
			         Vec4(column0.y, column1.y, column2.y, column3.y),
					 Vec4(column0.z, column1.z, column2.z, column3.z),
				     Vec4(column0.w, column1.w, column2.w, column3.w));
	}

	//! Unary minus
	INLINE Mat44 operator-() const
	{
		return Mat44(-column0, -column1, -column2, -column3);
	}

	//! Add
	INLINE Mat44 operator+(const Mat44& other) const
	{
		return Mat44( column0+other.column0,
					  column1+other.column1,
					  column2+other.column2,
					  column3+other.column3);
	}

	//! Subtract
	INLINE Mat44 operator-(const Mat44& other) const
	{
		return Mat44( column0-other.column0,
					  column1-other.column1,
					  column2-other.column2,
					  column3-other.column3);
	}

	//! Scalar multiplication
	INLINE Mat44 operator*(Real scalar) const
	{
		return Mat44(column0*scalar, column1*scalar, column2*scalar, column3*scalar);
	}

	friend Mat44 operator*(Real, const Mat44&);

	//! Matrix multiplication
	INLINE Mat44 operator*(const Mat44& other) const
	{
		//Rows from this <dot> columns from other
		//column0 = transform(other.column0) etc
		return Mat44(transform(other.column0), transform(other.column1), transform(other.column2), transform(other.column3));
	}

	// a <op>= b operators

	//! Equals-add
	INLINE Mat44& operator+=(const Mat44& other)
	{
		column0 += other.column0;
		column1 += other.column1;
		column2 += other.column2;
		column3 += other.column3;
		return *this;
	}

	//! Equals-sub
	INLINE Mat44& operator-=(const Mat44& other)
	{
		column0 -= other.column0;
		column1 -= other.column1;
		column2 -= other.column2;
		column3 -= other.column3;
		return *this;
	}

	//! Equals scalar multiplication
	INLINE Mat44& operator*=(Real scalar)
	{
		column0 *= scalar;
		column1 *= scalar;
		column2 *= scalar;
		column3 *= scalar;
		return *this;
	}

	//! Element access, mathematical way!
	FORCE_INLINE Real operator()(unsigned int row, unsigned int col) const
	{
		return (*this)[col][row];
	}

	//! Element access, mathematical way!
	FORCE_INLINE Real& operator()(unsigned int row, unsigned int col)
	{
		return (*this)[col][row];
	}
	
	//! Transform vector by matrix, equal to v' = M*v
	INLINE Vec4 transform(const Vec4& other) const
	{
		return column0*other.x + column1*other.y + column2*other.z + column3*other.w;
	}

	//! Transform vector by matrix, equal to v' = M*v
	INLINE Vec3 transform(const Vec3& other) const
	{
		return transform(Vec4(other,1)).getXYZ();
	}

	//! Rotate vector by matrix, equal to v' = M*v
	INLINE Vec4 rotate(const Vec4& other) const
	{
		return column0*other.x + column1*other.y + column2*other.z;// + column3*0;
	}

	//! Rotate vector by matrix, equal to v' = M*v
	INLINE Vec3 rotate(const Vec3& other) const
	{
		return rotate(Vec4(other,1)).getXYZ();
	}


	INLINE Vec3 getBasis(int num) const
	{
		ASSERT(num>=0 && num<3);
		return (&column0)[num].getXYZ();
	}

	INLINE Vec3 getPosition() const
	{
		return column3.getXYZ();
	}

	INLINE void setPosition(const Vec3& position)
	{
		column3.x = position.x;
		column3.y = position.y;
		column3.z = position.z;
	}

	FORCE_INLINE const Real* front() const
	{
		return &column0.x;
	}

	FORCE_INLINE			Vec4& operator[](int num)			{return (&column0)[num];}
	FORCE_INLINE const	Vec4& operator[](int num)	const	{return (&column0)[num];}

	INLINE void	scale(const Vec4& p)
	{
		column0 *= p.x;
		column1 *= p.y;
		column2 *= p.z;
		column3 *= p.w;
	}

	INLINE Mat44 inverseRT(void) const
	{
		Vec3 r0(column0.x, column1.x, column2.x),
			r1(column0.y, column1.y, column2.y),
			r2(column0.z, column1.z, column2.z);

		return Mat44(r0, r1, r2, -(r0 * column3.x + r1 * column3.y + r2 * column3.z));
	}

	INLINE bool isFinite() const
	{
		return column0.isFinite() && column1.isFinite() && column2.isFinite() && column3.isFinite();
	}


	//Data, see above for format!

	Vec4 column0, column1, column2, column3; //the four base vectors
};

// implementation from Transform.h
FORCE_INLINE Transform::Transform(const Mat44& m)
{
	Vec3 column0  = Vec3(m.column0.x, m.column0.y, m.column0.z);
	Vec3 column1  = Vec3(m.column1.x, m.column1.y, m.column1.z);
	Vec3 column2  = Vec3(m.column2.x, m.column2.y, m.column2.z);

	q = Quat(Mat33(column0, column1, column2));
	p = Vec3(m.column3.x, m.column3.y, m.column3.z);
}

/** @} */
#endif // MAT44_H
