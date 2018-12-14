//
//  modifier_infinitescroller.c
//  kineticui
//
//  Created by mtoth on 23/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//


    #include "modifier_infinitescroller.h"


    // creates scroller

	modifier_infinitescroller_t* modifier_infinitescroller_default( )
	{
		modifier_infinitescroller_t* modifier = malloc( sizeof( modifier_infinitescroller_t ) );
		
		modifier->_new	= modifier_infinitescroller_new;
		modifier->_free = modifier_infinitescroller_free;
		modifier->type	= "infinitescroller";

		modifier->items = vector_create();
		modifier->last_point = vector3_default( 0.0 , 0.0 , 0.0 );
        modifier->swipedetector = NULL;

		return modifier;
	}

	// deletes scroller

	void modifier_infinitescroller_free( modifier_infinitescroller_t* modifier )
	{
		free( modifier );
	}

	// adds item to top

	void modifier_infinitescroller_addtopitem( modifier_infinitescroller_t* modifier , modifier_args_t* args )
	{
		view_t* itemview = (view_t*)args->command.arguments;
		if ( modifier->items->length > 0 )
		{
			view_t*		firstitem = modifier->items->data[ 0 ];
			vector2_t	position  = view_getposition( firstitem );
			position.y -= itemview->height;
			view_setposition( itemview , position );
		}
		vector_adddataatindex( modifier->items , itemview , 0 );
		view_addsubview( args->views.view , itemview );
	}

	// adds item to bottom

	void modifier_infinitescroller_addbottomitem( modifier_infinitescroller_t* modifier , modifier_args_t* args )
	{
		view_t* itemview = (view_t*)args->command.arguments;
		if ( modifier->items->length > 0 )
		{
			view_t*		lastitem = modifier->items->data[ modifier->items->length - 1 ];
			vector2_t	position = view_getposition( lastitem );
			position.y += lastitem->height;
			view_setposition( itemview , position );
		}
        vector_adddata( modifier->items , itemview );
		view_addsubview( args->views.view , itemview );
	}

	// removes top item

	void modifier_infinitescroller_removetopitem( modifier_infinitescroller_t* modifier , modifier_args_t* args )
	{
		if ( modifier->items->length > 0 )
		{
			view_t* itemview = modifier->items->data[ 0 ];
			vector_removedataatindex( modifier->items , 0 );
			view_removesubview( args->views.view , itemview );
		}
	}

	// removes bottom item

	void modifier_infinitescroller_removebottomitem( modifier_infinitescroller_t* modifier , modifier_args_t* args )
	{
		if ( modifier->items->length > 0 )
		{
			view_t* itemview = modifier->items->data[ modifier->items->length - 1 ];
			vector_removedataatindex( modifier->items , modifier->items->length - 1 );
			view_removesubview( args->views.view , itemview );
		}
	}

	// mouse move event, updating scrollers

	void modifier_infinitescroller_move( modifier_infinitescroller_t* modifier , modifier_args_t* arguments )
	{
		input_t* input = ( input_t* ) arguments->command.arguments;
		vector3_t ulc = arguments->views.view->realcorners.ul;
		vector3_t ip = vector3_default( input->value_x - ulc.x , input->value_y - ulc.y , 1.0 );
        
		vector2_t delta = vector2_default( ip.x - modifier->last_point.x , ip.y - modifier->last_point.y );
		modifier->last_point = ip;
		
		if ( modifier->items->length > 0 )
		{
		
			for ( uint32_t index = 0 ; index < modifier->items->length ; index++ )
			{
				view_t*		itemview = modifier->items->data[ index ];
				vector2_t	position = view_getposition( itemview );
				
				position.y += delta.y;
				view_setposition( modifier->items->data[ index ], position );
				
				if ( index == 0 )
				{
					if ( position.y < 0 )
					{
						// needs new top item
						object_t* eventobject = modifier_defaultevent( "newitem" , "needstop" , modifier , NULL );
						vector_adddata( arguments->toolset->events , eventobject );
					}
					else if ( position.y > 0 && position.y + itemview->height > 0 )
					{
						// remove top item
						object_t* eventobject = modifier_defaultevent( "newitem" , "removetop" , modifier , NULL );
						vector_adddata( arguments->toolset->events , eventobject );
					}
				}
				
				if ( index == modifier->items->length - 1 )
				{
					if ( position.y < arguments->views.view->height )
					{
						// remove bottom item
						object_t* eventobject = modifier_defaultevent( "newitem" , "removebottom" , modifier , NULL );
						vector_adddata( arguments->toolset->events , eventobject );
					}
					else if ( position.y + itemview->height > arguments->views.view->height )
					{
						// needs new bottom item
						object_t* eventobject = modifier_defaultevent( "newitem" , "needsbottom" , modifier , NULL );
						vector_adddata( arguments->toolset->events , eventobject );
					}
				}
			}
		}
		else
		{
			// needs new bottom item
			object_t* eventobject = modifier_defaultevent( "newitem" , "needsbottom" , modifier , NULL );
			vector_adddata( arguments->toolset->events , eventobject );
		}
		
		*(arguments->generator.changed_flag) = 1;
		
	}

	// mouse down event, storing point

	void modifier_infinitescroller_down( modifier_infinitescroller_t* modifier , modifier_args_t* arguments )
	{
		input_t* input = ( input_t* ) arguments->command.arguments;
		vector3_t ulc = arguments->views.view->realcorners.ul;
		vector3_t ip = vector3_default( input->value_x - ulc.x , input->value_y - ulc.y , 1.0 );

        printf( "\ndown %f %f" , ip.x , ip.y );

		modifier->last_point = ip;
		*(arguments->generator.changed_flag) = 1;
	}

	// mouse up event, storing point

	void modifier_infinitescroller_up( modifier_infinitescroller_t* modifier , modifier_args_t* arguments )
	{
		input_t* input = ( input_t* ) arguments->command.arguments;
		vector3_t ulc = arguments->views.view->realcorners.ul;
		vector3_t ip = vector3_default( input->value_x - ulc.x , input->value_y - ulc.y , 1.0 );

        printf( "\nup %f %f" , ip.x , ip.y );

		modifier->last_point = ip;
		*(arguments->generator.changed_flag) = 1;
	}

	// generate new state

    void modifier_infinitescroller_new( modifier_infinitescroller_t* modifier , modifier_args_t* args )
    {
		if ( strcmp( args->command.name , "addtopitem" ) == 0 )
		{
			modifier_infinitescroller_addtopitem( modifier , args );
		}
		else if ( strcmp( args->command.name , "addbottomitem" ) == 0 )
		{
			modifier_infinitescroller_addbottomitem( modifier , args );
		}
		else if ( strcmp( args->command.name , "removetopitem" ) == 0 )
		{
			modifier_infinitescroller_removetopitem( modifier , args );
		}
		else if ( strcmp( args->command.name , "removebottomitem" ) == 0 )
		{
			modifier_infinitescroller_removebottomitem( modifier , args );
		}
		else if ( strcmp( args->command.name , "touchdrag" ) == 0 )
		{
			// modifier_infinitescroller_move( modifier , args );
		}
		else if ( strcmp( args->command.name , "touchdown" ) == 0 )
		{
			// modifier_infinitescroller_down( modifier , args );
		}
		else if ( strcmp( args->command.name , "touchup" ) == 0 )
		{
			// modifier_infinitescroller_up( modifier , args );
		}
    }
