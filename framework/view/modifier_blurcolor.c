//
//  modifier_blurcolor.c
//  c36
//
//  Created by Milan Toth on 19/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#include "modifier_blurcolor.h"

	void modifier_blurcolor_free( modifier_t* modifier );
	void modifier_blurcolor_new( modifier_t* modifier , modifier_args_t* args );

    // creates new solidcolor

    modifier_t* modifier_blurcolor_default( uint32_t color )
    {
        modifier_t* modifier = malloc( sizeof( modifier_t ) );
        
		modifier->type = "blurcolor";
		modifier->data = NULL;
		modifier->_new = modifier_blurcolor_new;
		modifier->_free = modifier_blurcolor_free;
		
        return modifier;
    }

	// delete modifier

	void modifier_blurcolor_free( modifier_t* modifier )
	{
		free( modifier );
	}

	// generates new modifier state

	void modifier_blurcolor_new( modifier_t* modifier , modifier_args_t* args )
	{
		if ( strcmp( args->command.name , "update" ) == 0 )
		{
		
			blursettings_t* blursettings = args->toolset->settings->userdata;

			view_t* view = args->views.view;
			vector2_t* coords = blursettings->blurcoords;
			vector3_t ulc, urc, llc, lrc;
			
			if ( coords == NULL ) return;

			ulc = view->realcorners.ul;
			urc = view->realcorners.ur;
			llc = view->realcorners.ll;
			lrc = view->realcorners.lr;
		
			float width = args->toolset->settings->screensize.x;
			float height = args->toolset->settings->screensize.y;

			float xpos = blursettings->blurorigin.x;
			float ypos = blursettings->blurorigin.y;

			float size = fmax( width , height );

			float ulx = -xpos + ulc.x;
			float urx = -xpos + urc.x;
			float uly = ypos - ulc.y;
			float lly = ypos - llc.y;

			float texwth = coords[1].x - coords[0].x;	// urx - ulx
			float texhth = coords[2].y - coords[0].y;	// lly - uly

			float ulxtoback = ulx / size;
			float ulytoback = uly / size;

			float lrxtoback = urx / size;
			float lrytoback = lly / size;

			float ultexturex = coords[0].x + texwth * ulxtoback;
			float ultexturey = coords[0].y + texhth * ulytoback;

			float lltexturex = coords[0].x + texwth * ulxtoback;
			float lltexturey = coords[0].y + texhth * lrytoback;

			float lrtexturex = coords[0].x + texwth * lrxtoback;
			float lrtexturey = coords[0].y + texhth * lrytoback;

			float urtexturex = coords[0].x + texwth * lrxtoback;
			float urtexturey = coords[0].y + texhth * ulytoback;

			view->realcoords.ul.x = ultexturex;
			view->realcoords.ul.y = ultexturey;
			view->realcoords.ll.x = lltexturex;
			view->realcoords.ll.y = lltexturey;
			view->realcoords.lr.x = lrtexturex;
			view->realcoords.lr.y = lrtexturey;
			view->realcoords.ur.x = urtexturex;
			view->realcoords.ur.y = urtexturey;
			view->coords = view->realcoords;
		}
	}