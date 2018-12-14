//
//  modifier_dragger.c
//  KineticUI
//
//  Created by Milan Toth on 25/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_dragger.h"

	void modifier_dragger_free( modifier_t* modifier );
	void modifier_dragger_new( modifier_t* modifier , modifier_args_t* args );


	modifier_t* modifier_dragger_default( )
	{
		modifier_t* modifier = malloc( sizeof( modifier_t ) );
		modifier_dragger_t* data = malloc( sizeof( modifier_dragger_t ) );

		modifier->type = "dragger";
		modifier->data = data;
		modifier->_new = modifier_dragger_new;
		modifier->_free = modifier_dragger_free;
		
		data->pressaction = NULL;
		data->releaseaction = NULL;
		
		return modifier;
	}

	void modifier_dragger_free( modifier_t* modifier )
	{
		free( modifier->data );
		free( modifier );
	}

	void modifier_dragger_new( modifier_t* modifier , modifier_args_t* args )
	{
		modifier_dragger_t* data = modifier->data;
		
		if ( strcmp( args->command.name , "unfocus" ) == 0 )
		{
			object_t* eventobject = modifier_defaultevent( "draggerfocus" , NULL , modifier , NULL );
			vector_adddata( args->toolset->events , eventobject );
		}
		else if ( strcmp( args->command.name , "touchdown" ) == 0 )
		{
			if ( data->pressaction )
			{
				object_t* eventobject = modifier_defaultevent( "draggerpressed" , data->pressaction , modifier , NULL );
				vector_adddata( args->toolset->events , eventobject );
			}
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0 )
		{
			if ( data->releaseaction )
			{
				object_t* eventobject = modifier_defaultevent( "draggerreleased" , data->releaseaction , modifier , NULL );
				vector_adddata( args->toolset->events , eventobject );
			}
		}
	}