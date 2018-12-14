//
//  colorviewmodifier.h
//  KineticUI
//
//  Created by Milan Toth on 15/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__modifier_solidcolor__
    #define __KineticUI__modifier_solidcolor__

    #include <stdio.h>
    #include "../core/vector.h"
	#include "view.h"
	#include "modifier.h"

    typedef struct _modifier_solidcolor_t modifier_solidcolor_t;
    struct _modifier_solidcolor_t
    {
		char inited;
		char* colorname;
        uint32_t color;
		vector2_t* coords;
		bitmap_t* bitmap;
    };

    modifier_t* modifier_solidcolor_default( uint32_t color );

    #endif /* defined(__KineticUI__modifier_solidcolor__) */
