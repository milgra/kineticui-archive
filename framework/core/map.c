//
//  map.c
//  c36
//
//  Created by Milan Toth on 23/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//	Copyright (c) 2009, 2011, 2013 Per Ola Kristensson.
//

	#include "map.h"

	// returns corresponding pair from bucket

	static pair_t* get_pair( bucket_t *bucket , const char *key )
	{
		unsigned int index;
		pair_t *pair;
		if ( bucket->count == 0 ) return NULL;
		pair = bucket->pairs;
		index = 0;
		while ( index < bucket->count )
		{
			if ( pair->key != NULL && pair->value != NULL )
			{
				if ( strcmp( pair->key , key ) == 0 ) return pair;
			}
			pair++;
			index++;
		}
		return NULL;
	}

	// returns a hash code for the provided string.

	static unsigned long hash( const char *str )
	{
		unsigned long hash = 5381;
		int chr;
		while ( ( chr = *str++) ) hash = ( ( hash << 5 ) + hash ) + chr;
		return hash;
	}

	// creates map with capacity

	map_t * map_create( unsigned int capacity )
	{
		map_t *map;
		map = malloc( sizeof( map_t ) );
		if ( map == NULL ) return NULL;
		map->count = capacity;
		map->buckets = malloc( map->count * sizeof( bucket_t ) );
		if ( map->buckets == NULL )
		{
			free( map );
			return NULL;
		}
		memset( map->buckets , 0 , map->count * sizeof(bucket_t) );
		return map;
	}

	// deletes map

	void map_free(map_t *map)
	{
		unsigned int index, bindex;
		bucket_t *bucket;
		pair_t *pair;

		if ( map == NULL ) return;
		bucket = map->buckets;
		index = 0;
		while ( index < map->count )
		{
			pair = bucket->pairs;
			bindex = 0;
			while ( bindex < bucket->count )
			{
				free( pair->key );
				pair++;
				bindex++;
			}
			free( bucket->pairs );
			bucket++;
			index++;
		}
		free( map->buckets );
		free( map );
	}

	// puts in value with key

	int map_put( map_t *map , const char *key , void *value )
	{
		size_t index;
		bucket_t *bucket;
		pair_t *tmp_pairs, *pair;
		if (map == NULL) return 0;
		if (key == NULL ) return 0;
		// get a pointer to the bucket the key string hashes to
		index = hash( key ) % map->count;
		bucket = &( map->buckets[ index ] );
		// check if we can handle insertion by simply replacing
		// an existing value in a key-value pair in the bucket.
		if ( ( pair = get_pair( bucket , key ) ) != NULL )
		{
			// the bucket contains a pair that matches the provided key,
			//change the value for that pair to the new value.
			pair->value = value;
			return 1;
		}
		// create a key-value pair
		if ( bucket->count == 0 )
		{
			// the bucket is empty, lazily allocate space for a single
			// key-value pair.
			bucket->pairs = malloc( sizeof( pair_t ) );
			if ( bucket->pairs == NULL ) return 0;
			bucket->count = 1;
		}
		else
		{
			// the bucket wasn't empty but no pair existed that matches the provided
			// key, so create a new key-value pair.
			tmp_pairs = realloc( bucket->pairs , ( bucket->count + 1 ) * sizeof( pair_t ) );
			if ( tmp_pairs == NULL ) return 0;
			bucket->pairs = tmp_pairs;
			bucket->count++;
		}
		// get the last pair in the chain for the bucket
		pair = &( bucket->pairs[ bucket->count - 1 ] );
		pair->key = malloc( ( strlen( key ) + 1 ) * sizeof(char) );
		pair->value = value;
		// copy the key
		strcpy( pair->key , key );
		return 1;
	}

	// returns value for key

	void* map_get( map_t *map, const char *key)
	{
		unsigned int index;
		bucket_t *bucket;
		pair_t *pair;

		if (map == NULL) return NULL;
		if (key == NULL) return NULL;
		index = hash( key ) % map->count;
		bucket = &( map->buckets[ index ] );
		pair = get_pair( bucket , key );
		if (pair == NULL) return NULL;
		return pair->value;
	}

	// checks key availability

	int map_haskey( map_t *map, const char *key)
	{
		unsigned int index;
		bucket_t *bucket;
		pair_t *pair;

		if (map == NULL) return 0;
		if (key == NULL) return 0;
		index = hash( key ) % map->count;
		bucket = &( map->buckets[ index ] );
		pair = get_pair( bucket , key );
		if (pair == NULL) return 0;
		return 1;
	}

	// calculates key-value count

	int map_size( map_t *map)
	{
		unsigned int index, bindex, count;
		bucket_t *bucket;
		pair_t *pair;

		if (map == NULL) return 0;
		bucket = map->buckets;
		index = 0;
		count = 0;
		while (index < map->count)
		{
			pair = bucket->pairs;
			bindex = 0;
			while (bindex < bucket->count)
			{
				count++;
				pair++;
				bindex++;
			}
			bucket++;
			index++;
		}
		return count;
	}

	// returns all keys in map

	vector_t* map_keys( map_t *map )
	{
		vector_t* result = vector_create( );
		unsigned int index, bindex;
		bucket_t *bucket;
		pair_t *pair;

		if ( map == NULL ) return NULL;
		bucket = map->buckets;
		index = 0;
		while ( index < map->count )
		{
			pair = bucket->pairs;
			bindex = 0;
			while ( bindex < bucket->count )
			{
				vector_adddata( result , pair->key );
				pair++;
				bindex++;
			}
			bucket++;
			index++;
		}
		return result;
	}

	// returns all values in map

	vector_t* map_values( map_t *map )
	{
		vector_t* result = vector_create( );
		unsigned int index, bindex;
		bucket_t *bucket;
		pair_t *pair;

		if ( map == NULL ) return NULL;
		bucket = map->buckets;
		index = 0;
		while ( index < map->count )
		{
			pair = bucket->pairs;
			bindex = 0;
			while ( bindex < bucket->count )
			{
				vector_adddata( result , pair->value );
				pair++;
				bindex++;
			}
			bucket++;
			index++;
		}
		return result;
	}

	// tests map

	#ifdef DEBUG
	void map_test( )
	{
		printf( "\nMAP TEST SESSION START" );
		printf( "\n1 CREATE EMPTY" );
		map_t* m1 = map_create( 10 );
		printf( "\n2 DELETE EMPTY" );
		map_free( m1 );
		printf( "\n3 ADDING DATA" );
		map_t* m2 = map_create( 10 );
		map_put( m2 , "fakk" , "fakkvalue" );
		map_put( m2 , "makk" , "makkvalue" );
		map_put( m2 , "takk" , "takkvalue" );
		map_put( m2 , "kakk" , "kakkvalue" );
		printf( "\nMAP SIZE %i" , map_size( m2 ) );
		printf( "\n4 GETTING DATA" );
		printf( "\n VALUE FOR makk : %s" , map_get( m2 , "makk" ) );
		printf( "\n5 SETTING DATA TO NULL" );
		map_put( m2 , "takk" , NULL );
		printf( "\n VALUE FOR takk : %s" , map_get( m2 , "takk" ) );
		printf( "\nMAP TEST SESSION END" );
	}
	#endif
