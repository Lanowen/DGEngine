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


#ifndef MAT33_H
#define MAT33_H
/** \addtogroup foundation
@{
*/

#include "Vec3.h"
#include "Quat.h"
#include "core.h"

/*!
\brief 3x3 matrix class

Some clarifications, as there have been much confusion about matrix formats etc in the past.

Short:
- Matrix have base vectors in columns (vectors are column matrices, 3x1 matrices).
- Matrix is physically stored in column major format
- Matrices are concaternated from left

Long:
Given three base vectors a, b and c the matrix is stored as
         
|a.x b.x c.x|
|a.y b.y c.y|
|a.z b.z c.z|

Vectors are treated as columns, so the vector v is 

|x|
|y|
|z|

And matrices are applied _before_ the vector (pre-multiplication)
v' = M*v

|x'|   |a.x b.x c.x|   |x|   |a.x*x + b.x*y + c.x*z|
|y'| = |a.y b.y c.y| * |y| = |a.y*x + b.y*y + c.y*z|
|z'|   |a.z b.z c.z|   |z|   |a.z*x + b.z*y + c.z*z|


Physical storage and indexing:
To be compatible with popular 3d rendering APIs (read D3d and OpenGL)
the physical indexing is

|0 3 6|
|1 4 7|
|2 5 8|

index = column*3 + row

which in C++ translates to M[column][row]

The mathematical indexing is M_row,column and this is what is used for _-notation 
so _12 is 1st row, second column and operator(row, column)!

*/
class Mat33
{
public:
	//! Default constructor
	INLINE Mat33()
	{}

	//! Construct from three base vectors
	Mat33(const Vec3& col0, const Vec3& col1, const Vec3& col2)
		: column0(col0), column1(col1), column2(col2)
	{}

	//! Construct from float[9]
	explicit INLINE Mat33(Real values[]):
		column0(values[0],values[1],values[2]),
		column1(values[3],values[4],values[5]),
		column2(values[6],values[7],values[8])
	{
	}

	//! Construct from a quaternion
	explicit FORCE_INLINE Mat33(const Quat& q)
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

		column0 = Vec3(1.0f - yy - zz, xy + zw, xz - yw);
		column1 = Vec3(xy - zw, 1.0f - xx - zz, yz + xw);
		column2 = Vec3(xz + yw, yz - xw, 1.0f - xx - yy);
	}

	//! Copy constructor
	INLINE Mat33(const Mat33& other)
		: column0(other.column0), column1(other.column1), column2(other.column2)
	{}

	//! Assignment operator
	FORCE_INLINE Mat33& operator=(const Mat33& other)
	{
		column0 = other.column0;
		column1 = other.column1;
		column2 = other.column2;
		return *this;
	}

	//! Set to identity matrix
	INLINE static Mat33 createIdentity()
	{
		return Mat33(Vec3(1,0,0), Vec3(0,1,0), Vec3(0,0,1));
	}

	//! Set to zero matrix
	INLINE static Mat33 createZero()
	{
		return Mat33(Vec3(0.0f), Vec3(0.0f), Vec3(0.0f));
	}

	//! Construct from diagonal, off-diagonals are zero.
	INLINE static Mat33 createDiagonal(const Vec3& d)
	{
		return Mat33(Vec3(d.x,0.0f,0.0f), Vec3(0.0f,d.y,0.0f), Vec3(0.0f,0.0f,d.z));
	}


	//! Get transposed matrix
	FORCE_INLINE Mat33 getTranspose() const
	{
		const Vec3 v0(column0.x, column1.x, column2.x);
		const Vec3 v1(column0.y, column1.y, column2.y);
		const Vec3 v2(column0.z, column1.z, column2.z);

		return Mat33(v0,v1,v2);   
	}

	//! Get the Real inverse
	INLINE Mat33 getInverse() const
	{
		const Real det = getDeterminant();
		Mat33 inverse;

		if(det != 0)
		{
			const Real invDet = 1.0f/det;

			inverse.column0[0] = invDet * (column1[1]*column2[2] - column2[1]*column1[2]);							
			inverse.column0[1] = invDet *-(column0[1]*column2[2] - column2[1]*column0[2]);
			inverse.column0[2] = invDet * (column0[1]*column1[2] - column0[2]*column1[1]);

			inverse.column1[0] = invDet *-(column1[0]*column2[2] - column1[2]*column2[0]);
			inverse.column1[1] = invDet * (column0[0]*column2[2] - column0[2]*column2[0]);
			inverse.column1[2] = invDet *-(column0[0]*column1[2] - column0[2]*column1[0]);

			inverse.column2[0] = invDet * (column1[0]*column2[1] - column1[1]*column2[0]);
			inverse.column2[1] = invDet *-(column0[0]*column2[1] - column0[1]*column2[0]);
			inverse.column2[2] = invDet * (column0[0]*column1[1] - column1[0]*column0[1]);

			return inverse;
		}
		else
		{
			return createIdentity();
		}
	}

	//! Get determinant
	INLINE Real getDeterminant() const
	{
		return column0.dot(column1.cross(column2));
	}

	//! Unary minus
	INLINE Mat33 operator-() const
	{
		return Mat33(-column0, -column1, -column2);
	}

	//! Add
	INLINE Mat33 operator+(const Mat33& other) const
	{
		return Mat33( column0+other.column0,
					  column1+other.column1,
					  column2+other.column2);
	}

	//! Subtract
	INLINE Mat33 operator-(const Mat33& other) const
	{
		return Mat33( column0-other.column0,
					  column1-other.column1,
					  column2-other.column2);
	}

	//! Scalar multiplication
	INLINE Mat33 operator*(Real scalar) const
	{
		return Mat33(column0*scalar, column1*scalar, column2*scalar);
	}

	friend Mat33 operator*(Real, const Mat33&);

	//! Matrix vector multiplication (returns 'this->transform(vec)')
	INLINE Vec3 operator*(const Vec3& vec) const
	{
		return transform(vec);
	}

	//! Matrix multiplication
	FORCE_INLINE Mat33 operator*(const Mat33& other) const
	{
		//Rows from this <dot> columns from other
		//column0 = transform(other.column0) etc
		return Mat33(transform(other.column0), transform(other.column1), transform(other.column2));
	}

	// a <op>= b operators

	//! Equals-add
	INLINE Mat33& operator+=(const Mat33& other)
	{
		column0 += other.column0;
		column1 += other.column1;
		column2 += other.column2;
		return *this;
	}

	//! Equals-sub
	INLINE Mat33& operator-=(const Mat33& other)
	{
		column0 -= other.column0;
		column1 -= other.column1;
		column2 -= other.column2;
		return *this;
	}

	//! Equals scalar multiplication
	INLINE Mat33& operator*=(Real scalar)
	{
		column0 *= scalar;
		column1 *= scalar;
		column2 *= scalar;
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

	// Transform etc
	
	//! Transform vector by matrix, equal to v' = M*v
	FORCE_INLINE Vec3 transform(const Vec3& other) const
	{
		return column0*other.x + column1*other.y + column2*other.z;
	}

	//! Transform vector by matrix transpose, v' = M^t*v
	INLINE Vec3 transformTranspose(const Vec3& other) const
	{
		return Vec3(	column0.dot(other),
						column1.dot(other),
						column2.dot(other));
	}

	FORCE_INLINE const Real* front() const
	{
		return &column0.x;
	}

	FORCE_INLINE			Vec3& operator[](int num)			{return (&column0)[num];}
	FORCE_INLINE const	Vec3& operator[](int num) const	{return (&column0)[num];}

	//Data, see above for format!

	Vec3 column0, column1, column2; //the three base vectors
};

// implementation from Quat.h
INLINE Quat::Quat(const Mat33& m)
{
	Real tr = m(0,0) + m(1,1) + m(2,2), h;
	if(tr >= 0)
	{
		h = Sqrt(tr +1);
		w = Real(0.5) * h;
		h = Real(0.5) / h;

		x = (m(2,1) - m(1,2)) * h;
		y = (m(0,2) - m(2,0)) * h;
		z = (m(1,0) - m(0,1)) * h;
	}
	else
	{
		int i = 0; 
		if (m(1,1) > m(0,0))
			i = 1; 
		if (m(2,2) > m(i,i))
			i = 2; 
		switch (i)
		{
		case 0:
			h = Sqrt((m(0,0) - (m(1,1) + m(2,2))) + 1);
			x = Real(0.5) * h;
			h = Real(0.5) / h;

			y = (m(0,1) + m(1,0)) * h; 
			z = (m(2,0) + m(0,2)) * h;
			w = (m(2,1) - m(1,2)) * h;
			break;
		case 1:
			h = Sqrt((m(1,1) - (m(2,2) + m(0,0))) + 1);
			y = Real(0.5) * h;
			h = Real(0.5) / h;

			z = (m(1,2) + m(2,1)) * h;
			x = (m(0,1) + m(1,0)) * h;
			w = (m(0,2) - m(2,0)) * h;
			break;
		case 2:
			h = Sqrt((m(2,2) - (m(0,0) + m(1,1))) + 1);
			z = Real(0.5) * h;
			h = Real(0.5) / h;

			x = (m(2,0) + m(0,2)) * h;
			y = (m(1,2) + m(2,1)) * h;
			w = (m(1,0) - m(0,1)) * h;
			break;
		default: // Make compiler happy
			x = y = z = w = 0;
			break;
		}
	}
}

/** @} */
#endif // MAT33_H
