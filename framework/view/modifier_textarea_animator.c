//
//  modifier_textarea_animator.c
//  c36
//
//  Created by Milan Toth on 29/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_textarea.h"


	// open glyph animation

	void modifier_textarea_animator_view_open( modifier_t* modifier , view_t* view , glyph_t* glyph , int delay , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		view_setposition( view , vector2_default( glyph->x , -glyph->y ) );
		
		view_new( &( ( modifier_args_t ) {
				.views		= { view , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "expand" , &( ( clipper_args ) {
					.steps = 8 ,
					.delay = delay ,
					.apply = 1 } ) } ,
				.toolset	= args->toolset } ) );
		
		if ( vector_indexofdata( data->animatingviews , view ) == UINT32_MAX ) vector_adddata( data->animatingviews , view );
	}

	// open cursor animation

	void modifier_textarea_animator_cursor_open( modifier_t* modifier , glyph_t* glyph , int delay , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		if ( data->cursor != NULL )
		{
			vector2_t glyphposition = vector2_default( glyph->x , -glyph->y );
			vector2_t glyphsize = vector2_default( glyph->width , glyph->height );
			glyphposition.x += glyphsize.x;
			
			// set cursors final position
			view_setposition( data->cursor , vector2_default( glyphposition.x , glyphposition.y ) );
			
			// animate cursor from old position to final position
			view_new( &( ( modifier_args_t ) {
					.views		= { data->cursor , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "setphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { -glyphsize.x , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } } ) } ,
					.toolset	= args->toolset } ) );

			view_new( &( ( modifier_args_t ) {
					.views		= { data->cursor , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { -glyphsize.x , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = delay ,
						.easing = 0 ,
						.action = NULL ,
						.force = 1 } ) } ,
					.toolset	= args->toolset } ) );

			view_new( &( ( modifier_args_t ) {
					.views		= { data->cursor , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 0.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 8 ,
						.easing = 0 ,
						.action = NULL ,
						.force = 0 } ) } ,
					.toolset	= args->toolset } ) );
			
			// restart hover animation
			modifier_t* hover = view_getmodifier( data->cursor , "hover" );
            modifier_hover_t* data = hover->data;
			data->radius = 5.3;
		}
	}

	// close cursor animation

	void modifier_textarea_animator_cursor_close( modifier_t* modifier , view_t* view , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		if ( data->cursor != NULL )
		{
			vector2_t glyphposition = view_getposition( view );
			vector2_t glyphsize = view_getsize( view );
			
			// set cursors final position
			view_setposition( data->cursor , vector2_default( glyphposition.x , glyphposition.y ) );
			
			// animate cursor from old position to final position
			view_new( &( ( modifier_args_t ) {
					.views		= { data->cursor , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "setphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { glyphsize.x , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } } ) } ,
					.toolset	= args->toolset } ) );

			view_new( &( ( modifier_args_t ) {
					.views		= { data->cursor , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 0.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 8 ,
						.easing = 0 ,
						.action = NULL ,
						.force = 1 } ) } ,
					.toolset	= args->toolset } ) );
			
			// restart hover animation
			modifier_t* hover = view_getmodifier( data->cursor , "hover" );
            modifier_hover_t* data = hover->data;
			data->radius = 5.3;
		}
	}

	// slide down animation

	void modifier_textarea_animator_view_slidedown( modifier_t* modifier , view_t* view , glyph_t* glyph , float dx , float dy , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		// set glyphs final position
		view_setposition( view , vector2_default( glyph->x , -glyph->y ) );
		
		// animate glyph from old position to final pos
		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "setphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { dx  , dy } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { dx , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 1 } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= args->toolset } ) );
	
		// add to animating
		if ( vector_indexofdata( data->animatingviews , view ) == UINT32_MAX ) vector_adddata( data->animatingviews , view );
	}

	// slide up animation

	void modifier_textarea_animator_view_slideup( modifier_t* modifier , view_t* view , glyph_t* glyph , float dx , float dy , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		// set glyphs final position
		view_setposition( view , vector2_default( glyph->x , -glyph->y ) );
	
		// animate glyph from old position to final pos
		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "setphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { dx  , dy } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , dy } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 1 } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= args->toolset } ) );
		
		// add to animating
		if ( vector_indexofdata( data->animatingviews , view ) == UINT32_MAX ) vector_adddata( data->animatingviews , view );
	}

	// slide animation

	void modifier_textarea_animator_view_slide( modifier_t* modifier , view_t* view , glyph_t* glyph , float dx , float dy , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		// set glyphs final position
		view_setposition( view , vector2_default( glyph->x , -glyph->y ) );

		// animate glyph from old position to final pos
		view_new( &( ( modifier_args_t ) {
				.views		= { view , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "setphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { dx  , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { view , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 1 } ) } ,
				.toolset	= args->toolset } ) );
		
		// add to animating
		if ( vector_indexofdata( data->animatingviews , view ) == UINT32_MAX ) vector_adddata( data->animatingviews , view );
	}

	// cursor slide down animation

	void modifier_textarea_animator_cursor_slidedown( modifier_t* modifier , view_t* view , glyph_t* glyph , vector2_t position , int delay , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		float dx = position.x - glyph->x;
		float dy = position.y - -glyph->y;

		// set cursors final position
		view_setposition( data->cursor , vector2_default( glyph->x + glyph->width , -glyph->y ) );

		// animate glyph from old position to final pos
		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "setphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { dx  , dy } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { dx  , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps  = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force  = 1 } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { -glyph->width , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= args->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { data->cursor , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 8 ,
					.easing = 0 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= args->toolset } ) );

	}

	// cursor slide up animation

	void modifier_textarea_animator_cursor_slideup( modifier_t* modifier , view_t* view , glyph_t* glyph , vector2_t position , int delay )
	{
        modifier_textarea_t* data = modifier->data;

		view_setposition( data->cursor , vector2_default( glyph->x + glyph->width , -glyph->y ) );
	}

	// align glyphs with open animation

	void modifier_textarea_animator_alignglyphs_open( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		// printf( "\nmodifier_textarea_animator_alignglyphs_open %zx" , ( size_t ) modifier );
		int counter = 1;
		for ( int index = 0 ; index < data->paragraph->letters->length ; index++ )
		{
            view_t* view = data->glyphviews->data[ index ];
            glyph_t* glyph = data->paragraph->letters->data[ index ];
			// open view to given glyphs position
			modifier_textarea_animator_view_open( modifier , view , glyph , counter++ , args );
        }
		
		// update cursor
		if ( data->cursor != NULL )
		{
			vector2_t coords = vector2_default( 0.0 , 0.0 );
			if ( data->glyphviews->length > 0 )
			{
				view_t* view = data->glyphviews->data[ data->glyphviews->length - 1 ];
				// set cursor position after last glyphs position
				vector2_t position = view_getposition( view );
				vector2_t size = view_getsize( view );
				coords.x = position.x + size.x;
				coords.y = position.y;
			}
			view_setposition( data->cursor , coords );
		}
	}

	// align glyphs with lastpos to newpos sliding animation

	void modifier_textarea_animator_alignglyphs_slide( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		// printf( "\nmodifier_textarea_animator_alignglyphs_open %zx" , ( size_t ) modifier );
		char sliding = 0;
		char lastslide = 0;
		vector2_t lastposition;
		
		for ( int index = 0 ; index < data->paragraph->letters->length ; index++ )
		{
            view_t* view = data->glyphviews->data[ index ];
            glyph_t* glyph = data->paragraph->letters->data[ index ];
			vector2_t position = view_getposition( view );
			
			// new letter
			if ( position.x < 0.0 )
			{
				if ( lastslide == 0 )
				{
					// add open animation to newly created glyph, animate cursor also
					modifier_textarea_animator_view_open( modifier , view , glyph , 1 , args );
					modifier_textarea_animator_cursor_open( modifier , glyph , 1 , args );
				}
				else if ( lastslide == 1 )
				{
					// if slide happened, add open animation after 16 frames, slide down cursor
					modifier_textarea_animator_view_open( modifier , view , glyph , 16 , args );
					modifier_textarea_animator_cursor_slidedown( modifier , view , glyph , lastposition , 1 , args );
				}
			}
			// old letter
			else
			{
				// stop old opening glyphs
				modifier_clipper_t* clipper = (void*) view_getmodifier( view , "clipper" );
				if ( clipper->playing == 1 ) clipper->playing = 0;
			
				// calculate difference between old and new position
				float dx = position.x - glyph->x;
				float dy = position.y - ( -glyph->y );
				lastslide = 0;
				
				// if there is difference
				if ( fabs( dx ) > 0 || fabs( dy ) > 0 )
				{
					// if there is vertical difference
					if ( fabs( dy ) > 0 )
					{
						lastposition = position;
						if ( dy > 0 )
						{
							// slide down
							lastslide = 1;
							modifier_textarea_animator_view_slidedown( modifier , view , glyph , dx , dy , args );
						}
						else
						{
							// slide up
							lastslide = 2;
							modifier_textarea_animator_view_slideup( modifier , view , glyph , dx , dy , args );
							// if last view
							if ( index == data->paragraph->letters->length - 1 )
							{
								sliding = 1;
								modifier_textarea_animator_cursor_slideup( modifier , view , glyph , lastposition , 1 );
							}
						}
					}
					// in case of horizontal difference simply slide view to position
					else modifier_textarea_animator_view_slide( modifier , view , glyph , dx , dy , args );
				}
			}
        }
		// handle removed glyphs
		if ( data->removedglyphs->length > 0 )
		{
			for ( int index = 0 ; index < data->removedglyphs->length ; index++ )
			{
				view_t* view = data->removedglyphs->data[ index ];

				// add shrinking animation to removed glyphs
				view_new( &( ( modifier_args_t ) {
						.views		= { view , NULL } ,
						.generator	= { &freechar , 0 } ,
						.command	= { "shrink" , &( ( clipper_args ) {
							.steps = 8 ,
							.delay = 0 ,
							.apply = 0 } ) } ,
						.toolset	= args->toolset } ) );

				// and close cursor
				if ( sliding == 0 ) modifier_textarea_animator_cursor_close( modifier , view , args );
				
				// add view to animating views
				if ( vector_indexofdata( data->animatingviews , view ) == UINT32_MAX ) vector_adddata( data->animatingviews , view );
				
				// add view to dying views
				vector_adddata( data->dyingviews , view );
			}
			// empty removed glyphs
			vector_reset( data->removedglyphs );
		}
	}

	// timer trigger

	void modifier_textarea_animator_timer( modifier_t* modifier , modifier_args_t* args )
    {
        modifier_textarea_t* data = modifier->data;

		// loop through timed views, check which can be removed
		for ( int index = 0 ; index < data->animatingviews->length ; index++ )
		{
			view_t* view = data->animatingviews->data[ index ];
			char localchanged = 0;
			
			view_new( &( ( modifier_args_t ) {
				.views      = { view			, NULL } ,
				.generator  = { &localchanged	, 0    } ,
				.command    = { "timer"	, NULL } ,
				.toolset    = args->toolset		       } ) );
			
			*(args->generator.changed_flag) |= localchanged;
			// if there's no change, we can remove the view from the animating views
			if ( localchanged == 0 ) vector_adddata( data->stoppedviews , view );
			// printf( "\nANIMSTEPPING %s" , view->name );
		}

		// remove finished views from animating views
		if ( data->stoppedviews->length > 0 )
		{
			for ( int index = 0 ; index < data->stoppedviews->length ; index++ )
			{
				view_t* view = data->stoppedviews->data[ index ];
				vector_removedata( data->animatingviews , view );
				// printf( "\nREMOVING FROM ANIMATED %s %i" , view->name , data->animatingviews->length );
			}
			vector_reset( data->stoppedviews );
		}
		
		// destruct dying views if they are ready
		if ( data->dyingviews->length > 0 )
		{
			for ( int index = 0 ; index < data->dyingviews->length ; index++ )
			{
				view_t* view = data->dyingviews->data[ index ];
				modifier_t* clipper = view_getmodifier( view, "clipper" );
				modifier_clipper_t* clipperdata = clipper->data;
				if ( clipperdata->playing == 0 ) vector_adddata( data->stoppedviews , view );
			}
			// remove finished views from dying views
			if ( data->stoppedviews->length > 0 )
			{
				for ( int index = 0 ; index < data->stoppedviews->length ; index++ )
				{
					view_t* view = data->stoppedviews->data[ index ];
					vector_removedata( data->dyingviews , view );
					vector_removedata( data->animatingviews , view );
					// printf( "\nREMOVING FROM DYING %s %i %i" , view->name , data->dyingviews->length , data->animatingviews->length );
					view_removesubview( data->contentview , view );
					view_free( view );
				}
				vector_reset( data->stoppedviews );
			}
		}
		
		// animate cursor
		if ( data->cursor != NULL && data->focused == 1 )
		{
			view_new( &( ( modifier_args_t ) {
				.views      = { data->cursor				, NULL } ,
				.generator  = { args->generator.changed_flag	, 0    } ,
				.command    = { "timer"							, NULL } ,
				.toolset    = args->toolset							   } ) );
		}
		
		if ( *(args->generator.changed_flag) == 0 )
		{
			object_t* eventobject = modifier_defaultevent( "animationfinished" , data->animendaction , modifier , NULL );
			vector_adddata( args->toolset->events , eventobject );
		}
	}