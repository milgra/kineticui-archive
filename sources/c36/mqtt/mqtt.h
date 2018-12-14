//
//  mqtt.h
//  c36
//
//  Created by Milan Toth on 04/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef __c36__mqtt__
	#define __c36__mqtt__

	#include <time.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <netdb.h>
	#include <string.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <sys/socket.h>
	#include <netinet/in.h>

	#include "../../../framework/core/map.h"
	#include "../../../framework/core/slots.h"
    #include "../../../framework/core/types.h"
    #include "../../../framework/core/string.h"
	#include "../../../framework/core/vector.h"
	#include "MQTTPacket.h"

	typedef struct mqttevent_t mqttevent_t;
	struct mqttevent_t
	{
		char* name;
		char* topic;
		char* message;
	};

	typedef struct rawmessage_t rawmessge_t;
	struct rawmessage_t
	{
		unsigned char* bytes;
		uint32_t length;
	};


	typedef struct mqtt_t mqtt_t;
	struct mqtt_t
	{
		int port;
		char* host;
		char* user;
		char* pass;
		char* clientid;
		time_t ping;

		int socket;
		char alive;
		char connected;
		map_t* idtotopic;

		slots_t* sendslots;
		slots_t* readslots;
	};

	mqtt_t* mqtt_default( char* host , int port );
	void mqtt_free( mqtt_t* mqtt );
	void mqtt_freeevent( object_t* object );

	void mqtt_connect( mqtt_t* mqtt , char* clientid , char* user , char* password );
	void mqtt_disconnect( mqtt_t* mqtt , char* success );

	void mqtt_subscribe( mqtt_t* mqtt , char* topic );
	void mqtt_unsubscribe( mqtt_t* mqtt , char* topic );
	void mqtt_publish( mqtt_t* mqtt , char* topic , char* text );

	void mqtt_readloop( mqtt_t* mqtt );
	void mqtt_sendloop( mqtt_t* mqtt );

	void mqtt_send( mqtt_t* mqtt , string_t* string , char* success );
	void mqtt_receive( mqtt_t* mqtt , vector_t* events );

	#endif /* defined(__c36__mqtt__) */
