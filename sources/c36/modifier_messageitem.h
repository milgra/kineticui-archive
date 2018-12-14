//
//  modifier_messageitem.h
//  KineticUI
//
//  Created by Milan Toth on 01/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_messageitem__
	#define __KineticUI__modifier_messageitem__

	#include <stdio.h>

	#include "../../framework/view/modifier.h"
	#include "../../framework/view/modifier_textarea.h"
	#include "../../framework/view/modifier_scroller.h"

	typedef struct _modifier_messageitem_t modifier_messageitem_t;
	struct _modifier_messageitem_t
	{
		char* type;
		void* data;
		void( *_new )( modifier_messageitem_t* modifier , modifier_args_t* args );
		void( *_free )( modifier_messageitem_t* modifier );

		string_t* text;
	};

	modifier_messageitem_t* modifier_messageitem_default( );
	void modifier_messageitem_free( modifier_messageitem_t* modifier );
	void modifier_messageitem_new( modifier_messageitem_t* modifier , modifier_args_t* args );

	#endif /* defined(__KineticUI__modifier_messageitem__) */
