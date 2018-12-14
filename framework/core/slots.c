//
//  slots.c
//  KineticUI
//
//  Created by Milan Toth on 05/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "slots.h"

	// a buffer ring for inter-thread data exchange without locking

	slots_t* slots_create( int size )
	{
        slots_t* result = malloc( sizeof( slots_t ) );
        result->size = size;
		result->data = malloc( sizeof( void* ) * size );
		result->getindex = 0;
		result->putindex = 0;
		for ( int index = 0 ; index < size ; index++ ) result->data[ index ] = NULL; // calloc is not working on Blackberry
        return result;
	}

	// deletes slots

	void slots_free( slots_t* slots )
	{
		free( slots->data );
		free( slots );
	}

	// puts data in slots

	void slots_put( slots_t* slots , void* data , char* success )
	{
		int  nextputindex =  slots->putindex + 1;
		if ( nextputindex == slots->size - 1 ) nextputindex = 0;
		// there is space for new data
		if ( nextputindex != slots->getindex )
		{
			slots->data[ nextputindex ] = data;
			slots->putindex = nextputindex;
			// printf( "\nputting data at %i" , nextputindex );
			*success = 1;
		}
		// no space for new data
		else *success = 0;
	}

	// gets data from slots

	void slots_get( slots_t* slots , void** data )
	{
		*data = slots->data[ slots->getindex ];
		// we have data for actual get index
		if ( *data != NULL )
		{
			// remove data
			// printf( "\ngettting data from %i" , slots->getindex );
			slots->data[ slots->getindex ] = NULL;
		}
		if ( slots->putindex != slots->getindex )
		{
			// increase get index
			int  nextgetindex =  slots->getindex + 1;
			if ( nextgetindex == slots->size - 1 ) nextgetindex = 0;
			slots->getindex = nextgetindex;
		}
	}

	// test slots

	#ifdef DEBUG
	void slots_test( )
	{
		printf( "\nSLOT TEST SESSION START" );
		printf( "\n1 SLOT CREATE" );
		slots_t* s1 = slots_create( 4 );
		printf( "\n2 SLOT DELETE" );
		slots_free( s1 );
		printf( "\n3 SLOT PUT" );
		char success = 0;
		slots_t* s2 = slots_create( 6 );
		slots_put( s2 , "fakk" , &success );
		printf( "\ngetindex %i putindex %i success %i" , s2->getindex , s2->putindex , success );
		slots_put( s2 , "makk" , &success );
		printf( "\ngetindex %i putindex %i success %i" , s2->getindex , s2->putindex , success );
		slots_put( s2 , "ehehe" , &success );
		printf( "\ngetindex %i putindex %i success %i" , s2->getindex , s2->putindex , success );
		slots_put( s2 , "muhahah" , &success );
		printf( "\ngetindex %i putindex %i success %i" , s2->getindex , s2->putindex , success );
		slots_put( s2 , "bazdmeg" , &success );
		printf( "\ngetindex %i putindex %i success %i" , s2->getindex , s2->putindex , success );
		slots_put( s2 , "kezdmeg" , &success );
		printf( "\ngetindex %i putindex %i success %i" , s2->getindex , s2->putindex , success );
		printf( "\n4 SLOT READ" );
		char* data = NULL;
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		slots_get( s2 , (void**)&data );
		if ( data != NULL ) printf( "\nREAD %s" , data );
		else printf( "\nNULL READ" );
		printf( "\nSLOT TEST SESSION END" );
	}
	#endif
