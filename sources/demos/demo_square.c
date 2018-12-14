//
//  test_square.c
//  c36
//
//  Created by Milan Toth on 10/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

    #include "demo_square.h"

	// constructor

    demo_square_t* demo_square_init( )
    {
        demo_square_t* demo = malloc( sizeof( demo_square_t ) );
        demo->setup = demo_square_setup;
        demo->timer = demo_square_timer;
        demo->event = demo_square_event;
		
        return demo;
    }

	// destructor

	void demo_square_free( demo_square_t* demo )
	{
		free( demo );
	}

	void demo_square_setupfromcode( demo_square_t* demo )
	{
        view_t* imageview = view_default(
			0.0 ,
			0.0 ,
			900.0 * demo->ui->toolset->settings->scale ,
			-800.0 * demo->ui->toolset->settings->scale );
		
		view_setname( imageview , "squareview" );
        
        modifier_t* imagearea = modifier_imagearea_default( "background.png" );
        modifier_t* layout = modifier_layout_default( 0x0F );
        
        view_addmodifier( imageview , imagearea , 0 );
        view_addmodifier( imageview , layout , 0 );
		
		demo->imageview = imageview;
	}

	void demo_square_setupfromdescriptor( demo_square_t* demo )
	{
		char* imageview_structure_desc =
		"squareview\n";

		char* imageview_desc =
		"view squareview x 0 y 0 width 900 height -800\n"
		"modifier layout options 001111\n"
		"modifier imagearea image background.png\n";

		view_t* imageview = generator_view_default( imageview_desc, imageview_structure_desc, demo->ui->toolset->settings->scale );

		demo->imageview = imageview;
	}

	// setup delegate, create views

    void demo_square_setup( demo_square_t* demo , char* path_resources , char* path_library )
    {
		// demo_square_setupfromcode( demo );
		demo_square_setupfromdescriptor( demo );

        // init imagearea and reset layout

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->imageview , NULL	} ,
				.generator  = { &freechar       , 1		} ,
				.command    = { "init"			, NULL	} ,
				.toolset    = demo->ui->toolset			} ) );

		
        // re-scale to current size

		view_new( &( ( modifier_args_t ) {
				.views      = { demo->imageview , demo->ui->viewbase->view	} ,
				.generator  = { &freechar       , 1		} ,
				.command    = { "layoutupdate"	, NULL	} ,
				.toolset    = demo->ui->toolset			} ) );
		
		// adding view
		
		viewbase_addview( demo->ui->viewbase , demo->imageview );
    }

	// timer input

	void demo_square_timer( demo_square_t* demo )
	{

	}

	// event input

    void demo_square_event( demo_square_t* demo , modifierevent_t* event )
    {
    
    }

