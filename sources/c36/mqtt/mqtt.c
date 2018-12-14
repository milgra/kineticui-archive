//
//  mqtt.c
//  c36
//
//  Created by Milan Toth on 04/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "mqtt.h"

	#define MQTT_BUFFER_SIZE 10000

	rawmessge_t* mqtt_packet_connect( );
	rawmessge_t* mqtt_packet_disconnect( );
	rawmessge_t* mqtt_packet_pingreq( );
	rawmessge_t* mqtt_packet_subscribe( char* topic , unsigned short* messageid );
	rawmessge_t* mqtt_packet_unsubscribe( char* topic );
	rawmessge_t* mqtt_packet_publish( char* message , char* topic );
	int mqtt_readdata( unsigned char* buffer , int size );
	void mqtt_senddata( mqtt_t* mqtt , unsigned char* buffer , int buffersize );
	void mqtt_readloop( mqtt_t* mqtt );
	void mqtt_sendloop( mqtt_t* mqtt );

	int mqttsocket = 0;

	// creates mqtt modifier

	mqtt_t* mqtt_default( char* host , int port )
	{
		printf( "\nmqtt_default %s %i" , host , port );
		mqtt_t* result = malloc( sizeof( mqtt_t ) );

		result->port = port;
		result->host = host;
		result->user = NULL;
		result->pass = NULL;
		result->ping = 0;
		result->alive = 0;
		result->clientid = NULL;
		result->connected = 0;
		result->idtotopic = map_create( 50 );
		result->readslots = slots_create( 50 );
		result->sendslots = slots_create( 50 );

		return result;
	}

	// deletes modifier

	void mqtt_free( mqtt_t* mqtt )
	{
		printf( "\nmqtt_free" );
		slots_free( mqtt->readslots );
		slots_free( mqtt->sendslots );
		if ( mqtt->user != NULL ) free( mqtt->user );
		if ( mqtt->pass != NULL ) free( mqtt->pass );
		if ( mqtt->clientid != NULL ) free( mqtt->clientid );
		// stops threads
		mqtt->alive = 0;
		// closes socket
		close( mqttsocket );
		free( mqtt );
	}

	// creates object containing an mqtt event

	object_t* mqtt_defaultevent( char* name , char* topic , char* message )
	{
		mqttevent_t* event = malloc( sizeof( mqttevent_t ) );
		event->name = string_copycstring( name );
		event->topic = string_copycstring( topic );
		event->message = string_copycstring( message );
		object_t* result = object_create( "mqttevent" , event );
		return result;
	}

	// deletes object containing an mqtt event

	void mqtt_freeevent( object_t* object )
	{
		mqttevent_t* event = object->data;
		if ( event->name != NULL ) free( event->name );
		if ( event->topic != NULL ) free( event->topic );
		if ( event->message != NULL ) free( event->message );
		free( event );
		object_free( object );
	}

	// connects or reconnects to remote mqtt server, starts listener and sender threads

	void mqtt_connect( mqtt_t* mqtt , char* clientid , char* user , char* pass )
	{
		printf( "\nmqtt_connect %s %s %s" , clientid , user , pass );
		mqtt->user = string_copycstring( user );
		mqtt->pass = string_copycstring( pass );
		mqtt->clientid = string_copycstring( clientid );
		pthread_t sendthread;
		pthread_create(&sendthread, NULL, (void*)mqtt_sendloop , mqtt );
	}

	// disconnects from remote server

	void mqtt_disconnect( mqtt_t* mqtt , char* success )
	{
		printf( "\nmqtt_disconnect" );
		// stops threads
		mqtt->alive = 0;
		mqtt->connected = 0;
		// send disconnect messages
		rawmessge_t* message = mqtt_packet_disconnect( );
		mqtt_senddata( mqtt , message->bytes , message->length );
		// closes socket
		close( mqttsocket );
	}

	// subscribes for topic

	void mqtt_subscribe( mqtt_t* mqtt , char* topic )
	{
		printf( "\nmqtt_subscribe %s" , topic );
		char success = 0;
		unsigned short id;
		rawmessge_t* message = mqtt_packet_subscribe( topic , &id );
		
		// store until suback message
		char* key = calloc( 10 , sizeof( char ) );
		sprintf( key , "%i" , id );
		string_t* value = string_createfrombytes( topic );
		map_put( mqtt->idtotopic , key , value );
		free( key );
		
		if ( message != NULL ) slots_put( mqtt->sendslots , message , &success );
	}

	// unsubscribes from topic

	void mqtt_unsubscribe( mqtt_t* mqtt , char* topic )
	{
		printf( "\nmqtt_unsubscribe %s" , topic );
		char success = 0;
		rawmessge_t* message = mqtt_packet_unsubscribe( topic );
		if ( message != NULL ) slots_put( mqtt->sendslots , message , &success );
	}

	// publishes message

	void mqtt_publish( mqtt_t* mqtt , char* topic , char* text )
	{
		printf( "\nmqtt_publish %s" , topic );
		char success = 0;
		rawmessge_t* message = mqtt_packet_publish( text , topic );
		if ( message != NULL ) slots_put( mqtt->sendslots , message , &success );
		// !!! store messages if slot is full
	}

	// gets socket address

	struct sockaddr_in mqtt_getsocketaddress( char* host , int port , char* success )
	{
		printf( "\nmqtt_getsocketaddress %s %i" , host , port );
		struct sockaddr_in client;
		struct hostent* hostentity = gethostbyname( host );
		if ( hostentity != NULL )
		{
			*success = 1;
			memset( &client , 0 , sizeof( client ) );
			memcpy( ( char* ) &client.sin_addr , hostentity->h_addr , hostentity->h_length );
			client.sin_family = hostentity->h_addrtype;
			client.sin_port = htons( port );
		}
		else *success = 0;
		return client;
	}

	// creates connect packet, returns raw bytes

	rawmessge_t* mqtt_packet_connect( mqtt_t* mqtt )
	{
		rawmessge_t* result = malloc( sizeof( rawmessge_t ) );
		result->bytes = malloc( sizeof( unsigned char ) * 1024 );
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
		data.clientID.cstring = mqtt->clientid;
		data.keepAliveInterval = 20;
		data.username.cstring = mqtt->user;
		data.password.cstring = mqtt->pass;
		data.cleansession = 1;
		// create connection packet
		result->length = MQTTSerialize_connect( result->bytes , 1024 , &data );
		if ( result->length == MQTTPACKET_BUFFER_TOO_SHORT ) return NULL;
		else return result;
	}

	// creates connect packet, returns raw bytes

	rawmessge_t* mqtt_packet_disconnect( )
	{
		rawmessge_t* result = malloc( sizeof( rawmessge_t ) );
		result->bytes = malloc( sizeof( unsigned char ) * 1024 );
		// create connection packet
		result->length = MQTTSerialize_disconnect( result->bytes , MQTT_BUFFER_SIZE );
		if ( result->length == MQTTPACKET_BUFFER_TOO_SHORT ) return NULL;
		else return result;
	}

	// creates ping packet, returns raw bytes

	rawmessge_t* mqtt_packet_pingreq( )
	{
		rawmessge_t* result = malloc( sizeof( rawmessge_t ) );
		result->bytes = malloc( sizeof( unsigned char ) * 1024 );
		result->length = MQTTSerialize_pingreq( result->bytes , 1024 );
		if ( result->length == MQTTPACKET_BUFFER_TOO_SHORT ) return NULL;
		else return result;
	}

	// creates subscribe packet, returns raw bytes

	rawmessge_t* mqtt_packet_subscribe( char* topic , unsigned short* messageid )
	{
		rawmessge_t* result = malloc( sizeof( rawmessge_t ) );
		result->bytes = malloc( sizeof( unsigned char ) * 1024 );
		MQTTString topic_string = MQTTString_initializer;
		topic_string.cstring = topic;
		static unsigned short message_id = 1;
		*messageid = message_id;
		int request_qos = 0;
		result->length = MQTTSerialize_subscribe( result->bytes , MQTT_BUFFER_SIZE , 0 , message_id++ , 1 , &topic_string , &request_qos );
		if ( result->length == MQTTPACKET_BUFFER_TOO_SHORT ) return NULL;
		else return result;
	}

	// creates subscribe packet, returns raw bytes

	rawmessge_t* mqtt_packet_unsubscribe( char* topic )
	{
		rawmessge_t* result = malloc( sizeof( rawmessge_t ) );
		result->bytes = malloc( sizeof( unsigned char ) * 1024 );
		MQTTString topic_string = MQTTString_initializer;
		topic_string.cstring = topic;
		MQTTString topicFilters[1];
		topicFilters[0]=topic_string;
		unsigned short message_id = 1;
		result->length = MQTTSerialize_unsubscribe( result->bytes , MQTT_BUFFER_SIZE , 0 , message_id , 0 , topicFilters );
		if ( result->length == MQTTPACKET_BUFFER_TOO_SHORT ) return NULL;
		else return result;
	}

	// creates publish packet

	rawmessge_t* mqtt_packet_publish( char* message , char* topic )
	{
		rawmessge_t* result = malloc( sizeof( rawmessge_t ) );
		result->bytes = malloc( sizeof( unsigned char ) * 1024 );
		MQTTString topicString = MQTTString_initializer;
		topicString.cstring = topic;
		char* payload = message;
		int payloadlen = strlen( payload );
		result->length = MQTTSerialize_publish( result->bytes , MQTT_BUFFER_SIZE , 0 , 0 , 0 , 0 , topicString , (unsigned char*)payload , payloadlen );
		if ( result->length == MQTTPACKET_BUFFER_TOO_SHORT ) return NULL;
		else return result;
	}

	// reads data from socket

	int mqtt_readdata( unsigned char* buffer , int size )
	{
		// printf( "\nmqtt_readdata" );
		int length = (int)recv( mqttsocket , buffer , size , 0 );
		return length;
	}

	// sends data to socket

	void mqtt_senddata( mqtt_t* mqtt , unsigned char* buffer , int buffersize )
	{
		// printf( "\nmqtt_senddata %i" , buffersize );
        while ( buffersize > 0 )
        {
            int writesize = ( int ) send( mqttsocket , buffer , buffersize , 0 );
            if ( writesize > -1 )
            {
				buffer += writesize;
				buffersize -= writesize;
            }
			else
			{
				printf( "\nmqttsubscriber error : socket closed!!!" );
				mqtt->alive = 0;
				mqtt->connected = 0;
				buffersize = 0;
				close( mqttsocket );
				// !!! SEND MESSAGE TO CONTROLLER AND POSSIBLY RECONNECT
				object_t* eventobject = mqtt_defaultevent( "disconnected" , NULL , NULL );
				char success;
				slots_put( mqtt->readslots , eventobject , &success );
			}
        }
	}

	// readloop

	void mqtt_readloop( mqtt_t* mqtt )
	{
		unsigned char* readbuffer = malloc( sizeof( unsigned char ) * MQTT_BUFFER_SIZE );
		while ( mqtt->alive )
		{
			memset( readbuffer , 0 , sizeof( unsigned char ) * MQTT_BUFFER_SIZE );
			int result = MQTTPacket_read( readbuffer , MQTT_BUFFER_SIZE , mqtt_readdata );

			if ( result == PUBLISH )
			{
				// message arrived
				int payloadsize;
				int granted_qos;
				unsigned short message_id = 1;
				unsigned char dupflag;
				unsigned char retained;
				unsigned char* payload_in;
				MQTTString received_topic;
				result = MQTTDeserialize_publish( &dupflag , &granted_qos , &retained , &message_id , &received_topic , &payload_in , &payloadsize , readbuffer , MQTT_BUFFER_SIZE );
				if ( result == 1 )
				{
					char success = 0;
					// create event for message
					char* topic = malloc( sizeof( char ) * received_topic.lenstring.len );
					memcpy( topic , received_topic.lenstring.data , received_topic.lenstring.len );
					// printf("\nmqtt publish packet dupflag %i granted qos %i retained %i message id %i topic %s payload %s payload size %i \n", dupflag , granted_qos , retained , message_id , topic , payload_in , payloadsize );
					object_t* eventobject = mqtt_defaultevent( "message" , topic , ( char* ) payload_in );
					free( topic );

					// put event to read slots
					slots_put( mqtt->readslots , eventobject , &success );
					// if no more room in slots, wait for controller thread to get data out
					while ( success == 0 )
					{
						struct timespec time;
						time.tv_sec = 0;
						time.tv_nsec = 100000000L;
						nanosleep(&time , (struct timespec *)NULL);
						slots_put( mqtt->readslots , eventobject , &success );
					}
					// send puback - not needed since QoS is 0
					// result = MQTTSerialize_puback( buffer , buffersize , message_id );
					// mqttsubscriber_senddata( subscriber , buffer , packetsize );
					// put message in message chain
				}
				else printf("\nmqtt publish packet error %i\n" , result );
			}
			else if ( result == CONNACK )
			{
				// connection acknowledged
				unsigned char session_present;
				unsigned char connack_result;
				result = MQTTDeserialize_connack( &session_present, &connack_result , readbuffer , MQTT_BUFFER_SIZE );
				if ( result == 1 && connack_result == 0 )
				{
					// printf("\nmqtt connack packet session present %i connact result %i\n", session_present , connack_result );
					mqtt->connected = 1;
					char success = 0;
					object_t* eventobject = mqtt_defaultevent( "connected" , NULL , NULL );
					slots_put( mqtt->readslots , eventobject , &success );
				}
				else printf( "\nmqtt connack packet error %i" , result );
			}
			else if ( result == SUBACK )
			{
				// subscribe acknowledged
				unsigned short packetid;
				int granted_qos;
				int count;
				result = MQTTDeserialize_suback(&packetid, 1, &count, &granted_qos, readbuffer, MQTT_BUFFER_SIZE );
				if ( result == 1 )
				{
					// printf("\nmqtt suback packet session packetid %i count %i granted qos %i\n", packetid , count , granted_qos );

					// store until suback message
					char* key = calloc( 10 , sizeof( char ) );
					char* topic = NULL;
					sprintf( key , "%i" , packetid );
					string_t* value = map_get( mqtt->idtotopic , key );
					if ( value != NULL )
					{
						map_put( mqtt->idtotopic , key , NULL );
						topic = string_bytes( value );
						string_free( value );
					}
					char success = 0;
					object_t* eventobject = mqtt_defaultevent( "subscribed" , topic , NULL );
					slots_put( mqtt->readslots , eventobject , &success );
					free( key );
					if ( topic != NULL ) free( topic );
				}
				else printf( "\nmqtt suback packet error %i" , result );
			}
			else if ( result == PINGRESP )
			{
				printf( "\nmqtt pingresp packet" );
				// ping acknowledged
				char success = 0;
				object_t* eventobject = mqtt_defaultevent( "pong" , NULL , NULL );
				slots_put( mqtt->readslots , eventobject , &success );
			}
		}
		free( readbuffer );
	}

	// sendloop

	void mqtt_sendloop( mqtt_t* mqtt )
	{
		int value = 1;
		char success = 0;
		// create socket
		mqttsocket = socket( PF_INET , SOCK_STREAM , IPPROTO_TCP );
        #ifdef BRY
        setsockopt( mqttsocket , SOL_SOCKET , MSG_NOSIGNAL , &value , sizeof( value ) );
        #else
        setsockopt( mqttsocket , SOL_SOCKET , SO_NOSIGPIPE , &value , sizeof( value ) );
        #endif
		setsockopt( mqttsocket , SOL_SOCKET , SO_KEEPALIVE , &value , sizeof( value ) );
		if ( mqttsocket > 0 )
		{
			// get address
			struct sockaddr_in address = mqtt_getsocketaddress( mqtt->host , mqtt->port , &success );
			if ( success == 1 )
			{
				// connect to peer
				int result = connect( mqttsocket , ( struct sockaddr* ) &address , sizeof( address ) );
				if ( result == 0 )
				{
					// send connection packet
					rawmessge_t* message = mqtt_packet_connect( mqtt );
					if ( message != NULL )
					{
						// set connection alive
						mqtt->alive = 1;
						// send connect message
						mqtt_senddata( mqtt , message->bytes , message->length );
						printf( "\n0 data sent %i" , message->length );
						// cleanup
						free( message->bytes );
						free( message );
						// start read thread
						pthread_t readthread;
						pthread_create(&readthread, NULL, (void*)mqtt_readloop , mqtt );
					}
					else printf( "\ncannot create connect message" );
				}
				else printf( "\ncannot connect to peer %s" , strerror( errno ) );
			}
			else printf( "\ncannot get address %s" , strerror( errno ) );
		}
		else printf( "\ncannot create socket %s" , strerror( errno ) );
	
		while ( mqtt->alive )
		{
			void* data = NULL;
			slots_get( mqtt->sendslots , &data );
			if ( data != NULL )
			{
				rawmessge_t* message = data;
				mqtt_senddata( mqtt , message->bytes , message->length );
				printf( "\n1 data sent %i %s" , message->length , (char*) ( message->bytes + 1 ) );
				free( message->bytes );
				free( message );
			}
			else
			{
				// send ping if needed
				time_t now = time( NULL );
				if ( now - mqtt->ping > 19 )
				{
					mqtt->ping = now;
					rawmessge_t* message = mqtt_packet_pingreq( );
					mqtt_senddata( mqtt , message->bytes , message->length );
					printf( "\n2 data sent %i" , message->length );
					free( message->bytes );
					free( message );
				}
				// sleep for .1 seconds
				struct timespec time;
				time.tv_sec = 0;
				time.tv_nsec = 100000000L;
				nanosleep(&time , (struct timespec *)NULL);
			}
		}
	}

	// read message from server, thread safe

	void mqtt_receive( mqtt_t* mqtt , vector_t* events )
	{
		//printf( "\nmqtt_receive" );
		// put incoming messages and mqtt connection events in events
		// only subscriber thread should put events in readslots
		object_t* eventobject;
		do
		{
			slots_get( mqtt->readslots , (void** ) &eventobject );
			if ( eventobject != NULL ) vector_adddata( events , eventobject );
		}
		while ( eventobject != NULL );
	}

