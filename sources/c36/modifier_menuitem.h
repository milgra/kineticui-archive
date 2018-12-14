//
//  modifier_menuitem.h
//  KineticUI
//
//  Created by Milan Toth on 09/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__modifier_menuitem__
	#define __KineticUI__modifier_menuitem__

	#include <stdio.h>

	#include "../../framework/view/modifier.h"
	#include "../../framework/view/modifier_textarea.h"
	#include "../../framework/view/modifier_scroller.h"

	typedef struct _modifier_menuitem_t modifier_menuitem_t;
	struct _modifier_menuitem_t
	{
		char* type;
		void* data;
		void( *_new )( modifier_menuitem_t* modifier , modifier_args_t* args );
		void( *_free )( modifier_menuitem_t* modifier );

		string_t* text;
		char* action;
	};

	modifier_menuitem_t* modifier_menuitem_default( string_t* action );
	void modifier_menuitem_free( modifier_menuitem_t* modifier );
	void modifier_menuitem_new( modifier_menuitem_t* modifier , modifier_args_t* args );


	#endif /* defined(__KineticUI__modifier_menuitem__) */
