//
//  memguard.c
//  c36
//
//  Created by mtoth on 02/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

    #include "mem.h"
#include <stdint.h>

	struct mem_link_t
	{
		uint64_t value;
		struct mem_link_t* next;
	};

	static struct mem_link_t* mem_firstlink = NULL;

    void mem_init( )
    {
        mem_firstlink = calloc( 1 , sizeof( struct mem_link_t ) );
    }

	void mem_insert( uint64_t value )
	{
		struct mem_link_t* link = mem_firstlink;
		struct mem_link_t* last = NULL;
		while ( link != NULL )
		{
			if ( link->value > value )
			{
				struct mem_link_t* next = calloc( 1 , sizeof( struct mem_link_t ) );
				next->value = value;
				next->next = link;
				last->next = next;
				return;
			}
			last = link;
			link = link->next;
		}
		struct mem_link_t* next = calloc( 1 , sizeof( struct mem_link_t ) );
		next->value = value;
		last->next = next;
	}

	void mem_remove( uint64_t value )
	{
		struct mem_link_t* link = mem_firstlink;
		struct mem_link_t* last = NULL;
		while ( link != NULL )
		{
			if ( link->value == value )
			{
				last->next = link->next;
				free( link );
				return;
			}
			if ( link->value > value ) return;
			last = link;
			link = link->next;
		}
	}

	char mem_find( uint64_t value )
	{
		struct mem_link_t* link = mem_firstlink;
		while ( link != NULL )
		{
			if ( link->value == value ) return 1;
			if ( link->value >  value ) return 0;
			link = link->next;
		}
		return 0;
	}

    void* mem_calloc( void* oldpointer , size_t elements , size_t size , const char* place )
    {
        void* result = calloc( elements , size );
        #ifdef DEBUG
        if ( mem_firstlink == NULL ) mem_init( );
        if ( oldpointer != NULL ) assert( mem_find( ( uint64_t ) oldpointer ) == 0 );
		mem_insert( ( uint64_t ) result );
        #endif
        return result;
    }

    void* mem_malloc( void* oldpointer , size_t size , const char* place )
    {
        void* result = malloc( size );
        #ifdef DEBUG
        if ( mem_firstlink == NULL ) mem_init( );
        if ( oldpointer != NULL ) assert( mem_find( ( uint64_t ) oldpointer ) == 0 );
		mem_insert( ( uint64_t ) result );
        #endif
        return result;
    }

    void* mem_realloc( void* pointer , size_t size , const char* place )
    {
        #ifdef DEBUG
        if ( mem_firstlink == NULL ) mem_init( );
		assert( mem_find( ( uint64_t ) pointer ) == 1 );
		mem_remove( ( uint64_t ) pointer );
        #endif

        void* result = realloc( pointer , size );

        #ifdef DEBUG
		mem_insert( ( uint64_t ) result );
        #endif

        return result;
    }

    void  mem_free( void* pointer , const char* place )
    {
		printf( "\nmem_free %.16llx %s" , ( uint64_t ) pointer , place );
        #ifdef DEBUG
        if ( mem_firstlink == NULL ) mem_init( );
		assert( mem_find( ( uint64_t ) pointer ) == 1 );
		mem_remove( ( uint64_t ) pointer );
        #endif
        free( pointer );
    }
