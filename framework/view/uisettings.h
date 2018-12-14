//
//  settings.h
//  KineticUI
//
//  Created by Milan Toth on 31/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__settings__
	#define __KineticUI__settings__

	#include "../core/string.h"
	#include "../core/map.h"
	#include "../core/file.h"
	#include "../core/math2.h"

	typedef struct _uisettings_t uisettings_t;
	struct _uisettings_t
	{
		vector2_t screensize;
		float scale;
		char* font;
		char* resources;
		void* userdata;			// generic container to put stuff in for special modifiers
	};

	uisettings_t* uisettings_init( char* cpath_resources , char* cpath_library , float scale , float width , float height );
	void uisettings_free( uisettings_t* settings );


	#endif /* defined(__KineticUI__settings__) */
