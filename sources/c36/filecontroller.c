//
//  filecontroller.c
//  KineticUI
//
//  Created by Milan Toth on 17/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "filecontroller.h"

	// creates filecontroller

	filecontroller_t* filecontroller_init( )
	{
		filecontroller_t* result = malloc( sizeof( filecontroller_t ) );

		result->alive = 1;
		result->readslots = slots_create( 50 );
		result->sendslots = slots_create( 50 );
	
		return result;
	}

	// deletes filecontroller

	void filecontroller_free( filecontroller_t* filecontroller )
	{
		filecontroller->alive = 0;
		slots_free( filecontroller->readslots );
		slots_free( filecontroller->sendslots );
		free( filecontroller );
	}

	// starts file thread

	void filecontroller_start( filecontroller_t* filecontroller )
	{
		pthread_t thread;
		pthread_create(&thread, NULL, (void*)filecontroller_work , filecontroller );
	}

	// work thread

	void filecontroller_work( filecontroller_t* filecontroller )
	{
		void* data = NULL;
		char success = 0;
		// event_t* event = NULL;
		filetask_t* task = NULL;
		// repeat while alive
		while ( filecontroller->alive == 1 )
		{
			slots_get( filecontroller->sendslots , &data );
			// check for new data
			if ( data != NULL )
			{
				task = data;
				if ( task->operation == kFileControllerOperationRead )
				{
//					string_t* result = string_create( );
					//string_addfilecontents( result , task->filename );
//					event = calloc( 1 , sizeof( event_t ) );
//					event->action = string_copycstring( task->action );
//					event->source = "filecontroller";
//					event->group = "file";
//					event->name = "fileread";
//					event->data = result;
				}
				else if ( task->operation == kFileControllerOperationWrite )
				{
//					string_t* string = task->buffer;
					//string_write_to_file( string , task->filename );
//					event = calloc( 1 , sizeof( event_t ) );
//					event->action = string_copycstring( task->action );
//					event->source = "filecontroller";
//					event->group = "file";
//					event->name = "filewrite";
				}
				else if ( task->operation == kFileControllerOperationAppend )
				{
//					string_t* string = task->buffer;
					//string_append_to_file( string , task->filename );
//					event = calloc( 1 , sizeof( event_t ) );
//					event->action = string_copycstring( task->action );
//					event->source = "filecontroller";
//					event->group = "file";
//					event->name = "fileappend";
				}
				// put event
				printf( "\nputting event" );
				// slots_put( filecontroller->readslots , event , &success );
				// if no more room in slots, wait for controller thread to get data out
				while ( success == 0 )
				{
					struct timespec time;
					time.tv_sec = 0;
					time.tv_nsec = 100000000L;
					nanosleep(&time , (struct timespec *)NULL);
					// slots_put( filecontroller->readslots , event , &success );
				}
			}
			// sleep for .1 seconds
			struct timespec time;
			time.tv_sec = 0;
			time.tv_nsec = 100000000L;
			nanosleep(&time , (struct timespec *)NULL);
		}
	}

	// add file io task

	void filecontroller_addtask( filecontroller_t* filecontroller , filetask_t* task , char* success )
	{
		slots_put( filecontroller->sendslots , task , success );
	}

	// receive events

	void filecontroller_getevents( filecontroller_t* filecontroller , vector_t* events )
	{
		//printf( "\nmqtt_receive" );
		object_t* eventobject;
		do
		{
			slots_get( filecontroller->readslots , (void** ) &eventobject );
			if ( eventobject != NULL )
			{
				printf( "\ngetting event" );
				vector_adddata( events , eventobject );
			}
		}
		while ( eventobject != NULL );
	}
