//
//  c36.c
//  c36
//
//  Created by mtoth on 09/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

    #include "c36.h"
	#include "headerui.h"
	#include "loginui.h"
	#include "mainui.h"
	#include "baseui.h"
	#include "settingsui.h"
	#include "statusui.h"
	#include "welcomeui.h"
	#include "messageitemui.h"
	#include "messagesui.h"
	#include "conversationitemui.h"
	#include "menuitemui.h"
	#include "newconversationui.h"

	// constructor

    c36_t* c36_init( )
    {
        c36_t* result = malloc( sizeof( c36_t ) );
		
        result->setup = c36_setup;
        result->timer = c36_timer;
        result->event = c36_event;
		
		result->views = vector_create( );
		result->events = vector_create( );
		
        return result;
    }

	// destructor

	void c36_free( c36_t* c36 )
	{
		// stop/delete mqtt
		mqtt_free( c36->mqtt );

		// delete views
		for ( int index = 0 ; index < c36->views->length ; index++ ) view_free( c36->views->data[ index ] );
		vector_free( c36->views );
		
		vector_free( c36->events );

		// stop/delete filecontroller
		filecontroller_free( c36->filecontroller );
	
		free( c36 );
	}

	// returns main view from main view list

	view_t* c36_getview( c36_t* controller , char* name )
	{
		for ( int index = 0 ; index < controller->views->length ; index++ )
		{
			view_t* view = controller->views->data[ index ];
			if ( strcmp( view->name , name ) == 0 ) return view;
		}
		return NULL;
	}

	// updates blur coordinates in settings for blur modifiers
	// !!! should be in ui

	void c36_updateblur( c36_t* c36 )
	{
		view_t* blur = c36_getview( c36 , "blur" );
		modifier_t* imagearea = view_getmodifier( blur , "imagearea" );
        modifier_imagearea_t* data = imagearea->data;
		blursettings_t* blursettings = c36->ui->settings->userdata;
		blursettings->blurcoords = data->coords;
	}

	// update view layouts
	// !!! should be in ui

	void c36_updatelayout( c36_t* c36 )
	{
		view_t* base = c36_getview( c36 , "base" );
		float size = fmaxf( c36->ui->settings->screensize.x , c36->ui->settings->screensize.y );
		float dx = c36->ui->settings->screensize.x - size;
		float dy = c36->ui->settings->screensize.y - size;
		
		blursettings_t* blursettings = c36->ui->settings->userdata;
		blursettings->blurorigin = vector2_default( -fabs( dx / 2 ) , -dy / 2 );
		
		view_setsize( base , vector2_default( size , -size ) );
		view_setposition( base , blursettings->blurorigin );
	}

	void c36_updateviewlayout( c36_t* c36 , view_t* view )
	{
		// new states based on layout input

		view_new( &( ( modifier_args_t ) {
			.views      = { view			, c36->ui->viewbase->view } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "layoutupdate"	, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		// scrollbar and textarea needs state update after their childs got resized

		view_new( &( ( modifier_args_t ) {
			.views      = { view			, c36->ui->viewbase->view } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "update"		, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );
	}

	// opens letters in text view

	void c36_openletters( c36_t* controller , char* name )
	{
		view_t* wanted = view_getsubview( controller->ui->viewbase->view , name );
		modifier_t* modifier = view_getmodifier( wanted , "textarea" );
        modifier_textarea_t* data = modifier->data;
		data->openletters = 1;
		
		view_new( &( ( modifier_args_t ) {
			.views      = { wanted			, NULL } ,
			.generator  = { &freechar		, 0    } ,
			.command    = { "update"		, NULL } ,
			.toolset    = controller->ui->toolset  } ) );

	}

	// set focus to views

	void c36_setfocus( c36_t* controller , char* nameA , char* nameB )
	{
		char changed = 0;
		vector_t* focuschain = vector_create();
		vector_adddata( focuschain , view_getsubview( controller->ui->viewbase->view , nameA ) );
		if ( nameB != NULL ) vector_adddata( focuschain , view_getsubview( controller->ui->viewbase->view , nameB ) );
		viewbase_setfocus( controller->ui->viewbase , focuschain , &changed , controller->ui->toolset );
		vector_free( focuschain );
	}

	// adds setting item

	void c36_adduisetting( c36_t* controller , string_t* itemtext , string_t* action )
	{
		view_t* view = c36_getview( controller , "settings.base" );
		// status view is visible
		if ( view != NULL )
		{
			view_t* scrollview = view_getsubview( view , "settings.messages" );
			view_t* itemlist = view_getsubview( view , "settings.messages.list" );
			modifier_t* itemmodifier = view_getmodifier( itemlist , "itemlist" );
            modifier_itemlist_t* data = itemmodifier->data;
			data->gap = 1.0;

			view_t* itemview = generator_view_default( menuitemui_desc , menuitemui_structure_desc , controller->ui->settings->scale );
			
			modifier_menuitem_t* modifier = modifier_menuitem_default( action );
			
			view_addmodifier( itemview , ( modifier_t* ) modifier , 0 );

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "init"			, NULL } ,
				.toolset    = controller->ui->toolset  } ) );

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "settext"		, itemtext } ,
				.toolset    = controller->ui->toolset  } ) );

			// add to messages list
			
			view_new( &( ( modifier_args_t ) {
				.views      = { itemlist		, NULL } ,
				.generator  = { &freechar		, 0    } ,
				.command    = { "additem"		, itemview } ,
				.toolset    = controller->ui->toolset  } ) );

			view_new( &( ( modifier_args_t ) {
				.views      = { scrollview		, NULL } ,
				.generator  = { &freechar		, 0    } ,
				.command    = { "update"		, NULL } ,
				.toolset    = controller->ui->toolset  } ) );
		}
	}

	// adds conversation item

	void c36_addconversation( c36_t* controller , char* text )
	{
		view_t* view = c36_getview( controller , "main.base" );
		// main view is visible
		if ( view != NULL )
		{
			string_t* string = string_createfrombytes( text );
			view_t* scrollview = view_getsubview( view , "main.conversations.base" );
			view_t* tableview = view_getsubview( view , "main.conversations.list" );
			
			view_t* itemview = generator_view_default( conversationitemui_desc , conversationitemui_structure_desc , controller->ui->settings->scale );
			modifier_conversationitem_t* modifier = modifier_conversationitem_default( );
			view_addmodifier( itemview , ( modifier_t* ) modifier , 0 );

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "init"			, NULL } ,
				.toolset    = controller->ui->toolset  } ) );

			// set label size

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "settext"		, string } ,
				.toolset    = controller->ui->toolset  } ) );
			
			// add to convesations list
			
			view_new( &( ( modifier_args_t ) {
				.views      = { tableview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "additem"		, itemview } ,
				.toolset    = controller->ui->toolset  } ) );

			view_new( &( ( modifier_args_t ) {
				.views      = { scrollview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "update"		, NULL } ,
				.toolset    = controller->ui->toolset  } ) );
			
			string_free( string );
		}
	}

	// adds messages to message list view

	void c36_adduimessage( c36_t* c36 , string_t* message , char updatelist )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		if ( mainview != NULL )
		{
			view_t* listbase = view_getsubview( mainview , "messages.base" );
			view_t* itemlist = view_getsubview( mainview , "messages.list" );
			modifier_t* itemmodifier = view_getmodifier( itemlist , "itemlist" );
            modifier_itemlist_t* data = itemmodifier->data;

			map_t* itemviews = generator_view_generate( messageitemui_desc , c36->ui->settings->scale );
			view_t* itemview = generator_view_structure( itemviews , messageitemui_structure_desc );
			
			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "init"			, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, itemlist } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "layoutupdate"	, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );

			uint64_t color = math_read_hexastring( "0x00000000" );
			if ( data->items->length % 3 == 0 ) color = math_read_hexastring( "0x00550009" );
			else if ( data->items->length % 3 == 1 ) color = math_read_hexastring( "0x55000009" );

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, itemlist } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "setcolor"		, ( void* ) color } ,
				.toolset    = c36->ui->toolset		   } ) );
			
			// generate item modifier, add to item ( should be done by viewgenerator )
			
			modifier_messageitem_t* modifier = modifier_messageitem_default( );
			
			view_addmodifier( itemview , ( modifier_t* ) modifier , 0 );
			
			// set label size

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, itemlist } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "settext"		, message } ,
				.toolset    = c36->ui->toolset		   } ) );
			
			// add to messages list
			
			view_new( &( ( modifier_args_t ) {
				.views      = { itemlist		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "additem"		, itemview } ,
				.toolset    = c36->ui->toolset		   } ) );
			
			if ( updatelist == 1 )
			{

				view_new( &( ( modifier_args_t ) {
					.views      = { listbase		, NULL } ,
					.generator  = { &freechar		, 1    } ,
					.command    = { "update"		, NULL } ,
					.toolset    = c36->ui->toolset		   } ) );
			}
		}
	}
	
	// adds status message items

	void c36_addstatusmessage( c36_t* c36 , string_t* message )
	{
		view_t* view = c36_getview( c36 , "status.base" );
		// status view is visible
		if ( view != NULL )
		{
			view_t* scrollview	= view_getsubview( view , "status.messages" );
			view_t* itemlist	= view_getsubview( view , "status.messages.list" );
			view_t* itemview	= generator_view_default( messageitemui_desc , messageitemui_structure_desc , c36->ui->settings->scale );
			
			modifier_messageitem_t* modifier = modifier_messageitem_default( );
			view_addmodifier( itemview , ( modifier_t* ) modifier , 0 );

			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "init"			, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );
			
			view_new( &( ( modifier_args_t ) {
				.views      = { itemview		, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "settext"		, message } ,
				.toolset    = c36->ui->toolset		   } ) );

			// add to status list

			view_new( &( ( modifier_args_t ) {
				.views      = { itemlist		, NULL } ,
				.generator  = { &freechar		, 0    } ,
				.command    = { "additem"		, itemview } ,
				.toolset    = c36->ui->toolset		   } ) );

			view_new( &( ( modifier_args_t ) {
				.views      = { scrollview		, NULL } ,
				.generator  = { &freechar		, 0    } ,
				.command    = { "update"		, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );

		}
	}

	// adds status message to status collector, shows message in ui if needed

	void c36_addstatus( c36_t* c36 , char* text )
	{
		struct tm *now = NULL;
		time_t time_value = time( NULL );
		now = localtime(&time_value);     /* Get time and date structure */
		char time[12];
		sprintf( time , "%.2d:%.2d:%.2d - " , now->tm_hour , now->tm_min , now->tm_sec );
		string_t* textstring = string_createfrombytes( time );
		string_addbytearray( textstring , text );
		vector_adddata( c36->settings->statusmessages , textstring );
		c36_addstatusmessage( c36 , textstring );
	}

	// adds status message to status collector, shows message in ui if needed

	void c36_addsubscribestatus( c36_t* c36 , char* text )
	{
		char* newtext = calloc( 100 , sizeof( char ) );
		sprintf( newtext , "Subscribed to topic %s" , text );
		c36_addstatus( c36 , newtext );
		free( newtext );
	}

	// adds message

	void c36_addmessage( c36_t* c36 , char* topic , char* message )
	{
		c36_addstatus( c36 , "MQTT : Message received" );
		printf( "\nmessage %s topic %s" , message , topic );
		string_t* topics = string_createfrombytes( topic );
		string_t* messages = string_createfrombytes( message );
		c36_adduimessage( c36 , messages , 1 );
		settings_addmessage( c36->settings , topics , messages );
		string_free(topics);
		string_free(messages);
		ui_render( c36->ui , 0 );
	}

	// opens login view

	void c36_showloginview( c36_t* c36 )
	{
		// login view, first view
		view_t* loginview = c36_getview( c36 , "login.base" );
		
		c36_updateviewlayout( c36 , loginview );
		
        viewbase_addviewatindex( c36->ui->viewbase , loginview , 1 );
		
		c36_openletters( c36 , "login.text" );
		c36_setfocus( c36 , "login.inputlabel" , "login.inputtext" );
		c36->main = loginview;
	}

	// sets title

	void c36_settitle( c36_t* c36 , string_t* title )
	{
		title = string_createfrombytes( "Kinetic UI" );
		view_t* headerview = c36_getview( c36 , "header.base" );
		view_t* textview = view_getsubview( headerview , "header.text" );

		view_new( &( ( modifier_args_t ) {
			.views      = { textview		, NULL } ,
			.generator  = { &freechar		, 0    } ,
			.command    = { "settext"		, title } ,
			.toolset    = c36->ui->toolset		   } ) );
	}

	// sets up ui

	void c36_setup_ui( c36_t* c36 )
	{
        printf( "\nc36_setup_ui %lli" , ( uint64_t ) c36 );
		// base view, always visible
		view_t* backview = generator_view_default(
			baseui_desc ,
			baseui_structure_desc ,
			c36->ui->settings->scale );
		
		// blur view, never attached, only for blur views
		view_t* blurview = generator_view_default(
			blurui_desc ,
			blurui_structure_desc ,
			c36->ui->settings->scale  );
		
		// header view, always visible
		view_t* headerview = generator_view_default(
			headerui_desc ,
			headerui_structure_desc ,
			c36->ui->settings->scale );
		
		// login view
		view_t* loginview = generator_view_default(
			loginui_desc ,
			loginui_structure_desc ,
			c36->ui->settings->scale );
		
		// main view
		view_t* mainview = generator_view_default(
			mainui_desc ,
			mainui_structure_desc ,
			c36->ui->settings->scale );
		
		// shows texture, for debug
		view_t* textureview = view_default(
			0.0 ,
			0.0 ,
			900.0 ,
			-800.0 );
		
		// status view
		view_t* statusview = generator_view_default(
			statusui_desc ,
			statusui_structure_desc ,
			c36->ui->settings->scale );
		
		// settings view
		view_t* settingsview = generator_view_default(
			settingsui_desc ,
			settingsui_structure_desc ,
			c36->ui->settings->scale  );

		if ( c36->ui->toolset->logs != NULL )
		{
			vector_adddata
			(
				c36->ui->toolset->logs ,
				string_createfrombytes(	"\nviewbase_initui" )
			);
		}

		// store views
		vector_adddata( c36->views , backview );
		vector_adddata( c36->views , blurview );
		vector_adddata( c36->views , headerview );
		vector_adddata( c36->views , loginview );
		vector_adddata( c36->views , mainview );
		vector_adddata( c36->views , statusview );
		vector_adddata( c36->views , settingsview );
		vector_adddata( c36->views , textureview );
		
		// init views
		
		view_new( &( ( modifier_args_t ) {
			.views      = { backview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { blurview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { headerview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { loginview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { mainview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { statusview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { settingsview	, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { textureview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );
		
		// now that the images are loaded, we can assign the blur text coords to settings, and align the background
		
		// create blur settings, put it in uisettings
		blursettings_t* blursettings = malloc( sizeof( blursettings_t ) );
		blursettings->blurorigin = vector2_default( 0.0 , 0.0 );
		c36->ui->settings->userdata = blursettings;

		c36_updateblur( c36 );
		c36_updatelayout( c36 );
		c36_updateviewlayout( c36 , headerview );
		
		// init ui

		// add to base view
        viewbase_addview( c36->ui->viewbase , backview );
        viewbase_addview( c36->ui->viewbase , headerview );
		// viewbase_addview( controller->ui->viewbase , textureview );	textureview->focuslevel = 0; // for debug
		
		// hide expose button for login view
		
		view_t* exposebutton = view_getsubview( headerview , "header.exposebutton" );
		view_t* exposeimage = view_getsubview( headerview , "header.exposebutton.image" );
		exposeimage->invisible = 1;
		exposebutton->focuslevel = 0;
	}

	// selects conversation

	void c36_selectconversation( c36_t* c36 , string_t* string )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* tableview = view_getsubview( mainview , "main.conversations.list" );
		modifier_t* listmodifier = (void*)view_getmodifier( tableview , "itemlist" );
        modifier_itemlist_t* data = listmodifier->data;
		for ( int index = 0 ; index < data->items->length ; index++ )
		{
			view_t* item = data->items->data[ index ];

			view_new( &( ( modifier_args_t ) {
				.views      = { item			, NULL } ,
				.generator  = { &freechar		, 0    } ,
				.command    = { "select"		, string } ,
				.toolset    = c36->ui->toolset		   } ) );
		}
	}
	
	// shows new conversation popup

	void c36_shownewconversationpopup( c36_t* c36 )
	{
		view_t* convview = generator_view_default( newconversationui_desc , newconversationui_structure_desc , c36->ui->settings->scale );
		
		view_new( &( ( modifier_args_t ) {
			.views      = { convview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );
		
		vector_adddata( c36->views , convview );

		view_t* mainview = c36_getview( c36 , "main.base" );
		view_addsubview( mainview , convview );

		c36_setfocus( c36 , "newconversation.inputlabel" , "newconversation.inputtext" );

        view_new( &( ( modifier_args_t ) {
                .views		= { convview , NULL } ,
                .generator	= { &freechar , 0 } ,
				.command	= { "setphase" , &( ( animation_phase_t ) {
					.transformation	 = {
						.translation = { c36->ui->settings->screensize.x / 2.0 , -c36->ui->settings->screensize.y / 2.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 0.0 , 0.0 } } } ) } ,
                .toolset	= c36->ui->toolset } ) );
		
        view_new( &( ( modifier_args_t ) {
                .views		= { convview , NULL } ,
                .generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 20 ,
					.easing = 1 ,
					.action = NULL ,
					.force = 0 } ) } ,
                .toolset	= c36->ui->toolset } ) );
		
		viewbase_addtimedview( c36->ui->viewbase , convview );

	}

	// shows messages list for conversation

	void c36_showmessagelist( c36_t* c36 , string_t* string )
	{
		
		view_t* mainview = c36_getview( c36 , "main.base" );
		if ( mainview != NULL )
		{
			view_t* messagesview = view_getsubview( mainview , "main.messages" );
			view_t* oldmessagesview = view_getsubview( mainview , "messages.base" );

			view_t* messagelistview = generator_view_default( messagesui_desc, messagesui_structure_desc, c36->ui->settings->scale );

			view_new( &( ( modifier_args_t ) {
				.views      = { messagelistview	, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "init"			, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );

			view_new( &( ( modifier_args_t ) {
				.views      = { messagelistview	, NULL } ,
				.generator  = { &freechar		, 1    } ,
				.command    = { "layoutupdate"	, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );

			if ( oldmessagesview != NULL )
			{
				// remove old
				view_t* oldlist = view_getsubview( oldmessagesview , "messages.list" );
				
				view_setname( oldlist , "oldmessages.list" );
				view_setname( oldmessagesview , "oldmessages.base" );
				
				view_new( &( ( modifier_args_t ) {
						.views		= { oldmessagesview , NULL } ,
						.generator	= { &freechar , 0 } ,
						.command	= { "addphase" , &( ( animation_phase_t ) {
							.transformation  = {
								.translation = { -c36->ui->settings->screensize.x , 0.0 } ,
								.rotation	 = { 0.0 , 0.0 } ,
								.scale		 = { 1.0 , 1.0 } } ,
							.steps = 20 ,
							.easing = 1 ,
							.action = "removeoldmessages" ,
							.force = 0 } ) } ,
						.toolset	= c36->ui->toolset } ) );
				
				viewbase_addtimedview( c36->ui->viewbase , oldmessagesview );
			}

			view_new( &( ( modifier_args_t ) {
					.views		= { messagelistview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "setphase" , &( ( animation_phase_t ) {
						.transformation	 = {
							.translation = { c36->ui->settings->screensize.x , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } } ) } ,
					.toolset	= c36->ui->toolset } ) );
			
			view_new( &( ( modifier_args_t ) {
					.views		= { messagelistview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 0.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = NULL ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );

			viewbase_addtimedview( c36->ui->viewbase , messagelistview );
			
			view_addsubview( messagesview , messagelistview );

			// show messages
			vector_t* messages = settings_getmessages( c36->settings , string );
			if ( messages != NULL )
			{
				for ( int index = 0 ; index < messages->length ; index++ )
				{
					type_container_t* container = messages->data[ index ];
					c36_adduimessage( c36 , container->data , 1 );
				}
			}
			
			// set title
			c36_settitle( c36 , string );
		}
	}

	// open conversation or new conversation

	void c36_openconversation( c36_t* c36 , char* conversation )
	{
		char* newconv = "New conversation";
		if ( strcmp( conversation , newconv ) == 0 ) c36_shownewconversationpopup( c36 );
		else
		{
			if ( strcmp( conversation , c36->conversation ) != 0 )
			{
				string_t* convstring = string_createfrombytes( conversation );
				c36_showmessagelist( c36 , convstring );
				c36_selectconversation( c36 , convstring );
				c36->conversation = string_bytes( convstring );
				string_free( convstring );
			}
		}
	}

	// adds setting

	void c36_addsetting( c36_t* c36 , char* text , char* action )
	{
		string_t* textstring = string_createfrombytes( text );
		string_t* actionstring = string_createfrombytes( action );
		vector_adddata( c36->settings->settingitems , textstring );
		vector_adddata( c36->settings->settingactions , actionstring );
		c36_adduisetting( c36 , textstring , actionstring );
	}

	// loads settings

	void c36_loadsettings( c36_t* c36 )
	{
		// set settings
		c36_addsetting( c36 , "Select background" , "selectbackground" );
		c36_addsetting( c36 , "Delete current account" , "deleteaccount" );
		c36_addsetting( c36 , "Link account to other accounts" , "linkaccount" );

		// open last state
		// string_t* username = settings_getusername( c36->settings );
		c36_showloginview( c36 );
		// if ( username == NULL )ui_showloginview( controller );
		// elseui_showmainview( controller );
		
		// subscribe to all previous conversations

		vector_t* conversations = settings_getconversations( c36->settings );
		if ( conversations != NULL )
		{
			for ( int index = 0 ; index < conversations->length ; index++ )
			{
				type_container_t* namecont = conversations->data[ index ];
				char* conversationcstr = string_bytes( namecont->data );
				mqtt_subscribe( c36->mqtt , conversationcstr );
				free( conversationcstr );
			}
		}
		
		// initial setup

		c36->conversation = "Milan Toth Korodi Tamas";

		char success = 0;
		filetask_t* filetask = malloc( sizeof( filetask_t ) );
		filetask->operation = kFileControllerOperationRead;
		filetask->filename = string_createfrombytes( c36->settings->settings );
		filetask->action = "showconversations";
		filecontroller_addtask( c36->filecontroller , filetask , &success );
	}

	// setup c36

    void c36_setup( c36_t* c36 , char* path_resources , char* path_library )
    {
        printf( "\nc36_setup %lli" , ( uint64_t ) c36 );
		
		c36_setup_ui( c36 );
		
		// mqtt client for communication, starts its own threads for reading/sending data
		c36->mqtt = mqtt_default( "kotyofromhell.com" , 1883 );
		c36->main = NULL;							// main view
		c36->settings = settings_init( path_resources , path_library );
		// file read/write controler, starts its own threads
		c36->filecontroller = filecontroller_init( );
		
		// load settings
		c36_loadsettings( c36 );

		#ifndef BRY
		mqtt_connect( c36->mqtt , "subscriber" , "Hiroo" , "Onoda" );
		filecontroller_start( c36->filecontroller );
		#endif
		
		// set status
		c36_addstatus( c36 , "Application started" );
		
    }

	// shows status view

	void c36_showmainview( c36_t* c36 )
	{
		view_t* loginview = view_getsubview( c36->ui->viewbase->view , "login.base" );
		if ( loginview != NULL )
		{
			view_new( &( ( modifier_args_t ) {
					.views		= { loginview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { -c36->ui->settings->screensize.x - 30.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = "removeloginview" ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );

			viewbase_addtimedview( c36->ui->viewbase , loginview );
		}

		view_t* mainview = c36_getview( c36, "main.base" );

		// update layout of view before adding it to view list
		c36_updateviewlayout( c36 , mainview );
		
		view_new( &( ( modifier_args_t ) {
				.views		= { mainview , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "setphase" , &( ( animation_phase_t ) {
					.transformation	 = {
						.translation = { c36->ui->settings->screensize.x , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } } ) } ,
				.toolset	= c36->ui->toolset } ) );
		
		view_new( &( ( modifier_args_t ) {
				.views		= { mainview , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { -30.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 20 ,
					.easing = 1 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= c36->ui->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { mainview , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { 0.0 , 0.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { 1.0 , 1.0 } } ,
					.steps = 10 ,
					.easing = 1 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= c36->ui->toolset } ) );

		viewbase_addtimedview( c36->ui->viewbase , mainview );
		viewbase_addviewatindex( c36->ui->viewbase , mainview , 1 );
		
		// add new conversation conversation item
		c36_addconversation( c36 , "New conversation" );
		
		// add conversations from settings
		vector_t* conversations = settings_getconversations( c36->settings );
		if ( conversations != NULL )
		{
			for ( int index = 0 ; index < conversations->length ; index++ )
			{
				type_container_t* namecont = conversations->data[ index ];
				char* conversationcstr = string_bytes( namecont->data );
				c36_addconversation( c36 , conversationcstr );
				
				if ( index == 0 ) c36_openconversation( c36 , conversationcstr );
				free( conversationcstr );
			}
		}

		// show welcome popup

		view_t* welcomeview = generator_view_default( welcomeui_desc , welcomeui_structure_desc , c36->ui->settings->scale );

		view_new( &( ( modifier_args_t ) {
			.views      = { welcomeview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "init"			, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_new( &( ( modifier_args_t ) {
			.views      = { welcomeview		, c36->ui->viewbase->view } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "layoutupdate"	, NULL } ,
			.toolset    = c36->ui->toolset		   } ) );

		view_addsubview( mainview , welcomeview );

		// show welcome text with nickname with animation
		
		string_t* nickname = settings_getusername( c36->settings );
		if ( nickname == NULL )	nickname = string_createfrombytes( "default" );
		char welcometext[ 200 ] = { 0 };
		char* nicknamecstr = string_bytes( nickname );
		snprintf( welcometext , 200 , "You will be identified on this device as %s . Your friends will find you with this identifier. You can link your other accounts together under Settings." , nicknamecstr );
		string_t* string = string_createfrombytes( welcometext );
		
		free( nicknamecstr );

		view_t* textview = view_getsubview( welcomeview , "welcome.text" );

		view_new( &( ( modifier_args_t ) {
			.views      = { textview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "settext"		, string } ,
			.toolset    = c36->ui->toolset		   } ) );

		c36_openletters( c36 , "welcome.text" );

		string_free( string );

		view_t* headerview = c36_getview( c36 , "header.base" );
		view_t* exposebutton = view_getsubview( headerview , "header.exposebutton" );
		view_t* exposeimage = view_getsubview( headerview , "header.exposebutton.image" );
		exposeimage->invisible = 0;
		exposebutton->focuslevel = 2;
		
		// c36_setfocus( controller , "main.composer.label" , "main.composer.text" );
	}

	// zoom out visible views

	void c36_expose( c36_t* c36 )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* statusview = c36_getview( c36 , "status.base" );
		view_t* settingsview = c36_getview( c36 , "settings.base" );
		
		// update layout
		c36_updateviewlayout( c36 , mainview );
		c36_updateviewlayout( c36 , statusview );
		c36_updateviewlayout( c36 , settingsview );
		
		viewbase_addviewatindex( c36->ui->viewbase , mainview , 1 );
		viewbase_addviewatindex( c36->ui->viewbase , statusview , 1 );
		viewbase_addviewatindex( c36->ui->viewbase , settingsview , 1 );
		
		float newwidth = c36->ui->settings->screensize.x * 0.25;
		float newheight = c36->ui->settings->screensize.y * 0.25;
		float half = c36->ui->settings->screensize.x / 2.0;
		float halfy = c36->ui->settings->screensize.y / 2.0;
		float third = half / 2.0;
		float twothird = half + third;
		
		view_new( &( ( modifier_args_t ) {
				.views		= { mainview , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { third - newwidth / 2.0  - c36->ui->settings->screensize.x * 0.05, -halfy + newheight / 2.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { .25 , .25 } } ,
					.steps = 20 ,
					.easing = 1 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= c36->ui->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { statusview , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { half - newwidth / 2.0 , -halfy + newheight / 2.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { .25 , .25 } } ,
					.steps = 20 ,
					.easing = 1 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= c36->ui->toolset } ) );

		view_new( &( ( modifier_args_t ) {
				.views		= { settingsview , NULL } ,
				.generator	= { &freechar , 0 } ,
				.command	= { "addphase" , &( ( animation_phase_t ) {
					.transformation  = {
						.translation = { twothird - newwidth / 2.0 + c36->ui->settings->screensize.x * 0.05 , -halfy + newheight / 2.0 } ,
						.rotation	 = { 0.0 , 0.0 } ,
						.scale		 = { .25 , .25 } } ,
					.steps = 20 ,
					.easing = 1 ,
					.action = NULL ,
					.force = 0 } ) } ,
				.toolset	= c36->ui->toolset } ) );
		
		view_t* overlay = view_getsubview( mainview , "overlay" );
		overlay->focuslevel = 2;

		overlay = view_getsubview( statusview , "overlay" );
		overlay->focuslevel = 2;
		
		overlay = view_getsubview( settingsview , "overlay" );
		overlay->focuslevel = 2;

		viewbase_addtimedview( c36->ui->viewbase , settingsview );
		viewbase_addtimedview( c36->ui->viewbase , statusview );
		viewbase_addtimedview( c36->ui->viewbase , mainview );
	}

	// zoom on selected view

	void c36_zoomto( c36_t* c36 , char* viewname )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* statusview = c36_getview( c36 , "status.base" );
		view_t* settingsview = c36_getview( c36 , "settings.base" );
		
		float half = c36->ui->settings->screensize.x / 2.0;
		float halfy = c36->ui->settings->screensize.y / 2.0;
		float newheight = c36->ui->settings->screensize.y * 0.25;
		
		if ( strcmp( viewname , "main" ) == 0 )
		{
			view_new( &( ( modifier_args_t ) {
					.views		= { mainview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 0.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = NULL ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );

			viewbase_removeview( c36->ui->viewbase , mainview );
			viewbase_addviewatindex( c36->ui->viewbase , mainview , 3 );
		}
		else
		{
			view_new( &( ( modifier_args_t ) {
					.views		= { mainview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { -half , -halfy + newheight / 2.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { .25 , .25 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = "removemainview" ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );
		}
		
		if ( strcmp( viewname , "status" ) == 0 )
		{
			viewbase_removeview( c36->ui->viewbase , statusview );
			viewbase_addviewatindex( c36->ui->viewbase , statusview , 3 );

			view_new( &( ( modifier_args_t ) {
					.views		= { statusview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 0.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = NULL ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );
		}
		else
		{
			if ( strcmp( viewname , "settings" ) != 0 )
			{
				view_new( &( ( modifier_args_t ) {
						.views		= { statusview , NULL } ,
						.generator	= { &freechar , 0 } ,
						.command	= { "addphase" , &( ( animation_phase_t ) {
							.transformation  = {
								.translation = { 2 * half , -halfy + newheight / 2.0 } ,
								.rotation	 = { 0.0 , 0.0 } ,
								.scale		 = { .25 , .25 } } ,
							.steps = 20 ,
							.easing = 1 ,
							.action = "removestatusview" ,
							.force = 0 } ) } ,
						.toolset	= c36->ui->toolset } ) );
			}
			else
			{
				view_new( &( ( modifier_args_t ) {
						.views		= { statusview , NULL } ,
						.generator	= { &freechar , 0 } ,
						.command	= { "addphase" , &( ( animation_phase_t ) {
							.transformation  = {
								.translation = { - half , -halfy + newheight / 2.0 } ,
								.rotation	 = { 0.0 , 0.0 } ,
								.scale		 = { .25 , .25 } } ,
							.steps = 20 ,
							.easing = 1 ,
							.action = "removestatusview" ,
							.force = 0 } ) } ,
						.toolset	= c36->ui->toolset } ) );
			}
		}
		
		if ( strcmp( viewname , "settings" ) == 0 )
		{
			viewbase_removeview( c36->ui->viewbase , settingsview );
			viewbase_addviewatindex( c36->ui->viewbase , settingsview , 3 );

			view_new( &( ( modifier_args_t ) {
					.views		= { settingsview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 0.0 , 0.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { 1.0 , 1.0 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = "removestatusview" ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );
		}
		else
		{
			view_new( &( ( modifier_args_t ) {
					.views		= { settingsview , NULL } ,
					.generator	= { &freechar , 0 } ,
					.command	= { "addphase" , &( ( animation_phase_t ) {
						.transformation  = {
							.translation = { 2 * half + half , -halfy + newheight / 2.0 } ,
							.rotation	 = { 0.0 , 0.0 } ,
							.scale		 = { .25 , .25 } } ,
						.steps = 20 ,
						.easing = 1 ,
						.action = "removesettingsview" ,
						.force = 0 } ) } ,
					.toolset	= c36->ui->toolset } ) );
		}

		view_t* loginoverlay = view_getsubview( mainview , "overlay" );
		loginoverlay->focuslevel = 0;

		view_t* statusoverlay = view_getsubview( statusview , "overlay" );
		statusoverlay->focuslevel = 0;

		view_t* settingsoverlay = view_getsubview( settingsview , "overlay" );
		settingsoverlay->focuslevel = 0;

		viewbase_addtimedview( c36->ui->viewbase , mainview );
		viewbase_addtimedview( c36->ui->viewbase , statusview );
		viewbase_addtimedview( c36->ui->viewbase , settingsview );
	}

	// resizes composer

	void c36_updatecomposer( c36_t* c36 )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* composer = view_getsubview( mainview , "main.composer" );
		view_t* text = view_getsubview( mainview , "main.composer.text" );
		
		vector2_t size = view_getsize( text );
		vector2_t position = vector2_default( 0.0 , -c36->ui->settings->screensize.y - size.y );
		
		view_setposition( composer , position );
	}

	// removes view

	void c36_removeview( c36_t* c36 , char* name )
	{
		view_t* view = c36_getview( c36 , name );
		if ( view != NULL )
		{
			viewbase_removeview( c36->ui->viewbase , view );
			ui_render( c36->ui , 0 );
		}
	}

	void c36_removewelcome( c36_t* c36  )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* welcome = view_getsubview( mainview , "welcome.base" );
		if ( welcome != NULL )
		{
			view_removesubview( mainview , welcome );
			ui_render( c36->ui , 0 );
		}
		c36_setfocus( c36 , "main.composer.label" , "main.composer.text" );
	}

	void c36_removenewconv( c36_t* c36  )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* newconv = view_getsubview( mainview , "newconversation.base" );
		if ( newconv != NULL )
		{
			view_removesubview( mainview , newconv );
			ui_render( c36->ui , 0 );
		}
	}

	void c36_removeoldmessages( c36_t* c36 )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* messagesview = view_getsubview( mainview , "main.messages" );
		view_t* oldmessagesview = view_getsubview( mainview , "oldmessages.base" );
		view_removesubview( messagesview , oldmessagesview );
		
		view_free( oldmessagesview );
		
		ui_render( c36->ui , 0 );
	}

	// loign user

	void c36_loginuser( c36_t* c36 )
	{
		// login, getting text from login.body.inputtext
		view_t* login = c36_getview( c36 , "login.base" );
		view_t* text = view_getsubview( login , "login.inputtext" );
		modifier_t* textarea = view_getmodifier( text , "textarea" );
		string_t* username = modifier_textarea_gettext( textarea );
		settings_setusername( c36->settings , username );
		string_free( username );
		c36_showmainview( c36 );
	}

	// open conversation or new conversation

	void c36_defaultconversation( c36_t* c36 )
	{
		view_t* login = c36_getview( c36 , "newconversation.base" );
		view_t* text = view_getsubview( login , "newconversation.inputtext" );
		modifier_t* textarea = view_getmodifier( text , "textarea" );
		string_t* conversation = modifier_textarea_gettext( textarea );
		c36_removenewconv( c36 );

		settings_addconversation( c36->settings , conversation );
		settings_addmessage( c36->settings , conversation , string_createfrombytes( "Welcome!" ) );

		char* topic = string_bytes( conversation );
		mqtt_subscribe( c36->mqtt , topic );
		c36_addconversation( c36 , topic );
		c36->conversation = topic;
		free( topic );
		
//		controller_adduimessage( controller , string_createfrombytes( "i" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "Muhahahaha mihihihih beszarsz geci mi a fasz van geci beszrsz geci pooo pooo tag alles ist schon du bist schon ich bin schon meine pooo pooo ist schon alles is schon fuhcs du hast die ganz gestohlen gib sie wieder her sieben monat ohne fragen mit dem switzgerei" ) );
//		controller_adduimessage( controller , string_createfrombytes( "i" ) );

	}

	// send new message to topic

	void c36_sendmessage( c36_t* c36 )
	{
		view_t* mainview = c36_getview( c36 , "main.base" );
		view_t* textview = view_getsubview( mainview , "main.composer.text" );
		modifier_t* textarea = view_getmodifier( textview , "textarea" );
		string_t* string = modifier_textarea_gettext( textarea );

		// empty composer

		view_new( &( ( modifier_args_t ) {
			.views      = { textview		, NULL } ,
			.generator  = { &freechar		, 1    } ,
			.command    = { "settext"		, string_create() } ,
			.toolset    = c36->ui->toolset		   } ) );

		c36_updatecomposer( c36 );
		
		string_t* message = string_create( );
		string_addstring( message , settings_getusername( c36->settings ) );
		string_addbytearray( message , " : " );
		string_addstring( message , string );

		char* text = string_bytes( message );
		mqtt_publish( c36->mqtt , c36->conversation , text );
		free( text );
		
		string_free( string );
		string_free( message );
		
		c36_addstatus( c36 , "Sending message" );
	}

	void c36_showconversations( c36_t* c36 )
	{

	}

	void c36_freeaccount( c36_t* c36 )
	{
		// !!! unsusbcribe from topics
		settings_reset( c36->settings );
		c36_showloginview( c36 );
		c36_removeview( c36 , "settings.base" );
	}

	void c36_timer( c36_t* c36 )
	{
		mqtt_receive( c36->mqtt , c36->events );

		for ( int index = 0 ; index < c36->events->length ; index++ )
		{
			object_t* eventobject = c36->events->data[ index ];
			
			if ( strcmp( eventobject->type , "mqttevent" ) == 0 )
			{
				mqttevent_t* event = eventobject->data;
				// analyze event
				if ( strcmp( event->name , "connected" ) == 0 )
					c36_addstatus( c36 , "Connected to server" ); else
					
				if ( strcmp( event->name , "disconnected" ) == 0 )
					c36_addstatus( c36 , "Disconnected from server" ); else

				if ( strcmp( event->name , "subscribed" ) == 0 )
					c36_addsubscribestatus( c36 , event->topic ); else

				if ( strcmp( event->name , "pong" ) == 0 )
					c36_addstatus( c36 , "Ping response from server" ); else

				if ( strcmp( event->name , "message" ) == 0 )
					c36_addmessage( c36 , event->topic , event->message );
				
				// cleanup
				mqtt_freeevent( eventobject );
            }
		}
		
		if ( c36->events->length > 0 ) vector_reset( c36->events );
		
		// receive file events
		// filecontroller_getevents( controller->filecontroller , controller->ui->toolset->events );
	}

	void c36_layout( c36_t* c36 )
	{
		c36_updatelayout( c36 );
		c36_updatecomposer( c36 );
	}

	// resets tilemap

	void c36_resettilemap( c36_t* c36 )
	{
		tilemap_reset( c36->ui->tilemap );
        char changed = 0;
		for ( int index = 0 ; index < c36->views->length ; index++ )
		{
			view_new( &( ( modifier_args_t ) {
				.views      = { c36->views->data[index]	, c36->ui->viewbase->view } ,
				.generator  = { &changed		, 1    } ,
				.command    = { "refilltilemap"	, NULL } ,
				.toolset    = c36->ui->toolset		   } ) );
		}
		// reset blur coords in settings
		c36_updateblur( c36 );
		if ( changed == 1 ) ui_render( c36->ui , 0 );
	}

	// ui event from ui controller

    void c36_event( c36_t* c36 , modifierevent_t* event )
    {
		if ( event->todo != NULL )
		{
			if ( strcmp( event->todo , "resize" ) == 0 )
				c36_layout( c36 );

			else if ( strcmp( event->todo , "expose" ) == 0 )
				c36_expose( c36 );

			else if ( strcmp( event->todo , "showmain" ) == 0 )
				c36_showmainview( c36 );

			else if ( strcmp( event->todo , "removemainview" ) == 0 )
				c36_removeview( c36 , "main.base" );

			else if ( strcmp( event->todo , "removestatusview" ) == 0 )
				c36_removeview( c36 , "status.base" );
				
			else if ( strcmp( event->todo , "removesettingsview" ) == 0 )
				c36_removeview( c36 , "settings.base" );
				
			else if ( strcmp( event->todo , "removeloginview" ) == 0 )
				c36_removeview( c36 , "login.base" );

			else if ( strcmp( event->todo , "loginbuttonpressed" ) == 0 )
				c36_loginuser( c36 );
				
			else if ( strcmp( event->todo , "loginenterpressed" ) == 0 )
				c36_loginuser( c36 );
			
			else if ( strcmp( event->todo , "removemenuview" ) == 0 )
				c36_removeview( c36 , "menu.base" );

			else if ( strcmp( event->todo , "closewindow" ) == 0 )
				proxy_closewindow();

			else if ( strcmp( event->todo , "maximizewindow" ) == 0 )
				proxy_maximizewindow();

			else if ( strcmp( event->todo , "dragwindow" ) == 0 )
				proxy_dragwindow();

			else if ( strcmp( event->todo , "dropwindow" ) == 0 )
				proxy_dropwindow();

			else if ( strcmp( event->todo , "openconversation" ) == 0 )
				c36_openconversation( c36 , event->data );

			else if ( strcmp( event->todo , "createconversation" ) == 0 )
				c36_defaultconversation( c36 );

			else if ( strcmp( event->todo , "sendmessage" ) == 0 )
				c36_sendmessage( c36 );

			else if ( strcmp( event->todo , "removeoldmessages" ) == 0 )
				c36_removeoldmessages( c36 );

			else if ( strcmp( event->todo , "resizecomposer" ) == 0 )
				c36_updatecomposer( c36 );

			else if ( strcmp( event->todo , "deleteaccount" ) == 0 )
				c36_freeaccount( c36 );

			else if ( strcmp( event->todo , "showkeyboard" ) == 0 )
				proxy_showkeyboard( );

			else if ( strcmp( event->todo , "hidekeyboard" ) == 0 )
				proxy_hidekeyboard( );

			else if ( strcmp( event->todo , "removenewconversation" ) == 0 )
				c36_removeview( c36 , "newconversation.base" );

			else if ( strcmp( event->todo , "removewelcome" ) == 0 )
				c36_removewelcome( c36 );

			else if ( strcmp( event->todo , "removenewconv" ) == 0 )
				c36_removenewconv( c36 );

			else if ( strcmp( event->todo , "zoomtologin" ) == 0 )
				c36_zoomto( c36 , "login" );

			else if ( strcmp( event->todo , "zoomtomain" ) == 0 )
				c36_zoomto( c36 , "main" );

			else if ( strcmp( event->todo , "zoomtostatus" ) == 0 )
				c36_zoomto( c36 , "status" );

			else if ( strcmp( event->todo , "zoomtosettings" ) == 0 )
				c36_zoomto( c36 , "settings" );
			
			else if ( strcmp( event->todo , "resettilemap" ) == 0 )
				c36_resettilemap( c36 );
		}
    }

