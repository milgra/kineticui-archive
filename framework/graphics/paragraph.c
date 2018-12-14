//
//  paragraph.c
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "paragraph.h"


    // creates paragraph

    paragraph_t* paragraph_default( float width , float height , char align , char multiline )
    {
        paragraph_t* paragraph = malloc( sizeof( paragraph_t ) );
		// printf( "\nparagraph_default %zx" , (size_t)paragraph );
        paragraph->letters = vector_create( );
        paragraph->width = width;
        paragraph->height = height;
		paragraph->alignment = align;
		paragraph->multiline = multiline;
		paragraph->paragraph_line_align = paragraph_align_line_justify;
		paragraph->minheight = height;
		
		if ( align == 1 ) paragraph->paragraph_line_align = paragraph_align_line_left;
		if ( align == 2 ) paragraph->paragraph_line_align = paragraph_align_line_center;
		
        return paragraph;
    }

	// deletes paragraph

	void paragraph_free( paragraph_t* paragraph )
	{
		// printf( "\nparagraph_free %zx" , (size_t)paragraph );
		vector_free( paragraph->letters );
		free( paragraph );
	}

	// resizes paragraph

	void paragraph_resize( paragraph_t* paragraph , float width )
	{
		// printf( "\nparagraph_resize %zx %f %f" , (size_t)paragraph , width , height );
		paragraph->width = width;
	}

    // left line in paragraph

    void paragraph_align_line_left( vector_t* line , uint32_t spaces , float line_width , float paragraph_width , float position )
    {
		// printf( "\nparagraph_align_line_left %zx spaces %i line_width %f paragraph_width %f position %f" , (size_t)line , spaces , line_width , paragraph_width , position );
        float actual_cursor_x = 0.0;
		for ( int index = 0 ; index < line->length ; index++ )
		{
            glyph_t* glyph = line->data[ index ];
			
            actual_cursor_x += glyph->kerning;
            glyph->x = actual_cursor_x;
            glyph->y = position;
            actual_cursor_x += glyph->width;
        }
    }

    // center line in paragraph

    void paragraph_align_line_center( vector_t* line , uint32_t spaces , float line_width , float paragraph_width , float position )
    {
		// printf( "\nparagraph_align_line_center %zx spaces %i line_width %f paragraph_width %f position %f" , (size_t)line , spaces , line_width , paragraph_width , position );
        float actual_cursor_x = ( paragraph_width - line_width ) / 2.0;
		for ( int index = 0 ; index < line->length ; index++ )
		{
            glyph_t* glyph = line->data[ index ];
			
            actual_cursor_x += glyph->kerning;
            glyph->x = actual_cursor_x;
            glyph->y = position;
            actual_cursor_x += glyph->width;
        }
    }

    // justify line in paragraph

    void paragraph_align_line_justify( vector_t* line , uint32_t spaces , float line_width , float paragraph_width , float position )
    {
		// printf( "\nparagraph_align_line_justify %zx spaces %i line_width %f paragraph_width %f position %f" , (size_t)line , spaces , line_width , paragraph_width , position );
        float gap = 0.0;
        if ( spaces > 0 ) gap = ( paragraph_width - line_width ) / ( float ) ( spaces - 1 );
        float actual_cursor_x = 0.0;
		for ( int index = 0 ; index < line->length ; index++ )
		{
            glyph_t* glyph = line->data[ index ];
			
            if ( glyph->codepoint == 32 ) actual_cursor_x += gap;
            
            actual_cursor_x += glyph->kerning;
            glyph->x = actual_cursor_x;
            glyph->y = position;
            actual_cursor_x += glyph->width;
        }
    }

    // align all glyphs

    void paragraph_align( paragraph_t* paragraph )
    {
		// printf( "\nparagraph_align %zx" , (size_t)paragraph );
		
		// paragraph->height = paragraph->minheight;
		
        vector_t* line = vector_create( );
        vector_t* word = vector_create( );
        
        uint32_t spaces = 0;
        
        float line_y = 0.0;
        float line_width = 0.0;
        float word_width = 0.0;
		
		for ( int index = 0 ; index < paragraph->letters->length ; index++ )
        {
            glyph_t* glyph = paragraph->letters->data[ index ];

            // check for enter
            if ( glyph->codepoint == 13 || glyph->codepoint == 10 )
            {
				vector_adddatainvector( line , word );
                paragraph->paragraph_line_align( line , 0 , line_width , paragraph->width , line_y );
                vector_reset( line );
                vector_reset( word );
                word_width = 0.0;
                line_width = 0.0;
                line_y += glyph->height;
                spaces = 0;
            }

            // adding glyph to actual word
            vector_adddata( word , glyph );
            word_width += glyph->kerning + glyph->width;

            // check for line break
            if ( paragraph->multiline == 1 && line_width + word_width > paragraph->width )
            {
                if ( spaces == 0 )
                {
                    vector_adddatainvector( line , word );
					vector_reset( word );
                    line_width += word_width;
                    word_width = 0.0;
                }
                paragraph->paragraph_line_align( line , spaces , line_width , paragraph->width , line_y );
				vector_reset( line );
                line_width = 0.0;
                line_y += glyph->height;
                spaces = 0;
            }

            // check for space
            if ( glyph->codepoint == 32 )
            {
                spaces += 1;
				vector_adddatainvector( line , word );
				vector_reset( word );
                line_width += word_width;
                word_width = 0.0;
            }
            
            // extend paragraph
            if ( line_y + glyph->height > paragraph->height )
			{
				paragraph->height = line_y + glyph->height;
				if ( paragraph->height < paragraph->minheight ) paragraph->height = paragraph->minheight;
			}
        }
        
        // update remaining line/word
		vector_adddatainvector( line , word );
		paragraph->paragraph_line_align( line , 0 , line_width + word_width , paragraph->width , line_y );
		if ( paragraph->multiline == 0 && paragraph->alignment != 2 )
		{
			paragraph->width = line_width + word_width;
			if ( paragraph->width < paragraph->minwidth ) paragraph->width = paragraph->minwidth;
		}
		
		vector_free( line );
		vector_free( word );
    }

    // add glyph to paragraph, calculate kerning

    glyph_t* paragraph_add_glyph( paragraph_t* paragraph , uint64_t index , uint32_t codepoint , char* bytes , float size , font_t* font )
    {
		// printf( "\nparagraph_add_glyph %zx %llu" , (size_t)paragraph , index );
		float glyph_width  = font_width( font , size , codepoint  );
		float glyph_height = size;
		glyph_t* glyph = glyph_default( ( uint8_t* ) bytes , codepoint , glyph_width , glyph_height );
        // empty paragraph, kerning remains 0.0
        if ( paragraph->letters->length == 0 ) vector_adddata( paragraph->letters , glyph );
        // already has glyphs
        else
        {
            // insert at the beginning, kerning remains 0
            if ( index == 0 )
            {
				vector_adddataatindex( paragraph->letters , glyph , 0 );
            }
            else
            {
                glyph_t* prev_glyph;
                // insert inside
                if ( index <= paragraph->letters->length - 1 )
                {
					prev_glyph = paragraph->letters->data[ index - 1 ];
					vector_adddataatindex( paragraph->letters , glyph , index );
                }
                // append to end
                else
                {
					prev_glyph = paragraph->letters->data[ paragraph->letters->length - 1 ];
					vector_adddata( paragraph->letters , glyph );
                }
                glyph->kerning += font_kerning( font , size , prev_glyph->codepoint , glyph->codepoint );
            }
        }
		// printf( "\ncodepoint %i width %f kerning %f" , codepoint , glyph_width , glyph->kerning );
		return glyph;
    }

    // remove from paragraph, calculate kerning

    void paragraph_remove_glyph( paragraph_t* paragraph , uint64_t index , float size , font_t* font )
    {
		// printf( "\nparagraph_remove_glyph %zx %llu" , (size_t)paragraph , index );
        if ( index <= paragraph->letters->length )
        {
            glyph_t* prev_glyph = NULL;
            glyph_t* next_glyph = NULL;
            // first glyph
            if ( index == 0 )
            {
                // we need 3 glyphs at least
                if ( paragraph->letters->length > 2 )
                {
                    prev_glyph = NULL;
					next_glyph = paragraph->letters->data[ 1 ];
                }
            }
            // inbetween glyph
            else if ( index < paragraph->letters->length - 1 )
            {
                prev_glyph = paragraph->letters->data[ index - 1 ];
                next_glyph = paragraph->letters->data[ index + 1 ];
            }
            // last glyph
            else if ( index == paragraph->letters->length - 1 )
            {
                prev_glyph = paragraph->letters->data[ index - 2 ];
                next_glyph = NULL;
            }
            
            if ( prev_glyph != NULL && next_glyph != NULL )
            {
                next_glyph->kerning = font_kerning( font , size , prev_glyph->codepoint , next_glyph->codepoint );
            }
            else if ( prev_glyph == NULL && next_glyph != NULL )
            {
                next_glyph->kerning = 0.0;
            }
            
            glyph_t* glyph = paragraph->letters->data[ index ];
			vector_removedataatindex( paragraph->letters , index );
			glyph_free( glyph );
        }
    }

	// removes all glyphs

	void paragraph_remove_all_glyphs( paragraph_t* paragraph )
	{
		for ( int index = 0 ; index < paragraph->letters->length ; index++ )
		{
			glyph_t* glyph = paragraph->letters->data[ index ];
			glyph_free( glyph );
		}
		if ( paragraph->multiline == 1 ) paragraph->height = paragraph->minheight;
		else if ( paragraph->alignment != 2 ) paragraph->width = paragraph->minwidth;
		
		vector_reset( paragraph->letters );
	}

	// returns text in paragraph

	string_t* paragraph_gettext( paragraph_t* paragraph )
	{
		string_t* result = string_create( );
		for ( int index = 0 ; index < paragraph->letters->length ; index++ )
		{
			glyph_t* glyph = paragraph->letters->data[ index ];
			string_addcodepoint( result , glyph->codepoint );
		}
		return result;
	}


