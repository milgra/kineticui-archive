//
//  bitmap_t.h
//  C36F
//
//  Created by Milan Toth on 11/06/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef C36F_GLBitmap_h
    #define C36F_GLBitmap_h

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>

    #include "../core/string.h"
    #include "stbpng.h"


    typedef struct _bitmap_t bitmap_t;
    struct _bitmap_t
    {

        int width;
        int height;

        uint8_t* bytes;
        uint32_t bytes_length;

    };


    bitmap_t* bitmap_copy( bitmap_t* the_bitmap );
    bitmap_t* bitmap_create( int the_width , int the_height );
    bitmap_t* bitmap_create_png( char* path );
    bitmap_t* bitmap_create_from_grayscale(
        int the_width ,
        int the_height ,
        int the_r ,
        int the_g ,
        int the_b ,
        unsigned char* the_bitmap );
	void bitmap_blend( bitmap_t* the_base , bitmap_t* the_bitmap , int the_x , int the_y );
	bitmap_t* bitmap_fill(
        bitmap_t* the_bitmap ,
        int the_sx ,
        int the_sy ,
        int the_ex ,
        int the_ey ,
        float the_r ,
        float the_g ,
        float the_b ,
        float the_a );
    void bitmap_free( bitmap_t* the_bitmap );


    #endif
