//
//  GLMath.c
//  C36F
//
//  Created by Milan Toth on 07/07/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "math2.h"

	// creates vector2

	vector2_t vector2_default( float x , float y )
	{
		vector2_t result;
		
		result.x = x;
		result.y = y;
		
		return result;
	}

	// describes vector

	void vector2_describe( vector2_t vector )
	{
		printf( "%f %f " , vector.x , vector.y );
	}

	// creates square from two vectors

	square2_t square_default( vector2_t origo , vector2_t extent )
	{
		square2_t square;
		square.origo = origo;
		square.extent = extent;
		return square;
	}

	// returns intersection product

	square2_t square_intersect( square2_t squarea , square2_t squareb )
	{
		float new_width		= squareb.extent.x;
		float new_height	= squareb.extent.y;
		float new_left		= fmin( squarea.origo.x , squareb.origo.x );
		float new_top		= fmax( squarea.origo.y , squareb.origo.y );

		if ( squareb.origo.x < squarea.origo.x )
		{
			new_width = squareb.origo.x + squareb.extent.x - squarea.origo.x;
			new_left = squarea.origo.x - squareb.origo.x;
		}

		if ( squareb.origo.y > squarea.origo.y )
		{
			new_height = squareb.origo.y + squareb.extent.y - squarea.origo.y;
			new_top = squarea.origo.y - squareb.origo.y;
		}

		if ( squareb.origo.x + squareb.extent.x > squarea.origo.x + squarea.extent.x )
		{
			new_width = squarea.origo.x + squarea.extent.x - squareb.origo.x;
		}

		if ( squareb.origo.y + squareb.extent.y < squarea.origo.y + squarea.extent.y )
		{
			new_height = squarea.origo.y + squarea.extent.y - squareb.origo.y;
		}

		if ( new_width  > squarea.extent.x ) new_width  = squarea.extent.x;
		if ( new_height < squarea.extent.y ) new_height = squarea.extent.y;

		square2_t result;
		result.origo = vector2_default( new_left , new_top );
		result.extent = vector2_default( new_width , new_height );
		
		return result;
	}

	square2_t square_intersect2( square2_t squarea , square2_t squareb )
	{
	
		float left = fmax( squarea.origo.x , squareb.origo.x );
		float right = fmin( squarea.origo.x + squarea.extent.x , squareb.origo.x + squareb.extent.x );
		float top = fmin( squarea.origo.y , squareb.origo.y );
		float bottom = fmax( squarea.origo.y + squarea.extent.y , squareb.origo.y + squareb.extent.y );
	
		square2_t result;
		result.origo = vector2_default( left , top );
		result.extent = vector2_default( right - left , bottom - top );
		
		return result;
	}

	// checks overlapping
	// returns 0 if theres no overlapping
	// returns 1 if squareb is inside squarea
	// returns 2 if there is partial overlapping

	uint8_t	square_checkoverlapping( square2_t squarea , square2_t squareb )
	{
		if ( squareb.origo.x + squareb.extent.x < squarea.origo.x ||
			 squareb.origo.x > squarea.origo.x + squarea.extent.x ||
			 squareb.origo.y + squareb.extent.y > squarea.origo.y ||
			 squareb.origo.y < squarea.origo.y + squarea.extent.y )
		{
			// view is outside the visible area
			return kSquareOverlappingNone;
		}
		else
		if ( squareb.origo.x >= squarea.origo.x &&
			 squareb.origo.x + squareb.extent.x <= squarea.origo.x + squarea.extent.x &&
			 squareb.origo.y <= squarea.origo.y &&
			 squareb.origo.y + squareb.extent.y >= squarea.origo.y + squarea.extent.y )
		{
			// view is in the visible area
			return kSquareOverlappingInside;
		}
		else return kSquareOverlappingPartial;
	}

	// hexa to decimal conversion

	static char hexa[] =
	{
		0,0,0,0,0,0,0,0,0,0,		// 0..9
		0,0,0,0,0,0,0,0,0,0,		// 10..19
		0,0,0,0,0,0,0,0,0,0,		// 20..29
		0,0,0,0,0,0,0,0,0,0,		// 30..39
		0,0,0,0,0,0,0,0,0,1,		// 40..49
		2,3,4,5,6,7,8,9,0,0,		// 50..59
		0,0,0,0,0,10,11,12,13,14,	// 60..69
		15,0,0,0,0,0,0,0,0,0,		// 70..79
		0,0,0,0,0,0,0,0,0,0,		// 80..89
		0,0,0,0,0,0,0,10,11,12,		// 90..99
		13,14,15,0,0,0,0,0,0,0		// 100..109
	};

	uint32_t math_read_hexastring( char* string )
	{
		uint32_t result = 0;
		while ( *string != 0 ) result = result << 4 | hexa[ *string++ ];
		return result;
	}

