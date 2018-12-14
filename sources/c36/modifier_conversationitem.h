//
//  modifier_conversationitem.h
//  KineticUI
//
//  Created by Milan Toth on 26/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_conversationitem__
	#define __KineticUI__modifier_conversationitem__

	#include <stdio.h>
	#include "../../framework/core/string.h"
	#include "../../framework/view/modifier.h"
	#include "../../framework/view/modifier_scroller.h"
	#include "../../framework/view/modifier_textarea.h"

	typedef struct _modifier_conversationitem_t modifier_conversationitem_t;
	struct _modifier_conversationitem_t
	{
		char* type;
		void* data;
		void( *_new )( modifier_conversationitem_t* modifier , modifier_args_t* args );
		void( *_free )( modifier_conversationitem_t* modifier );

		string_t* text;
		char pressed;
	};

	modifier_conversationitem_t* modifier_conversationitem_default( );
	void modifier_conversationitem_free( modifier_conversationitem_t* modifier );
	void modifier_conversationitem_new( modifier_conversationitem_t* modifier , modifier_args_t* args );

	#endif /* defined(__KineticUI__modifier_conversationitem__) */
