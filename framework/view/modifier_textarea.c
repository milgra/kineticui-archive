//
//  modifier.c
//  KineticUI
//
//  Created by Milan Toth on 06/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "modifier_textarea.h"

	void modifier_textarea_free( modifier_t* modifier );
	void modifier_textarea_new( modifier_t* modifier , modifier_args_t* args );

    // creates new modifier

    modifier_t* modifier_textarea_default( textarea_initializer* initializer )
    {
        modifier_t* modifier = malloc( sizeof( modifier_t ) );
        modifier_textarea_t* data = malloc( sizeof( modifier_textarea_t ) );
		
		// modifier related
		modifier->type = "textarea";
        modifier->data = data;
		modifier->_new = modifier_textarea_new;
		modifier->_free = modifier_textarea_free;
		
		// text properties
		data->margin = initializer->margin;
		data->linetype = initializer->linetype;
        data->textsize = initializer->linesize;
		data->textcolor = initializer->color;
		data->aligntype = initializer->aligntype;
		data->inputtype = initializer->inputtype;
		
		// containers
		data->glyphviews = vector_create( );		// contains all glyphs
		data->dyingviews = vector_create( );		// contains dying glyphs ( under delete animation )
		data->stoppedviews = vector_create( );		// contains stopped animating views, for timing
		data->removedglyphs = vector_create( );		// contains deleted glyphs, will be reset align
		data->animatingviews = vector_create( );	// views under animation to be able to trigger them
		
		// modifier actions
		data->enteraction = NULL;
		data->touchaction = NULL;
		data->resizeaction = NULL;
		data->animendaction = NULL;

		// inner views
		data->cursor = NULL;
		data->contentview = NULL;
		
		// other stuff
		data->string = string_createfromstring( initializer->string );
		data->focused = 0;
		data->paragraph = paragraph_default( data->textsize , data->textsize , data->aligntype , data->linetype );
		data->cursorindex = 0;
		data->openletters = 0;
		
        return modifier;
    }

	// deletes modifier

	void modifier_textarea_free( modifier_t* modifier )
	{
        modifier_textarea_t* data = modifier->data;
		// !!! should emtpy and release container internals
		vector_free( data->glyphviews );
		vector_free( data->dyingviews );
		vector_free( data->stoppedviews );
		vector_free( data->removedglyphs );
		vector_free( data->animatingviews );
		
		// release actions
		if ( data->enteraction != NULL ) free( data->enteraction );
		if ( data->touchaction != NULL ) free( data->touchaction );
		if ( data->resizeaction != NULL ) free( data->resizeaction );
		
		// views will be deleted by view release chain
		// release other stuff
		if ( data->string != NULL ) string_free( data->string );
		paragraph_free( data->paragraph );
		
        free( modifier->data );
		free( modifier );
	}

	// creates content view. contentview is mainly for keeping the margins

	view_t* modifier_textarea_defaultcontentview( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		view_t* result = view_default( data->margin , -data->margin , args->views.view->width - 2 * data->margin , args->views.view->height + 2 * data->margin );
		view_setname( result , "content" );
		result->focuslevel = 0;
		result->invisible = 1;
		
		modifier_t* cropper = modifier_cropper_default( );
		modifier_t* layout = modifier_layout_default( ( 1 << 3 ) | ( 1 << 2 ) );

		view_addmodifier( result , cropper , 1 );
		view_addmodifier( result , layout , 0 );

//		// for debug
//		modifier_solidcolor_t* color = modifier_solidcolor_default( math_read_hexastring( "00000044" ) );
//		view_addmodifier( data->contentview , color );
		return result;
	}

	// creates content view. contentview is mainly for keeping the margins

	view_t* modifier_textarea_defaultcursorview( modifier_t* modifier , modifier_args_t* arguments )
	{
        modifier_textarea_t* data = modifier->data;

		view_t* result = view_default( 0.0 , 0.0 , arguments->toolset->settings->scale , -data->textsize );
		view_setname( result , "cursor" );
		result->focuslevel = 0;
		result->invisible = 1;

		modifier_t* hover = modifier_hover_default( );
		modifier_t* cropper = modifier_cropper_default( );
		modifier_t* animator = modifier_animator_default( transformation_default( ) );

		view_addmodifier( result , ( modifier_t* ) cropper , 1 );
		view_addmodifier( result , ( modifier_t* ) animator,  0 );
		view_addmodifier( result , ( modifier_t* ) hover , 0 );
		
		// generate state for hover

		view_new( &( ( modifier_args_t ) {
				.views      = { result			, NULL	  } ,
				.generator  = { &freechar       , 0		  } ,
				.command    = { "init"			, NULL	  } ,
				.toolset    = arguments->toolset		  } ) );

		return result;
	}

	// sets focus

    void modifier_textarea_setfocus( modifier_t* modifier , char focus )
	{
        modifier_textarea_t* data = modifier->data;

		if ( data->inputtype == textarea_inputtype_input )
		{
			if ( focus ) view_addsubview( data->contentview , data->cursor );
			else view_removesubview( data->contentview , data->cursor );
			
			data->focused = focus;
			// jump to animation start with cursor
			modifier_t* hover = view_getmodifier( data->cursor , "hover" );
            modifier_hover_t* data = hover->data;
			data->radius = 5.3;
		}
	}

	// removes all glyphs

	void modifier_textarea_remove_all_glyphs( modifier_t* modifier )
	{
        modifier_textarea_t* data = modifier->data;

		paragraph_remove_all_glyphs( data->paragraph );
		for ( int index = 0 ; index < data->glyphviews->length ; index++ )
		{
            view_t* view = data->glyphviews->data[ index ];
			view_removesubview( data->contentview , view );
			view_free( view );
		}
		vector_reset( data->glyphviews );
		vector_reset( data->animatingviews );
	}

	// removes last glyph

    void modifier_textarea_removeglyph( modifier_t* modifier , uint32_t codepoint , modifier_args_t* arguments )
    {
        modifier_textarea_t* data = modifier->data;

		if ( data->paragraph->letters->length > 0 )
		{
			int index = data->paragraph->letters->length - 1;
			paragraph_remove_glyph( data->paragraph , index , data->textsize , arguments->toolset->font );
			view_t* glyphview = data->glyphviews->data[ index ];
			vector_removedataatindex( data->glyphviews , index );
			vector_adddata( data->removedglyphs , glyphview );
		}
	}

    // add codepoint/backspace. creates glyph in paragraph and view in glyphs/contentview

    void modifier_textarea_addcodepoint( modifier_t* modifier , uint32_t codepoint , modifier_args_t* arguments )
    {
        modifier_textarea_t* data = modifier->data;

		// generate glyph id
		char* bytes = malloc( sizeof( char ) * 30 );
		snprintf( bytes , 30 , "%s-%.1f-%x-%i-%c" , "avenir" , data->textsize , data->textcolor , codepoint , codepoint );

		// add to paragraph, get glyph
		glyph_t* glyph = paragraph_add_glyph( data->paragraph , UINT64_MAX , codepoint , bytes , data->textsize , arguments->toolset->font );
		
		// create glyph view, it has to be at -1,1, to let align know that we are a new glyph
		view_t* glyphview = view_default( -1.0 , 0.0 , ceilf( glyph->width ) , -ceilf( glyph->height ) );
		view_setname( glyphview , bytes );
		free( bytes );
		glyphview->focuslevel = 0;

		// create modifiers
		modifier_t* modifier_cropper = modifier_cropper_default( glyphview );
		modifier_t* modifier_clipper = modifier_clipper_default( glyphview );
		modifier_t* modifier_animator = modifier_animator_default( transformation_default( ) );
		modifier_t* modifier_glypharea = modifier_glypharea_default( data->textcolor , codepoint , data->textsize );

		// order matters, glypharea has to be the first non-final modifier
		view_addmodifier( glyphview , ( modifier_t* ) modifier_cropper , 1 );
		view_addmodifier( glyphview , ( modifier_t* ) modifier_glypharea , 0 );
		view_addmodifier( glyphview , ( modifier_t* ) modifier_animator , 0 );
		view_addmodifier( glyphview , ( modifier_t* ) modifier_clipper , 0 );

		// init modifiers

		view_new( &( ( modifier_args_t ) {
			.views      = { glyphview						  , NULL } ,
			.generator  = { arguments->generator.changed_flag , 0    } ,
			.command    = { "init"							  , NULL } ,
			.toolset    = arguments->toolset						 } ) );
		
		// add to containers
		vector_adddata( data->glyphviews , glyphview );
		view_addsubview( data->contentview , glyphview );
		
		// set cursor index
		data->cursorindex = data->glyphviews->length;
		
		// printf( "\nadd glyph %i %f %f %f %f" , codepoint , glyph->width , glyph->height , ceilf( glyph->width ) , -ceilf( glyph->height ) );
    }

	// align glyphs without animation

	void modifier_textarea_alignglyphs( modifier_t* modifier )
	{
        modifier_textarea_t* data = modifier->data;

		// printf( "\nmodifier_textarea_alignglyphs %zx" , ( size_t ) modifier );
		for ( int index = 0 ; index < data->paragraph->letters->length ; index++ )
		{
            glyph_t* glyph = data->paragraph->letters->data[ index ];
            view_t* view = data->glyphviews->data[ index ];
			// set view's position to glyph's position
			view_setposition( view , vector2_default( glyph->x , -glyph->y ) );
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
		
		// handle removed glyphs
		if ( data->removedglyphs->length > 0 )
		{
			for ( int index = 0 ; index < data->removedglyphs->length ; index++ )
			{
				view_t* view = data->removedglyphs->data[ index ];
				view_removesubview( data->contentview , view );
				view_free( view );
			}
			vector_reset( data->removedglyphs );
		}
	}

	// resize content view to fit around changed paragraph

	void modifier_textarea_resize( modifier_t* modifier , view_t* view)
	{
        modifier_textarea_t* data = modifier->data;

		if ( data->linetype == textarea_linetype_singleline )
		{
			// resize content view
			float safemargin = 12.0;	// safety margin for cursor+last character when deleting, cropping would chop them out
			vector2_t viewsize = vector2_default( data->paragraph->width + safemargin , -data->paragraph->height );
			view_setsize( data->contentview , viewsize );
			// resize view
			vector2_t parentsize = view_getsize( view );
			viewsize = vector2_default( data->paragraph->width + 2 * data->margin , parentsize.y );
			view_setsize( view , viewsize );
		}
		else
		{
			// resize content view
			vector2_t viewsize = vector2_default( data->paragraph->width + 2.0 , -data->paragraph->height );
			view_setsize( data->contentview , viewsize );
			// resize view
			viewsize = vector2_default( data->paragraph->width + 2 * data->margin , -data->paragraph->height - 2 * data->margin );
			view_setsize( view , viewsize );
		}
	}

	// updates cursor

	void modifier_textarea_updatecursor( modifier_t* modifier )
	{
        modifier_textarea_t* data = modifier->data;

		vector2_t position = vector2_default( 0.0 , 0.0 );
		if ( data->cursorindex > 0 && data->glyphviews->length > 0 )
		{
			uint32_t glyphindex;
			if ( data->cursorindex >= data->glyphviews->length ) glyphindex = data->glyphviews->length - 1;
			else glyphindex = data->cursorindex;
			view_t* glyph = data->glyphviews->data[ glyphindex ];
			position = view_getposition( glyph );
			vector2_t size = view_getsize( glyph );
			position.x += size.x;
		}
		view_setposition( data->cursor , position );
	}

	string_t* modifier_textarea_gettext( modifier_t* modifier )
	{
        modifier_textarea_t* data = modifier->data;

		return paragraph_gettext( data->paragraph );
	}

	// generates state

	void modifier_textarea_new( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_textarea_t* data = modifier->data;

		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			// create and attach content view
			data->contentview = modifier_textarea_defaultcontentview( modifier , args );
			view_addsubview( args->views.view , data->contentview );
			
			// create cursor if needed
			if ( data->inputtype == textarea_inputtype_input )
			{
				data->cursor = modifier_textarea_defaultcursorview( modifier , args );
				modifier_textarea_updatecursor( modifier );
			}
			
			// resize paragraph to content view
			paragraph_resize( data->paragraph , args->views.view->width - 2 * data->margin );
			// !!! kell nekunk a minwidthes faszkodas?!?!
			data->paragraph->minwidth = data->paragraph->width;
			data->paragraph->minheight = data->paragraph->height;

			// create starting text if needed
			if ( data->string != NULL )
			{
				for ( int index = 0 ; index < data->string->length ; index++ ) modifier_textarea_addcodepoint( modifier , data->string->codepoints[ index ] , args );
				string_free( data->string );
				data->string = NULL;
				paragraph_align( data->paragraph );
				modifier_textarea_alignglyphs( modifier );
			}
		}
		else if ( strcmp( args->command.name , "settext" ) == 0  )
		{
			data->string = string_createfromstring( args->command.arguments );
			modifier_textarea_remove_all_glyphs( modifier );
			for ( int index = 0 ; index < data->string->length ; index++ ) modifier_textarea_addcodepoint( modifier , data->string->codepoints[ index ] , args );
			string_free( data->string );
			data->string = NULL;

			// resize contenview and paragraph
			paragraph_align( data->paragraph );
			modifier_textarea_resize( modifier , args->views.view );
			modifier_textarea_alignglyphs( modifier );
			if ( data->inputtype == textarea_inputtype_input ) modifier_textarea_updatecursor( modifier );

			*(args->generator.changed_flag) = 1;
			
			// if have to open letters, request timing
			if ( data->openletters == 1 )
			{
				data->openletters = 0;
				modifier_textarea_animator_alignglyphs_open( modifier , args );
				vector_adddata( args->toolset->timeds , args->views.view );
			}
		}
		else if ( strcmp( args->command.name , "layoutupdate" ) == 0  )
		{
			// layout changed, resize paragraph
			paragraph_resize( data->paragraph , args->views.view->width - 2 * data->margin );
			// resize contenview and paragraph
			paragraph_align( data->paragraph );
			// modify view
			modifier_textarea_resize( modifier , args->views.view );
			modifier_textarea_alignglyphs( modifier );
			if ( data->inputtype == textarea_inputtype_input ) modifier_textarea_updatecursor( modifier );
			
			// dispatch layout event
			*(args->generator.changed_flag) = 1;
			if ( data->resizeaction != NULL )
			{
				object_t* eventobject = modifier_defaultevent( "textlayout" , data->resizeaction , modifier , NULL );
				vector_adddata( args->toolset->events , eventobject );
			}
		}
		else if ( strcmp( args->command.name , "focus" ) == 0  )
		{
			modifier_textarea_setfocus( modifier , 1 );
			*(args->generator.changed_flag) = 1;

			vector_adddata( args->toolset->timeds , args->views.view );

			// if we are input area, we need keyboard also
			if ( data->inputtype == textarea_inputtype_input )
			{
				object_t* eventobject = modifier_defaultevent( "textfocused" , "showkeyboard" , modifier , NULL );
				vector_adddata( args->toolset->events , eventobject );
			}
		}
		else if ( strcmp( args->command.name , "unfocus" ) == 0  )
		{
			// modify view
			modifier_textarea_setfocus( modifier , 0 );
			*(args->generator.changed_flag) = 1;

			// hide keyboard if input
			if ( data->inputtype == textarea_inputtype_input )
			{
				object_t* eventobject = modifier_defaultevent( "textunfocused" , "hidekeyboard" , modifier , NULL );
				vector_adddata( args->toolset->events , eventobject );
			}
		}
		else if ( strcmp( args->command.name , "timer" ) == 0  )
		{
			// timing event
			modifier_textarea_animator_timer( modifier , args );
		}
		else if ( strcmp( args->command.name , "keypress" ) == 0  )
		{
			input_t* input = ( input_t* ) args->command.arguments;
			if ( input->key == kInputKeyTypeReturn )
			{
				if ( data->enteraction != NULL )
				{
					string_t* text = modifier_textarea_gettext( modifier );
					char* textc = string_bytes( text );
					object_t* eventobject = modifier_defaultevent( "textsent" , data->enteraction , modifier , textc );
					free( textc );
					string_free( text );
					vector_adddata( args->toolset->events , eventobject );
				}
			}
			else if ( input->key == kInputKeyTypeBackspace ) modifier_textarea_removeglyph( modifier , 0  , args );
			else modifier_textarea_addcodepoint( modifier , (input->characters)->codepoints[0] , args );

			*(args->generator.changed_flag) = 1;
			paragraph_align( data->paragraph );
			modifier_textarea_resize( modifier , args->views.view );
			modifier_textarea_animator_alignglyphs_slide( modifier , args );
			
			// request timing
			vector_adddata( args->toolset->timeds , args->views.view );

			// dispatch keypressed event
			object_t* eventobject = modifier_defaultevent( "textkeypressed" , data->resizeaction , modifier , NULL );
			vector_adddata( args->toolset->events , eventobject );
			
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0  )
		{
			if ( data->touchaction != NULL )
			{
				// dispatch tapped event
				object_t* eventobject = modifier_defaultevent( "tapped" , data->touchaction , modifier , NULL );
				vector_adddata( args->toolset->events , eventobject );
			}
		}
		else if ( strcmp( args->command.name , "refilltilemap" ) == 0  )
		{
			if ( data->inputtype == textarea_inputtype_input && !data->focused )
			{
				view_new( &( ( modifier_args_t ) {
					.views      = { data->cursor			 , NULL } ,
					.generator  = { args->generator.changed_flag , 0    } ,
					.command    = { "refilltilemap"				 , NULL } ,
					.toolset    = args->toolset							} ) );
			}
		}
	}