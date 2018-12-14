//
//  glyph.c
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "glyph.h"


    // create glyph
	
    glyph_t* glyph_default( uint8_t* bytes , uint32_t codepoint , float width , float height )
    {
        // carriage return is invisible
        if ( codepoint == 13 || codepoint == 10 ) width = 0;
    
        glyph_t* glyph = malloc( sizeof( glyph_t ) );
        
        glyph->x = 0.0;
        glyph->y = 0.0;
        glyph->bytes = string_copycstring( (char*) bytes );
        glyph->width = width;
        glyph->height = height;
        glyph->kerning = 0.0;
        glyph->codepoint = codepoint;
        
        return glyph;
    }

	// delete glyph

	void glyph_free( glyph_t* glyph )
	{
		free( glyph->bytes );
		free( glyph );
	}

