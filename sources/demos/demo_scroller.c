//
//  demo_scroller.c
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "demo_scroller.h"

	// constructor

    demo_scroller_t* demo_scroller_init( )
    {
        demo_scroller_t* demo = malloc( sizeof( demo_scroller_t ) );
        demo->setup = demo_scroller_setup;
        demo->timer = demo_scroller_timer;
        demo->event = demo_scroller_event;
		
        return demo;
    }

	// destructor

	void demo_scroller_free( demo_scroller_t* demo )
	{
		free( demo );
	}

	void demo_scroller_setupfromdescriptor( demo_scroller_t* demo )
	{
	
		// single line textarea
	
		char* scroller_structure_desc =
		"scroller\n"
		"	content\n";

		char* scroller_desc =
		"view scroller x 0 y -50 width 500 height -700 focuslevel 2\n"
		"modifier layout options 100000\n"
		"modifier scroller enablescrollbarx 1 enablescrollbary 1\n"
		"modifier solidcolor color 0x00000022\n"

		"view content x 0 y 0 width 500 height -700\n"
		"modifier cropper\n"
		"modifier solidcolor color 0x00000011\n";

		view_t* scroller = generator_view_default( scroller_desc, scroller_structure_desc, demo->ui->toolset->settings->scale );

		demo->scroller = scroller;
	}

	// setup delegate, create views

    void demo_scroller_setup( demo_scroller_t* demo , char* path_resources , char* path_library )
    {
		// demo_scroller_setupfromcode( demo );
		demo_scroller_setupfromdescriptor( demo );

        // init views and reset layout

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->scroller  , NULL	} ,
				.generator  = { &freechar       , 1		} ,
				.command    = { "init"			, NULL	} ,
				.toolset    = demo->ui->toolset			} ) );

		
        // re-scale to current size

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->scroller  , demo->ui->viewbase->view	} ,
				.generator  = { &freechar       , 1		} ,
				.command    = { "layoutupdate"	, NULL	} ,
				.toolset    = demo->ui->toolset			} ) );
		
		// adding view

		viewbase_addview( demo->ui->viewbase , demo->scroller );
    }

	// timer input

	void demo_scroller_timer( demo_scroller_t* demo )
	{

	}

	// event input

    void demo_scroller_event( demo_scroller_t* demo , modifierevent_t* event )
    {
    
    }
