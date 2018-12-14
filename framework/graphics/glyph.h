//
//  glyph.h
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__glyph__
    #define __KineticUI__glyph__


    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>
	#include "string.h"

    typedef struct _glyph_t glyph_t;
    struct _glyph_t
    {
        uint32_t codepoint;
        char* bytes;
        float x;
        float y;
        float width;
        float height;
        float kerning;
    };


    glyph_t* glyph_default( uint8_t* bytes , uint32_t codepoint , float width , float height );
	void glyph_free( glyph_t* glyph );

    #endif /* defined(__KineticUI__glyph__) */
