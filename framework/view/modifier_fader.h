//
//  modifier_fader.h
//  KineticUI
//
//  Created by Milan Toth on 11/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_fader__
	#define __KineticUI__modifier_fader__

	#include <stdio.h>
    #include "../core/vector.h"
	#include "view.h"
	#include "modifier.h"
	#include "modifier_cropper.h"
	#include "modifier_solidcolor.h"

	typedef struct _modifier_fader_t modifier_fader_t;
	struct _modifier_fader_t
	{
		float alpha;
		float speed;
		char inited;
		char orientation;
		char direction;
		char stopattop;
		char stopatbottom;
		view_t* inside;
	};

	modifier_t* modifier_fader_default( );

	#endif /* defined(__KineticUI__modifier_fader__) */
