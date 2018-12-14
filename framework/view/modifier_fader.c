//
//  modifier_fader.c
//  KineticUI
//
//  Created by Milan Toth on 11/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_fader.h"

	// creates view fader

	void modifier_fader_free( modifier_t* modifier );
	void modifier_fader_new( modifier_t* modifier , modifier_args_t* args );


	modifier_t* modifier_fader_default( )
	{
		modifier_t* modifier	= malloc( sizeof( modifier_t ) );
		modifier_fader_t* data	= malloc( sizeof( modifier_fader_t ) );

		modifier->type = "fader";
		modifier->data = data;
		modifier->_new = modifier_fader_new;
		modifier->_free = modifier_fader_free;

		data->speed	= 0.1;
		data->alpha	= 0.0;
		data->inited = 0;
		data->inside = NULL;
		data->direction = 0;
		data->orientation = 1;
		data->stopattop = 0;
		data->stopatbottom = 0;

		return modifier;
	}

	// deletes view fader

	void modifier_fader_free( modifier_t* modifier )
	{
		free( modifier->data );
		free( modifier );
	}

	// timer event for fader, changing view's alpha

	void modifier_fader_timer( modifier_t* modifier , char* changed )
	{
		modifier_fader_t* data = modifier->data;
		if ( data->direction == 0 )
		{
			if ( data->alpha <= 0.00001 )
			{
				if ( data->stopatbottom == 1 )
				{
					data->direction = 3;
				}
				else
				{
					data->direction = 1;
					*changed = 1;
				}
			}
			else
			{
				data->alpha -= data->speed;
				*changed = 1;
			}
		}
		else if ( data->direction == 1 )
		{
			if ( data->alpha >= 0.99999 )
			{
				if ( data->stopattop == 1 )
				{
					data->direction = 3;
				}
				else
				{
					data->direction = 0;
					*changed = 1;
				}
			}
			else
			{
				data->alpha += data->speed;
				*changed = 1;
			}
		}
	}

	void modifier_fader_new( modifier_t* modifier , modifier_args_t* args )
	{
		modifier_fader_t* data = modifier->data;
		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			if ( data->inited == 0 )
			{
				view_t* inside = view_default( 0.0 , 0.0 , 0.0 , 0.0 );
				modifier_t* color = modifier_solidcolor_default( math_read_hexastring( "000000FF" ) );
				modifier_t* cropper = modifier_cropper_default( );
				view_addmodifier( inside , ( modifier_t* ) color , 0 );
				view_addmodifier( inside , ( modifier_t* ) cropper , 1 );
				view_setname( inside , "inside" );
				view_addsubview( args->views.view , inside );
				
				view_new( &( ( modifier_args_t ) {
					.views      = { inside						 , NULL } ,
					.generator  = { args->generator.changed_flag , 0    } ,
					.command    = { "init"						 , NULL } ,
					.toolset    = args->toolset							} ) );
				
				data->inside = inside;
				data->inited = 1;
			}
		}
		else if ( strcmp( args->command.name , "timer" ) == 0 )
		{
			// modify modifier state
			modifier_fader_timer( modifier , args->generator.changed_flag );
			float value = data->alpha;
			vector2_t position = view_getposition( data->inside );
			// modify view
			if ( data->orientation == 0 )
			{
				view_setsize( data->inside , vector2_default( args->views.view->width * value , args->views.view->height ) );
				view_setposition( data->inside , vector2_default( ( args->views.view->width - args->views.view->width * value ) / 2.0 , position.y ) );
			}
			else
			{
				view_setsize( data->inside , vector2_default( args->views.view->width , args->views.view->height * value ) );
				view_setposition( data->inside , vector2_default( position.x , ( args->views.view->height - args->views.view->height * value ) / 2.0 ) );
			}
			if ( args->toolset->logs != NULL )
			{
				vector_adddata
				(
					args->toolset->logs ,
					string_createfromformat
					(
						200 ,
						"fader timer %f %f %s" ,
						data->inside->width ,
						data->inside->height ,
						args->views.view->path
					)
				);
			}
		}
	}