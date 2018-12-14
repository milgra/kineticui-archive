//
//  filecontroller.h
//  KineticUI
//
//  Created by Milan Toth on 17/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__filecontroller__
	#define __KineticUI__filecontroller__

	#include <time.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <pthread.h>
	#include "../../framework/core/slots.h"
    #include "../../framework/core/string.h"
    #include "../../framework/core/vector.h"
    #include "../../framework/core/types.h"

	#define kFileControllerOperationRead 0
	#define kFileControllerOperationWrite 1
	#define kFileControllerOperationAppend 2

	typedef struct _filetask_t filetask_t;
	struct _filetask_t
	{
		int operation;	// read, write, append
		char* action;
		string_t* buffer;
		string_t* filename;
	};

	typedef struct _filecontroller_t filecontroller_t;
	struct _filecontroller_t
	{
		char alive;
		slots_t* sendslots;
		slots_t* readslots;
	};

	filecontroller_t* filecontroller_init( );
	void filecontroller_free( filecontroller_t* filecontroller );
	void filecontroller_start( filecontroller_t* filecontroller );
	void filecontroller_work( );
	void filecontroller_addtask( filecontroller_t* filecontroller , filetask_t* task , char* success );
	void filecontroller_getevents( filecontroller_t* filecontroller , vector_t* events );

	#endif /* defined(__KineticUI__filecontroller__) */
