//
//  demo_scroller.h
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__demo_scroller__
#define __c36__demo_scroller__


    #include <stdio.h>
    #include "../../framework/view/ui.h"
    #include "../../framework/view/modifier.h"
    #include "../../framework/view/modifier_imagearea.h"
	#include "../../framework/view/generator_view.h"

	typedef struct _demo_scroller_t demo_scroller_t;
	struct _demo_scroller_t
	{
		void( *setup )( demo_scroller_t* delegate , char* path_resources , char* path_library );
		void( *timer )( demo_scroller_t* delegate );
		void( *event )( demo_scroller_t* delegate , modifierevent_t* event );
		ui_t* ui;
		view_t* scroller;
    };

    demo_scroller_t* demo_scroller_init( );
    void demo_scroller_setup( demo_scroller_t* demo , char* path_resources , char* path_library );
    void demo_scroller_timer( demo_scroller_t* demo );
    void demo_scroller_event( demo_scroller_t* demo , modifierevent_t* event );


#endif /* defined(__c36__demo_scroller__) */
