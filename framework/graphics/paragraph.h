//
//  paragraph.h
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__paragraph__
    #define __KineticUI__paragraph__

    #include <stdio.h>
    #include "../core/map.h"
    #include "../core/vector.h"
    #include "../core/string.h"
    #include "font.h"
    #include "glyph.h"
    #include "bitmap.h"

    typedef struct _paragraph_t paragraph_t;
    struct _paragraph_t
    {
        vector_t* letters;
		void(*paragraph_line_align)(vector_t*, uint32_t,float,float,float);
        float width;
        float height;
        float minwidth;
        float minheight;
		char alignment;
		char multiline;
    };

    paragraph_t* paragraph_default( float width , float height , char align , char multiline );
	void paragraph_free( paragraph_t* paragraph );
	void paragraph_resize( paragraph_t* paragraph , float width );
    void paragraph_align_line_left( vector_t* line , uint32_t spaces , float line_width , float paragraph_width , float position );
    void paragraph_align_line_center( vector_t* line , uint32_t spaces , float line_width , float paragraph_width , float position );
    void paragraph_align_line_justify( vector_t* line , uint32_t spaces , float line_width , float paragraph_width , float position );
    void paragraph_align( paragraph_t* paragraph );
    glyph_t* paragraph_add_glyph( paragraph_t* paragraph , uint64_t index , uint32_t codepoint , char* bytes , float size , font_t* font );
    void paragraph_remove_glyph( paragraph_t* paragraph , uint64_t index , float size , font_t* font );
	void paragraph_remove_all_glyphs( paragraph_t* paragraph );
	string_t* paragraph_gettext( paragraph_t* paragraph );


    #endif /* defined(__KineticUI__paragraph__) */
