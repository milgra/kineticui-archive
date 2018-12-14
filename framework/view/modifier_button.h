//
//  modifier_flipper.h
//  KineticUI
//
//  Created by Milan Toth on 16/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_button__
	#define __KineticUI__modifier_button__

	#include <stdio.h>
	#include "../core/math2.h"
    #include "../core/vector.h"
	#include "view.h"
	#include "modifier.h"
	#include "modifier_animator.h"

	typedef struct _modifier_button_t modifier_button_t;
	struct _modifier_button_t
	{
		char state;
		char isToggle;
		char toggled;
		char animated;
		char* action;
		modifier_t* animator;
	};

	modifier_t* modifier_button_default( );

	#endif /* defined(__KineticUI__modifier_flipper__) */
