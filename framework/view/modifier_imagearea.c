//
//  modifier_imagearea.c
//  KineticUI
//
//  Created by Milan Toth on 06/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "modifier_imagearea.h"

	void modifier_imagearea_new( modifier_t* modifier , modifier_args_t* args );
	void modifier_imagearea_free( modifier_t* modifier );

    // generate default state

    modifier_t* modifier_imagearea_default( char* imagename )
    {
        modifier_t* modifier        = malloc( sizeof( modifier_t ) );
        modifier_imagearea_t* data  = malloc( sizeof( modifier_imagearea_t ) );
        
		modifier->type = "imagearea";
        modifier->data = data;
		modifier->_new = modifier_imagearea_new;
		modifier->_free = modifier_imagearea_free;

		data->path = NULL;
        data->image = NULL;
		data->coords = NULL;
		data->imagename = string_copycstring( imagename );
		
        return modifier;
    }

	// delete image area

	void modifier_imagearea_free( modifier_t* modifier )
	{
        modifier_imagearea_t* data = modifier->data;
		if ( data->path != NULL ) free( data->path );
		if ( data->image != NULL ) bitmap_free( data->image );
		free( modifier->data );
        free( modifier );
	}

	// generates state

	void modifier_imagearea_new( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_imagearea_t* data = modifier->data;
		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			if ( data->imagename != NULL )
			{
				string_t* paths = string_createfrombytes( args->toolset->settings->resources );
				string_addbytearray( paths , "/" );
				string_addbytearray( paths , data->imagename );
				data->path = string_bytes( paths );
				string_free( paths );

				data->image = bitmap_create_png( data->path );
				data->coords = tilemap_get( args->toolset->tilemap , data->imagename );
				
				if ( data->coords == NULL )
				{
					data->coords = tilemap_put( args->toolset->tilemap , data->imagename , data->image , 0 );
					if ( data->coords == NULL )
					{
						object_t* eventobject = modifier_defaultevent( "tilemaperror" , "resettilemap" , modifier , NULL );
						vector_adddata( args->toolset->events , eventobject );
					}
				}
			}
			*(args->generator.changed_flag) = 1;
			if ( data->imagename != NULL && data->coords != NULL )
			{
				args->views.view->coords.ul = data->coords[0];
				args->views.view->coords.ur = data->coords[1];
				args->views.view->coords.ll = data->coords[2];
				args->views.view->coords.lr = data->coords[3];
				args->views.view->realcoords = args->views.view->coords;
				args->views.view->redraw = 1;
			}
		}
		else if ( strcmp( args->command.name , "refilltilemap" ) == 0 )
		{
			data->coords = tilemap_get( args->toolset->tilemap , data->path );
			if ( data->coords == NULL ) data->coords = tilemap_put( args->toolset->tilemap , data->imagename , data->image , 0 );
			*(args->generator.changed_flag) = 1;
			if ( data->imagename != NULL && data->coords != NULL )
			{
				args->views.view->coords.ul = data->coords[0];
				args->views.view->coords.ur = data->coords[1];
				args->views.view->coords.ll = data->coords[2];
				args->views.view->coords.lr = data->coords[3];
				args->views.view->realcoords = args->views.view->coords;
				args->views.view->redraw = 1;
			}
		}
	}