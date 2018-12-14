//
//  settings.c
//  c36
//
//  Created by Milan Toth on 23/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "settings.h"

	// creates working paths

	settings_t* settings_init( char* cpath_resources , char* cpath_library )
	{
		settings_t* result = malloc( sizeof( settings_t ) );
		
		string_t* path_library = string_createfrombytes( cpath_library );
		string_t* path_settings = string_createfromstring( path_library );
		string_addbytearray( path_settings , "/settings" );
		
		result->library = string_bytes( path_library );
		result->settings = string_bytes( path_settings );
		result->settingitems = vector_create( );
		result->settingactions = vector_create( );
		result->statusmessages = vector_create( );
		
		string_free( path_library );
		string_free( path_settings );
		
		// read settings
		
		result->defaults = file_readfile( result->settings );

		if ( result->defaults != NULL )
		{
			// try to read username
			type_container_t* namecont = map_get( result->defaults , "name" );
			if ( namecont != NULL )
			{
				// string_t* string = namecont->data;
			}
			// try to read saved conversations
			type_container_t* convcont = map_get( result->defaults , "conversations" );
			if ( convcont != NULL )
			{
				vector_t* vektor = convcont->data;
				for ( int index = 0 ; index < vektor->length ; index++ )
				{
					// type_container_t* c1 = vektor->data[ index ];
				}
			}
		}
		else
		{
			result->defaults = map_create( 50 );
			string_t* defconv = string_createfrombytes( "general" );
			string_t* message = string_createfrombytes( "Welcome to the factory chat" );
			settings_addconversation( result , defconv );
			settings_addmessage( result , defconv , message );
			string_free( defconv );
			string_free( message );
		}
		
		return result;
	}

	// sets username

	void settings_setusername( settings_t* settings , string_t* username )
	{
		// release old
		type_container_t* namecont = map_get( settings->defaults , "name" );
		if ( namecont != NULL )
		{
			string_free( namecont->data );
			namecont->data = string_createfromstring( username );
		}
		else map_put( settings->defaults , "name" , file_defaultcontainer( kTypeString , string_createfromstring( username ) ) );
		settings_save( settings );
	}

	// gets username

	string_t* settings_getusername( settings_t* settings )
	{
		type_container_t* namecont = map_get( settings->defaults , "name" );
		if ( namecont != NULL ) return namecont->data;
		return NULL;
	}

	// adds conversation

	void settings_addconversation( settings_t* settings , string_t* conversation )
	{
		type_container_t* convcont = map_get( settings->defaults , "conversations" );
		if ( convcont == NULL )
		{
			convcont = file_defaultcontainer( kTypeVector , vector_create( ) );
			map_put( settings->defaults , "conversations" , convcont );
		}
		vector_t* conversations = convcont->data;
		vector_adddata( conversations , file_defaultcontainer( kTypeString , string_createfromstring( conversation ) ) );
		settings_save( settings );
	}

	// removes conversation

	void settings_removeconversation( settings_t* settings , string_t* conversation )
	{
		type_container_t* convcont = map_get( settings->defaults , "conversations" );
		if ( convcont != NULL )
		{
			vector_t* conversations = convcont->data;
			for ( int index = 0 ; index < conversations->length ; index++ )
			{
				type_container_t* namecont = conversations->data[ index ];
				if ( string_compare( namecont->data , conversation ) == 0 )
				{
					vector_removedataatindex( conversations , index );
					string_free( namecont->data );
					free( namecont );
					return;
				}
			}
		}
		settings_save( settings );
	}

	// returns conversations

	vector_t* settings_getconversations( settings_t* settings )
	{
		type_container_t* convcont = map_get( settings->defaults , "conversations" );
		if ( convcont != NULL ) return convcont->data;
		return NULL;
	}

	void settings_addmessage( settings_t* settings , string_t* conversation , string_t* message )
	{
		string_t* idstring = string_createfrombytes( "messages" );
		string_addstring( idstring , conversation );
		char* idstringc = string_bytes( idstring );
		string_free( idstring );
		type_container_t* messagescont = map_get( settings->defaults , idstringc );
		if ( messagescont == NULL )
		{
			messagescont = file_defaultcontainer( kTypeVector , vector_create( ) );
			map_put( settings->defaults , idstringc , messagescont );
		}
		vector_t* messages = messagescont->data;
		if ( messages->length > 20 ) vector_removedataatindex( messages , 0 );
		vector_adddata( messages , file_defaultcontainer( kTypeString , string_createfromstring( message ) ) );
		settings_save( settings );
	}

	vector_t* settings_getmessages( settings_t* settings , string_t* conversation )
	{
		string_t* idstring = string_createfrombytes( "messages" );
		string_addstring( idstring , conversation );
		char* idstringc = string_bytes( idstring );
		string_free( idstring );
		type_container_t* messagescont = map_get( settings->defaults , idstringc );
		if ( messagescont != NULL ) return messagescont->data;
		return NULL;
	}

	// saves settings

	void settings_save( settings_t* settings )
	{
		file_writetofile( settings->defaults , settings->settings );
	}

	void settings_reset( settings_t* settings )
	{
		vector_reset( settings->statusmessages );
		vector_reset( settings->settingactions );
		vector_reset( settings->settingitems );
		map_free( settings->defaults );
		remove( settings->settings );
		settings->defaults = map_create( 50 );
	}

	// deletes working path structure

	void settings_free( settings_t* info )
	{
		vector_t* strings = map_values( info->defaults );
		string_freestrings( strings );
		vector_free( strings );
		map_free( info->defaults );
		
		string_freestrings( info->statusmessages );
		vector_free( info->statusmessages );
		vector_free( info->settingitems );
		
		free( info->library );
		free( info->settings );
		free( info );
	}
