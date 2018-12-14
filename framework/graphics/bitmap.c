//
//  bitmap_t.c
//  C36F
//
//  Created by Milan Toth on 11/06/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "bitmap.h"


    // creates empty bitmap with width and height

    bitmap_t* bitmap_create( int the_width , int the_height )
    {
        bitmap_t* bitmap = malloc( sizeof( bitmap_t ) );
        
        bitmap->width = the_width;
        bitmap->height = the_height;

		bitmap->bytes_length = 4 * the_width * the_height;
		bitmap->bytes = calloc( bitmap->bytes_length , sizeof( unsigned char* ) );
        
        return bitmap;
    }

    // creates bitmap from a png

    bitmap_t* bitmap_create_png( char* path )
    {
        bitmap_t* bitmap = malloc( sizeof( bitmap_t ) );
    
        if ( path != NULL )
        {

            FILE *file = fopen( path , "r" );
            
            if ( file == NULL ) printf( "\nERROR file does not exists %s" , path );
            else
            {
            
                fclose( file );
        
                int components;

                bitmap->bytes = stbi_load( path , &(bitmap->width) , &(bitmap->height) , &components , 4 );
                bitmap->bytes_length = 4 * bitmap->width * bitmap->height;
                
            }

        }

//        for ( int index = 0 ; index < 800 ; index++ )
//        {
//            printf( "%.2i " , bitmap->bytes[ index ] );
//            if ( index % bitmap->width == 0 ) printf( "\n" );
//        }
        
        return bitmap;
    }

    // creates bitmap from grayscale bitmap with given color

    bitmap_t* bitmap_create_from_grayscale(
        int the_width ,
        int the_height ,
        int the_r ,
        int the_g ,
        int the_b ,
        unsigned char* the_bitmap )
    {
    
        bitmap_t* bitmap = malloc( sizeof( bitmap_t ) );
        
        bitmap->width = the_width;
        bitmap->height = the_height;

		bitmap->bytes_length = 4 * the_width * the_height;
		bitmap->bytes = calloc( bitmap->bytes_length , sizeof( unsigned char* ) );

		for ( int index = 0 ; index < the_width * the_height ; index++ )
		{

            bitmap->bytes[ index * 4 	 ] = the_r;
            bitmap->bytes[ index * 4 + 1 ] = the_g;
            bitmap->bytes[ index * 4 + 2 ] = the_b;
            bitmap->bytes[ index * 4 + 3 ] = the_bitmap[ index ];
			
			// if ( bitmap->bytes[ index * 4 + 3 ] < 10 ) bitmap->bytes[ index * 4 + 3 ] = 100;

		}
        
        return bitmap;

    }

    // creates empty bitmap with width and height

    bitmap_t* bitmap_copy( bitmap_t* the_bitmap )
    {
        bitmap_t* bitmap = malloc( sizeof( bitmap_t ) );
        
        bitmap->width = the_bitmap->width;
        bitmap->height = the_bitmap->height;

		bitmap->bytes_length = the_bitmap->bytes_length;
		bitmap->bytes = malloc( the_bitmap->bytes_length );
        
        memcpy( bitmap->bytes , the_bitmap->bytes , the_bitmap->bytes_length );
        
        return bitmap;
    }

    // deletes bitmap

    void bitmap_free( bitmap_t* the_bitmap )
    {
        if ( the_bitmap->bytes != NULL ) free( the_bitmap->bytes );
        free( the_bitmap );
    }

    // fills up rectangle area in bitmap
	
	bitmap_t* bitmap_fill(
        bitmap_t* bitmap ,
        int the_sx ,
        int the_sy ,
        int the_ex ,
        int the_ey ,
        float the_r ,
        float the_g ,
        float the_b ,
        float the_a )
	{
		
		// body

        if ( the_ex < the_sx ) return bitmap;
        if ( the_ey < the_sy ) return bitmap;
        if ( the_sx < 0 ) the_sx = 0;
        if ( the_sy < 0 ) the_sy = 0;
        if ( the_ex >= bitmap->width  ) the_ex = bitmap->width - 1;
        if ( the_ey >= bitmap->height ) the_ey = bitmap->height - 1;

		int r = ( int ) ( the_r * 255.0 );
		int g = ( int ) ( the_g * 255.0 );
		int b = ( int ) ( the_b * 255.0 );
		int a = ( int ) ( the_a * 255.0 );
		
		for ( int y = the_sy ; y < the_ey ; y++ )
		{
		
			for ( int x = the_sx ; x < the_ex ; x++ )
			{
			
				int position = ( y * bitmap->width + x ) * 4;
				
				bitmap->bytes[ position ] 	 = r;
				bitmap->bytes[ position + 1 ] = g;
				bitmap->bytes[ position + 2 ] = b;
				bitmap->bytes[ position + 3 ] = a;
		
			}
			
		}
        
        return bitmap;

	}

    // blends a bitmap on bitmap

	void bitmap_blend( bitmap_t* the_base , bitmap_t* the_bitmap , int the_x , int the_y )
	{

        bitmap_t* bitmap = the_base;
	
		int x, y, srcIndex, dstIndex;
		float srcR, srcG, srcB, srcA, dstR, dstG, dstB, dstA, outA, outR, outG, outB;
		
		// body

		int bx = the_x + the_bitmap->width;
		if ( bx > the_base->width ) bx = the_base->width;
		int by = the_y + the_bitmap->height;
		if ( by > the_base->height ) by = the_base->height;
		
		uint8_t* srcbytes = the_bitmap->bytes;
		uint8_t* dstbytes = bitmap->bytes;
		
		for ( y = the_y ; y < by ; y++ )
		{

			for ( x = the_x ; x < bx ; x++ )
			{
			
				srcIndex = ( ( y - the_y ) * the_bitmap->width + ( x - the_x ) ) * 4;
				dstIndex = ( y * bitmap->width + x ) * 4;

				srcR = ( float ) srcbytes[ srcIndex     ] / 255.0;
				srcG = ( float ) srcbytes[ srcIndex + 1 ] / 255.0;
				srcB = ( float ) srcbytes[ srcIndex + 2 ] / 255.0;
				srcA = ( float ) srcbytes[ srcIndex + 3 ] / 255.0;

				dstR = ( float ) dstbytes[ dstIndex     ] / 255.0;
				dstG = ( float ) dstbytes[ dstIndex + 1 ] / 255.0;
				dstB = ( float ) dstbytes[ dstIndex + 2 ] / 255.0;
				dstA = ( float ) dstbytes[ dstIndex + 3 ] / 255.0;
				
				outA = srcA + dstA * ( 1 - srcA );
                
				outR = ( srcR * srcA + dstR * dstA * ( 1 - srcA ) ) / outA;
				outG = ( srcG * srcA + dstG * dstA * ( 1 - srcA ) ) / outA;
				outB = ( srcB * srcA + dstB * dstA * ( 1 - srcA ) ) / outA;
				
				dstbytes[ dstIndex 	   ] = ( int ) ( outR * 255.0 );
				dstbytes[ dstIndex + 1 ] = ( int ) ( outG * 255.0 );
				dstbytes[ dstIndex + 2 ] = ( int ) ( outB * 255.0 );
				dstbytes[ dstIndex + 3 ] = ( int ) ( outA * 255.0 );
                
                // printf( "\n%f %f %f %i" , srcA , dstA , outA , dstbytes[ dstIndex + 3 ] );
		
			}
			
		}
		
	}
