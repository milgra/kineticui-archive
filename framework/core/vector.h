//
//  vector.h
//  c36
//
//  Created by Milan Toth on 23/02/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__vector__
#define __c36__vector__

    #include <stdio.h>
    #include <stdint.h>
    #include <stdlib.h>
	#include <string.h>

    typedef struct vector_t vector_t;
    struct vector_t
    {
		// char* type;				// types in vector, for a controlled release of content later
        void** data;
        uint32_t length;
        uint32_t length_real;
    };

    vector_t* vector_create( );
	void vector_free( vector_t* vector );
	void vector_reset( vector_t* vector );

	void vector_adddata( vector_t* vector , void* data );
	void vector_adddataatindex( vector_t* vector, void* data , unsigned long index );
	void vector_adddatainvector( vector_t* vector_a , vector_t* vector_b );

	void vector_removedata( vector_t* vector , void* data );
	void vector_removedataatindex( vector_t* vector , uint32_t index );
	void vector_removedatainrange( vector_t* vector , uint32_t start , uint32_t end );
	void vector_removedatainvector( vector_t* vector_a , vector_t* vector_b );

	uint32_t vector_indexofdata( vector_t* vector , void* data );
	#ifdef DEBUG
	void vector_test( );
	#endif

#endif /* defined(__c36__vector__) */
