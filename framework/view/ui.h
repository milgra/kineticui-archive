//
//  ui.h
//  ui
//
//  Created by Milan Toth on 18/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __ui__
	#define __ui__

	#include <time.h>
	#include <stdio.h>
    #include <sys/time.h>
    #include "../../framework/core/vector.h"
    #include "../../framework/core/types.h"
    #include "../../framework/view/viewbase.h"
    #include "../../framework/view/modifier.h"
    #include "../../framework/view/uisettings.h"
    #include "../../framework/graphics/font.h"
    #include "../../framework/graphics/tilemap.h"
    #include "../../framework/graphics/renderer.h"

	typedef struct _uidelegate_t uidelegate_t;
	typedef struct _counters_t counters_t;
	typedef struct _ui_t ui_t;

	struct _counters_t
	{
		uint32_t updatecount;
		uint32_t quadcount;
		uint32_t drawcount;
	};

	struct _ui_t
	{
		// basic instances
		
        uidelegate_t* delegate;		// user created ui delegate
		uisettings_t* settings;		// ui settings
		viewbase_t* viewbase;		// view container
		renderer_t* renderer;		// quad renderer
		tilemap_t* tilemap;			// texture tilemap
		font_t* font;				// truetype font
		
		// modifier & view generation
		
		counters_t* counters;		// execution counters
		toolset_t* toolset;			// modifier toolset
		vector_t* events;			// modifier events
		vector_t* timeds;			// timed view collector
		vector_t* logs;				// log collector

		// rendering

		struct timeval lastrender;	// last rendering time
		GLfloat* vertexbuffer;		// vertex container
		uint32_t maxquads;			// max quad count in memory
		char framedrop;				// is frame dropped?
	};

	struct _uidelegate_t
	{
		void( *setup )( uidelegate_t* delegate , char* path_resources , char* path_library );
		void( *timer )( uidelegate_t* delegate );
		void( *event )( uidelegate_t* delegate , modifierevent_t* event );
		ui_t* ui;
    };

	// main functions

	ui_t* ui_init( float width , float height , float scale , char* path_resources , char* path_library , uidelegate_t* delegate );
	void ui_free( ui_t* ui );
	void ui_resize( ui_t* ui , float width , float height , char resizecontext );
	void ui_touch_up( ui_t* ui , float touch_x , float touch_y );
	void ui_touch_down( ui_t* ui , float touch_x , float touch_y );
	void ui_touch_move( ui_t* ui , float touch_x , float touch_y );
	void ui_key_down( ui_t* ui , char* characters , char key );
	void ui_scroll_start( ui_t* ui , float dx , float dy );
	void ui_scroll_move( ui_t* ui , float dx , float dy );
	void ui_scroll_end( ui_t* ui , float dx , float dy );
	void ui_timer( ui_t* ui );
	void ui_resettilemap( ui_t* ui );
	void ui_render( ui_t* ui , char force );
	void ui_handleevents( ui_t* ui );
	void ui_handletimeds( ui_t* ui );
	void ui_printlogs( ui_t* ui );

	// proxy functions

	void proxy_dragwindow( );
	void proxy_dropwindow( );
	void proxy_closewindow( );
	void proxy_flushbuffer( );
	void proxy_showkeyboard( );
	void proxy_hidekeyboard( );
	void proxy_maximizewindow( );

	#endif /* defined(__ui__) */
