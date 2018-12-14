//
//  transformation.c
//  KineticUI
//
//  Created by Milan Toth on 29/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "transformation.h"


	transformation_t transformation_default( )
	{
		transformation_t result;
		result.translation = vector3_default( 0.0, 0.0, 0.0);
		result.rotation = vector3_default( 0.0, 0.0, 0.0);
		result.scale = vector3_default( 1.0, 1.0, 1.0);
		
		return result;
	}
