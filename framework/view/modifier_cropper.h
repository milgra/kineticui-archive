//
//  modifier_cropper.h
//  c36
//
//  Created by Milan Toth on 19/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__modifier_cropper__
#define __c36__modifier_cropper__

	#include "view.h"

	typedef struct _modifier_cropper_t modifier_cropper_t;
	struct _modifier_cropper_t
	{
		char cropped;
		
		vector2_t lastorigo;
		vector2_t lastextent;
		matrix3_t lastparent;
	};

	modifier_t* modifier_cropper_default( );

#endif /* defined(__c36__modifier_cropper__) */
