//
//  modifier_animator.h
//  KineticUI
//
//  Created by Milan Toth on 28/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_animator__
	#define __KineticUI__modifier_animator__

	#include <stdio.h>
    #include "../core/vector.h"
    #include "../core/string.h"
    #include "../graphics/tilemap.h"
    #include "../graphics/transformation.h"
	#include "view.h"
	#include "modifier.h"

	typedef struct _animation_phase_t animation_phase_t;
	struct _animation_phase_t
	{
		transformation_t transformation;
		int steps;
		char easing;
		char* action;
        char force;
	};

	typedef struct _modifier_animator_t modifier_animator_t;
	struct _modifier_animator_t
	{
		char easing;
		char playing;
		char* action;
		vector_t* queue;
		uint32_t steps_left;
		transformation_t delta;
		transformation_t actual;
	};

	modifier_t* modifier_animator_default( transformation_t transformation );

	#endif /* defined(__KineticUI__modifier_animator__) */
