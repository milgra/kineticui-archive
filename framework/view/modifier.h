//
//  modifier.h
//  KineticUI
//
//  Created by Milan Toth on 24/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef KineticUI_modifier_h
	#define KineticUI_modifier_h

    #include "../graphics/font.h"
    #include "../graphics/tilemap.h"
	#include "../core/types.h"
	#include "uisettings.h"
	#include "input.h"
    #include "view_types.h"

    typedef struct _view_t view_t;
    typedef struct _modifier_t modifier_t;
	typedef struct _modifier_args_t modifier_args_t;


	typedef struct modifierevent_t modifierevent_t;
	struct modifierevent_t
	{
		char* name;
		char* todo;
		void* data;
        void* root;
	};

	// TODO pointerek legyenek belul
    typedef struct _modifier_args_t modifier_args_t;
    struct _modifier_args_t
    {
        struct view_views_t
        {
            view_t* view;			// actual view
            view_t* parent;			// parent view ( optional ), NULL by default
        } views;
        
        struct view_command_t
        {
            char* name;				// command
            void* arguments;		// command arguments
        } command;
        
        struct view_generator_t
        {
            char* changed_flag;		// changed flag, if 1, render happens after event loop
            char subviews_flag;		// do subviews?
        } generator;
        
        toolset_t* toolset;
    };

    struct _modifier_t
    {
		char* type;
		void* data;
		void( *_new )( modifier_t* modifier , modifier_args_t* args );
		void( *_free )( modifier_t* modifier );
    };

	object_t* modifier_defaultevent( char* name , char* todo , void* root , void* data );
	void modifier_freeevent( object_t* eventobject );

	#endif
