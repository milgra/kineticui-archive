//
//  modifier_hover.h
//  c36
//
//  Created by Milan Toth on 09/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__modifier_hover__
#define __c36__modifier_hover__

	#include <stdio.h>
    #include "../core/vector.h"
	#include "view.h"
	#include "modifier.h"
	#include "modifier_cropper.h"
	#include "modifier_solidcolor.h"

	typedef struct _modifier_hover_t modifier_hover_t;
	struct _modifier_hover_t
	{
		float radius;
		char inited;
		char orientation;
		view_t* insideblack;
	};

	modifier_t* modifier_hover_default( );

#endif /* defined(__c36__modifier_hover__) */
