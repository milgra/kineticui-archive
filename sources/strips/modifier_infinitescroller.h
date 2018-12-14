//
//  modifier_infinitescroller.h
//  kineticui
//
//  Created by mtoth on 23/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __kineticui__modifier_infinitescroller__
#define __kineticui__modifier_infinitescroller__


	#include <stdio.h>
	#include <time.h>
	#include <sys/time.h>
	#include "view.h"
	#include "vector.h"
	#include "modifier.h"
	#include "generator_view.h"

	typedef struct _modifier_infinitescroller_t modifier_infinitescroller_t;
	struct _modifier_infinitescroller_t
	{
		char* type;
		void* data;
		void( *_new )( modifier_infinitescroller_t* modifier , modifier_args_t* args );
		void( *_free )( modifier_infinitescroller_t* modifier );
		
		vector_t* items;

		vector3_t last_point;
        void* swipedetector;
	};

	modifier_infinitescroller_t* modifier_infinitescroller_default( );
	void modifier_infinitescroller_free( modifier_infinitescroller_t* modifier );
    void modifier_infinitescroller_new( modifier_infinitescroller_t* modifier , modifier_args_t* args );


#endif /* defined(__kineticui__modifier_infinitescroller__) */
