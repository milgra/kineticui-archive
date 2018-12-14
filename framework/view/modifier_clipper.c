//
//  hanlder_clipper.c
//  KineticUI
//
//  Created by Milan Toth on 24/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_clipper.h"

	void modifier_clipper_free( modifier_t* modifier );
	void modifier_clipper_new( modifier_t* modifier , modifier_args_t* args );

	// animates geometry, mainly for letters

	// creates animator for view with starting transformation

	modifier_t* modifier_clipper_default( view_t* view )
	{
		modifier_t* modifier	 = malloc( sizeof( modifier_t ) );
		modifier_clipper_t* data = malloc( sizeof( modifier_clipper_t ) );

		modifier->type = "clipper";
		modifier->data = data;
		modifier->_new = modifier_clipper_new;
		modifier->_free = modifier_clipper_free;

		data->playing = 0;
		data->changed = 0;

		return modifier;
	}

	// deletes animator

	void modifier_clipper_free( modifier_t* modifier )
	{
		free( modifier->data );
		free( modifier );
	}

	// applies actual transformation to view

	void modifier_clipper_apply_actual( modifier_t* modifier , view_t* view )
	{
		modifier_clipper_t* data = modifier->data;
		view_setsize( view , vector2_default( data->geowidth * data->ratio , data->geoheight ) );
		view->coords.ur.x = data->realcoords.ul.x + data->texwidth * data->ratio;
		view->coords.lr.x = data->realcoords.ll.x + data->texwidth * data->ratio;
		view->realcoords = view->coords;
		view->redraw = 1;
	}

	// expands view

	void modifier_clipper_expand( modifier_t* modifier , int steps , int delay )
	{
		modifier_clipper_t* data = modifier->data;
		data->ratio = 0.0;
		data->playing = 1;
		data->stepping = 1.0 / ( float ) steps;
		data->steps_left = steps;
		data->steps_delay = delay;
	}

	// shrink view

	void modifier_clipper_shrink( modifier_t* modifier , int steps , int delay )
	{
		modifier_clipper_t* data = modifier->data;
		data->ratio = 1.0;
		data->playing = 1;
		data->changed = 0;
		data->stepping = -1.0 / ( float ) steps;
		data->steps_left = steps;
		data->steps_delay = delay;
	}

	// animator stepping from timer

	void modifier_clipper_timer( modifier_t* modifier , view_t* view , vector_t* events , char* changed )
	{
		modifier_clipper_t* data = modifier->data;
		if ( data->playing == 1 )
		{
			if ( data->steps_delay > 0 ) data->steps_delay -= 1;
			else
			{
				data->ratio += data->stepping;
				data->changed = 1;
				data->steps_left -= 1;
				if ( data->steps_left < 1 )
				{
					data->playing = 0;
					data->ratio = 1.0;
				}
			}
			*changed = 1;
		}
	}

	// create new state based on input

	void modifier_clipper_new( modifier_t* modifier , modifier_args_t* args )
	{
		modifier_clipper_t* data = modifier->data;
		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			data->realcoords = args->views.view->coords;
			data->geowidth = args->views.view->width;
			data->geoheight = args->views.view->height;
			data->texwidth = args->views.view->coords.ur.x - args->views.view->coords.ul.x;
		}
		else if ( strcmp( args->command.name , "timer" ) == 0 )
		{
			if ( data->playing == 1 )
			{
				modifier_clipper_timer( modifier , args->views.view , args->toolset->events , args->generator.changed_flag );
			}
			// modify view
			if ( data->changed == 1 )
			{
				modifier_clipper_apply_actual( modifier , args->views.view );
				data->changed = 0;
			}
			if ( data->playing == 0 )
			{
				view_setsize( args->views.view , vector2_default( data->geowidth , data->geoheight ) );
				args->views.view->realcoords = data->realcoords;
				args->views.view->coords = data->realcoords;
				args->views.view->redraw = 1;
			}
		}
		else if ( strcmp( args->command.name , "expand" ) == 0 )
		{
			clipper_args* cargs = ( clipper_args* ) args->command.arguments;
			modifier_clipper_expand(modifier , cargs->steps , cargs->delay );
			if ( cargs->apply == 1 ) modifier_clipper_apply_actual( modifier , args->views.view );
		}
		else if ( strcmp( args->command.name , "shrink" ) == 0 )
		{
			clipper_args* cargs = ( clipper_args* ) args->command.arguments;
			modifier_clipper_shrink(modifier , cargs->steps , cargs->delay );
			if ( cargs->apply == 1 ) modifier_clipper_apply_actual( modifier , args->views.view );
		}
	}