//
//  scroller.h
//  KineticUI
//
//  Created by Milan Toth on 09/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__scroller__
	#define __KineticUI__scroller__

	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>

	#define kScrollerModeManual  0
	#define kScrollerModeStop	 1
	#define kScrollerModeMove	 2
	#define kScrollerModeEdge	 3

	typedef struct _scroller_t scroller_t;
	struct _scroller_t
	{
		float head;
		float size;
		float size_content;
		float speed;
		float angle;
		float delta;
		float edge;
		float target;
		char mode;
		char steps;
		char rubber;
	};

	scroller_t* scroller_default( char rubber );
	void scroller_free( scroller_t* scorller );
	char scroller_update( scroller_t* scroller );
	void scroller_move( scroller_t* scroller , float head );
	void scroller_auto( scroller_t* scroller );
	void scroller_scroll( scroller_t* scroller , float delta );
	void scroller_setsize( scroller_t* scroller , float size );
	void scroller_setcontentsize( scroller_t* scroller , float size );
	void scroller_setheadposition( scroller_t* scroller , float position );

	#endif /* defined(__KineticUI__scroller__) */
