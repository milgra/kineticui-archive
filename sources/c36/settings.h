//
//  settings.h
//  c36
//
//  Created by Milan Toth on 23/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__settings__
#define __c36__settings__

	#include "../../framework/core/map.h"
	#include "../../framework/core/file.h"
	#include "../../framework/core/math2.h"
	#include "../../framework/core/string.h"

	typedef struct _settings_t settings_t;
	struct _settings_t
	{
		char* library;
		char* settings;
		map_t* defaults;
		vector_t* settingitems;
		vector_t* settingactions;
		vector_t* statusmessages;
	};

	settings_t* settings_init( char* cpath_resources , char* cpath_library );
	void settings_free( settings_t* info );

	void settings_setusername( settings_t* settings , string_t* username );
	string_t* settings_getusername( settings_t* settings );
	void settings_addconversation( settings_t* settings , string_t* conversation );
	void settings_removeconversation( settings_t* settings , string_t* conversation );
	vector_t* settings_getconversations( settings_t* settings );
	void settings_addmessage( settings_t* settings , string_t* conversation , string_t* message );
	vector_t* settings_getmessages( settings_t* settings , string_t* conversation );
	void settings_save( settings_t* settings );
	void settings_reset( settings_t* settings );

#endif /* defined(__c36__settings__) */
