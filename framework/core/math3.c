//
//  math3.c
//  c36
//
//  Created by Milan Toth on 02/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "math3.h"


	// creates vector3
    
	vector3_t vector3_default( float x , float y , float z )
	{
		vector3_t result;
		
		result.x = x;
		result.y = y;
		result.z = z;
		
		return result;
	}

	// adds vector3s

	vector3_t vector3_add( vector3_t a , vector3_t b )
	{
		vector3_t result;
		
		result.x = a.x + b.x;
		result.y = a.y + b.y;
		result.z = a.z + b.z;
		
		return result;
	}

	// substracts vector3s
	
	vector3_t vector3_sub( vector3_t a , vector3_t b )
	{
		vector3_t result;
		
		result.x = a.x - b.x;
		result.y = a.y - b.y;
		result.z = a.z - b.z;
		
		return result;
	}

	// calculates vector3 length

	float vector3_length( vector3_t a )
    {
        return sqrtf( a.x * a.x + a.y * a.y + a.z * a.z );
    }

	// calculates angle between two vector3s

	float vector3_angle( vector3_t a , vector3_t b )
    {
        return acosf( vector3_dot( a , b ) / ( vector3_length( a ) * vector3_length( b ) )  );
    }

	// creates dot product of two vector3s

	float vector3_dot( vector3_t a , vector3_t b )
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	// creates cross product of two vector 3s

	vector3_t vector3_cross( vector3_t left , vector3_t right)
	{
		vector3_t v;
		
		v.x = left.y * right.z - left.z * right.y;
		v.y = left.z * right.x - left.x * right.z;
		v.z = left.x * right.y - left.y * right.x;

		return v;
	}

	// scales vector3

	vector3_t vector3_scale( vector3_t a , float f )
	{
		vector3_t result;

		result.x = a.x * f;
		result.y = a.y * f;
		result.z = a.z * f;
		
		return result;
	}

	// normalizes vector3

	vector3_t vector3_normalize( vector3_t matrix )
    {
		float scale;

		scale = 1.0f / sqrtf( matrix.x * matrix.x + matrix.y * matrix.y + matrix.z * matrix.z );

		matrix.x *= scale;
		matrix.y *= scale;
		matrix.z *= scale;
		
		return matrix;
    }

	// intersects vector3 with plane

	vector3_t vector3_intersectwithplane( vector3_t linev1 , vector3_t linev2 , vector3_t planev , vector3_t planen )
	{
		vector3_t u, w, scale, result;
		float 	  div;

		u = vector3_sub( linev2 , linev1 );
		w = vector3_sub( planev , linev1 );
		div = vector3_dot( planen , w ) / vector3_dot( planen , u );
		scale = vector3_scale( u , div );
		result = vector3_add( linev1 , scale );

		return result;
	}

	// converts vector3 to array

    void vector3_toarray( vector3_t* vector , float* result )
    {
		result[ 0 ] = vector->x;
		result[ 1 ] = vector->y;
		result[ 2 ] = vector->z;
    }

	// rotates vector 3 around it y axis

    vector3_t vector3_rotatearoundy( vector3_t vector , float the_angle )
    {
        float epsilon = 0.00001;
        if ( fabs(vector.x) > epsilon || fabs(vector.z) > epsilon )
        {
            float angle = atan2( vector.z , vector.x );
            float length = sqrtf( vector.x * vector.x + vector.z * vector.z );
            
            vector.z = sin( angle + the_angle ) * length;
            vector.x = cos( angle + the_angle ) * length;
        }
        return vector;
    }

	// rotates vector 3 around it x axis

    vector3_t vector3_rotatearoundx( vector3_t vector , float the_angle )
    {
        float epsilon = 0.00001;
        if ( fabs(vector.y) > epsilon || fabs(vector.z) > epsilon )
        {
            float angle = atan2( vector.z , vector.y );
            float length = sqrtf( vector.y * vector.y + vector.z * vector.z );
            
            vector.z = sin( angle + the_angle ) * length;
            vector.y = cos( angle + the_angle ) * length;
        }
        return vector;
    }

	// rotates vector 3 around it z axis

    vector3_t vector3_rotatearoundz( vector3_t vector , float the_angle )
    {
        float epsilon = 0.00001;
        if ( fabs(vector.y) > epsilon || fabs(vector.x) > epsilon )
        {
            float angle = atan2( vector.y , vector.x );
            float length = sqrtf( vector.x * vector.x + vector.y * vector.y );
            
            vector.y = sin( angle + the_angle ) * length;
            vector.x = cos( angle + the_angle ) * length;
        }
        return vector;
    }

	// rotates back plane to origo, returns rotation vector

	vector3_t vector3_getxyunitrotation( vector3_t vx , vector3_t vy )
    {
        vector3_t rotation;
        
        float angle;
        float epsilon = 0.00001;
        
        // rotate back final_x to base_x on Z axis
        
        if ( fabs(vx.x) > epsilon || fabs(vx.y) > epsilon )
        {

            angle = atan2( vx.y , vx.x  );

            rotation.z = angle;
            
            vx = vector3_rotatearoundz( vx , -angle );
            vy = vector3_rotatearoundz( vy , -angle );

        }
        else rotation.z = 0.0;

        // rotate back final_x to base_x on Y axis

        if ( fabs(vx.x) > epsilon || fabs(vx.z) > epsilon )
        {

            angle = atan2( vx.z , vx.x  );

            rotation.y = -angle;

            vx = vector3_rotatearoundy( vx , -angle );
            vy = vector3_rotatearoundy( vy , -angle );

        }
        else rotation.y = 0.0;
        
        // finally rotate back final_y to base_y on X axis

        if ( fabs(vy.y) > epsilon || fabs(vy.z) > epsilon )
        {

            angle = atan2( vy.z , vy.y  );
            
            rotation.x = angle;

            vx = vector3_rotatearoundx( vx , -angle );
            vy = vector3_rotatearoundx( vy , -angle );
            
        }
        else rotation.x = 0.0;
        
        return rotation;
    }

	// calculates distance of two vector3s

    float vector3_distance( vector3_t vectorA , vector3_t vectorB )
    {
        float dx , dy, dz;
        
        dx = vectorB.x - vectorA.x;
        dy = vectorB.y - vectorA.y;
        dz = vectorB.z - vectorA.z;
        
        return sqrtf( dx*dx + dy*dy + dz*dz );
    }

	// 3x3 matrix

	matrix3_t	matrix3_defaultidentity( )
	{
		matrix3_t matrix;
		
		matrix.m00 = 1.0f;
		matrix.m01 = 0.0f;
		matrix.m02 = 0.0f;
		matrix.m10 = 0.0f;
		matrix.m11 = 1.0f;
		matrix.m12 = 0.0f;
		matrix.m20 = 0.0f;
		matrix.m21 = 0.0f;
		matrix.m22 = 1.0f;
		
		return matrix;
	}
	
    matrix3_t matrix3_defaultscale( float x , float y )
	{
        matrix3_t matrix = matrix3_defaultidentity( );
    
		matrix.m00 = x;
		matrix.m11 = y;
		
		return matrix;
	}
	
	matrix3_t matrix3_defaulttranslation( float x , float y )
	{
		matrix3_t result;
		
		result = matrix3_defaultidentity( );
		result.m02 = x;
		result.m12 = y;

		return result;
	}

	matrix3_t matrix3_defaultrotationz( float rad )
	{
		matrix3_t result;
		
		result = matrix3_defaultidentity( );
		result.m00 = cosf( rad );
		result.m01 = sinf( rad );
		result.m10 = -sinf( rad );
		result.m11 = cosf( rad );

		return result;
	}

	matrix3_t matrix3_defaultrotationx( float rad )
	{
		matrix3_t result;
		
		result = matrix3_defaultidentity( );
		result.m11 = cosf( rad );
		result.m12 = -sinf( rad );
		result.m21 = sinf( rad );
		result.m22 = cosf( rad );

		return result;
	}

	matrix3_t matrix3_defaultrotationy( float rad )
	{
		matrix3_t result;
		
		result = matrix3_defaultidentity( );
		result.m00 = cosf( rad );
		result.m02 = sinf( rad );
		result.m20 = -sinf( rad );
		result.m22 = cosf( rad );

		return result;
	}

	matrix3_t matrix3_multiply( matrix3_t a , matrix3_t b )
	{
		matrix3_t matrix;
		
		matrix.m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
		matrix.m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
		matrix.m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
		
		matrix.m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
		matrix.m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
		matrix.m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
		
		matrix.m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
		matrix.m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
		matrix.m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
		
		return matrix;
	}

	matrix3_t matrix3_invert( matrix3_t source , char* success )
	{
		return matrix3_defaultidentity();
	}

	matrix3_t matrix3_transpose( matrix3_t matrix )
	{
		matrix3_t result;

		result.m00 = matrix.m00;
		result.m11 = matrix.m11;
		result.m22 = matrix.m22;
		
		result.m10 = matrix.m01;
		result.m01 = matrix.m10;
		result.m20 = matrix.m02;
		result.m02 = matrix.m20;
		
		result.m21 = matrix.m12;
		result.m12 = matrix.m21;
		
		return result;
	}

	void matrix3_multiplywithnumber( matrix3_t* matrix , float number )
	{
		matrix->m00 *= number;
		matrix->m01 *= number;
		matrix->m02 *= number;
		matrix->m10 *= number;
		matrix->m11 *= number;
		matrix->m12 *= number;
		matrix->m20 *= number;
		matrix->m21 *= number;
		matrix->m22 *= number;
	}
	
	void matrix3_describe( matrix3_t matrix )
	{
		printf( "%.2f %.2f %.2f | %.2f %.2f %.2f | %.2f %.2f %.2f" ,
				matrix.m00 ,
				matrix.m01 ,
				matrix.m02 ,
				matrix.m10 ,
				matrix.m11 ,
				matrix.m12 ,
				matrix.m20 ,
				matrix.m21 ,
				matrix.m22 );
	}

	// multiplies matrix4 with vector 4

	vector3_t matrix3_multiply_vector3( matrix3_t matrix , vector3_t vector )
	{
		vector3_t result;

    	result.x = matrix.m00 * vector.x + matrix.m01  * vector.y + matrix.m02 * vector.z;
		result.y = matrix.m10 * vector.x + matrix.m11  * vector.y + matrix.m12 * vector.z;
		result.z = matrix.m20 * vector.x + matrix.m21  * vector.y + matrix.m22 * vector.z;

	    return result;
	}

