//
//  c36.h
//  c36
//
//  Created by mtoth on 09/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

    #ifndef __c36__c36__
    #define __c36__c36__

    #include <stdio.h>
    #include "../../framework/view/ui.h"
	#include "../../framework/view/modifier_blurcolor.h"
	#include "mqtt/mqtt.h"
	#include "settings.h"
	#include "filecontroller.h"
	#include "modifier_menuitem.h"
	#include "modifier_messageitem.h"
	#include "modifier_conversationitem.h"

	typedef struct _c36_t c36_t;
	struct _c36_t
	{
		void( *setup )( c36_t* delegate , char* path_resources , char* path_library );
		void( *timer )( c36_t* delegate );
		void( *event )( c36_t* delegate , modifierevent_t* event );
		ui_t* ui;

		char* conversation;
		mqtt_t* mqtt;
		view_t* main;
		vector_t* views;
		vector_t* events;
		settings_t* settings;
		filecontroller_t* filecontroller;
    };

    c36_t* c36_init( );
    void c36_setup( c36_t* delegate , char* path_resources , char* path_library );
    void c36_timer( c36_t* delegate );
    void c36_layout( c36_t* delegate );
    void c36_event( c36_t* delegate , modifierevent_t* event );

    #endif /* defined(__c36__c36__) */
