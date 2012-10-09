#ifndef _FOUNDATION_HPP_
#define _FOUNDATION_HPP_

#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#define D3DX10
#define ASSERT(x) assert(x)
#define PROPERTY_GET_SET(propertyName, get_func, set_func) __declspec( property(get=get_func, put=set_func) ) propertyName
#define PROPERTY_SET(propertyName, set_func) __declspec( property(put=set_func) ) propertyName
#define PROPERTY_GET(propertyName, get_func) __declspec( property(get=get_func) ) propertyName

#define FORCE_INLINE __forceinline
#define INLINE inline
#define WINDOWS
//#define _STD_USING

#ifdef WINDOWS
	typedef signed __int64				I64;
	typedef signed int					I32;
	typedef signed short				I16;
	typedef signed char					I8;

	typedef unsigned __int64			U64;
	typedef unsigned int				U32;
	typedef unsigned short				U16;
	typedef unsigned char				U8;

	typedef float						F32;
	typedef double						F64;
	typedef double						Real;
#endif

#define	MAX_REAL					MAX_F32
#define EPS_REAL					EPS_F32
#define NORMALIZATION_EPSILON		Real(1e-20f)	

#endif