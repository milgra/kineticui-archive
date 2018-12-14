//
//  font_t.c
//  C36F
//
//  Created by Milan Toth on 12/06/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "font.h"


    // creates font

    font_t* font_default( char* font_path )
    {
    
        font_t* font = malloc( sizeof( font_t ) );
        
        // read font data into memory
        
        long font_file_size = 0;
        FILE* font_file = fopen( font_path , "rb" );
        
        fseek( font_file , 0 , SEEK_END );
        font_file_size = ftell( font_file );
        fseek( font_file , 0 , SEEK_SET );
        
        font->buffer = malloc( font_file_size );

        fread( font->buffer , font_file_size , 1 , font_file );
        fclose( font_file );

        // create font
        
        int success = stbtt_InitFont( &(font->info) , font->buffer , stbtt_GetFontOffsetForIndex( font->buffer , 0 ) );
        
        if ( success == 1 )
        {

            // get font_ascent font_descent and line font_gap

            stbtt_GetFontVMetrics( &(font->info) , &(font->ascent) , &(font->descent) , &(font->gap) );
            
        }
        else printf( "\nERROR : CANNOT INIT FONT" );
        
        return font;
    }


    // deletes font

    void font_free( font_t* the_font )
    {
    
        free( the_font->buffer );
        free( the_font );
    
    }


    // returns glyph width at given scale

    float font_width( font_t* the_font , float the_size , uint32_t the_codepoint )
    {
    
        // get glyph width

        int width;
        int bearing;
		
        stbtt_GetCodepointHMetrics( &(the_font->info) , the_codepoint , &width , &bearing );
            
        // get font_scale
        
        float scale = stbtt_ScaleForPixelHeight( &(the_font->info) , the_size );
		float result = ( float ) width * scale;
		if ( bearing < 0 ) result -= ( float ) bearing * scale;
        
        return result;
        
    }

    // returns kerning between two codepoints

    float font_kerning( font_t* the_font , float the_size , uint32_t the_codepoint_a , uint32_t the_codepoint_b )
    {
    
        int kerni = stbtt_GetCodepointKernAdvance( &(the_font->info) , the_codepoint_a , the_codepoint_b );
        float scale = stbtt_ScaleForPixelHeight( &(the_font->info) , the_size );
        return ( float ) kerni * scale;
        
    }

    // renders codepoint to a grayscale bitmap

    uint8_t* font_render( font_t* the_font , float the_size , uint32_t the_codepoint , int* the_width , int* the_height )
    {
    
        int dwidth,dbearing,x1,x2,y1,y2;

        float scale = stbtt_ScaleForPixelHeight( &(the_font->info) , the_size );
    
        // get glyph width

        stbtt_GetCodepointHMetrics( &(the_font->info) , the_codepoint , &dwidth , &dbearing );

        // get glyph bounding box
        
        stbtt_GetCodepoint_bitmapBox( &(the_font->info) , the_codepoint , scale , scale , &x1 , &y1 , &x2 , &y2 );

        // font_scale values
		
        float fwidth =  ( float ) dwidth * scale;
		if ( dbearing < 0 ) fwidth -= ( float ) dbearing * scale;
        int asc = ( int ) roundf( ( float ) the_font->ascent * scale );
        int width = ( int ) ceilf( fwidth );
        int height = ( int ) ceilf( the_size );

		// printf( "\ncodepoint %i scale %f dwidth %i fwidth %f width %i" , the_codepoint , scale , dwidth , fwidth , width );
		// printf( "\nbox %i %i %i %i" , x1 , y1 , x2 , y2 );
		
        // calculate offset, align glyph to middle of bitmap
        
        if ( width < ( x2 - x1 ) ) width = x2 - x1;

        int diff = ( width - ( x2 - x1 ) ) >> 1;
        int offset = diff + ( asc + y1 ) * width;

        // create grayscale bitmap and render glyph in it

        uint8_t* bitmap = calloc( width * height , sizeof( uint8_t ) );
        stbtt_MakeCodepoint_bitmap( &the_font->info , bitmap + offset , x2 - x1 , y2 - y1 , width , scale , scale , the_codepoint );
        
        *the_width = width;
        *the_height = height;

        // printf( "\nwidth %i height %i kerning %i font_ascent %i font_descent %i linefont_gap %i font_scale %f" , width , theHeight , kerning , font_ascent, font_descent, font_gap , font_scale );
    
        return bitmap;
    }

