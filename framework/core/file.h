//
//  corefile.h
//  c36
//
//  Created by Milan Toth on 05/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__corefile__
#define __c36__corefile__

	#include <stdio.h>
	#include <stdlib.h>
	#include "map.h"
	#include "string.h"

	#define kTypeMap 1
	#define kTypeString 3
	#define kTypeVector 2

	typedef struct type_container_t type_container_t;
	struct type_container_t
	{
		char  type;
		void* data;
	};

	void file_writetofile( map_t* map , char* path );
	map_t* file_readfile( char* path );

	void file_appendstringtofile( string_t* string , FILE* file_pointer );
	void file_appendvectortofile( vector_t* vector , FILE* file_pointer );
	void file_appendmaptofile( map_t* map , FILE* file_pointer );

	string_t* file_readstring( FILE* file_pointer );
	vector_t* file_readvector( FILE* file_pointer );
	map_t* file_readmap( FILE* file_pointer );
	
	type_container_t* file_defaultcontainer( char type , void* data );

#endif /* defined(__c36__corefile__) */
