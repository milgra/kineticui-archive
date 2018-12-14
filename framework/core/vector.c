//
//  vector.c
//  c36
//
//  Created by Milan Toth on 23/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

    #include "vector.h"

	// creates new vector

    vector_t* vector_create( )
    {
        vector_t* vector = malloc( sizeof( vector_t ) );
		vector->data = malloc( sizeof( void* ) * 10 );
		vector->length = 0;
		vector->length_real = 10;
        return vector;
    }

	// deletes vector

	void vector_free( vector_t* vector )
	{
		free( vector->data );
		free( vector );
	}

	// resets vector

	void vector_reset( vector_t* vector )
	{
		vector->data = realloc( vector->data , sizeof( void* ) * 10 );
		vector->length = 0;
		vector->length_real = 10;
	}

	// expands storage

	void vector_expand( vector_t* vector )
	{
		if ( vector->length == vector->length_real )
		{
			vector->length_real += 10;
			vector->data = realloc( vector->data , sizeof( void* ) * vector->length_real );
		}
	}

	// adds single data

	void vector_adddata( vector_t* vector , void* data )
	{
		vector_expand( vector );
		vector->data[ vector->length ] = data;
		vector->length += 1;
	}

	// adds data at given index

	void vector_adddataatindex( vector_t* vector, void* data , unsigned long index )
	{
		vector_expand( vector );
		memmove( vector->data + index + 1 , vector->data + index , ( vector->length - index ) * sizeof( void* ) );
		vector->data[ index ] = data;
		vector->length += 1;
	}

	// adds all items in vector to vector
	
	void vector_adddatainvector( vector_t* vector_a , vector_t* vector_b )
	{
		vector_a->length_real += vector_b->length_real;
		vector_a->data = realloc( vector_a->data , sizeof( void* ) * vector_a->length_real );
		memcpy( vector_a->data + vector_a->length , vector_b->data , vector_b->length * sizeof( void* ) );
		vector_a->length += vector_b->length;
	}

	// removes single data

	void vector_removedata( vector_t* vector , void* data )
	{
		uint32_t index = vector_indexofdata( vector , data );
		if ( index < UINT32_MAX ) vector_removedataatindex( vector , index );
	}

	// removes data at index

	void vector_removedataatindex( vector_t* vector , uint32_t index )
	{
		memmove( vector->data + index , vector->data + index + 1 , ( vector->length - index ) * sizeof( void* ) );
		vector->length -= 1;
	}

	// removes data in range

	void vector_removedatainrange( vector_t* vector , uint32_t start , uint32_t end )
	{
		memmove( vector->data + start , vector->data + end + 1 , ( vector->length - end ) * sizeof( void* ) );
		vector->length -= end - start + 1;
	}

	// removes data in vector

	void vector_removedatainvector( vector_t* vector_a , vector_t* vector_b )
	{
		for ( int index = 0 ; index < vector_b->length ; index++ )
		{
			vector_removedata( vector_a , vector_b->data[ index ] );
		}
	}

	// returns index of data

	uint32_t vector_indexofdata( vector_t* vector , void* data )
	{
		void** actual = vector->data;
		uint32_t index = 0;
		while ( index < vector->length )
		{
			if ( *actual == data ) return index;
			index++;
			actual += 1;
		}
		return UINT32_MAX;
	}

	// tests vector

	#ifdef DEBUG
	void vector_test( )
	{
		printf( "\nVECTOR TEST SESSION START" );
		printf( "\n1 CREATE EMPTY" );
		vector_t* v1 = vector_create( );
		printf( "\n2 DELETE EMPTY" );
		vector_free( v1 );
		printf( "\n3 ADDING DATA" );
		vector_t* v2 = vector_create( );
		vector_adddata( v2 , "Milan" );
		printf( "\nCHECKING DATA" );
		printf( "\nDATA %s LENGTH %i LENGTH_REAL %i VERIFIED %i" , v2->data[0] , v2->length , v2->length_real , strcmp( v2->data[0] , "Milan" ) == 0 );
		printf( "\n4 CHECKING EXPAND" );
		vector_adddata( v2 , "Károly" );
		vector_adddata( v2 , "Peti" );
		vector_adddata( v2 , "Laci" );
		vector_adddata( v2 , "Béla" );
		vector_adddata( v2 , "Sári" );
		vector_adddata( v2 , "Dóri" );
		vector_adddata( v2 , "Nelli" );
		vector_adddata( v2 , "Ferenc" );
		vector_adddata( v2 , "István" );
		vector_adddata( v2 , "Zsuzsa" );
		printf( "\nLENGTH IS %i , REAL LENGTH IS %i , VERIFIED %i" , v2->length , v2->length_real , v2->length == 11 );
		printf( "\nPRINTING CONTENT\n" );
		for ( int index = 0 ; index < v2->length ; index++ ) printf( "%i:%s " , index , v2->data[ index ] );
		printf( "\n5 ADDING DATA Penziás AT INDEX 2 " );
		vector_adddataatindex( v2 , "Penziás" , 2 );
		printf( "\nCHECK NAMES\n" );
		for ( int index = 0 ; index < v2->length ; index++ ) printf( "%i:%s " , index , v2->data[ index ] );
		printf( "\n6 ADDING VECTOR" );
		vector_t* v3 = vector_create( );
		vector_adddata( v3 , "Baja" );
		vector_adddata( v3 , "Lala" );
		vector_adddata( v3 , "Mama" );
		vector_adddatainvector( v2 , v3 );
		printf( "\nCHECK NAME\n" );
		for ( int index = 0 ; index < v2->length ; index++ ) printf( "%i:%s " , index , v2->data[ index ] );
		printf( "\n7 REMOVE DATA Dóri" );
		vector_removedata( v2 , "Dóri" );
		printf( "\nCHECK NAMES\n" );
		for ( int index = 0 ; index < v2->length ; index++ ) printf( "%i:%s " , index , v2->data[ index ] );
		printf( "\n8 REMOVE DATA AT INDEX 8" );
		vector_removedataatindex( v2 , 8 );
		printf( "\nCHECK NAMES\n" );
		for ( int index = 0 ; index < v2->length ; index++ ) printf( "%i:%s " , index , v2->data[ index ] );
		printf( "\n9 REMOVE RANGE BETWEEN 6 AND 9" );
		vector_removedatainrange( v2 , 6 , 9 );
		printf( "\nCHECK NAMES\n" );
		for ( int index = 0 ; index < v2->length ; index++ ) printf( "%i:%s " , index , v2->data[ index ] );
		printf( "\n10 INDEX OF Baja" );
		uint8_t index = vector_indexofdata( v2 , "Baja" );
		printf( "\nINDEX IS %i" , index );
		printf( "\nVECTOR TEST SESSION END" );
	}
	#endif
