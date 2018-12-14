//
//  modifier.c
//  c36
//
//  Created by Milan Toth on 30/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier.h"


	// creates object containing an mqtt event

	object_t* modifier_defaultevent( char* name , char* todo , void* root , void* data )
	{
		modifierevent_t* event = malloc( sizeof( modifierevent_t ) );
		event->name = string_copycstring( name );
		event->todo = string_copycstring( todo );
		event->data = data;
        event->root = root;
		object_t* result = object_create( "modifierevent" , event );
		return result;
	}

	// deletes object containing an mqtt event

	void modifier_freeevent( object_t* object )
	{
		modifierevent_t* event = object->data;
		if ( event->name != NULL ) free( event->name );
		if ( event->todo != NULL ) free( event->todo );
		free( event );
		object_free( object );
	}
