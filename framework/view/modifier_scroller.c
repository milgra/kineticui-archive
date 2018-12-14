//
//  touch_modifier_scroller.c
//  KineticUI
//
//  Created by Milan Toth on 21/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_scroller.h"

	void modifier_scroller_free( modifier_t* modifier );
	void modifier_scroller_new( modifier_t* modifier , modifier_args_t* args );

	// creates scroller

	modifier_t* modifier_scroller_default( char rubber )
	{
		modifier_t* modifier        = malloc( sizeof( modifier_t ) );
		modifier_scroller_t* data   = malloc( sizeof( modifier_scroller_t ) );
		
		modifier->type = "scroller";
		modifier->data = data;
		modifier->_new = modifier_scroller_new;
		modifier->_free = modifier_scroller_free;

		data->last_point = vector3_default( 0.0 , 0.0 , 0.0 );
		data->scroller_y = scroller_default( rubber );
		data->scroller_x = scroller_default( rubber );
		data->enable_x = 1;
		data->enable_y = 1;
		data->scrollviewx = NULL;
		data->scrollviewy = NULL;
		data->delta_last.x = 0.0;
		data->delta_last.y = 0.0;

		return modifier;
	}

	// deletes scroller

	void modifier_scroller_free( modifier_t* modifier )
	{
        modifier_scroller_t* data = modifier->data;
		scroller_free( data->scroller_x );
		scroller_free( data->scroller_y );
		free( modifier->data );
        free( modifier );
	}

	// sets content view

	void modifier_scroller_initdimensions( modifier_t* modifier , view_t* view , modifier_args_t* args )
	{
        modifier_scroller_t* data = modifier->data;

		view_t* contentview = view->subviews->data[ 0 ];
		vector2_t viewsize = view_getsize( view );
		vector2_t contentsize = view_getsize( contentview );
		
		scroller_setsize( data->scroller_y , -viewsize.y );
		scroller_setcontentsize( data->scroller_y , -contentsize.y );
		scroller_setsize( data->scroller_x , viewsize.x );
		scroller_setcontentsize( data->scroller_x , contentsize.x );
		
		data->contentsize_last = view_getsize( contentview );
		data->headposition_last = view_getposition( contentview );

		// create scrollview

		char* viewdesc =
		"view scroller x 0 y 0 width 10.0 height -10.0 focuslevel 0 invisible 1\n"
		"modifier cropper\n"
		"modifier fader\n";
	
		map_t* viewmap;
		viewmap = generator_view_generate( viewdesc , 1.0 );
		data->scrollviewx = map_get( viewmap , "scroller" );
		map_free( viewmap );
		viewmap = generator_view_generate( viewdesc , 1.0 );
		data->scrollviewy = map_get( viewmap , "scroller" );
		map_free( viewmap );
		
		view_new( &( ( modifier_args_t ) {
			.views      = { data->scrollviewx		 , NULL } ,
			.generator  = { args->generator.changed_flag , 0    } ,
			.command    = { "init"						 , NULL } ,
			.toolset    = args->toolset							} ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { data->scrollviewy		 , NULL } ,
			.generator  = { args->generator.changed_flag , 0    } ,
			.command    = { "init"						 , NULL } ,
			.toolset    = args->toolset							} ) );
		
		view_setposition( data->scrollviewx , vector2_default( 0.0 , viewsize.y + 1.0 ));
		view_setposition( data->scrollviewy , vector2_default( viewsize.x - 1.0 , 0.0 ));

		if ( data->enable_scrollbar_x == 1 ) view_addsubview( view , data->scrollviewx );
		if ( data->enable_scrollbar_y == 1 ) view_addsubview( view , data->scrollviewy );

		modifier_t* fadera = view_getmodifier( data->scrollviewx , "fader" );
		modifier_t* faderb = view_getmodifier( data->scrollviewy , "fader" );
		
		modifier_fader_t* dataa = fadera->data;
		modifier_fader_t* datab = faderb->data;
		
		dataa->stopattop = 1;
		dataa->stopatbottom = 1;
		dataa->orientation = 0;
		datab->stopattop = 1;
		datab->stopatbottom = 1;
		datab->orientation = 1;
	}

	// mouse move event, updating scrollers

	void modifier_scroller_move( modifier_t* modifier , modifier_args_t* arguments )
	{
        modifier_scroller_t* data = modifier->data;

		input_t* input = ( input_t* ) arguments->command.arguments;
		vector3_t ulc = arguments->views.view->realcorners.ul;
		vector3_t ip = vector3_default( input->value_x - ulc.x , input->value_y - ulc.y , 1.0 );

		data->delta_last = vector2_default( ip.x - data->last_point.x , ip.y - data->last_point.y );
		data->last_point = ip;
		data->headposition_last = vector2_default( data->scroller_x->head , data->scroller_y->head );
		gettimeofday( &(data->lasttouch) , NULL );
		
		if ( data->enable_x == 0 ) data->delta_last.x = 0.0;
		if ( data->enable_y == 0 ) data->delta_last.y = 0.0;
		
		scroller_scroll( data->scroller_y , -data->delta_last.y );
		scroller_scroll( data->scroller_x ,  data->delta_last.x );
	}

	// mouse down event, storing point

	void modifier_scroller_down( modifier_t* modifier , modifier_args_t* arguments )
	{
        modifier_scroller_t* data = modifier->data;

		input_t* input = ( input_t* ) arguments->command.arguments;
		vector3_t ulc = arguments->views.view->realcorners.ul;
		vector3_t ip = vector3_default( input->value_x - ulc.x , input->value_y - ulc.y , 1.0 );

		data->last_point = ip;
		gettimeofday( &(data->lasttouch) , NULL );
	}

	// mouse up event, storing point

	void modifier_scroller_up( modifier_t* modifier , modifier_args_t* arguments )
	{
        modifier_scroller_t* data = modifier->data;

		input_t* input = ( input_t* ) arguments->command.arguments;
		vector3_t ulc = arguments->views.view->realcorners.ul;
		vector3_t ip = vector3_default( input->value_x - ulc.x , input->value_y - ulc.y , 1.0 );

		struct timeval now;
		gettimeofday( &now , NULL );
		long secs = now.tv_sec - data->lasttouch.tv_sec;
		long usecs = now.tv_usec - data->lasttouch.tv_usec;
		
		if ( secs < 1 && usecs < 50000 )
		{
			if ( data->enable_x == 1 ) scroller_scroll( data->scroller_x , data->delta_last.x );
			if ( data->enable_y == 1 ) scroller_scroll( data->scroller_y , -data->delta_last.y );
		}

		if ( data->enable_x == 1 ) scroller_auto( data->scroller_x );
		if ( data->enable_y == 1 ) scroller_auto( data->scroller_y );

		data->delta_last.x = 0.0;
		data->delta_last.y = 0.0;
		
		data->last_point = ip;
	}

	// mouse scroll event

	void modifier_scroller_scroll( modifier_t* modifier , modifier_args_t* arguments )
	{
        modifier_scroller_t* data = modifier->data;

		input_t* input = ( input_t* ) arguments->command.arguments;
		if ( data->enable_x == 1 ) scroller_scroll( data->scroller_x , input->value_x );
		if ( data->enable_y == 1 ) scroller_scroll( data->scroller_y , input->value_y  );
		if ( input->type == kInputTypeScrollEnd )
		{
			if ( data->enable_x == 1 ) scroller_auto( data->scroller_x );
			if ( data->enable_y == 1 ) scroller_auto( data->scroller_y );
		}
		data->headposition_last = vector2_default( data->scroller_x->head , data->scroller_y->head );
	}

	// timer event, updating scrollers

	void modifier_scroller_timer( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_scroller_t* data = modifier->data;

		// check scroll
		char x = scroller_update( data->scroller_x );
		char y = scroller_update( data->scroller_y );
		if ( x == 1 || y == 1 )
		{
			data->headposition_last = vector2_default( data->scroller_x->head , data->scroller_y->head );
			data->lastaction = 0;
			*(args->generator.changed_flag) = 1;
		}
		
		view_new( &( ( modifier_args_t ) {
			.views      = { data->scrollviewx		 , NULL } ,
			.generator  = { args->generator.changed_flag , 0    } ,
			.command    = { "timer"						 , NULL } ,
			.toolset    = args->toolset							} ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { data->scrollviewy		 , NULL } ,
			.generator  = { args->generator.changed_flag , 0    } ,
			.command    = { "timer"						 , NULL } ,
			.toolset    = args->toolset							} ) );
	}

	// updates dimensions

	void modifier_scroller_updatedimensions( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_scroller_t* data = modifier->data;

		// check content size
		view_t* contentview = args->views.view->subviews->data[ 0 ];
		vector2_t contentsize = view_getsize( contentview );
		vector2_t headposition = view_getposition( contentview );

		scroller_setsize( data->scroller_x , args->views.view->width );
		scroller_setsize( data->scroller_y , -args->views.view->height );

		if ( data->headposition_last.x != headposition.x ||
			 data->headposition_last.y != headposition.y )
		{
			scroller_setheadposition( data->scroller_y , headposition.y );
			scroller_setheadposition( data->scroller_x , headposition.x );
			data->headposition_last = headposition;
		}

		if ( data->contentsize_last.x != contentsize.x )
		{
			scroller_setcontentsize( data->scroller_x , contentsize.x );
			// jump to end in case of changed content
			// scroller_setheadposition( data->scroller_x , -contentsize.x );
			if ( data->scroller_x->size < data->scroller_x->size_content ) scroller_move( data->scroller_x , data->scroller_x->size - contentsize.x );
		}
		if ( data->contentsize_last.y != contentsize.y )
		{
			scroller_setcontentsize( data->scroller_y , -contentsize.y );
			// jump to end in case of changed content
			if ( data->scroller_y->size < data->scroller_y->size_content ) scroller_move( data->scroller_y , data->scroller_y->size + contentsize.y );
		}
		data->contentsize_last = contentsize;
	}

	// updates scroll bars

	void modifier_scroller_updatescrollbars( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_scroller_t* data = modifier->data;

		if ( data->enable_y == 1 )
		{
			float ratio = 1.0;
			if ( data->scroller_y->size_content > data->scroller_y->size ) ratio = data->scroller_y->size / data->scroller_y->size_content;
			float sizey = -data->scroller_y->size * ratio;
			float ypos = ( -data->scroller_y->head / data->scroller_y->size_content ) * data->scroller_y->size;
			vector2_t viewsize = view_getsize( args->views.view );
			view_setsize( data->scrollviewy , vector2_default( 3.0 * args->toolset->settings->scale , sizey ) );
			view_setposition( data->scrollviewy , vector2_default( viewsize.x - 3.0 * args->toolset->settings->scale , -ypos ) );
		}
		if ( data->enable_x == 1 )
		{
			float ratio = 1.0;
			if ( data->scroller_x->size_content > data->scroller_x->size ) ratio = data->scroller_x->size / data->scroller_x->size_content;
			float sizex = data->scroller_x->size * ratio;
			float xpos = ( -data->scroller_x->head / data->scroller_x->size_content ) * data->scroller_x->size;
			vector2_t viewsize = view_getsize( args->views.view );
			view_setsize( data->scrollviewx , vector2_default( sizex , -3.0 * args->toolset->settings->scale ) );
			view_setposition( data->scrollviewx , vector2_default( xpos , viewsize.y + 3.0 * args->toolset->settings->scale ) );
		}
	}

	// adds scrollbars

	void modifier_scroller_addscrollbars( modifier_t* modifier , view_t* view )
	{
        modifier_scroller_t* data = modifier->data;

		modifier_t* fadera = view_getmodifier( data->scrollviewx , "fader" );
		modifier_t* faderb = view_getmodifier( data->scrollviewy , "fader" );

		modifier_fader_t* dataa = fadera->data;
		modifier_fader_t* datab = faderb->data;
		
		dataa->stopattop = 1;
		dataa->direction = 1;
		datab->stopattop = 1;
		datab->direction = 1;
	}

	// remove scrollbars

	void modifier_scroller_removescrollbars( modifier_t* modifier , view_t* view )
	{
        modifier_scroller_t* data = modifier->data;

		modifier_t* fadera = view_getmodifier( data->scrollviewx , "fader" );
		modifier_t* faderb = view_getmodifier( data->scrollviewy , "fader" );

		modifier_fader_t* dataa = fadera->data;
		modifier_fader_t* datab = faderb->data;

		dataa->stopatbottom = 1;
		dataa->direction = 0;
		datab->stopatbottom = 1;
		datab->direction = 0;
	}

	// generate state

	void modifier_scroller_new( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_scroller_t* data = modifier->data;

		if ( strcmp( args->command.name , "init" ) == 0  )
		{
			modifier_scroller_initdimensions( modifier , args->views.view , args );
		}
		else if ( strcmp( args->command.name , "timer" ) == 0  )
		{
			modifier_scroller_timer( modifier , args );
		}
		else if ( strcmp( args->command.name , "scroll" ) == 0  )
		{
			modifier_scroller_scroll( modifier , args );
			modifier_scroller_addscrollbars( modifier , args->views.view );

			vector_adddata( args->toolset->timeds , args->views.view );

			data->lastaction = 0;
			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "scrollend" ) == 0  )
		{
			modifier_scroller_scroll( modifier , args );
			modifier_scroller_removescrollbars( modifier , args->views.view );

			vector_adddata( args->toolset->timeds , args->views.view );

			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "touchdrag" ) == 0  )
		{
			modifier_scroller_move( modifier , args );
			data->lastaction = 0;
			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "touchdown" ) == 0  )
		{
			modifier_scroller_down( modifier , args );
			modifier_scroller_addscrollbars( modifier , args->views.view );

			vector_adddata( args->toolset->timeds , args->views.view );

			data->lastaction = 0;
			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0  )
		{
			modifier_scroller_up( modifier , args );
			modifier_scroller_removescrollbars( modifier , args->views.view );

			vector_adddata( args->toolset->timeds , args->views.view );

			data->lastaction = 0;
			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "keypress" ) == 0  )
		{
			modifier_scroller_updatedimensions( modifier , args );

			vector_adddata( args->toolset->timeds , args->views.view );

			data->lastaction = 0;
			*(args->generator.changed_flag) = 1;
		}
		else if ( strcmp( args->command.name , "update" ) == 0  || strcmp( args->command.name , "layoutupdate" ) == 0  )
		{
			modifier_scroller_updatedimensions( modifier , args );

			vector_adddata( args->toolset->timeds , args->views.view );

			*(args->generator.changed_flag) = 1;
		}
		// modify view
		if ( strcmp( args->command.name , "init" ) == 0  ||
			 strcmp( args->command.name , "timer" ) == 0  ||
			 strcmp( args->command.name , "scroll" ) == 0  ||
			 strcmp( args->command.name , "touchdrag" ) == 0  ||
			 strcmp( args->command.name , "touchdown" ) == 0  ||
			 strcmp( args->command.name , "touchup" ) == 0  ||
			 strcmp( args->command.name , "keypress" ) == 0  )
		{
			modifier_scroller_updatescrollbars( modifier , args );
			view_t* contentview = args->views.view->subviews->data[0];
			view_setposition( contentview , vector2_default( data->scroller_x->head , -data->scroller_y->head ) );
		}
	}