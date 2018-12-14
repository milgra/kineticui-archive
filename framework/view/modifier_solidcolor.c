//
//  colorviewmodifier.c
//  KineticUI
//
//  Created by Milan Toth on 15/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "modifier_solidcolor.h"

	void modifier_solidcolor_free( modifier_t* modifier );
	void modifier_solidcolor_new( modifier_t* modifier , modifier_args_t* args );
	void modifier_solidcolor_setcolor( modifier_t* modifier , uint32_t color );

    // creates new solidcolor

    modifier_t* modifier_solidcolor_default( uint32_t color )
    {
		modifier_t* modifier		= malloc( sizeof( modifier_t ) );
        modifier_solidcolor_t* data = malloc( sizeof( modifier_solidcolor_t ) );

		data->inited = 0;
		data->coords  = NULL;
		data->bitmap = NULL;
		data->colorname = NULL;
		
		modifier->type = "solidcolor";
		modifier->data = data;
		modifier->_new = modifier_solidcolor_new;
		modifier->_free = modifier_solidcolor_free;

		modifier_solidcolor_setcolor( modifier , color );
		
        return modifier;
    }

	// delete modifier

	void modifier_solidcolor_free( modifier_t* modifier )
	{
		modifier_solidcolor_t* data = modifier->data;
		
		bitmap_free( data->bitmap );
		
		free( data->colorname );
		free( data );
		free( modifier );
	}

	// sets new color

	void modifier_solidcolor_setcolor( modifier_t* modifier , uint32_t color )
	{
		modifier_solidcolor_t* data = modifier->data;

		char* name = malloc( sizeof( char ) * 8 );
		sprintf( name , "%x" , color );

		bitmap_t* bitmap = bitmap_create( 30 , 30 );	// needs release
		float r = ( float ) ( ( color >> 24 ) & 0xff ) / 255.0;
		float g = ( float ) ( ( color >> 16 ) & 0xff ) / 255.0;
		float b = ( float ) ( ( color >>  8 ) & 0xff ) / 255.0;
		float a = ( float ) ( ( color       ) & 0xff ) / 255.0;
		bitmap_fill( bitmap , 0 , 0 , 30 , 30 , r , g , b , a );
		
		data->color = color;
		if ( data->bitmap != NULL ) bitmap_free( data->bitmap );
		if ( data->colorname != NULL ) free( data->colorname );
		data->bitmap = bitmap;
		data->colorname = name;
	}

	// generates new modifier state

	void modifier_solidcolor_new( modifier_t* modifier , modifier_args_t* args )
	{
		modifier_solidcolor_t* data = modifier->data;

		// get texture from tilemap or generate and place it if doesn't exist
		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			if ( data->inited == 0 )
			{
				// get tilemap
				data->coords = tilemap_get( args->toolset->tilemap , data->colorname );
				// add to tilemap if doesn't exist
				if ( data->coords == NULL )
				{
					data->coords = tilemap_put( args->toolset->tilemap , data->colorname , data->bitmap , 2 );
					if ( data->coords == NULL )
					{
						object_t* eventobject = modifier_defaultevent( "tilemaperror" , "resettilemap" , modifier , NULL );
						vector_adddata( args->toolset->events , eventobject );
					}
				}
				// set coords
				if ( data->coords != NULL )
				{
					args->views.view->coords.ul = data->coords[0];
					args->views.view->coords.ur = data->coords[1];
					args->views.view->coords.ll = data->coords[2];
					args->views.view->coords.lr = data->coords[3];
					args->views.view->realcoords = args->views.view->coords;
					args->views.view->redraw = 1;
				}
				*(args->generator.changed_flag) = 1;
				data->inited = 1;
			}
			else
			{
			
			}
		}
		else if ( strcmp( args->command.name , "setcolor" ) == 0 )
		{
			uint32_t color = ( uint32_t ) args->command.arguments;
			char* name = malloc( sizeof( char ) * 8 );
			sprintf( name , "%x" , color );

			bitmap_t* bitmap = bitmap_create( 30 , 30 );	// needs release
			float r = ( float ) ( ( color >> 24 ) & 0xff ) / 255.0;
			float g = ( float ) ( ( color >> 16 ) & 0xff ) / 255.0;
			float b = ( float ) ( ( color >>  8 ) & 0xff ) / 255.0;
			float a = ( float ) ( ( color       ) & 0xff ) / 255.0;
			bitmap_fill( bitmap , 0 , 0 , 30 , 30 , r , g , b , a );
			
			data->color = color;
			if ( data->bitmap != NULL ) bitmap_free( data->bitmap );
			if ( data->colorname != NULL ) free( data->colorname );
			data->bitmap = bitmap;
			data->colorname = name;

			// get tilemap
			data->coords = tilemap_get( args->toolset->tilemap , data->colorname );
			// add to tilemap if doesn't exist
			if ( data->coords == NULL )
			{
				data->coords = tilemap_put( args->toolset->tilemap , data->colorname , data->bitmap , 2 );
				if ( data->coords == NULL )
				{
					object_t* eventobject = modifier_defaultevent( "tilemaperror" , "resettilemap" , modifier , NULL );
					vector_adddata( args->toolset->events , eventobject );
				}
			}
			// set coords
			if ( data->coords != NULL )
			{
				args->views.view->coords.ul = data->coords[0];
				args->views.view->coords.ur = data->coords[1];
				args->views.view->coords.ll = data->coords[2];
				args->views.view->coords.lr = data->coords[3];
				args->views.view->realcoords = args->views.view->coords;
				args->views.view->redraw = 1;
			}
			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "refilltilemap" ) == 0   )
		{
			data->coords = tilemap_get( args->toolset->tilemap , data->colorname );
			// set coords
			if ( data->coords == NULL ) data->coords = tilemap_put( args->toolset->tilemap , data->colorname , data->bitmap , 2 );
			if ( data->coords != NULL )
			{
				args->views.view->coords.ul = data->coords[0];
				args->views.view->coords.ur = data->coords[1];
				args->views.view->coords.ll = data->coords[2];
				args->views.view->coords.lr = data->coords[3];
				args->views.view->realcoords = args->views.view->coords;
				args->views.view->redraw = 1;
			}
			*(args->generator.changed_flag) = 1;
		}
	}