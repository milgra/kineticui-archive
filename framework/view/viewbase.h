//
//  Simulator.h
//  KineticUI
//
//  Created by Milan Toth on 11/09/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef KineticUI_Simulator_h
    #define KineticUI_Simulator_h

    #ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
    #elif defined OSX
    #include <OpenGL/gl3.h>
    #elif defined BRY
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #endif
    #ifdef __MACH__
    #include <mach/clock.h>
    #include <mach/mach.h>
    #include <mach/mach_time.h>
    #endif
    #include <stdio.h>
    #include <stdlib.h>
    #include "../core/vector.h"
    #include "../graphics/font.h"
    #include "../graphics/bitmap.h"
    #include "../graphics/tilemap.h"
	#include "view.h"
	#include "modifier.h"
	#include "modifier_layout.h"
	#include "modifier_cropper.h"


	typedef struct _viewbase_t viewbase_t;
	struct _viewbase_t
	{
		view_t* view;
		vector_t* timed;
		vector_t* timedtoremove;
		vector_t* touchfocused;
		vector_t* scrollfocused;
	};


    viewbase_t* viewbase_init( float width , float height );
	void viewbase_free( viewbase_t* viewbase );
	view_t* viewbase_defaultbaseview( float width , float height );
	void viewbase_resize( viewbase_t* viewbase , toolset_t* uiutols , float width , float height );
	void viewbase_addview( viewbase_t* viewbase , view_t* view );
	void viewbase_addviewatindex( viewbase_t* viewbase , view_t* view , uint32_t index );
	char viewbase_containsview( viewbase_t* viewbase , view_t* view );
	void viewbase_removeview( viewbase_t* viewbase , view_t* view );
	void viewbase_addtimedview( viewbase_t* viewbase , view_t* view );
	void viewbase_finalize_geometry( viewbase_t* viewbase , toolset_t* toolset , uint32_t* counter , uint32_t* quadcounter );
	void viewbase_refill_tilemap( viewbase_t* viewbase );
	void viewbase_setfocus( viewbase_t* viewbase , vector_t* views , char* changed , toolset_t* toolset );
	void viewbase_logcounters( viewbase_t* base );
    void viewbase_collectvertexdata( viewbase_t* viewbase , GLfloat* vertexes , uint32_t* quadcount );
	void viewbase_generateviewchain( viewbase_t* viewbase , vector_t* vector , struct view_command_t* , char* changed , toolset_t* toolset );

	// inputs

    void viewbase_timer( viewbase_t* viewbase , char* changed , toolset_t* toolset );
    void viewbase_scroll_start( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );
    void viewbase_scroll_move( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );
    void viewbase_scroll_end( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );
    void viewbase_keydown( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );
    void viewbase_touchup( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );
    void viewbase_touchdown( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );
    void viewbase_touchdrag( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset );


    #endif
