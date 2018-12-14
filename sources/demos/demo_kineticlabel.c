//
//  demo_kineticlabel.c
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "demo_kineticlabel.h"

	// constructor

    demo_kineticlabel_t* demo_kineticlabel_init( )
    {
        demo_kineticlabel_t* demo = malloc( sizeof( demo_kineticlabel_t ) );
        demo->setup = demo_kineticlabel_setup;
        demo->timer = demo_kineticlabel_timer;
        demo->event = demo_kineticlabel_event;
		
        return demo;
    }

	// destructor

	void demo_kineticlabel_free( demo_kineticlabel_t* demo )
	{
		free( demo );
	}

	void demo_kineticlabel_setupfromdescriptor( demo_kineticlabel_t* demo )
	{
	
		// single line kineticlabel
	
		char* labelviewa_structure_desc =
		"labelviewa\n";

		char* labelviewa_desc =
		"view labelviewa x 0 y -325 width 900 height -100 invisible 1\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 70.0 aligntype center linetype multi inputtype normal marginsize 5.0 textcolor 0x000000FF openletters 1 animendaction nextlabel\n";

		view_t* labelviewa = generator_view_default( labelviewa_desc, labelviewa_structure_desc, demo->ui->toolset->settings->scale );

		demo->labelviewa = labelviewa;
		
	}

	// setup delegate, create views

    void demo_kineticlabel_setup( demo_kineticlabel_t* demo , char* path_resources , char* path_library )
    {
		// demo_kineticlabel_setupfromcode( demo );
		demo_kineticlabel_setupfromdescriptor( demo );

        // init views and reset layout

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewa , NULL } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "init"	 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

		
        // re-scale to current size

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewa , demo->ui->viewbase->view } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );
		
		// set text

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewa , demo->ui->viewbase->view } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "settext"	 , string_createfrombytes(
		//			"Animated Cursor"
		//			"Animated Scrollbars"
		//			"Animated Glyphs"
		//			"Animated Line Break And Text Align"
		//			"Multi-View Paradigm With Expose"
		//			"Super-Responsive Design"
		//			"Structure And View Descriptor Strings"
					"Works On OSX, iOS, QNX, BBOS10, Windows, Android"
					) } ,
				.toolset    = demo->ui->toolset			  } ) );

        // open letters

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewa , NULL } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "update"		 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );
		
		// adding view
		
		viewbase_addview( demo->ui->viewbase , demo->labelviewa );
    }

	// timer input

	void demo_kineticlabel_timer( demo_kineticlabel_t* demo )
	{

	}

	// event input

    void demo_kineticlabel_event( demo_kineticlabel_t* demo , modifierevent_t* event )
    {

    }
