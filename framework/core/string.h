//
//  string.h
//  c36
//
//  Created by Milan Toth on 25/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__string__
#define __c36__string__

    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>
	#include <stdarg.h>
	#include "vector.h"

    typedef struct _string_t string_t;
    struct _string_t
    {
        uint32_t  length;
        uint32_t  length_real;
        uint32_t  length_bytes;
        uint32_t* codepoints;
    };

    string_t*	string_create( );
	string_t*	string_createfromformat( int size , const char* format , ... );
	string_t*	string_createfrombytes( char* bytes );
	string_t*	string_createfromstring( string_t* string );
    void		string_free( string_t* string );
	void		string_freestrings( vector_t* vector );
	void		string_reset( string_t* string );
	char*		string_bytes( string_t* string );
    void		string_addcodepoint( string_t* string , uint32_t codepoint );
    void		string_addbytearray( string_t* string , char* bytearray );
    void		string_addstring( string_t* string_a , string_t* string_b );
	void		string_removecodepointatindex( string_t* string , uint32_t index );
	void		string_removecodepointsinrange( string_t* string , uint32_t start , uint32_t end );
	void		string_replacecodepoints( string_t* string , uint32_t oldcp , uint32_t newcp );
	int8_t		string_compare( string_t* string_a , string_t* string_b );
	vector_t*	string_split( string_t* string , char character );
	char*		string_copycstring( char* string );
	#ifdef DEBUG
	void		string_test( );
	#endif

#endif /* defined(__c36__string__) */
