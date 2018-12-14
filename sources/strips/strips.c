//
//  strips.c
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "strips.h"
	#include "mainui.h"

	// constructor

    strips_t* strips_init( )
    {
        strips_t* strips = malloc( sizeof( strips_t ) );
        strips->setup = strips_setup;
        strips->timer = strips_timer;
        strips->event = strips_event;
		
        return strips;
    }

	// destructor

	void strips_free( strips_t* strips )
	{
		free( strips );
	}

	// setup delegate, create views

    void strips_setup( strips_t* strips , char* path_resources , char* path_library )
    {

		strips->base = generator_view_default( strips_mainui_desc, strips_mainui_structure_desc, strips->ui->toolset->settings->scale );
		
        modifier_infinitescroller_t* scroller = modifier_infinitescroller_default();

        view_addmodifier( strips->base , ( modifier_t* ) scroller , 0 );

		// generate item's initial state

		view_new( &( ( modifier_args_t ) {
				.views      = { strips->base	, NULL	  } ,
				.generator  = { &freechar       , 1		  } ,
				.command    = { "init"			, NULL	  } ,
				.toolset    = strips->ui->toolset		  } ) );

		// re-layout element to screen's size

		view_new( &( ( modifier_args_t ) {
				.views      = { strips->base	, strips->ui->viewbase->view } ,
				.generator  = { &freechar       , 1		  } ,
				.command    = { "layoutupdate"	, NULL	  } ,
				.toolset    = strips->ui->toolset		  } ) );

		// adding view
		
		viewbase_addview( strips->ui->viewbase , strips->base );
		
		strips_showcalendar( 1444 , 2244 , 1.0 );
    }

	// timer input

	void strips_timer( strips_t* strips )
	{

	}

	// event input

	static int itemcounter = 0;

    void strips_event( strips_t* strips , modifierevent_t* event )
    {
		printf( "\nevent : %s" , event->todo );
		
		if ( strcmp( event->todo , "needstop" ) == 0 )
		{
			// create new item
			
			view_t* newitem = view_default( 0, 0, 200, -rand()%500 );
			modifier_t* color = modifier_solidcolor_default( itemcounter++ % 2 == 0 ? 0x00FF00FF : 0xFF0000FF );
			view_addmodifier( newitem, ( modifier_t* ) color, 0 );

			// generate item's initial state

			view_new( &( ( modifier_args_t ) {
                    .views      = { newitem			, NULL	  } ,
                    .generator  = { &freechar       , 1		  } ,
                    .command    = { "init"			, NULL	  } ,
                    .toolset    = strips->ui->toolset		  } ) );

			// generate infinitescroller's new state with new itemm
			
			modifier_infinitescroller_new( event->root , &( ( modifier_args_t ) {
                    .views      = { strips->base    , NULL	  } ,
                    .generator  = { &freechar       , 1		  } ,
                    .command    = { "addtopitem"	, newitem } ,
                    .toolset    = strips->ui->toolset		  } ) );
		}
		else if ( strcmp( event->todo , "needsbottom" ) == 0 )
		{
			view_t* newitem = view_default(0, 0, 200, -rand()%500);
			modifier_t* color = modifier_solidcolor_default( itemcounter++ % 2 == 0 ? 0x00FF00FF : 0xFF0000FF );
			view_addmodifier(newitem,( modifier_t* ) color, 0);
			
			// generate item's initial state

			view_new( &( ( modifier_args_t ) {
                    .views      = { newitem			, NULL	  } ,
                    .generator  = { &freechar       , 1		  } ,
                    .command    = { "init"			, NULL	  } ,
                    .toolset    = strips->ui->toolset		  } ) );

			// generate infinitescroller's new state with new itemm
			
			modifier_infinitescroller_new( event->root , &( ( modifier_args_t ) {
                    .views      = { strips->base    , NULL	  } ,
                    .generator  = { &freechar       , 1		  } ,
                    .command    = { "addbottomitem" , newitem } ,
                    .toolset    = strips->ui->toolset		  } ) );
		}
		if ( strcmp( event->todo , "removetop" ) == 0 )
		{
			modifier_infinitescroller_new( event->root , &( ( modifier_args_t ) {
                    .views      = { strips->base    , NULL } ,
                    .generator  = { &freechar       , 1    } ,
                    .command    = { "removetopitem" , NULL } ,
                    .toolset    = strips->ui->toolset	   } ) );
		}
		else if ( strcmp( event->todo , "removebottom" ) == 0 )
		{
			modifier_infinitescroller_new( event->root , &( ( modifier_args_t ) {
                    .views      = { strips->base		, NULL } ,
                    .generator  = { &freechar			, 1    } ,
                    .command    = { "removebottomitem"	, NULL } ,
                    .toolset    = strips->ui->toolset		   } ) );
		}
    }


	void strips_showcalendar( float left , float right , float scale )
	{
		// scale = 1
		// one day width = 200 pixel
		// head - 0 is january first actual year
		// calculate which days to show
		
		float daywidth = 200 / scale;
		float firstfromhead = left / daywidth;
		float lastfromhead = right / daywidth;
		float firstday = floorf( firstfromhead );
		float lastday = ceilf( lastfromhead );
		float daysinscreen = lastday - firstday;
		
		printf( "firstday %f lastday %f daysinscreen %f" , firstday , lastday , daysinscreen );
	
	}
