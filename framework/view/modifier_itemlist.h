//
//  modifier_itemlist.h
//  KineticUI
//
//  Created by Milan Toth on 27/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_itemlist__
	#define __KineticUI__modifier_itemlist__

	#include <stdio.h>
    #include "../core/vector.h"
	#include "view.h"
	#include "modifier.h"
	#include "modifier_layout.h"
	#include "modifier_cropper.h"

	typedef struct _modifier_itemlist_t modifier_itemlist_t;
	struct _modifier_itemlist_t
	{
		float gap;
		view_t* contentview;
		vector_t* items;
	};

	modifier_t* modifier_itemlist_default( );

	#endif /* defined(__KineticUI__modifier_itemlist__) */
