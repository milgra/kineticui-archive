//
//  modifier_cropper.c
//  c36
//
//  Created by Milan Toth on 19/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_cropper.h"

	void modifier_cropper_free( modifier_t* cropper );
	void modifier_cropper_new( modifier_t* cropper , modifier_args_t* args );

	modifier_t* modifier_cropper_default( )
	{
		modifier_t* modifier = malloc( sizeof( modifier_t ) );
		modifier_cropper_t* data = malloc( sizeof( modifier_cropper_t ) );

		modifier->type = "cropper";
		modifier->data = data;
		modifier->_new = modifier_cropper_new;
		modifier->_free = modifier_cropper_free;
		
		data->cropped = 0;
		data->lastextent = vector2_default( 0.0 , 0.0 );
		data->lastorigo = vector2_default( 0.0 , 0.0 );
		data->lastparent = matrix3_defaultidentity( );
		
		return modifier;
	}

	void modifier_cropper_free( modifier_t* cropper )
	{
		free( cropper->data );
		free( cropper );
	}

	void modifier_cropper_new( modifier_t* cropper , modifier_args_t* args )
	{
		modifier_cropper_t* data = cropper->data;
	
		view_t* view = args->views.view;
		view_t* parent = args->views.parent;

		if ( parent == NULL ) return;
		
		square2_t squarea = square_default( vector2_default( parent->realcorners.ul.x , parent->realcorners.ul.y ) , vector2_default( parent->realcorners.ur.x - parent->realcorners.ul.x , parent->realcorners.ll.y - parent->realcorners.ul.y ) );
		square2_t squareb = square_default( vector2_default( view->realcorners.ul.x , view->realcorners.ul.y ) , vector2_default( view->realcorners.ur.x - view->realcorners.ul.x , view->realcorners.ll.y - view->realcorners.ul.y ) );
		uint8_t	overlap = square_checkoverlapping( squarea , squareb );
	
		// view is outside the visible area
		if ( overlap == kSquareOverlappingNone )
		{
			view->nodraw = 1;
		}
		// view is in the visible area
		else if ( overlap == kSquareOverlappingInside )
		{
			if ( data->cropped == 1 )
			{
				// uncrop
				data->cropped = 0;
				view->realcoords  = view->coords;
				view->redraw = 1;
			}
			view->nodraw = 0;
		}
		// view is intersecting visible area
		else
		{
			// crop
			square2_t cropped = square_intersect2( squarea , squareb );
			
//			printf( "\n" );
//			printf( "\n\ncropping %s %s" , parent->name , view->name );
//			printf( "\nparent  %f %f %f %f" , squarea.origo.x , squarea.origo.y , squarea.extent.x , squarea.extent.y );
//			printf( "\nview  %f %f %f %f" , squareb.origo.x , squareb.origo.y , squareb.extent.x , squareb.extent.y );
//			printf( "\nintersection %f %f %f %f" , cropped.origo.x , cropped.origo.y , cropped.extent.x , cropped.extent.y );

			if ( args->toolset->logs != NULL )
			{
				vector_adddata
				(
					args->toolset->logs ,
					string_createfromformat
					(
						200 ,
						"cropping %s %s\n" ,
						parent->path ,
						view->path
					)
				);
			}

			float texwth = view->coords.ur.x - view->coords.ul.x;
			float texhth = view->coords.ll.y - view->coords.ul.y;
			float texlr = 0.0;
			float texrr = 1.0;
			float textr = 0.0;
			float texbr = 1.0;

			if ( squareb.origo.x < squarea.origo.x ) texlr = ( cropped.origo.x - squareb.origo.x ) / squareb.extent.x;
			if ( squareb.origo.x + squareb.extent.x > squarea.origo.x + squarea.extent.x ) texrr = ( cropped.origo.x + cropped.extent.x - squareb.origo.x ) / squareb.extent.x;
			if ( squareb.origo.y > squarea.origo.y ) textr = ( cropped.origo.y - squareb.origo.y ) / squareb.extent.y;
			if ( squareb.origo.y + squareb.extent.y < squarea.origo.y + squarea.extent.y ) texbr = ( cropped.origo.y + cropped.extent.y - squareb.origo.y ) / squareb.extent.y;

			view->realcorners.ul = vector3_default( cropped.origo.x , cropped.origo.y , 1.0 );
			view->realcorners.ur = vector3_default( cropped.origo.x + cropped.extent.x , cropped.origo.y , 1.0 );
			view->realcorners.ll = vector3_default( cropped.origo.x , cropped.origo.y + cropped.extent.y , 1.0 );
			view->realcorners.lr = vector3_default( cropped.origo.x + cropped.extent.x , cropped.origo.y + cropped.extent.y , 1.0 );

			view->realcoords.ul = vector2_default( view->coords.ul.x + texwth * texlr , view->coords.ul.y + texhth * textr );
			view->realcoords.ur = vector2_default( view->coords.ul.x + texwth * texrr , view->coords.ul.y + texhth * textr );
			view->realcoords.ll = vector2_default( view->coords.ll.x + texwth * texlr , view->coords.ul.y + texhth * texbr );
			view->realcoords.lr = vector2_default( view->coords.ll.x + texwth * texrr , view->coords.ul.y + texhth * texbr );

			view->nodraw = 0;
			view->redraw = 1;
			data->cropped = 1;
		}

	}

