//
//  settings.c
//  c36
//
//  Created by Milan Toth on 26/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "uisettings.h"

	// creates working paths

	uisettings_t* uisettings_init( char* cpath_resources , char* cpath_library , float scale , float width , float height )
	{
		uisettings_t* result = malloc( sizeof( uisettings_t ) );
		
        string_t* path_resources = string_createfrombytes( cpath_resources );
        string_t* path_font	= string_createfromstring( path_resources );
		string_addbytearray( path_font , "/Avenir.ttc" );
		string_t* path_library = string_createfrombytes( cpath_library );
		
		result->font = string_bytes( path_font );
		result->scale = scale;
		result->userdata = NULL;
		result->resources = string_bytes( path_resources );
		result->screensize = vector2_default( width , height );

		string_free( path_font );
		string_free( path_library );
		string_free( path_resources );
		
		return result;
	}

	// deletes working path structure

	void uisettings_free( uisettings_t* settings )
	{
		free( settings->font );
		free( settings->resources );
		free( settings );
	}
