//
//  types.h
//  c36
//
//  Created by Milan Toth on 30/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_types_h
	#define c36_types_h

	#include "string.h"

	extern char freechar;

    typedef struct _object_t object_t;
    struct _object_t
    {
		char* type;
		void* data;
    };

	object_t* object_create( char* type , void* data );
	void object_free( object_t* object );


#endif
