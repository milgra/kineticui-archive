//
//  math4.h
//  c36
//
//  Created by Milan Toth on 02/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__math4__
#define __c36__math4__

	#include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
	#include "math3.h"

    #ifndef M_PI
        #define M_PI 3.14159265358979323846
    #endif

	typedef struct _vector4_t vector4_t;
	struct _vector4_t
	{
		float x, y, z, w;
	};

	typedef struct _matrix4_t matrix4_t;
	struct _matrix4_t
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};

	matrix4_t	matrix4_defaultidentity( );
	matrix4_t	matrix4_defaultortho( float left , float right	, float bottom , float top , float near , float far );
	matrix4_t	matrix4_defaultperspective( float fovy , float aspect , float nearz , float farz );
    matrix4_t	matrix4_defaultscale( float x , float y , float z );
	matrix4_t	matrix4_defaultrotation( float radians , float x , float y , float z );
	matrix4_t	matrix4_defaulttranslation( float x , float y , float z );
	matrix4_t	matrix4_multiply( matrix4_t a , matrix4_t b );
	matrix4_t	matrix4_translate( matrix4_t other , float x , float y , float z );
	matrix4_t	matrix4_scale( matrix4_t matrix ,	float x , float	y , float z );
	matrix4_t	matrix4_rotate( matrix4_t	matrix , float radians , float x , float y , float z );
	matrix4_t	matrix4_invert( matrix4_t source , char* success );
	matrix4_t	matrix4_transpose( matrix4_t matrix );
	void		matrix4_multiplywithnumber( matrix4_t* matrix , float number );
    void		matrix4_extractangles( matrix4_t matrix , float* x , float* y , float* z );
	void		matrix4_toarray( matrix4_t* matrix , float* result );
	void		matrix4_fromarray( matrix4_t* matrix , float* array );
	void		matrix4_describe( matrix4_t matrix );

	vector4_t	vector4_default( float x , float y , float z , float w );
    vector4_t	vector4_sub( vector4_t a , vector4_t b );
	void		vector4_describe( vector4_t vector );
	vector3_t vector4_quadlineintersection( vector4_t pointul , vector4_t pointur , vector4_t pointll , matrix4_t matrix , vector3_t linea , vector3_t lineb );

	vector4_t	matrix4_multiply_vector4( matrix4_t matrix , vector4_t vector );
	vector4_t	matrix4_screenproject_vector4( matrix4_t mvpmatrix , vector4_t mdlvector , float width , float height );
	vector4_t	matrix4_unprojectvector4fromscreen( matrix4_t mvpmatrix , vector4_t scrvector , float width ,	float height );
	void		matrix4_extract( matrix4_t trafo , vector3_t* scale , vector3_t* rotation , vector3_t* translation );

#endif /* defined(__c36__math4__) */
