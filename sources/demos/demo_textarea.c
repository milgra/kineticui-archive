//
//  demo_textarea.c
//  c36
//
//  Created by Milan Toth on 13/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "demo_textarea.h"

	// constructor

    demo_textarea_t* demo_textarea_init( )
    {
        demo_textarea_t* demo = malloc( sizeof( demo_textarea_t ) );
        demo->setup = demo_textarea_setup;
        demo->timer = demo_textarea_timer;
        demo->event = demo_textarea_event;
		
        return demo;
    }

	// destructor

	void demo_textarea_free( demo_textarea_t* demo )
	{
		free( demo );
	}

	void demo_textarea_setupfromdescriptor( demo_textarea_t* demo )
	{
	
		// single line textarea
	
		char* labelviewa_structure_desc =
		"labelviewa\n";

		char* labelviewa_desc =
		"view labelviewa x 0 y -20 width 500 height -50\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 36.0 aligntype center linetype single inputtype normal marginsize 7.0 textcolor 0x000000FF text Please_enter_your_nickname_:\n"
		"modifier solidcolor color 0x00000011";

		view_t* labelviewa = generator_view_default( labelviewa_desc, labelviewa_structure_desc, demo->ui->toolset->settings->scale );

		demo->labelviewa = labelviewa;
		
		// multi line textarea

		char* labelviewb_structure_desc =
		"labelviewb\n";

		char* labelviewb_desc =
		"view labelviewb x 0 y -200 width 500 height -150\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 25.0 aligntype justify linetype multi inputtype normal marginsize 5.0 textcolor 0x000000FF text Please_enter_your_nickname_:\n"
		"modifier solidcolor color 0x00000011";

		view_t* labelviewb = generator_view_default( labelviewb_desc, labelviewb_structure_desc, demo->ui->toolset->settings->scale );
		
		demo->labelviewb = labelviewb;

		// single line input textarea
	
		char* labelviewc_structure_desc =
		"labelviewc\n";

		char* labelviewc_desc =
		"view labelviewc x 0 y -400 width 500 height -50 focuslevel 1\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 36.0 aligntype left linetype single inputtype input marginsize 7.0 textcolor 0x000000FF text click:\n"
		"modifier solidcolor color 0x00000011";

		view_t* labelviewc = generator_view_default( labelviewc_desc, labelviewc_structure_desc, demo->ui->toolset->settings->scale );

		demo->labelviewc = labelviewc;

		// single line input textarea
	
		char* labelviewd_structure_desc =
		"labelviewd\n";

		char* labelviewd_desc =
		"view labelviewd x 0 y -500 width 500 height -150 focuslevel 1\n"
		"modifier layout options 100000\n"
		"modifier textarea linesize 30.0 aligntype justify linetype multi inputtype input marginsize 7.0 textcolor 0x000000FF text click:\n"
		"modifier solidcolor color 0x00000011";

		view_t* labelviewd = generator_view_default( labelviewd_desc, labelviewd_structure_desc, demo->ui->toolset->settings->scale );

		demo->labelviewd = labelviewd;

	}

	// setup delegate, create views

    void demo_textarea_setup( demo_textarea_t* demo , char* path_resources , char* path_library )
    {
		// demo_textarea_setupfromcode( demo );
		demo_textarea_setupfromdescriptor( demo );

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

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewd , NULL	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "init"			 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );


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

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewd , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "layoutupdate"	 , NULL	  } ,
				.toolset    = demo->ui->toolset			  } ) );


		view_new( &( ( modifier_args_t ) {
				.views      = { demo->labelviewd , demo->ui->viewbase->view	  } ,
				.generator  = { &freechar        , 1	  } ,
				.command    = { "settext"		 , string_createfrombytes(
										"egyedem begyedem tengertánc, cica mica mit kívánsz "
										"nem kívánok egyebet, fogjuk meg az egeret. meg ilyeneket hogy "
										"bort búzát békességet, kétpetéjű feleséget" ) } ,
				.toolset    = demo->ui->toolset			  } ) );
		
		// adding view
		viewbase_addview( demo->ui->viewbase , demo->labelviewa );
		viewbase_addview( demo->ui->viewbase , demo->labelviewb );
		viewbase_addview( demo->ui->viewbase , demo->labelviewc );
		viewbase_addview( demo->ui->viewbase , demo->labelviewd );
    }

	// timer input

	void demo_textarea_timer( demo_textarea_t* demo )
	{

	}

	// event input

    void demo_textarea_event( demo_textarea_t* demo , modifierevent_t* event )
    {
    
    }

