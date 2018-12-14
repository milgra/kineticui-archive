//
//  modifier_itemlist.c
//  KineticUI
//
//  Created by Milan Toth on 27/12/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_itemlist.h"

	void modifier_itemlist_free( modifier_t* modifier );
	void modifier_itemlist_new( modifier_t* modifier , modifier_args_t* args );

	// init

	modifier_t* modifier_itemlist_default( )
	{
		modifier_t* modifier = malloc( sizeof( modifier_t ) );
		modifier_itemlist_t* data = malloc( sizeof( modifier_itemlist_t ) );
        
		modifier->type = "itemlist";
        modifier->data = data;
		modifier->_new = modifier_itemlist_new;
		modifier->_free = modifier_itemlist_free;
        
		data->items = vector_create( );		// items inside table
		data->contentview = NULL;
		data->gap = 0.0;
		
		return modifier;
	}

	// deletes modifier

	void modifier_itemlist_free( modifier_t* modifier )
	{
        modifier_itemlist_t* data = modifier->data;
		vector_free( data->items );
        free( modifier->data );
		free( modifier );
	}

	// creates contentview

	view_t* modifier_itemlist_defaultcontentview( float width , float height )
	{
		view_t* result = view_default( 0.0 , 0.0 , width , height );
		result->invisible = 1;
		
		modifier_t* cropper = modifier_cropper_default();
		modifier_t* mymodifier = modifier_layout_default( ( 1 << 3 ) | ( 1 << 2 ) );

		view_addmodifier( result , cropper , 1 );
		view_addmodifier( result , mymodifier , 0 );
	
		return result;
	}

	// generates new state

	void modifier_itemlist_new( modifier_t* modifier , modifier_args_t* args )
	{
        modifier_itemlist_t* data = modifier->data;
		if ( strcmp( args->command.name , "init" ) == 0 )
		{
			data->contentview = modifier_itemlist_defaultcontentview( args->views.view->width , -data->gap );
			view_addsubview( args->views.view , data->contentview );
		}
		else if ( strcmp( args->command.name , "additem" ) == 0 )
		{
			// add new items
			view_t* newitem = args->command.arguments;
			view_addsubview( data->contentview , newitem );
			vector_adddata( data->items , newitem );
			
			// rearrange
			float height = -data->gap;
			for ( int index = 0 ; index < data->items->length ; index++ )
			{
				view_t* itemview = data->items->data[index];
				vector2_t itemsize = view_getsize( itemview );
				view_setposition( itemview , vector2_default( 0.0 , height ) );
				height += itemsize.y - data->gap;
			}
			
			// update content size
			vector2_t contsize = view_getsize( data->contentview );
			view_setsize( data->contentview , vector2_default( contsize.x , height ) );
			view_setsize( args->views.view , vector2_default( contsize.x , height ) );
		}
	}