//
//  types.c
//  c36
//
//  Created by Milan Toth on 30/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "types.h"

	char freechar = 0;

	object_t* object_create( char* type , void* data )
	{
		object_t* result = malloc( sizeof( object_t ) );
		result->type = string_copycstring( type );
		result->data = data;
		return result;
	}


	void object_free( object_t* object )
	{
		free( object->type );
		free( object );
	}