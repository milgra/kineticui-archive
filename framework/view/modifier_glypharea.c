//
//  modifier_glypharea.c
//  c36
//
//  Created by Milan Toth on 03/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_glypharea.h"

	void modifier_glypharea_new( modifier_t* modifier , modifier_args_t* args );
	void modifier_glypharea_free( modifier_t* modifier );

    // creates new image area

    modifier_t* modifier_glypharea_default( uint32_t color , uint32_t codepoint , float size )
    {
        modifier_t* modifier = malloc( sizeof( modifier_t ) );
        modifier_glypharea_t* data = malloc( sizeof( modifier_glypharea_t ) );
        
		modifier->type = "glypharea";
        modifier->data = data;
		modifier->_new = modifier_glypharea_new;
		modifier->_free = modifier_glypharea_free;

		data->name = malloc( sizeof( char ) * 30 );
		data->size = size;
		data->color = color;
		data->coords = NULL;
		data->codepoint = codepoint;

		sprintf( data->name , "%s-%.1f-%x-%i-%c" , "avenir" , size , color , codepoint , codepoint );
		
        return modifier;
    }

	// delete image area

	void modifier_glypharea_free( modifier_t* modifier )
	{
        modifier_glypharea_t* data = modifier->data;
		free( data->name );
        free( data );
		free( modifier );
	}

	// generates new modifier state

	void modifier_glypharea_new( modifier_t* modifier , modifier_args_t* arguments )
	{
        modifier_glypharea_t* data = modifier->data;
		if ( strcmp( arguments->command.name , "init" ) == 0 || strcmp( arguments->command.name , "update" ) == 0 )
		{
			if ( data->name != NULL )
			{
				data->coords = tilemap_get( arguments->toolset->tilemap , data->name );
				if ( data->coords == NULL )
				{
					int width  = 0;
					int height = 0;
					uint8_t* graymap = font_render( arguments->toolset->font , data->size , data->codepoint , &width , &height );
					bitmap_t* glyphmap = bitmap_create_from_grayscale( width, height, ( data->color >> 24 ) & 0xFF , ( data->color >> 16 ) & 0xFF , ( data->color >> 8 ) & 0xFF , graymap );
					data->coords = tilemap_put( arguments->toolset->tilemap , data->name , glyphmap , 0 );
					if ( data->coords == NULL )
					{
						object_t* eventobject = modifier_defaultevent( "tilemaperror" , "resettilemap" , modifier , NULL );
						vector_adddata( arguments->toolset->events , eventobject );
					}
					
					free( graymap );
					bitmap_free( glyphmap );
				}
			}
			// modify view
			if ( data->name != NULL && data->coords != NULL )
			{
				arguments->views.view->coords.ul = data->coords[0];
				arguments->views.view->coords.ur = data->coords[1];
				arguments->views.view->coords.ll = data->coords[2];
				arguments->views.view->coords.lr = data->coords[3];
				arguments->views.view->realcoords = arguments->views.view->coords;
				arguments->views.view->redraw = 1;
			}
		}
		else if ( strcmp( arguments->command.name , "refilltilemap" ) == 0 )
		{
			// check if its already in
			data->coords = tilemap_get( arguments->toolset->tilemap , data->name );
			if ( data->coords == NULL )
			{
				int width  = 0;
				int height = 0;
				uint8_t* graymap = font_render( arguments->toolset->font , data->size , data->codepoint , &width , &height );
				bitmap_t* glyphmap = bitmap_create_from_grayscale( width, height, ( data->color >> 24 ) & 0xFF , ( data->color >> 16 ) & 0xFF , ( data->color >> 8 ) & 0xFF , graymap );
				vector2_t* coords = tilemap_put( arguments->toolset->tilemap , data->name , glyphmap , 0 );
				if ( coords != NULL )
				{
					data->coords = coords;
				}
				free( graymap );
				bitmap_free( glyphmap );
			}
			*(arguments->generator.changed_flag) = 1;
			// modify view
			if ( data->name != NULL && data->coords != NULL )
			{
				arguments->views.view->coords.ul = data->coords[0];
				arguments->views.view->coords.ur = data->coords[1];
				arguments->views.view->coords.ll = data->coords[2];
				arguments->views.view->coords.lr = data->coords[3];
				arguments->views.view->realcoords = arguments->views.view->coords;
				arguments->views.view->redraw = 1;
			}
		}
	}