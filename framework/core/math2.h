//
//  GLMath.h
//  C36F
//
//  Created by Milan Toth on 07/07/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef C36F_GLMath_h
    #define C36F_GLMath_h


	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>
    #include <stdint.h>

	typedef struct _vector2_t vector2_t;
	struct _vector2_t
	{
		float x, y;
	};

	typedef struct square2_t square2_t;
	struct square2_t
	{
		vector2_t origo;
		vector2_t extent;
	};

	vector2_t	vector2_default( float x , float y );
	void		vector2_describe( vector2_t vector );

	#define kSquareOverlappingNone 0
	#define kSquareOverlappingInside 1
	#define kSquareOverlappingPartial 2

	square2_t	square_default( vector2_t origo , vector2_t extent );
	square2_t	square_intersect( square2_t squarea , square2_t squareb );
	square2_t	square_intersect2( square2_t squarea , square2_t squareb );
	uint8_t		square_checkoverlapping( square2_t squarea , square2_t squareb );

	uint32_t	math_read_hexastring( char* string );
	

    #endif
