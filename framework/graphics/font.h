//
//  font_t.h
//  C36F
//
//  Created by Milan Toth on 12/06/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef C36F_TLFont_h
    #define C36F_TLFont_h

    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include "stbtt.h"
    #include "string.h"


    typedef struct _font_t font_t;
    struct _font_t
    {
        stbtt_fontinfo info;
        unsigned char* buffer;
        int gap;
        int ascent;
        int descent;
    };


    font_t* font_default( char* the_font_path );
    void font_free( font_t* the_font );
    float font_width( font_t* the_font , float the_size , uint32_t the_codepoint );
    float font_kerning( font_t* the_font , float the_size , uint32_t the_codepoint_a , uint32_t the_codepoint_b );
    uint8_t* font_render( font_t* the_font , float the_size , uint32_t the_codepoint , int* the_width , int* the_height );


    #endif
