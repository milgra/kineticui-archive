//
//  modifier_dragger.h
//  KineticUI
//
//  Created by Milan Toth on 25/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_dragger__
	#define __KineticUI__modifier_dragger__

	#include <stdio.h>
	#include "view.h"
	#include "math.h"
	#include "modifier.h"

	typedef struct _modifier_dragger_t modifier_dragger_t;
	struct _modifier_dragger_t
	{
		char playing;
		char* pressaction;
		char* releaseaction;
		vector2_t last_point;
	};

	modifier_t* modifier_dragger_default( );

	#endif /* defined(__KineticUI__modifier_dragger__) */
