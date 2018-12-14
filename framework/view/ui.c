//
//  strips.c
//  strips
//
//  Created by Milan Toth on 18/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "ui.h"

	#define STARTING_QUAD_COUNT 1000

	// creates new ui state

	ui_t* ui_init(
		float width ,
		float height ,
		float scale ,
		char* path_resources ,
		char* path_library ,
		uidelegate_t* delegate )
	{
		// assetions

		assert( path_resources != NULL );
		assert( path_library != NULL );
		assert( delegate != NULL );
		
		// ui
		
		ui_t* result = malloc( sizeof(ui_t ) );

        result->delegate = delegate;
		result->settings = uisettings_init( path_resources , path_library , scale , width , height );
		result->viewbase = viewbase_init( width , height );
		result->renderer = renderer_default( width , height );
		result->tilemap = tilemap_create( );
		result->font = font_default( result->settings->font );

		result->counters = malloc( sizeof( counters_t ) );
		result->toolset = malloc( sizeof( toolset_t ) );
		result->events = vector_create( );
		result->timeds = vector_create( );
		result->logs = NULL;

		result->lastrender.tv_sec = 0;
		result->lastrender.tv_usec = 0;
		result->vertexbuffer = malloc( sizeof( GLushort ) * STARTING_QUAD_COUNT * 4 * 5 );
		result->framedrop = 0;
		result->maxquads = STARTING_QUAD_COUNT;

		// setup modifier toolset
	
		result->toolset->settings = result->settings;
		result->toolset->tilemap = result->tilemap;
		result->toolset->events = result->events;
		result->toolset->timeds = result->timeds;
		result->toolset->logs = result->logs;
		result->toolset->font = result->font;
		
		// setup delegate
		
		result->delegate->ui = result;
        result->delegate->setup( result->delegate , path_resources , path_library );
		
		// render before io stuff, twice to reset old buffer in gpu
		
		ui_render( result , 1 );
		ui_render( result , 1 );
		
		return result;
	}

	// deletes ui

	void ui_free( ui_t* ui )
	{
		// cleanup
		
		uisettings_free( ui->settings );
		viewbase_free( ui->viewbase );
		renderer_free( ui->renderer );
		tilemap_free( ui->tilemap );
		font_free( ui->font );

		free( ui->counters );
		free( ui->toolset );
		vector_free( ui->events );
		vector_free( ui->timeds );
		vector_free( ui->logs );
		
		free( ui->vertexbuffer );
		free( ui );
	}

	// resize external event

	void ui_resize( ui_t* ui , float width , float height , char contextresized )
	{
		if ( ui->settings->screensize.x != width || ui->settings->screensize.y != height )
		{
			// store new values
			
			ui->settings->screensize.x = width;
			ui->settings->screensize.y = height;

			// resize renderer and viewbase
			
			if ( contextresized == 1 ) renderer_resize( ui->renderer , width , height );
			viewbase_resize( ui->viewbase , ui->toolset , width , -height );

			// notify delegate
			
//			object_t* eventobject = modifier_defaultevent( "resize" , "default" , "resize" , ui->viewbase->view );
//			ui->delegate->event( ui->delegate , eventobject->data );
//			modifier_freeevent( eventobject );
			
			// render
			
			ui_render( ui , 1 );
		}
	}

	// touch up external event, events must be handled after draw, since they usually cause full state change

	void ui_touch_up( ui_t* ui , float touch_x , float touch_y )
	{
		input_t input = { .type = kInputTypeTouchUp , .value_x = touch_x , .value_y = touch_y };
		struct view_command_t command = { .name = "touchup" , .arguments = &input };
		
        char changed = 0;
		viewbase_touchup( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );
		if ( ui->logs != NULL ) ui_printlogs( ui );
		
		if ( changed == 1 ) ui_render( ui , 0 );
	}

	// touch down external event, events must be handled after draw, since they usually cause full state change

	void ui_touch_down( ui_t* ui , float touch_x , float touch_y )
	{
		input_t input = { .type = kInputTypeTouchDown , .value_x = touch_x , .value_y = touch_y };
		struct view_command_t command = { .name = "touchdown" , .arguments = &input };

        char changed = 0;
		viewbase_touchdown( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );
		
		if ( changed == 1 ) ui_render( ui , 0 );
	}

	// touch move external event, events must be handled after draw, since they usually cause full state change

	void ui_touch_move( ui_t* ui , float touch_x , float touch_y )
	{
		input_t input = { .type = kInputTypeTouchDrag , .value_x = touch_x , .value_y = touch_y };
		struct view_command_t command = { .name = "touchdrag" , .arguments = &input };

        char changed = 0;
		viewbase_touchdrag( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );

		if ( changed == 1 )ui_render( ui , 0 );
	}

	// key down external event, events must be handled after draw, since they usually cause full state change

	void ui_key_down( ui_t* ui , char* characters , char key )
    {
		string_t* charstring = NULL;
		if ( characters != NULL ) charstring = string_createfrombytes( characters );
		input_t input = { .type = kInputTypeKeyPress , .key = key , .characters = charstring }; // needs release
		struct view_command_t command = { .name = "keypress" , .arguments = &input };

		// have to copy characters when using different threads
        char changed = 0;
		viewbase_keydown( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );

		if ( changed == 1 )ui_render( ui , 0 );
	
		if ( charstring != NULL ) string_free( charstring );
    }

	// scroll start external event, events must be handled after draw, since they usually cause full state change

	void ui_scroll_start( ui_t* ui , float touch_x , float touch_y )
	{
		input_t input = { .type = kInputTypeScroll , .value_x = touch_x , .value_y = touch_y };
		struct view_command_t command = { .name = "scroll" , .arguments = &input };

        char changed = 0;
		viewbase_scroll_start( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );

		if ( changed == 1 )ui_render( ui , 0 );
	}

	// scroll move external event, events must be handled after draw, since they usually cause full state change

	void ui_scroll_move( ui_t* ui , float dx , float dy )
	{
		input_t input = { .type = kInputTypeScroll , .value_x = dx , .value_y = dy };
		struct view_command_t command = { .name = "scroll" , .arguments = &input };

        char changed = 0;
		viewbase_scroll_move( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );

		if ( changed == 1 )ui_render( ui , 0 );
	}

	// scroll end external event, events must be handled after draw, since they usually cause full state change

	void ui_scroll_end( ui_t* ui , float dx , float dy )
	{
		input_t input = { .type = kInputTypeScrollEnd , .value_x = dx , .value_y = dy };
		struct view_command_t command = { .name = "scrollend" , .arguments = &input };

        char changed = 0;
		viewbase_scroll_end( ui->viewbase , &changed , &command , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );

		if ( changed == 1 )ui_render( ui , 0 );
	}

	// timer external event, events must be handled after draw, since they usually cause full state change

	void ui_timer( ui_t* ui )
	{
		// notify delegate
		
		ui->delegate->timer( ui->delegate );
		
		// trigger timed views

		char changed = 0;
		viewbase_timer( ui->viewbase , &changed , ui->toolset );
		if ( ui->events->length > 0 ) ui_handleevents( ui );
		if ( ui->timeds->length > 0 ) ui_handletimeds( ui );
		if ( ui->logs != NULL ) ui_printlogs( ui );

		if ( changed == 1 || ui->framedrop == 1 ) ui_render( ui , 0 );
	}

	// checks frame dropping

	char ui_framedrop( ui_t* ui )
	{
		struct timeval now;
		gettimeofday( &now , NULL );
		
		long secs = now.tv_sec - ui->lastrender.tv_sec;
		long usecs = now.tv_usec - ui->lastrender.tv_usec;
		
		// drop frames over 60 fps
		
		if ( secs == 0 && usecs < 14000 )
		{
			if ( ui->logs != NULL )
			{
				vector_adddata(
					ui->logs ,
					string_createfromformat(
						200 ,
						"\nFRAMEDROP draws %i quads %i masks %i updates %i " ,
						ui->counters->drawcount ,
						ui->counters->quadcount ,
						ui->counters->updatecount ) );
			}
			ui->framedrop = 1;
			return 1;
		}

		ui->framedrop = 0;
		ui->lastrender = now;
		return 0;
	}

	// render state

	void ui_render(ui_t* ui , char force )
	{
		if ( ui_framedrop( ui ) == 0 || force )
		{
			// reset counters
			
			ui->counters->quadcount = 0;
			ui->counters->drawcount = 0;
			ui->counters->updatecount = 0;
			
			// update matrixes and count all attached quads, execute final modifiers
			
			viewbase_finalize_geometry( ui->viewbase , ui->toolset , &(ui->counters->updatecount) , &(ui->counters->quadcount));
			
			// increase vertex buffer size if needed
			
			if ( ui->counters->quadcount > ui->maxquads )
			{
				ui->maxquads += 1000;
				ui->vertexbuffer = realloc( ui->vertexbuffer , sizeof( GLfloat ) * ui->maxquads * 4 * 4 );
			}
			
			// collect vertex data
			
			viewbase_collectvertexdata( ui->viewbase , ui->vertexbuffer , &(ui->counters->drawcount) );
			
			// draw vertexbuffer
			
			renderer_draw( ui->renderer , ui->tilemap , ui->vertexbuffer , ui->counters->drawcount );
			
			// flush buffer
			
			proxy_flushbuffer( );
		}
	}

	// process events after inputs and drawing

	void ui_handleevents( ui_t* ui )
	{
		for ( int index = 0 ; index < ui->events->length ; index++ )
		{
			object_t* eventobject = ui->events->data[ index ];
			ui->delegate->event( ui->delegate , eventobject->data );
			modifier_freeevent( eventobject );
		}
		
		if ( ui->events->length > 0 ) vector_reset( ui->events );
	}

	// process events after inputs and drawing

	void ui_handletimeds( ui_t* ui )
	{
		for ( int index = 0 ; index < ui->timeds->length ; index++ )
		{
			viewbase_addtimedview( ui->viewbase , ui->timeds->data[ index ] );
		}
		
		if ( ui->timeds->length > 0 ) vector_reset( ui->timeds );
	}

	// prints logs

	void ui_printlogs(ui_t* ui )
	{
		for ( int index = 0 ; index < ui->logs->length ; index++ )
		{
			string_t* string = ui->logs->data[ index ];
			char* bytes = string_bytes( string );
			printf( "\n%s" , bytes );
			free( bytes );
			string_free( string );
		}
		
		if ( ui->logs->length > 0 )	vector_reset( ui->logs );
	}