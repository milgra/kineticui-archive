//
//  string.c
//  c36
//
//  Created by Milan Toth on 25/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "string.h"

    #define UTF8_BOM "\xEF\xBB\xBF"

	// creates string

    string_t* string_create( )
	{
        string_t* string = malloc( sizeof( string_t ) );
        
        string->length = 0;			// current length of codepoint array
		string->length_real = 10;	// backing length of codepoint array
        string->length_bytes = 0;	// needed length of byte array for all codepoints
        string->codepoints = calloc( string->length_real , sizeof( uint32_t ) );
		
		return string;
	}

	// creates string from utf8 bytearray

	string_t* string_createfromformat( int size , const char* format , ... )
	{
		char *text = malloc( sizeof( char ) * size );
		va_list argptr;
		va_start( argptr , format );
		vsnprintf( text , size , format , argptr );
		va_end( argptr );
		string_t* string = string_createfrombytes( text );
		free( text );
		return string;
	}

	// creates string from utf8 bytearray

	string_t* string_createfrombytes( char* bytes )
	{
		string_t* string = string_create( );
		string_addbytearray( string , bytes );
		return string;
	}

	// creates string from string

	string_t* string_createfromstring( string_t* string )
	{
		string_t* result = string_create( );
		string_addstring( result , string );
		return result;
	}

	// deletes string

    void string_free( string_t* string )
	{
		free( string->codepoints );
		free( string );
	}

	// deletes strings in vector

	void string_freestrings( vector_t* vector )
	{
		for ( int index = 0 ; index < vector->length ; index++ ) string_free( vector->data[index] );
	}

	// resets string

	void string_reset( string_t* string )
	{
        string->length = 0;
		string->length_real = 10;
        string->length_bytes = 0;
        string->codepoints = calloc( string->length_real , sizeof( uint32_t ) );
	}

	// returns backing bytearray

	char* string_bytes( string_t* string )
	{
		char* bytes = calloc( string->length_bytes + 1 , sizeof( char ) );
		uint32_t position = 0;
		for ( int index = 0 ; index < string->length ; index++ )
		{
			uint32_t codepoint = string->codepoints[ index ];
			if ( codepoint < 0x80 )
			{
				bytes[ position++ ] = codepoint;
			}
			else if ( codepoint < 0x800 )
			{
				bytes[ position++ ] = ( codepoint >> 6 ) | 0xC0;
				bytes[ position++ ] = ( codepoint & 0x3F ) | 0x80;
			}
			else if ( codepoint < 0x1000 )
			{
				bytes[ position++ ] = ( codepoint >> 12 ) | 0xE0;
				bytes[ position++ ] = ( ( codepoint >> 6 ) & 0x3F ) | 0x80;
				bytes[ position++ ] = ( codepoint & 0x3F ) | 0x80;
			}
			else
			{
				bytes[ position++ ] = ( codepoint >> 18 ) | 0xF0;
				bytes[ position++ ] = ( ( codepoint >> 12 ) & 0x3F ) | 0x80;
				bytes[ position++ ] = ( ( codepoint >> 6  ) & 0x3F ) | 0x80;
				bytes[ position++ ] = ( codepoint & 0x3F ) | 0x80;
			}
		}
		return bytes;
	}

	uint8_t string_getcodebytelength( uint32_t codepoint )
	{
		uint8_t codelength = 4;
		if ( codepoint < 0x80 ) codelength = 1;
        else if ( codepoint < 0x800 ) codelength = 2;
        else if ( codepoint < 0x1000 ) codelength = 3;
		return codelength;
	}

	// adds a single codepoint

    void string_addcodepoint( string_t* string , uint32_t codepoint )
	{
		uint8_t codelength = string_getcodebytelength( codepoint );
		// expand
		if ( string->length_real == string->length )
		{
			string->codepoints = realloc( string->codepoints , sizeof( uint32_t ) * ( string->length_real + 10 ) );
			string->length_real += 10;
		}
		string->codepoints[ string->length ] = codepoint;
		string->length += 1;
		string->length_bytes += codelength;
	}

	// adds buffer containing bytes for one codepoint

	void string_addbuffer( string_t* string , char* buffer , char length )
	{
		// filter byte order mark
		if ( strcmp( buffer , UTF8_BOM ) != 0 )
		{
			uint32_t codepoint = 0;
			// extract codepoint
			if      ( length == 1 ) codepoint = buffer[0] & 0x7F;
			else if ( length == 2 ) codepoint = ( buffer[0] & 0x1F ) << 6 | ( buffer[1] & 0x3F );
			else if ( length == 3 ) codepoint = ( buffer[0] & 0xF ) << 12 | ( buffer[1] & 0x3F ) << 6 | ( buffer[2] & 0x3F );
			else if ( length == 4 ) codepoint = ( buffer[0] & 0x7 ) << 18 | ( buffer[1] & 0x3F ) << 12 | ( buffer[2] & 0x3F ) << 6 | ( buffer[3] & 0x3F );
			// add codepoint
			string_addcodepoint( string , codepoint );
		}
	}

	// adds utf8 encoded byte array

    void string_addbytearray( string_t* string , char* bytearray )
	{
        char buffer[ 4 ];
        char buffer_position = 0;
        while ( *bytearray != 0 )
        {
			// checking unicode closing characters or last character
            if ( ( *bytearray & 0xFF ) >> 7 == 0 || ( *bytearray & 0xFF ) >> 6 == 3 )
            {
                if ( buffer_position > 0 )
                {
					string_addbuffer( string , buffer , buffer_position );
					// reset unicode buffer
					memset( &buffer , 0 , 4 );
					buffer_position = 0;
                }
            }
			// storing actual byte in unicode codepoint buffer
            buffer[ buffer_position++ ] = *bytearray;
            // step in byte array
            bytearray += 1;
            // invalid utf sequence, aborting
            if ( buffer_position == 4 ) return;
        }
		// add remaining buffer content
		if ( buffer_position > 0 ) string_addbuffer( string , buffer , buffer_position );
	}

	// adds string

    void string_addstring( string_t* string_a , string_t* string_b )
	{
		if ( string_b != NULL )
		{
			uint32_t newlength = string_a->length + string_b->length;
			uint32_t newlength_real = string_a->length_real + string_b->length_real;
			uint32_t newlength_bytes = string_a->length_bytes + string_b->length_bytes;
			
			string_a->codepoints = realloc( string_a->codepoints , sizeof( uint32_t ) * newlength_real );
			memcpy( (void*) ( string_a->codepoints + string_a->length ) , ( void* ) string_b->codepoints , string_b->length * sizeof( uint32_t ) );
			
			string_a->length = newlength;
			string_a->length_real = newlength_real;
			string_a->length_bytes = newlength_bytes;
		}
	}

	// removes codepoint

	void string_removecodepointatindex( string_t* string , uint32_t index )
	{
		uint32_t codepoint = string->codepoints[ index ];
		uint8_t codelength = string_getcodebytelength( codepoint );
		string->length_bytes -= codelength;
		memmove( string->codepoints + index , string->codepoints + index + 1 , ( string->length - index ) * sizeof( uint32_t ) );
		string->length -= 1;
	}

	// removes codepoints in range

	void string_removecodepointsinrange( string_t* string , uint32_t start , uint32_t end )
	{
		if ( end > string->length ) end = string->length;
		for ( int index = start ; index < end ; index++ )
		{
			uint32_t codepoint = string->codepoints[ index ];
			uint8_t codelength = string_getcodebytelength( codepoint );
			string->length_bytes -= codelength;
		}
		if ( end < string->length )
		{
			memmove
			(
				string->codepoints + start ,
				string->codepoints + end + 1 ,
				( string->length - end ) * sizeof( uint32_t )
			);
		}
		string->length -= end - start + 1;
	}

	// replaces codepoints

	void string_replacecodepoints( string_t* string , uint32_t oldcp , uint32_t newcp )
	{
		for ( int index = 0 ; index < string->length ; index++ )
		{
			if ( string->codepoints[ index ] == oldcp ) string->codepoints[ index ] = newcp;
		}
	}

	// compares two string

	int8_t string_compare( string_t* string_a , string_t* string_b )
	{
		char* bytes_a = string_bytes( string_a );
		char* bytes_b = string_bytes( string_b );
		int8_t result = strcmp( bytes_a , bytes_b );
		free( bytes_a );
		free( bytes_b );
		return result;
	}

	// splits string at codepoint to a vector

	vector_t* string_split( string_t* string , char codepoint )
	{
		vector_t* vector = vector_create( );
		string_t* segment = string_create( );
		for ( int index = 0 ; index < string->length ; index++ )
		{
			if ( string->codepoints[ index ] == codepoint )
			{
				// add word to result, create new word
				if ( segment->length > 0 )
				{
					vector_adddata( vector , segment );
					segment = string_create( );
				}
			}
			else string_addcodepoint( segment , string->codepoints[ index ] );
		}
		// add word to result
		if ( segment->length > 0 ) vector_adddata( vector , segment );
		else string_free( segment );
		return vector;
	}

	// allocs copies c string

	char* string_copycstring( char* string )
	{
		char* result = NULL;
		if ( string != NULL )
		{
			result = calloc( strlen( string ) + 1 , sizeof( char ) );
			memcpy( result , string , strlen( string ) );
		}
		return result;
	}

	// test string functions

	#ifdef DEBUG
	void string_test( )
	{
		printf( "\nSTRING TEST SESSION START" );
		printf( "\n1 CREATE EMTPY STRING" );
		string_t* s1 = string_create( );
		printf( "\n2 DELETE EMTPY STRING" );
		string_free( s1 );
		printf( "\n3 ADDING CODEPOINTS" );
		string_t* s2 = string_create( );
		string_addcodepoint( s2 , (uint32_t)'A' );
		string_addcodepoint( s2 , (uint32_t)'B' );
		string_addcodepoint( s2 , (uint32_t)'C' );
		printf( "\nCHECK STRING" );
		char* s2s0 = string_bytes( s2 );
		printf( "\n%s" , s2s0 );
		printf( "\nlength %i length_real %i length_bytes %i" , s2->length , s2->length_real , s2->length_bytes );
		printf( "\n3 ADDING BYTEARRAY" );
		string_addbytearray( s2 , " na meg a kisfássszomát!!!" );
		printf( "\nCHECK STRING" );
		char* s2s1 = string_bytes( s2 );
		printf( "\n%s" , s2s1 );
		printf( "\nlength %i length_real %i length_bytes %i" , s2->length , s2->length_real , s2->length_bytes );
		free( s2s1 );
		printf( "\n4 REMOVING CODEPOINT AT INDEX 2" );
		string_removecodepointatindex( s2 , 2 );
		printf( "\nCHECK STRING" );
		char* s2s2 = string_bytes( s2 );
		printf( "\n%s" , s2s2 );
		printf( "\nlength %i length_real %i length_bytes %i" , s2->length , s2->length_real , s2->length_bytes );
		free( s2s2 );
		printf( "\n5 REMOVING CODEPOINTS RANGE 4 AND 7" );
		string_removecodepointsinrange( s2 , 4 , 7 );
		printf( "\nCHECK STRING" );
		char* s2s3 = string_bytes( s2 );
		printf( "\n%s" , s2s3 );
		printf( "\nlength %i length_real %i length_bytes %i" , s2->length , s2->length_real , s2->length_bytes );
		free( s2s3 );
		printf( "\n6 ADDING STRING" );
		string_t* s3 = string_create( );
		string_addbytearray( s3 , "ó ha rózsabimbaú lehetnék" );
		printf( "\nCHECK STRING" );
		char* s3s0 = string_bytes( s3 );
		printf( "\n%s" , s3s0 );
		printf( "\nlength %i length_real %i length_bytes %i" , s3->length , s3->length_real , s3->length_bytes );
		free( s3s0 );
		string_addstring( s2 , s3 );
		printf( "\nCHECK STRING" );
		char* s2s4 = string_bytes( s2 );
		printf( "\n%s" , s2s4 );
		printf( "\nlength %i length_real %i length_bytes %i" , s2->length , s2->length_real , s2->length_bytes );
		free( s2s4 );
		printf( "\nSTRING TEST SESSION END" );
	}
	#endif
