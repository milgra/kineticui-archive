//
//  modifier_blurcolor.h
//  c36
//
//  Created by Milan Toth on 19/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__modifier_blurcolor__
#define __c36__modifier_blurcolor__

	#include "modifier.h"
	#include "../core/math3.h"
	#include "view.h"

	typedef struct _blursettings_t blursettings_t;
	struct _blursettings_t
	{
		vector2_t blurorigin;
		vector2_t* blurcoords;
	};

	modifier_t* modifier_blurcolor_default( );


#endif /* defined(__c36__modifier_blurcolor__) */
