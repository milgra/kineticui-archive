//
//  generator_view.c
//  KineticUI
//
//  Created by Milan Toth on 02/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "generator_view.h"

	// creates view from parameters

	view_t* generator_view_view( map_t* map , float scale )
	{
		view_t* view = view_default( 0.0 , 0.0 , 0.0 , 0.0 );
		
		char* name = map_get( map , "view" );
		char* x = map_get( map , "x" );
		char* y = map_get( map , "y" );
		char* width = map_get( map , "width" );
		char* height = map_get( map , "height" );
		char* invisible = map_get( map , "invisible" );
		char* focuslevel = map_get( map , "focuslevel" );
		
		if ( name != NULL )	view_setname( view , name );
		if ( focuslevel != NULL ) view->focuslevel = atoi( focuslevel );
		if ( invisible != NULL ) view->invisible = atoi( invisible );
		if ( x != NULL & y != NULL ) view_setposition( view , vector2_default( atof( x ) * scale , atof( y ) * scale ) );
		if ( width != NULL & width != NULL ) view_setsize( view , vector2_default( atof( width ) * scale , atof( height ) * scale ) );

		return view;
	}

	// creates modifier from parameters, and adds

	modifier_t* generator_view_modifier( map_t* map , view_t* view , float scale )
	{
	
		modifier_t* modifier = NULL;
	
		char* name = map_get( map , "modifier" );
		char* size = map_get( map , "size" );
		char* text = map_get( map , "text" );
		char* image = map_get( map , "image" );
		char* color = map_get( map , "color" );
		char* press = map_get( map , "press" );
		char* action = map_get( map , "action" );
		char* speed = map_get( map , "speed" );
		char* release = map_get( map , "release" );
		char* toggle = map_get( map , "toggle" );
		char* rubber = map_get( map , "rubber" );
		char* enablex = map_get( map , "enablex" );
		char* enabley = map_get( map , "enabley" );
		char* linesize = map_get( map , "linesize" );
		char* linetype = map_get( map , "linetype" );
		char* animated = map_get( map , "animated" );
		char* minwidth = map_get( map , "minwidth" );
		char* minheight = map_get( map , "minheight" );
		char* inputtype = map_get( map , "inputtype" );
		char* aligntype = map_get( map , "aligntype" );
		char* textcolor = map_get( map , "textcolor" );
		char* codepoint = map_get( map , "codepoint" );
		char* marginsize = map_get( map , "marginsize" );
		char* touchaction = map_get( map , "touchaction" );
		char* enteraction = map_get( map , "enteraction" );
		char* openletters = map_get( map , "openletters" );
		char* resizeaction = map_get( map , "resizeaction" );
		char* animendaction = map_get( map , "animendaction" );
		char* enablescrollbarx = map_get( map , "enablescrollbarx" );
		char* enablescrollbary = map_get( map , "enablescrollbary" );

		if ( strcmp( name , "solidcolor" ) == 0 ) modifier = ( modifier_t* ) modifier_solidcolor_default( atof( color ) );
		else if ( strcmp( name , "blurcolor" ) == 0 ) modifier = ( modifier_t* ) modifier_blurcolor_default( );
		else if ( strcmp( name , "animator" ) == 0 ) modifier = ( modifier_t* ) modifier_animator_default( transformation_default() );
		else if ( strcmp( name , "itemlist" ) == 0 ) modifier = ( modifier_t* ) modifier_itemlist_default( );
		else if ( strcmp( name , "cropper" ) == 0 ) modifier = ( modifier_t* ) modifier_cropper_default( );
		else if ( strcmp( name , "hover" ) == 0 ) modifier = ( modifier_t* ) modifier_hover_default( );
		else if ( strcmp( name , "layout" ) == 0 )
		{
			char* options = map_get( map , "options" );
			uint8_t intopts = 0;
			if ( options[0] == '1' ) intopts |= 1 << 5;
			if ( options[1] == '1' ) intopts |= 1 << 4;
			if ( options[2] == '1' ) intopts |= 1 << 3;
			if ( options[3] == '1' ) intopts |= 1 << 2;
			if ( options[4] == '1' ) intopts |= 1 << 1;
			if ( options[5] == '1' ) intopts |= 1 << 0;
			modifier_t* lmodifier = modifier_layout_default( intopts );
            modifier_layout_t* data = lmodifier->data;
			if ( minwidth != NULL ) data->minextent.x = atof( minwidth ) * scale;
			if ( minheight != NULL ) data->minextent.y = atof( minheight ) * scale;
			modifier = ( modifier_t* ) lmodifier;
		}
		else if ( strcmp( name , "fader" ) == 0 )
		{
			modifier_t* fader = modifier_fader_default( );
			modifier_fader_t* data = fader->data;
			if ( speed != NULL ) data->speed = atof( speed );
			modifier = ( modifier_t* ) fader;
		}
		else if ( strcmp( name , "dragger" ) == 0 )
		{
			modifier_t* button = modifier_dragger_default( );
			modifier_dragger_t* data = button->data;
			if ( press != NULL ) data->pressaction = string_copycstring( press );
			if ( release != NULL ) data->releaseaction = string_copycstring( release );
			modifier = ( modifier_t* ) button;
		}
		else if ( strcmp( name , "button" ) == 0 )
		{
			modifier_t* button = modifier_button_default( );
			modifier_button_t* data = button->data;
			if ( action != NULL ) data->action = string_copycstring( action );
			if ( toggle != NULL ) data->isToggle = atoi( toggle );
			if ( animated != NULL )
			{
				data->animated = atoi( animated );
			}
			modifier = ( modifier_t* ) button;
		}
		else if ( strcmp( name , "scroller" ) == 0 )
		{
			if ( rubber == NULL ) rubber = "1";
			modifier_t* scroller = modifier_scroller_default( atoi( rubber ) );
            modifier_scroller_t* data = scroller->data;
			modifier = ( modifier_t* ) scroller;
			if ( enablex != NULL ) data->enable_x = atoi( enablex );
			if ( enabley != NULL ) data->enable_y = atoi( enabley );
			if ( enablescrollbarx != NULL ) data->enable_scrollbar_x = atoi( enablescrollbarx );
			if ( enablescrollbary != NULL ) data->enable_scrollbar_y = atoi( enablescrollbary );
		}
		else if ( strcmp( name , "imagearea" ) == 0 )
		{
			modifier_t* imagearea = modifier_imagearea_default( image );
			modifier = ( modifier_t* ) imagearea;
		}
		else if ( strcmp( name , "glypharea" ) == 0 )
		{
			uint32_t mycolor = math_read_hexastring( color );
			uint32_t mycodepoint = atoi( codepoint );
			float mysize = atof( size );
			modifier_t* glypharea = modifier_glypharea_default( mycolor , mycodepoint , mysize );
			modifier = ( modifier_t* ) glypharea;
		}
		else if ( strcmp( name , "textarea" ) == 0 )
		{
			textarea_aligntype atype = textarea_aligntype_justify;
			if ( strcmp( aligntype , "left" ) == 0 ) atype = textarea_aligntype_left;
			if ( strcmp( aligntype , "center" ) == 0 ) atype = textarea_aligntype_center;
			if ( strcmp( aligntype , "right" ) == 0 ) atype = textarea_aligntype_right;
			textarea_linetype ltype = textarea_linetype_multiline;
			if ( strcmp( linetype , "single" ) == 0 ) ltype = textarea_linetype_singleline;
			textarea_inputtype itype = textarea_inputtype_normal;
			if ( strcmp( inputtype , "input" ) == 0 ) itype = textarea_inputtype_input;
			textarea_initializer args;
			// text properties
			args.color = math_read_hexastring( textcolor );
			args.margin = atof( marginsize ) * scale;
			args.linetype = ltype;
			args.linesize = atof( linesize ) * scale;
			args.inputtype = itype;
			args.aligntype = atype;
			// starting string
			if ( text != NULL )
			{
				string_t* textstring = string_createfrombytes( text );
				string_replacecodepoints( textstring , '_' , ' ' );
				args.string = textstring;
			}
			else args.string = NULL;
			modifier_t* textarea = modifier_textarea_default( &args );
            modifier_textarea_t* data = textarea->data;
			if ( touchaction != NULL ) data->touchaction = string_copycstring( touchaction );
			if ( enteraction != NULL ) data->enteraction = string_copycstring( enteraction );
			if ( resizeaction != NULL ) data->resizeaction = string_copycstring( resizeaction );
			if ( animendaction != NULL ) data->animendaction = string_copycstring( animendaction );
			if ( openletters != NULL ) data->openletters = atoi( openletters );
			modifier = ( modifier_t* ) textarea;
			// cleanup
			if ( args.string != NULL ) string_free( args.string );
		}
		else
		{
			printf( "invalid modifier name : %s\n" , name );
		}
		return modifier;
	}

	// !!! tilemap, font kivezetese inicializatorokbol, state genkor menjen csak at

	map_t* generator_view_generate( char* text , float scale )
	{
		map_t*		result	   = map_create( 100 );
		string_t*	string     = string_createfrombytes( text );	// needs delete
		vector_t*	lines      = string_split( string , '\n' );		// needs delete
		view_t*		view	   = NULL;
		modifier_t*  modifier  = NULL;
		
		for ( int linei = 0 ; linei < lines->length ; linei++  )
		{
			string_t* line = lines->data[ linei ];
			vector_t* tokens = string_split( line , ' ' );			// needs delete
			map_t*	  linemap = map_create( 100 );					// needs delete
			
			char* key = NULL;
			char* value = NULL;
			
			for ( int tokeni = 0 ; tokeni < tokens->length ; tokeni++ )
			{
				string_t* token = tokens->data[ tokeni ];
				if ( key == NULL ) key = string_bytes( token );		// needs delete
				else
				{
					value = string_bytes( token );					// needs delete
					map_put( linemap , key , value );
					free( key );
					key = NULL;
				}
			}
			
			char* viewname = map_get( linemap , "view" );
			char* modifiername = map_get( linemap , "modifier" );
			
			if ( viewname != NULL )
			{
				// create and store view
				view = generator_view_view( linemap , scale );
				map_put( result , viewname , view );
			}
			if ( modifiername != NULL )
			{
				// create and store modifier
				modifier = generator_view_modifier( linemap , view , scale );
				if ( strcmp( modifiername , "blurcolor" ) == 0 ) view_addmodifier( view , modifier , 1 );
				else if ( strcmp( modifiername , "cropper" ) == 0 ) view_addmodifier( view , modifier , 1 );
				else view_addmodifier( view , modifier , 0 );
			}

			// cleanup

			vector_t* cstrings = map_values( linemap );
			for ( int index = 0 ; index < cstrings->length ; index++ ) free( cstrings->data[ index ] );
			vector_free( cstrings );
			map_free( linemap );
			string_freestrings( tokens );
			vector_free( tokens );
		}
		
		// cleanup
		
		string_free( string );
		string_freestrings( lines );
		vector_free( lines );
		
		return result;
	}

	int generator_view_count_tabs( string_t* text )
	{
		char* bytes = string_bytes( text );
		char* first = bytes;
		int count = 0;
		while ( *bytes++ == '\t' ) ++count;
		free( first );
		return count;
	}

	// generate view structure from descriptor

	view_t* generator_view_structure( map_t* viewmap , char* text )
	{
		view_t*		views[ 20 ] = { NULL };
		string_t*	string = string_createfrombytes( text );
		vector_t*	lines = string_split( string , '\n' );

		for ( int linei = 0 ; linei < lines->length ; linei++ )
		{
			string_t* line = lines->data[ linei ];
			
			// get depth
			int depth = generator_view_count_tabs( line );
			
			// get view
			vector_t* vector = string_split( line , '\t' );
			string_t* name = vector->data[ 0 ];
			char* namebytes = string_bytes( name );
			view_t* view = map_get( viewmap , namebytes );
			free( namebytes );

			// add to parent
			if ( depth > 0 ) view_addsubview( views[ depth - 1 ] , view );
			
			// store last for level
			views[ depth ] = view;
			
			// cleanup
			string_freestrings( vector );
			vector_free( vector );
		}
		
		// cleanup
		
		string_free( string );
		string_freestrings( lines );
		vector_free( lines );
	
		return views[0];
	}


	// creates view

	view_t* generator_view_default( char* viewdesc , char* structuredesc , float scale )
	{
		map_t* views = generator_view_generate( viewdesc , scale );
		view_t* view = generator_view_structure( views , structuredesc );
		map_free( views );
		return view;
	}
