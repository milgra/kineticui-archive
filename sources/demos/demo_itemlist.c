//
//  demo_itemlist.c
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "demo_itemlist.h"


	// constructor

    demo_itemlist_t* demo_itemlist_init( )
    {
        demo_itemlist_t* demo = malloc( sizeof( demo_itemlist_t ) );
        demo->setup = demo_itemlist_setup;
        demo->timer = demo_itemlist_timer;
        demo->event = demo_itemlist_event;
		
        return demo;
    }

	// destructor

	void demo_itemlist_free( demo_itemlist_t* demo )
	{
		free( demo );
	}

	void demo_itemlist_setupfromdescriptor( demo_itemlist_t* demo )
	{
		// single line textarea
	
		char* itemlist_structure_desc =
		"scroller\n"
		"	content\n";

		char* itemlist_desc =
		"view scroller x 0 y -50 width 500 height -700 focuslevel 2\n"
		"modifier layout options 100000\n"
		"modifier scroller enablescrollbarx 1 enablescrollbary 1\n"
		"modifier solidcolor color 0x00000022\n"

		"view content x 0 y 0 width 500 height -700\n"
		"modifier cropper\n"
		"modifier itemlist\n"
		"modifier solidcolor color 0x00000011\n";

		view_t* scroller = generator_view_default( itemlist_desc, itemlist_structure_desc, demo->ui->toolset->settings->scale );

        // init views and reset layout

		view_new( &( ( modifier_args_t ) {
				.views      = { scroller	, NULL	} ,
				.generator  = { &freechar   , 1		} ,
				.command    = { "init"		, NULL  } ,
				.toolset    = demo->ui->toolset	    } ) );

		
        // re-scale to current size

		view_new( &( ( modifier_args_t ) {
				.views      = { scroller		, demo->ui->viewbase->view	} ,
				.generator  = { &freechar		, 1		} ,
				.command    = { "layoutupdate"	, NULL  } ,
				.toolset    = demo->ui->toolset			} ) );

		view_t* itemlistview = view_getsubview( scroller , "content" );

		for ( int index = 0 ; index < 100 ; index++ )
		{
			// create solid colored items
			
			view_t* itemview = view_default(0.0, 0.0, 500, -50);
			modifier_t* color;
			if ( index % 2 == 0 ) color = modifier_solidcolor_default( 0x00FF0022 );
			else color = modifier_solidcolor_default( 0x00FF0044 );
			view_addmodifier( itemview , color , 0 );
			
			modifier_t* cropper = modifier_cropper_default();
			view_addmodifier( itemview , ( modifier_t* ) cropper , 1 );

			view_new( &( ( modifier_args_t ) {
					.views      = { itemview	, NULL	} ,
					.generator  = { &freechar   , 1		} ,
					.command    = { "init"		, NULL  } ,
					.toolset    = demo->ui->toolset	    } ) );
			
			view_new( &( ( modifier_args_t ) {
					.views      = { itemlistview	, demo->ui->viewbase->view	} ,
					.generator  = { &freechar		, 1		} ,
					.command    = { "additem"		, itemview  } ,
					.toolset    = demo->ui->toolset			} ) );
		}
		
		// update itemlist's scroller

		view_new( &( ( modifier_args_t ) {
				.views      = { scroller		, demo->ui->viewbase->view	} ,
				.generator  = { &freechar		, 1		} ,
				.command    = { "update"		, NULL  } ,
				.toolset    = demo->ui->toolset			} ) );
		
		// update itemlist

		demo->itemlist = scroller;
	}

	// setup delegate, create views

    void demo_itemlist_setup( demo_itemlist_t* demo , char* path_resources , char* path_library )
    {
		// demo_itemlist_setupfromcode( demo );
		demo_itemlist_setupfromdescriptor( demo );
		
		// adding view
		viewbase_addview( demo->ui->viewbase , demo->itemlist );
		
    }

	// timer input

	void demo_itemlist_timer( demo_itemlist_t* demo )
	{

	}

	// event input

    void demo_itemlist_event( demo_itemlist_t* demo , modifierevent_t* event )
    {
    
    }
