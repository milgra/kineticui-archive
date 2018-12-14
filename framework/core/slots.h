//
//  slots.h
//  KineticUI
//
//  Created by Milan Toth on 05/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__slots__
	#define __KineticUI__slots__

	#include <stdio.h>
	#include <stdlib.h>

    typedef struct _slots_t slots_t;
    struct _slots_t
    {
		int		size;
		int		getindex;
		int		putindex;
		void**  data;
    };

    slots_t* slots_create( int size );
	void slots_free( slots_t* slots );
	void slots_put( slots_t* slots , void* data , char* success );
	void slots_get( slots_t* slots , void** data );
	#ifdef DEBUG
	void slots_test( );
	#endif

	#endif /* defined(__KineticUI__slots__) */
