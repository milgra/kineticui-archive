//
//  modifier_conversationitem.c
//  KineticUI
//
//  Created by Milan Toth on 26/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_conversationitem.h"

	// creates button

	modifier_conversationitem_t* modifier_conversationitem_default( )
	{
		modifier_conversationitem_t* modifier = malloc( sizeof( modifier_conversationitem_t ) );
		
		modifier->_new = modifier_conversationitem_new;
		modifier->_free = modifier_conversationitem_free;
		modifier->type = "convitem";
		return modifier;
	}

	// deletes button modifier

	void modifier_conversationitem_free( modifier_conversationitem_t* modifier )
	{
		free( modifier );
	}

	// generates new modifier state based on input, collecting events, timed views and setting changed state

	void modifier_conversationitem_new( modifier_conversationitem_t* modifier , modifier_args_t* args )
	{
		if ( strcmp( args->command.name , "focus" ) == 0  )
		{
			view_t* scrollview = view_getsubview( args->views.view , "convitem.label" );
			modifier_t* scrollmodifier = ( void*) view_getmodifier( scrollview , "scroller" );
            modifier_scroller_t* data = scrollmodifier->data;
			data->enable_y = 0;
		}
		else if ( strcmp( args->command.name , "touchdown" ) == 0  )
		{
			modifier->pressed = 1;
		}
		else if ( strcmp( args->command.name , "touchdrag" ) == 0  )
		{
			modifier->pressed = 0;
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0  )
		{
			if ( modifier->pressed == 1 )
			{
				view_t* textview = view_getsubview( args->views.view , "convitem.text" );
				modifier_t* textarea = view_getmodifier( textview , "textarea" );
				string_t* string = modifier_textarea_gettext( textarea );

				char* cstring = string_bytes( string );
				object_t* eventobject = modifier_defaultevent( "itemselected" , "openconversation" , modifier , string_bytes( string ) );
				free( cstring );
				string_free( string );
				
				vector_adddata( args->toolset->events , eventobject );
			}
		}
		else if ( strcmp( args->command.name , "settext" ) == 0  )
		{
			modifier->text = string_createfromstring( args->command.arguments );
			view_t* textview = view_getsubview( args->views.view , "convitem.text" );

			// update textview's basicmodifiers, mainly textarea

			view_new( &( ( modifier_args_t ) {
				.views      = { textview					 , NULL } ,
				.generator  = { args->generator.changed_flag , 0    } ,
				.command    = { "settext"					 , modifier->text } ,
				.toolset    = args->toolset							} ) );

			// update scrollview
			
			view_t* scrollview = view_getsubview( args->views.view , "convitem.label" );

			view_new( &( ( modifier_args_t ) {
				.views      = { scrollview					 , NULL } ,
				.generator  = { args->generator.changed_flag , 0    } ,
				.command    = { "update"					 , NULL } ,
				.toolset    = args->toolset							} ) );
		}
		else if ( strcmp( args->command.name , "select" ) == 0  )
		{
			string_t* text = args->command.arguments;
			if ( string_compare( text , modifier->text ) == 0 )
			{
				view_t* overlay = view_getsubview( args->views.view , "convitem.overlay" );
				view_setsize( overlay , vector2_default( 240.0 * args->toolset->settings->scale , -50.0 * args->toolset->settings->scale ) );
			}
			else
			{
				view_t* overlay = view_getsubview( args->views.view , "convitem.overlay" );
				view_setsize( overlay , vector2_default( 0.0 , 0.0 ) );
			}
		}
	}