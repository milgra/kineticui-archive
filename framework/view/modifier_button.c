//
//  modifier_flipper.c
//  KineticUI
//
//  Created by Milan Toth on 16/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_button.h"

	void modifier_button_free( modifier_t* modifier );
	void modifier_button_new( modifier_t* modifier , modifier_args_t* args );

	// creates button

	modifier_t* modifier_button_default( )
	{
		modifier_t* modifier	= malloc( sizeof( modifier_button_t ) );
		modifier_button_t* data = malloc( sizeof( modifier_button_t ) );
		
		modifier->type = "button";
		modifier->data = data;
		modifier->_new = modifier_button_new;
		modifier->_free = modifier_button_free;

		data->state = 0;
		data->action = NULL;
		data->toggled = 0;
		data->isToggle = 0;
		data->animated = 0;
		data->animator = modifier_animator_default( transformation_default() );

		return modifier;
	}

	// deletes button modifier

	void modifier_button_free( modifier_t* modifier )
	{
		modifier_button_t* data = modifier->data;
		
		data->animator->_free( data->animator );
		free( data );
		free( modifier );
	}

	// generates new modifier state based on input, collecting events, timed views and setting changed state

	void modifier_button_new( modifier_t* modifier , modifier_args_t* args )
	{
		modifier_button_t* data = modifier->data;

		if ( strcmp( args->command.name , "touchdown" ) == 0 )
		{
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0 )
		{
			if ( data->isToggle == 1 )
			{
				data->toggled = 1 - data->toggled;
				if ( data->toggled == 0 )
				{
					//modifier->state = 0;
				}
			}
			else
			{
				//modifier->state = 0;
			}
			
			object_t* eventobject = modifier_defaultevent( "tapped" , data->action , modifier , NULL );
			vector_adddata( args->toolset->events , eventobject );
		}
		else if ( strcmp( args->command.name , "timer" ) == 0 )
		{
			// view state generation
			data->animator->_new( data->animator , args );
		}
	}
