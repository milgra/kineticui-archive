//
//  touch_modifier_scroller.h
//  KineticUI
//
//  Created by Milan Toth on 21/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__touch_modifier_scroller__
	#define __KineticUI__touch_modifier_scroller__

	#include <stdio.h>
	#include <time.h>
	#include <sys/time.h>
	#include "../core/math2.h"
	#include "../core/math4.h"
    #include "../core/vector.h"
	#include "view.h"
	#include "modifier.h"
	#include "scroller.h"
	#include "generator_view.h"

	typedef struct _modifier_scroller_t modifier_scroller_t;
	struct _modifier_scroller_t
	{
		view_t* scrollviewx;
		view_t* scrollviewy;
		
		vector3_t last_point;
		
		scroller_t* scroller_x;
		scroller_t* scroller_y;
		
		vector2_t delta_last;
		vector2_t contentsize_last;
		vector2_t headposition_last;
		
		char enable_x;
		char enable_y;
		
		char enable_scrollbar_x;
		char enable_scrollbar_y;
		
		uint8_t lastaction;
		struct timeval lasttouch;
	};

	modifier_t* modifier_scroller_default( char rubber );

	#endif /* defined(__KineticUI__touch_modifier_scroller__) */
