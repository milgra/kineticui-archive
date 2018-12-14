//
//  modifier_layout.c
//  c36
//
//  Created by Milan Toth on 19/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_layout.h"

	void modifier_layout_free( modifier_t* layout );
	void modifier_layout_new( modifier_t* modifier , modifier_args_t* args );

	// creates layout modifier
    // options bitmask ( 5 4 3 2 1 0 ):
    //
    // 5 : keep x center
    // 4 : keep y center
    // 3 : keep left margin
    // 2 : keep right margin
    // 1 : keep top margin
    // 0 : keep bottom margin
    //
    // keep screen center : 110000
    // keep top margin and width : 001110

	modifier_t* modifier_layout_default( uint8_t options )
	{
		modifier_t* modifier = malloc( sizeof( modifier_t ) );
		modifier_layout_t* data = malloc( sizeof( modifier_layout_t ) );
		
		modifier->type = "layout";
        modifier->data = data;
		modifier->_new = modifier_layout_new;
		modifier->_free = modifier_layout_free;

		// layout setup
		data->options   = options;
		data->centers   = vector4_default( 0 , 0 , 0 , 0 );
		data->distances = vector4_default( 0 , 0 , 0 , 0 );
		data->minextent = vector2_default( 0 , 0 );

		return modifier;
	}

	// deletes layout modifier

	void modifier_layout_free( modifier_t* modifier )
	{
		free( modifier->data );
        free( modifier );
	}

	// fixes layout in view and subviews

	void modifier_layout_fix( modifier_t* modifier , view_t* view , vector2_t extent )
	{
        modifier_layout_t* data = modifier->data;
        
		square2_t square = view_getsquare( view );

		data->centers.x = ( square.origo.x + square.extent.x / 2.0 ) / extent.x;
		data->centers.y = ( square.origo.y + square.extent.y / 2.0 ) / extent.y;
		
		data->distances.x = square.origo.x;
		data->distances.y = extent.x - ( square.origo.x + square.extent.x );
		data->distances.z = square.origo.y;
		data->distances.w = extent.y - ( square.origo.y + square.extent.y );

	}

	// fixes layout in view and subviews

	void modifier_layout_update( modifier_t* modifier , view_t* view , vector2_t extent )
	{
        modifier_layout_t* data = modifier->data;

		square2_t square = view_getsquare( view );

		if ( data->options > 0 )
		{			
			float nulx = square.origo.x;
			float nuly = square.origo.y;
			
			if ( ( ( data->options >> 5 ) & 0x01 ) == 1 )
			{
				// keep horizontal center ratio
				nulx = ( data->centers.x * extent.x ) - square.extent.x / 2.0;
			}
			if ( ( ( data->options >> 4 ) & 0x01 ) == 1 )
			{
				// keep vertical center ratio
				nuly = ( data->centers.y * extent.y ) - square.extent.y / 2.0;
			}

			float nurx = nulx + square.extent.x;
			float nury = nuly;
			float nllx = nulx;
			float nlly = nuly + square.extent.y;

			if ( ( ( data->options >> 3 ) & 0x01 ) == 1 )
			{
				// keep left margin
				nulx = data->distances.x;
				nllx = nulx;
			}
			if ( ( ( data->options >> 2 ) & 0x01 ) == 1 )
			{
				// keep right margin
				nurx = extent.x - data->distances.y;
				if ( data->minextent.x > 0.0 && nurx - nulx < data->minextent.x )
				{
					nurx = nulx + data->minextent.x;
				}
				if ( ( ( data->options >> 3 ) & 0x01 ) == 0 )
				{
					nulx = nurx - square.extent.x;
					nllx = nulx;
				}
			}
			if ( ( ( data->options >> 1 ) & 0x01 ) == 1 )
			{
				// keep top margin
				nury = data->distances.z;
				nuly = nury;
			}
			if ( ( ( data->options ) & 0x01 ) == 1 )
			{
				// keep bottom margin
				nlly = extent.y - data->distances.w;
				if ( ( ( data->options >> 1 ) & 0x01 ) == 0 )
				{
					nuly = nlly - square.extent.y;
					nury = nuly;
				}
			}
			square.origo = vector2_default( nulx , nuly );
			square.extent = vector2_default( nurx - nulx , nlly - nuly );
		}

		view_setsquare( view , square );
	}

	// generates state

	void modifier_layout_new( modifier_t* modifier , modifier_args_t* args )
	{
		if ( strcmp( args->command.name , "layoutfix" ) == 0 || strcmp( args->command.name , "init" ) == 0 )
		{
			view_t* parent = args->views.parent;
			vector2_t extent;
			if ( parent != NULL ) extent = vector2_default( parent->width , parent->height );
			else extent = vector2_default( args->views.view->width , args->views.view->height );
			modifier_layout_fix( modifier , args->views.view , extent );
		}
		else if ( strcmp( args->command.name , "layoutupdate" ) == 0 )
		{
			view_t* parent = args->views.parent;
			vector2_t extent;
			if ( parent != NULL ) extent = vector2_default( parent->width , parent->height );
			else extent = vector2_default( args->views.view->width , args->views.view->height );
			modifier_layout_update( modifier , args->views.view , extent );
			if ( args->toolset->logs != NULL )
			{
				vector_adddata
				(
					args->toolset->logs ,
					string_createfromformat
					(
						200 ,
						"layout update %f %f %s" ,
						args->views.view->width ,
						args->views.view->height ,
						args->views.view->path
					)
				);
			}
		}
	}