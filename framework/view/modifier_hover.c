//
//  modifier_hover.c
//  c36
//
//  Created by Milan Toth on 09/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_hover.h"

	void modifier_hover_free( modifier_t* modifier );
	void modifier_hover_new( modifier_t* modifier , modifier_args_t* args );

	// creates view fader

	modifier_t* modifier_hover_default( )
	{
		modifier_t* modifier = malloc( sizeof( modifier_t ) );
		modifier_hover_t* data = malloc( sizeof( modifier_hover_t ) );

		modifier->type = "hover";
        modifier->data = data;
		modifier->_new = modifier_hover_new;
		modifier->_free = modifier_hover_free;

		data->inited = 0;
		data->radius = 0.0;
		data->insideblack = NULL;
		data->orientation = 1;

		return modifier;
	}

	// deletes view fader

	void modifier_hover_free( modifier_t* modifier )
	{
        free( modifier->data );
		free( modifier );
	}

	// timer event for fader, changing view's alpha

	void modifier_hover_timer( modifier_t* modifier , char* changed )
	{
        modifier_hover_t* data = modifier->data;
		data->radius += .08;
		*changed = 1;
	}


	void modifier_hover_new( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_hover_t* data = modifier->data;
		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			if ( data->inited == 0 )
			{
				view_t* insideblack = view_default( 0.0 , 0.0 , 1.0 * args->toolset->settings->scale , args->views.view->height );
				view_setname( insideblack , "insideblack" );
				modifier_t* color = modifier_solidcolor_default( math_read_hexastring( "000000FF" ) );
				modifier_t* cropper = modifier_cropper_default( );
				view_addmodifier( insideblack , ( modifier_t* ) color , 0 );
				view_addmodifier( insideblack , ( modifier_t* ) cropper , 1 );
				
				view_new( &( ( modifier_args_t ) {
					.views      = { insideblack					 , NULL } ,
					.generator  = { args->generator.changed_flag , 0    } ,
					.command    = { "init"						 , NULL } ,
					.toolset    = args->toolset							} ) );
				
				view_addsubview( args->views.view , insideblack );
				data->insideblack = insideblack;
				data->inited = 1;
			}
		}
		else if ( strcmp( args->command.name , "timer" ) == 0 )
		{
			modifier_hover_timer( modifier , args->generator.changed_flag );
			// modify view
			float size = args->views.view->height / 1.5 + cosf( data->radius ) * args->views.view->height / 0.5;
			float deltay = ( args->views.view->height - size ) / 2.0;
			view_setposition( data->insideblack , vector2_default( 0.0 , deltay ) );
			view_setsize( data->insideblack , vector2_default( 1.0 * args->toolset->settings->scale , size ) );
			if ( data->radius > M_PI * 2 ) data->radius = 0.0;

			if ( args->toolset->logs != NULL )
			{
				vector_adddata
				(
					args->toolset->logs ,
					string_createfromformat
					(
						200 ,
						"hover timer %f %f %s" ,
						data->insideblack->width ,
						data->insideblack->height ,
						args->views.view->path
					)
				);
			}
		}
	}
