//
//  demo_kineticui.c
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "demo_kineticui.h"


	// constructor

    demo_kineticui_t* demo_kineticui_init( )
    {
        demo_kineticui_t* demo = malloc( sizeof( demo_kineticui_t ) );
        demo->setup = demo_kineticui_setup;
        demo->timer = demo_kineticui_timer;
        demo->event = demo_kineticui_event;
		
        return demo;
    }

	// destructor

	void demo_kineticui_free( demo_kineticui_t* demo )
	{
		free( demo );
	}

	void demo_kineticui_setupfromdescriptor( demo_kineticui_t* demo )
	{
	
		// single line kineticui
	
		char* labelviewa_structure_desc =
		"labelviewa\n";

		char* labelviewa_desc =
		"view labelviewa x 0 y -150 width 500 height -100 invisible 1\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 90.0 aligntype center linetype single inputtype normal marginsize 5.0 textcolor 0x000000FF openletters 1 text Kinetic_UI animendaction nextlabel\n";

		view_t* labelviewa = generator_view_default( labelviewa_desc, labelviewa_structure_desc, demo->ui->toolset->settings->scale );

		demo->labelviewa = labelviewa;
		
		// multi line kineticui

		char* labelviewb_structure_desc =
		"labelviewb\n";

		char* labelviewb_desc =
		"view labelviewb x 0 y -300 width 800 height -250 invisible 1\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 50.0 aligntype center linetype multi inputtype normal marginsize 5.0 textcolor 0x000000FF openletters 1 text Please_enter_your_nickname_: animendaction nextlabel\n";

		view_t* labelviewb = generator_view_default( labelviewb_desc, labelviewb_structure_desc, demo->ui->toolset->settings->scale );
		
		demo->labelviewb = labelviewb;

		// multi line kineticui

		char* labelviewc_structure_desc =
		"labelviewb\n";

		char* labelviewc_desc =
		"view labelviewb x 0 y -500 width 800 height -250 invisible 1\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 50.0 aligntype center linetype multi inputtype normal marginsize 5.0 textcolor 0x000000FF openletters 1 text Please_enter_your_nickname_: animendaction nextlabel\n";

		view_t* labelviewc = generator_view_default( labelviewc_desc, labelviewc_structure_desc, demo->ui->toolset->settings->scale );

		demo->labelviewc = labelviewc;

	}

	// setup delegate, create views

    void demo_kineticui_setup( demo_kineticui_t* demo , char* path_resources , char* path_library )
    {
		// demo_kineticui_setupfromcode( demo );
		demo_kineticui_setupfromdescriptor( demo );

        // init views and reset layout

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewa , NULL	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "init"			 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewb , NULL	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "init"			 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewc , NULL	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "init"			 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

        // re-scale to current size

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewa , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewb , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewc , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );

		// set text

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewb , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , string_createfrombytes(
			"multi-platform, multi-device, multi-resolution ui rendering engine with a lot of interaction and subtle animations" )  } ,
				.toolset    = demo->ui->toolset			  } ) );

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewc , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , string_createfrombytes(
						" openGL3 / openGL ES2 based,"
						" written in functional C" )  } ,
				.toolset    = demo->ui->toolset			  } ) );

        // open letters

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewc , NULL   } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "update"		 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );
		
		// adding view
		
		viewbase_addview( demo->ui->viewbase , demo->labelviewa );
    }

	// timer input

	void demo_kineticui_timer( demo_kineticui_t* demo )
	{

	}

	// event input

    void demo_kineticui_event( demo_kineticui_t* demo , modifierevent_t* event )
    {
		if ( strcmp( event->todo , "nextlabel" ) == 0 )
		{
		
			if ( viewbase_containsview( demo->ui->viewbase , demo->labelviewb ) == 0 )
			{
				view_new( &( ( modifier_args_t ) {
					.views      = { demo->labelviewb , NULL   } ,
					.generator  = { &freechar        , 1	  } ,
					.command    = { "update"		 , NULL	  } ,
					.toolset    = demo->ui->toolset			  } ) );

				viewbase_addview( demo->ui->viewbase , demo->labelviewb );
				return;
			}

			if ( viewbase_containsview( demo->ui->viewbase , demo->labelviewc ) == 0 )
			{
				view_new( &( ( modifier_args_t ) {
					.views      = { demo->labelviewc , NULL   } ,
					.generator  = { &freechar        , 1	  } ,
					.command    = { "update"		 , NULL	  } ,
					.toolset    = demo->ui->toolset			  } ) );

				viewbase_addview( demo->ui->viewbase , demo->labelviewc );
				return;
			}
		}
    }
