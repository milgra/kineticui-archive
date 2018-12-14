//
//  modifier_messageitem.c
//  KineticUI
//
//  Created by Milan Toth on 01/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_messageitem.h"


	// creates button

	modifier_messageitem_t* modifier_messageitem_default( )
	{
		modifier_messageitem_t* modifier = malloc( sizeof( modifier_messageitem_t ) );
		
		modifier->_new = modifier_messageitem_new;
		modifier->_free = modifier_messageitem_free;
		modifier->type = "messageitem";
		modifier->text = NULL;
		
		return modifier;
	}

	// deletes button modifier

	void modifier_messageitem_free( modifier_messageitem_t* modifier )
	{
		if ( modifier->text != NULL ) string_free( modifier->text );
		free( modifier );
	}

	// generates new modifier state based on input, collecting events, timed views and setting changed state

	void modifier_messageitem_new( modifier_messageitem_t* modifier , modifier_args_t* args )
	{
		if ( strcmp( args->command.name , "settext" ) == 0 )
		{
			modifier->text = string_createfromstring( args->command.arguments );
			view_t* textview = view_getsubview( args->views.view , "messageitem.text" );
			
			// update textview's basicmodifiers, mainly textarea

			view_new( &( ( modifier_args_t ) {
				.views      = { textview					 , NULL } ,
				.generator  = { args->generator.changed_flag , 0    } ,
				.command    = { "settext"					 , modifier->text } ,
				.toolset    = args->toolset							} ) );

			// modify view
			// resize item and position box based on text view size
			// we assume that text view is already resized by an update input from viewbase
			// ***
			// 0 message_item
			// 1  box
			// 1  text
			// ***
			
			textview = view_getsubview( args->views.view , "messageitem.text" );
			vector2_t textviewsize = view_getsize( textview );
			vector2_t itemviewsize = view_getsize( args->views.view );
			itemviewsize.y = textviewsize.y;
			view_setsize( args->views.view , itemviewsize );
		}
	}