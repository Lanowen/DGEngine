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


#ifndef MATH_H
#define MATH_H

/** \addtogroup foundation
@{
*/

#include <math.h>
#include <float.h>
#include "../preprocessor.h"

#include "WindowsIntrinsics.h"
#include "Assert.h"


	// constants
	static const Real Pi		=	Real(3.141592653589793);
	static const Real HalfPi	=	Real(1.57079632679489661923);
	static const Real TwoPi		=	Real(6.28318530717958647692);
	static const Real InvPi		=	Real(0.31830988618379067154);


	/**
	\brief The return value is the greater of the two specified values. 
	*/
	template<class T>
	FORCE_INLINE T Max(T a, T b)							{ return a<b ? b : a;	}

	//! overload for float to use fsel on xbox
	template<>
	FORCE_INLINE float Max(float a, float b)				{ return intrinsics::selectMax(a, b); }

	/**
	\brief The return value is the lesser of the two specified values. 
	*/
	template<class T>
	FORCE_INLINE T Min(T a, T b)							{ return a<b ? a : b;	}

	template<>
	//! overload for float to use fsel on xbox
	FORCE_INLINE float Min(float a, float b)				{ return intrinsics::selectMin(a, b); }


	/**
	\brief abs returns the absolute value of its argument. 
	*/
	FORCE_INLINE F32 Abs(F32 a)						{ return intrinsics::abs(a);	}

	FORCE_INLINE bool Equals(F32 a, F32 b,F32 eps)	{ return (Abs(a - b) < eps);	}

	/**
	\brief abs returns the absolute value of its argument. 
	*/
	FORCE_INLINE F64 Abs(F64 a)						{ return ::fabs(a);	}

	/**
	\brief abs returns the absolute value of its argument. 
	*/
	FORCE_INLINE I32 Abs(I32 a)						{ return ::abs(a);	}

	/**
	\brief Clamps v to the range [hi,lo]
	*/
	template<class T>
	FORCE_INLINE T Clamp(T v, T lo, T hi)					{ ASSERT(lo<=hi); return Min(hi, Max(lo, v)); }

	//!	\brief Square root.
	FORCE_INLINE F32 Sqrt(F32 a)						{ return intrinsics::sqrt(a);		}

	//!	\brief Square root.
	FORCE_INLINE F64 Sqrt(F64 a)						{ return ::sqrt(a);	}

	//!	\brief reciprocal square root.
	FORCE_INLINE F32 RecipSqrt(F32 a)					{ return intrinsics::recipSqrt(a);	}

	//!	\brief reciprocal square root.
	FORCE_INLINE F64 RecipSqrt(F64 a)					{ return 1/::sqrt(a);	}

	//!trigonometry -- all angles are in radians.

	//!	\brief Sine of an angle ( <b>Unit:</b> Radians )
	FORCE_INLINE F32 Sin(F32 a)						{ return intrinsics::sin(a);				}

	//!	\brief Sine of an angle ( <b>Unit:</b> Radians )
	FORCE_INLINE F64 Sin(F64 a)						{ return ::sin(a);							}

	//!	\brief Cosine of an angle (<b>Unit:</b> Radians)
	FORCE_INLINE F32 Cos(F32 a)						{ return intrinsics::cos(a);				}

	//!	\brief Cosine of an angle (<b>Unit:</b> Radians)
	FORCE_INLINE F64 Cos(F64 a)						{ return ::cos(a);							}

	/**
	\brief Tangent of an angle.
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F32 Tan(F32 a)						{ return ::tan(a);							}

	/**
	\brief Tangent of an angle.
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F64 Tan(F64 a)						{ return ::tan(a);							}

	/**
	\brief Arcsine.
	Returns angle between -PI/2 and PI/2 in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F32 Asin(F32 f)						{ return ::asin(Clamp(f,-1.0f,1.0f));	}

	/**
	\brief Arcsine.
	Returns angle between -PI/2 and PI/2 in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F64 Asin(F64 f)						{ return ::asin(Clamp(f,-1.0,1.0));		}

	/**
	\brief Arccosine.
	Returns angle between 0 and PI in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F32 Acos(F32 f)						{ return ::acos(Clamp(f,-1.0f,1.0f));			}

	/**
	\brief Arccosine.
	Returns angle between 0 and PI in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F64 Acos(F64 f)						{ return ::acos(Clamp(f,-1.0,1.0));				}

	/**
	\brief ArcTangent.
	Returns angle between -PI/2 and PI/2 in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F32 Atan(F32 a)						{ return ::atan(a);	}

	/**
	\brief ArcTangent.
	Returns angle between -PI/2 and PI/2 in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F64 Atan(F64 a)						{ return ::atan(a);	}

	/**
	\brief Arctangent of (x/y) with correct sign.
	Returns angle between -PI and PI in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F32 Atan2(F32 x, F32 y)			{ return ::atan2(x,y);	}

	/**
	\brief Arctangent of (x/y) with correct sign.
	Returns angle between -PI and PI in radians
	<b>Unit:</b> Radians
	*/
	FORCE_INLINE F64 Atan2(F64 x, F64 y)			{ return ::atan2(x,y);	}

	//!	\brief returns true if the passed number is a finite floating point number as opposed to INF, NAN, etc.
	FORCE_INLINE bool IsFinite(F32 f)					{ return intrinsics::isFinite(f);	}

	//!	\brief returns true if the passed number is a finite floating point number as opposed to INF, NAN, etc.
	FORCE_INLINE bool IsFinite(F64 f)					{ return intrinsics::isFinite(f);	}

	FORCE_INLINE F32 Floor(F32 a)						{ return ::floorf(a);					}

	FORCE_INLINE F32 Exp(F32 a)						{ return ::expf(a);	}

	FORCE_INLINE F32 Ceil(F32 a)						{ return ::ceilf(a);	}

	FORCE_INLINE F32 Sign(F32 a)						{ return intrinsics::sign(a); }

	FORCE_INLINE F32 Pow(F32 x,F32 y)				{ return ::powf(x,y); };

	FORCE_INLINE F32 Log(F32 x)						{ return ::log(x); };

/** @} */
#endif // MATH_H
