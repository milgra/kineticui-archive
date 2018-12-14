//
//  scroller.c
//  KineticUI
//
//  Created by Milan Toth on 09/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "scroller.h"

	// creates scroller

	scroller_t* scroller_default( char rubber )
	{
		scroller_t* scroller = malloc( sizeof( scroller_t ) );
		scroller->mode = kScrollerModeManual;
		scroller->head = 0.0;
		scroller->size = 0.0;
		scroller->size_content = 0.0;
		scroller->delta = 0.0;
		scroller->angle = 0.0;
		scroller->rubber = rubber;
		
		return scroller;
	}

	// deletes scroller

	void scroller_free( scroller_t* scroller )
	{
		free( scroller );
	}

	// set scroller size

	void scroller_setsize( scroller_t* scroller , float size )
	{
		scroller->size = size;
	}

	// set content size

	void scroller_setcontentsize( scroller_t* scroller , float size )
	{
		scroller->size_content = size;
	}

	// set head position

	void scroller_setheadposition( scroller_t* scroller , float position )
	{
		scroller->head = position;
		// validate position
		if ( scroller->head + scroller->size_content < scroller->size ) scroller->head = scroller->size - scroller->size_content;
		if ( scroller->head > 0 ) scroller->head = 0;
	}

	// move head in 12 steps

	void scroller_move( scroller_t* scroller , float head )
	{
		if ( scroller->rubber == 1 )
		{
			scroller->target = head;
			scroller->delta = ( scroller->target - scroller->head ) / 12;
			scroller->mode = kScrollerModeMove;
			scroller->steps = 12;
		}
	}

	// let head move automatically from edges

	void scroller_auto( scroller_t* scroller )
	{
		if ( scroller->rubber == 1 )
		{
			if ( scroller->head > 0 )
			{
				scroller->delta = scroller->head;
				scroller->edge = 0.0;
				scroller->angle = 0.0;
				scroller->mode = kScrollerModeEdge;
			}
			else
			if ( scroller->head + scroller->size_content < scroller->size )
			{
				if ( scroller->size_content > scroller->size )
				{
					scroller->delta = -(scroller->size - scroller->head - scroller->size_content);
					scroller->edge = scroller->size - scroller->size_content;
					scroller->angle = 0.0;
					scroller->mode = kScrollerModeEdge;
				}
				else
				{
					scroller->delta = scroller->head;
					scroller->edge = 0.0;
					scroller->angle = 0.0;
					scroller->mode = kScrollerModeEdge;
				}
			}
			else
			{
				scroller->mode = kScrollerModeStop;
			}
		}
	}

	// increase/decrease head position manually

	void scroller_scroll( scroller_t* scroller , float delta )
	{
		scroller->mode = kScrollerModeManual;
		scroller->head += delta;
		scroller->delta = delta;
		
		if ( scroller->rubber == 0 )
		{
			if ( scroller->head > 0.0 ) scroller->head = 0.0;
			if ( scroller->head + scroller->size_content < scroller->size )
			{
				scroller->head = 0.0;
				if ( scroller->size_content > scroller->size ) scroller->head = scroller->size - scroller->size_content;
			}
		}
	}

	// update scroller

	char scroller_update( scroller_t* scroller )
	{
		if ( scroller->mode == kScrollerModeEdge )
		{
			// progressive slowing down at edges
			scroller->head = scroller->edge + cosf(scroller->angle) * scroller->delta;
			scroller->delta *= .8;
			scroller->angle += .3;
			if ( fabs( scroller->delta ) < 0.1 ) scroller->mode = kScrollerModeManual;
			return 1;
		}
		else if ( scroller->mode == kScrollerModeStop )
		{
			// simply stop movement
			scroller->head += scroller->delta;
			scroller->delta *= .8;
			if ( fabs( scroller->delta ) < 0.1 ) return 0;
		}
		else if ( scroller->mode == kScrollerModeMove )
		{
			// step
			scroller->head += scroller->delta;
			scroller->steps -= 1;
			if ( scroller->steps == 0 ) scroller->mode = kScrollerModeManual;
			return 1;
		}
		return 0;
	}
