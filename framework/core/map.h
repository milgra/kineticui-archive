//
//  map.h
//  c36
//
//  Created by Milan Toth on 23/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//	Copyright (c) 2009, 2011, 2013 Per Ola Kristensson.

#ifndef _STRMAP_H_
#define _STRMAP_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "vector.h"

	typedef struct pair_t pair_t;
	struct pair_t {
		char *key;
		void *value;
	};

	typedef struct bucket_t bucket_t;
	struct bucket_t {
		unsigned int count;
		pair_t *pairs;
	};

	typedef struct map_t map_t;
	struct map_t {
		// char* type;				// types in map, for a controlled release of content later
		unsigned int count;
		bucket_t *buckets;
	};

	map_t * map_create( unsigned int capacity );
	void	map_free( map_t *map );
	int		map_put( map_t *map , const char *key , void *value );
	void*	map_get( map_t *map , const char *key );
	int		map_haskey( map_t *map , const char *key );
	int		map_size( map_t *map );
	vector_t* map_keys( map_t *map );
	vector_t* map_values( map_t *map );

	void	map_writefofile( map_t* map , char* path );
	void	map_addfilecontents( map_t* map , char* path );
	#ifdef DEBUG
	void	map_test( );
	#endif

#endif