//
//  modifier_layout.h
//  c36
//
//  Created by Milan Toth on 19/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__modifier_layout__
#define __c36__modifier_layout__

	#include "view.h"

	typedef struct _modifier_layout_t modifier_layout_t;
	struct _modifier_layout_t
	{
		uint8_t		options;
		vector4_t	centers;
		vector4_t	distances;
		vector2_t	minextent;
	};

	modifier_t* modifier_layout_default( uint8_t options );

#endif /* defined(__c36__modifier_layout__) */
