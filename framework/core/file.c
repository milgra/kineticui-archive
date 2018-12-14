//
//  corefile.c
//  c36
//
//  Created by Milan Toth on 05/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "file.h"


	// appends string to opened file

	void file_appendstringtofile( string_t* string , FILE* file_pointer )
	{
		// write type
		size_t writeup;
		char typebytes[ 1 ] = { kTypeString };
		writeup = fwrite( typebytes , sizeof( uint8_t ) , 1 , file_pointer );
		// write bytes
		char* bytes = string_bytes( string );
		writeup = fwrite( bytes , sizeof( uint8_t ), string->length_bytes , file_pointer );
		// write closing 0
		char zerobytes[ 1 ] = { 0 };
		writeup = fwrite( zerobytes , sizeof( uint8_t ) , 1 , file_pointer );
		// cleanup
		free( bytes );
	}

	// appends vector to opened file

	void file_appendvectortofile( vector_t* vector , FILE* file_pointer )
	{
		// write type
		char typebytes[ 1 ] = { kTypeVector };
		size_t writeup = fwrite( typebytes , sizeof( uint8_t ) , 1 , file_pointer );
		for ( int index = 0 ; index < vector->length ; index++ )
		{
			type_container_t* container = vector->data[ index ];
			if		( container->type == kTypeString ) file_appendstringtofile( container->data , file_pointer );
			else if ( container->type == kTypeVector )file_appendvectortofile( container->data , file_pointer );
			else if ( container->type == kTypeMap	 ) file_appendmaptofile( container->data , file_pointer );
		}
		// write closing 0
		char zerobytes[ 1 ] = { 0 };
		writeup = fwrite( zerobytes , sizeof( uint8_t ) , 1 , file_pointer );
	}

	// appends map to opened file

	void file_appendmaptofile( map_t* map , FILE* file_pointer )
	{
		// write type
		char typebytes[ 1 ] = { kTypeMap };
		size_t writeup = fwrite( typebytes , sizeof( uint8_t ) , 1 , file_pointer );
		
		vector_t* keys = map_keys( map );
		for ( int index = 0 ; index < keys->length ; index++ )
		{
			// write key
			string_t* keystring = string_createfrombytes( keys->data[ index ] );
			file_appendstringtofile( keystring , file_pointer );
			string_free( keystring );
			// write value
			type_container_t* container = map_get( map , keys->data[ index ] );
			if		( container->type == kTypeString ) file_appendstringtofile( container->data , file_pointer );
			else if ( container->type == kTypeVector ) file_appendvectortofile( container->data , file_pointer );
			else if ( container->type == kTypeMap	 ) file_appendmaptofile( container->data , file_pointer );
		}
		vector_free( keys );
		// write closing 0
		char zerobytes[ 1 ] = { 0 };
		writeup = fwrite( zerobytes , sizeof( uint8_t ) , 1 , file_pointer );
	}

	// writes map to file recursively

	void file_writetofile( map_t* map , char* path )
	{
		remove( path );
		FILE* file_pointer = fopen( path , "a" );
        if ( file_pointer != NULL )
        {
			file_appendmaptofile( map , file_pointer );
            fclose( file_pointer );
        }
	}

	// reads string from file

	string_t* file_readstring( FILE* file_pointer )
	{
		char* bytes = malloc( sizeof( char ) * 50 );
		int index = 0;
		int length = 50;
		int nextbyte = fgetc( file_pointer );
		while ( nextbyte != EOF && nextbyte != 0 )
		{
			bytes[ index++ ] = nextbyte;
			if ( index == length )
			{
				length += 50;
				bytes = realloc( bytes , sizeof( char ) * length );
			}
			nextbyte = fgetc( file_pointer );
		}
		string_t* result = string_createfrombytes( bytes );
		free( bytes );
		return result;
	}

	// reads vector from file

	vector_t* file_readvector( FILE* file_pointer )
	{
		vector_t* result = vector_create( );
		int nextbyte = fgetc( file_pointer );
		while ( nextbyte != EOF && nextbyte != 0 )
		{
			if ( nextbyte == kTypeMap )
			{
				map_t* map = file_readmap( file_pointer );
				type_container_t* container = malloc( sizeof( type_container_t ) );
				container->type = kTypeMap;
				container->data = map;
				vector_adddata( result , container );
			}
			else if ( nextbyte == kTypeVector )
			{
				vector_t* vector = file_readvector( file_pointer );
				type_container_t* container = malloc( sizeof( type_container_t ) );
				container->type = kTypeVector;
				container->data = vector;
				vector_adddata( result , container );
			}
			else if ( nextbyte == kTypeString )
			{
				string_t* string = file_readstring( file_pointer );
				type_container_t* container = malloc( sizeof( type_container_t ) );
				container->type = kTypeString;
				container->data = string;
				vector_adddata( result , container );
			}
			// read next values type
			nextbyte = fgetc( file_pointer );
		}
		return result;
	}

	// reads map from file

	map_t* file_readmap( FILE* file_pointer )
	{
		map_t* result = map_create( 100 );	// !!! should be dynamic somehow
		int nextbyte = fgetc( file_pointer );
		while ( nextbyte != EOF && nextbyte != 0 )
		{
			// read key
			string_t* key = file_readstring( file_pointer );
			char* keycstring = string_bytes( key );
			// read value
			nextbyte = fgetc( file_pointer );
			if ( nextbyte == kTypeMap )
			{
				map_t* map = file_readmap( file_pointer );
				type_container_t* container = malloc( sizeof( type_container_t ) );
				container->type = kTypeMap;
				container->data = map;
				map_put( result , keycstring , container );
			}
			else if ( nextbyte == kTypeVector )
			{
				vector_t* vector = file_readvector( file_pointer );
				type_container_t* container = malloc( sizeof( type_container_t ) );
				container->type = kTypeVector;
				container->data = vector;
				map_put( result , keycstring , container );
			}
			else if ( nextbyte == kTypeString )
			{
				string_t* string = file_readstring( file_pointer );
				type_container_t* container = malloc( sizeof( type_container_t ) );
				container->type = kTypeString;
				container->data = string;
				map_put( result , keycstring , container );
			}
			free( keycstring );
			string_free( key );
			// read key type
			nextbyte = fgetc( file_pointer );
		}
		return result;
	}

	// reads up file, returns map

	map_t* file_readfile( char* path )
	{
		map_t* result = NULL;
		FILE* file_pointer = fopen( path , "r" );
        if ( file_pointer != NULL )
		{
			int nextbyte = fgetc(  file_pointer );
			if ( nextbyte == kTypeMap )
			{
				result = file_readmap( file_pointer );
			}
			fclose( file_pointer );
		}
		return result;
	}

	// creates type container

	type_container_t* file_defaultcontainer( char type , void* data )
	{
		type_container_t* result = malloc( sizeof( type_container_t ) );
		result->type = type;
		result->data = data;
		return result;
	}