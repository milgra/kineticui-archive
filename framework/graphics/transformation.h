//
//  transformation.h
//  KineticUI
//
//  Created by Milan Toth on 29/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__transformation__
	#define __KineticUI__transformation__

	#include <stdio.h>
	#include "../core/math3.h"

	typedef struct _transformation_t transformation_t;
	struct _transformation_t
	{
		vector3_t translation;
		vector3_t rotation;
		vector3_t scale;
	};


	transformation_t transformation_default( );


	#endif /* defined(__KineticUI__transformation__) */
