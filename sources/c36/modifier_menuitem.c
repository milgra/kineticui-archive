//
//  modifier_menuitem.c
//  KineticUI
//
//  Created by Milan Toth on 09/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_menuitem.h"


	// creates button

	modifier_menuitem_t* modifier_menuitem_default( string_t* action )
	{
		modifier_menuitem_t* modifier = malloc( sizeof( modifier_menuitem_t ) );
		
		modifier->_new = modifier_menuitem_new;
		modifier->_free = modifier_menuitem_free;
		modifier->type = "menuitem";
		
		modifier->action = string_bytes( action );
		modifier->text = NULL;
		return modifier;
	}

	// deletes button modifier

	void modifier_menuitem_free( modifier_menuitem_t* modifier )
	{
		free( modifier );
	}

	// generates new modifier state based on input, collecting events, timed views and setting changed state

	void modifier_menuitem_new( modifier_menuitem_t* modifier , modifier_args_t* args )
	{
		if ( strcmp( args->command.name , "settext" ) == 0  )
		{
			modifier->text = string_createfromstring( args->command.arguments );
			view_t* textview = view_getsubview( args->views.view , "menuitem.text" );

			// update textview's basicmodifiers, mainly textarea

			view_new( &( ( modifier_args_t ) {
				.views      = { textview					 , NULL } ,
				.generator  = { args->generator.changed_flag , 0    } ,
				.command    = { "settext"					 , modifier->text } ,
				.toolset    = args->toolset							} ) );
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0  )
		{
			object_t* eventobject = modifier_defaultevent( "tapped" , modifier->action , modifier , NULL );
			vector_adddata( args->toolset->events , eventobject );
		}
	}