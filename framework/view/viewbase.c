//
//  Simulator.c
//  KineticUI
//
//  Created by Milan Toth on 11/09/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "viewbase.h"


    #pragma mark viewbase

    // creates viewbase

    viewbase_t* viewbase_init( float width , float height )
    {
		viewbase_t* result = malloc( sizeof( viewbase_t ) );
		
		result->view = viewbase_defaultbaseview( width , -height );
		result->timed = vector_create( );
		result->touchfocused = vector_create( );
		result->scrollfocused = vector_create( );
		result->timedtoremove = vector_create( );
		
		return result;
    }

	// deletes viewbase

	void viewbase_free( viewbase_t* viewbase )
	{
		view_free( viewbase->view );
		vector_free( viewbase->timed );
		vector_free( viewbase->touchfocused );
		vector_free( viewbase->scrollfocused );
		vector_free( viewbase->timedtoremove );
		
		free( viewbase );
	}

	// create base view

	view_t* viewbase_defaultbaseview( float width , float height )
	{
		view_t* result = view_default( 0.0 , 0.0 ,	width ,	height );
		view_setname( result , "baseview" );
		
        result->focuslevel = 0;
        result->invisible = 1;
		
		modifier_t* layout = modifier_layout_default( ( 1 << 3 ) | ( 1 << 2 ) | ( 1 << 1 ) | ( 1 << 0 ) );
		
		view_addmodifier ( result ,	( modifier_t* ) layout , 0 );
		
		return result;
	}

	// resize

	void viewbase_resize( viewbase_t* viewbase , toolset_t* toolset , float width , float height )
	{
		if ( toolset->logs != NULL ) vector_adddata( toolset->logs , string_createfromformat( 200 , "viewbase_resize %f %f" , width , height ) );

		view_setsize( viewbase->view , vector2_default( width , height ) );
		
		// new states based on layout input
		
		view_new( &( ( modifier_args_t ) {
			.views      = { viewbase->view	, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "layoutupdate"	, NULL } ,
			.toolset    = toolset			   } ) );
		
		// scrollbar and textarea needs state update after their childs got resized

		view_new( &( ( modifier_args_t ) {
			.views      = { viewbase->view	, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "update"		, NULL } ,
			.toolset    = toolset				   } ) );
		
	}

	// adds view to view base

	void viewbase_addview( viewbase_t* viewbase , view_t* view )
	{
		view_addsubview( viewbase->view , view );
	}

	void viewbase_addviewatindex( viewbase_t* viewbase , view_t* view , uint32_t index )
	{
		view_addsubviewatindex( viewbase->view , view , index );
	}

	char viewbase_containsview( viewbase_t* viewbase , view_t* view )
	{
		return view_containssubview( viewbase->view , view );
	}

	// removes view from view base

	void viewbase_removeview( viewbase_t* viewbase , view_t* view )
	{
		// remove view and subviews from timed and focused vectors

		vector_t* views = vector_create( );
		vector_t* timedtoremove = vector_create( );
		vector_t* focusedtoremove = vector_create( );

		vector_adddata( views , view );
		view_collect_subviews( view , views );
		
		for ( int index = 0 ; index < viewbase->timed->length ; index++ )
		{
			uint32_t catch = vector_indexofdata( views , viewbase->timed->data[ index ] );
			if ( catch < UINT32_MAX ) vector_adddata( timedtoremove , views->data[ catch ] );
		}
		for ( int index = 0 ; index < viewbase->touchfocused->length ; index++ )
		{
			uint32_t catch = vector_indexofdata( views , viewbase->touchfocused->data[ index ] );
			if ( catch < UINT32_MAX ) vector_adddata( focusedtoremove , views->data[ catch ] );
		}

		view_removesubview( viewbase->view , view );
		
		vector_removedatainvector( viewbase->timed , timedtoremove );
		vector_removedatainvector( viewbase->touchfocused , focusedtoremove );
		
		vector_free( views );
		vector_free( timedtoremove );
		vector_free( focusedtoremove );
	}

	// sets focus to given view

	void viewbase_setfocus( viewbase_t* viewbase , vector_t* views , char* changed , toolset_t* toolset )
	{
		// send unfocus event to old views not in new chain
		
		for ( int index = 0 ; index < views->length ; index++ )
		{
			view_t* view = views->data[ index ];
			vector_removedata( viewbase->touchfocused , view );
		}

		struct view_command_t ufcommand = { .name = "unfocus" , .arguments = NULL };
		viewbase_generateviewchain( viewbase , viewbase->touchfocused , &ufcommand , changed , toolset );
		
		// set chain as new focused chain
		
		vector_reset( viewbase->touchfocused );
		vector_adddatainvector( viewbase->touchfocused , views );
		
		struct view_command_t fcommand = { .name = "focus" , .arguments = NULL };
		viewbase_generateviewchain( viewbase , viewbase->touchfocused , &fcommand , changed , toolset );
	}

	// scroll start, collect views under cursor

    void viewbase_scroll_start( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
		input_t* input = ( input_t* ) command->arguments;
		vector_reset( viewbase->scrollfocused );
        vector_t* vector = vector_create( );
        view_collect_touched_views( viewbase->view , vector , input->value_x , input->value_y );
		
		for ( int index = 0 ;
				  index < vector->length ;
				  index++ )
		{
			view_t* view = vector->data[ index ];
			
			if ( view->focuslevel > 1 ) vector_reset( viewbase->scrollfocused );	// view doesn't allow focusables under itself
			if ( view->focuslevel > 0 ) vector_adddata( viewbase->scrollfocused , view );
		}
		
		vector_free( vector );
	}

	// scroll move, generate new states for scrollfocused views

    void viewbase_scroll_move( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
        if ( viewbase->scrollfocused->length > 0 )
		{
			viewbase_generateviewchain(
				viewbase ,
				viewbase->scrollfocused ,
				command ,
				changed ,
				toolset	);
		}
    }

	// scroll ended, generate new states for scrollfocused views

    void viewbase_scroll_end( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
        if ( viewbase->scrollfocused->length > 0 )
		{
			viewbase_generateviewchain(
				viewbase ,
				viewbase->scrollfocused ,
				command ,
				changed ,
				toolset );
		}
	}

	// keypress event from outside, returns state change, collects events during change

    void viewbase_keydown( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
        if ( viewbase->touchfocused != NULL )
		{
			viewbase_generateviewchain
			(
				viewbase ,
				viewbase->touchfocused ,
				command ,
				changed ,
				toolset
			);
		}
    }

	// touch event down, returns state change, collects events during change

    void viewbase_touchdown( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
		input_t* input = ( input_t* ) command->arguments;
        vector_t* vector = vector_create( );
        view_collect_touched_views( viewbase->view , vector , input->value_x , input->value_y );
        vector_t* focusable = vector_create( );
		for ( int index = 0 ; index < vector->length ; index++ )
		{
			view_t* view = vector->data[ index ];
			if ( view->focuslevel > 1 ) vector_reset( focusable );	// view doesn't allow focusables under itself
			if ( view->focuslevel > 0 ) vector_adddata( focusable , view );
		}
		viewbase_setfocus( viewbase , focusable , changed , toolset );
		viewbase_generateviewchain( viewbase , viewbase->touchfocused , command , changed , toolset );
		// cleanup
		vector_free( vector );
		vector_free( focusable );
    }

	// touch event drag, returns state change, collects events during change

    void viewbase_touchdrag( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
		if ( viewbase->touchfocused != NULL ) viewbase_generateviewchain( viewbase , viewbase->touchfocused , command , changed , toolset );
    }

	// touch event up, returns state change, collects events during change

    void viewbase_touchup( viewbase_t* viewbase , char* changed , struct view_command_t* command , toolset_t* toolset )
    {
		input_t* input = ( input_t* ) command->arguments;
		if ( toolset->logs != NULL ) vector_adddata( toolset->logs , string_createfromformat( 200 , "viewbase_touchup %f %f" , input->value_x , input->value_y ) );
		
		if ( viewbase->touchfocused != NULL )
		{
			viewbase_generateviewchain
			(
				viewbase ,
				viewbase->touchfocused ,
				command ,
				changed ,
				toolset
			);
		}
    }

	// generate new state for views in a chain

	void viewbase_generateviewchain( viewbase_t* viewbase , vector_t* vector , struct view_command_t* command , char* changed , toolset_t* toolset )
	{
		// going backwards to let lower layers detect changes in upper layers
		for ( int index = vector->length - 1 ; index > -1 ; index-- )
		{
			view_t* view = vector->data[ index ];
			view_new( &( ( modifier_args_t ) {
				.views      = { view	, NULL } ,
				.generator  = { changed	, 0    } ,
				.command    = *command	,
				.toolset    = toolset	} ) );
		}
	}

	// adds timeds view to update by timer

	void viewbase_addtimedview( viewbase_t* viewbase , view_t* view )
	{
		if ( vector_indexofdata( viewbase->timed , view ) == UINT32_MAX ) vector_adddata( viewbase->timed , view );
	}

	// timer input from outside, generating new state

    void viewbase_timer( viewbase_t* viewbase , char* changed , toolset_t* toolset )
	{
		// generate new states for views in timed list
		for ( int timedi = 0 ; timedi < viewbase->timed->length ; timedi++ )
		{
			view_t* view = viewbase->timed->data[ timedi ];
			char localchanged = 0;
			
			view_new( &( ( modifier_args_t ) {
                    .views      = { view			, NULL	 } ,
                    .generator  = { &localchanged   , 0 } ,
                    .command    = { "timer"			, NULL	  } ,
                    .toolset    = toolset		  } ) );
			
			*changed |= localchanged;
			if ( localchanged == 0 ) vector_adddata( viewbase->timedtoremove , view );
		}
		// remove finished views from times views
		if ( viewbase->timedtoremove->length > 0 )
		{
			for ( int removi = 0 ; removi < viewbase->timedtoremove->length ; removi++ )
			{
				view_t* view = viewbase->timedtoremove->data[ removi ];
				vector_removedata( viewbase->timed , view );
			}
			vector_reset( viewbase->timedtoremove );
		}
	}

	// udpates subview matrixes, needed before quad generation if change happened

	void viewbase_finalize_geometry( viewbase_t* viewbase , toolset_t* toolset , uint32_t* counter , uint32_t* quadcounter )
	{
		view_finalize_geometry( viewbase->view , NULL , matrix3_defaultidentity( ) , 0 , toolset , counter , quadcounter );
	}

    // generates raw quad vertex data from actual state ( position, texure, alpha )

    void viewbase_collectvertexdata( viewbase_t* viewbase , GLfloat* vertexes , uint32_t* quadcount )
    {
        view_collect_vertexes( viewbase->view , vertexes , quadcount );
    }


//	// 3D additions
//	// projects a line from screen points into the model world, returns two points on the line
//
//    void viewbase_getworldspacelines( viewbase_t* viewbase , vector3_t* line_a , vector3_t* line_b , float width , float height , float mouse_x , float mouse_y )
//    {
//        // create screen-plane-normal vectors
//        vector4_t vector_a = vector4_default( mouse_x , mouse_y , -1.0 , 0 );
//        vector4_t vector_b = vector4_default( mouse_x , mouse_y ,  1.0 , 0 );
//		
//        // unproject vectors to 3d world
//		matrix4_t proj_matrix = matrix4_defaultortho(0.0,width,-height,0.0,0.0,1.0);
//        vector4_t result_a = matrix4_unprojectvector4fromscreen( proj_matrix , vector_a , width , height );
//        vector4_t result_b = matrix4_unprojectvector4fromscreen( proj_matrix , vector_b , width , height );
//		
//        // create lines
//        *line_a = vector3_default( result_a.x , result_a.y , result_a.z );
//        *line_b = vector3_default( result_b.x , result_b.y , result_b.z );
//    }
