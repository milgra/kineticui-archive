//
//  hanlder_clipper.h
//  KineticUI
//
//  Created by Milan Toth on 24/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__hanlder_clipper__
	#define __KineticUI__hanlder_clipper__

	#include <stdio.h>
	#include "view.h"
	#include "modifier.h"

	typedef struct _modifier_clipper_t modifier_clipper_t;
	struct _modifier_clipper_t
	{
		char* type;
		void* data;
		void( *_new )( modifier_clipper_t* modifier , modifier_args_t* args );
		void( *_free )( modifier_clipper_t* modifier );
		
		char playing;
		char changed;
		texcoords_t realcoords;
		uint32_t steps_left;
		uint32_t steps_delay;
		
		float ratio;
		float stepping;
		float geowidth;
		float geoheight;
		float texwidth;
	};

	typedef struct _clipper_args clipper_args;
	struct _clipper_args
	{
		int steps;
		int delay;
		char apply;
	};

	modifier_t* modifier_clipper_default( );

	#endif /* defined(__KineticUI__hanlder_clipper__) */
