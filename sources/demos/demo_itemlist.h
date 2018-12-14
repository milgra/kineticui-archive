//
//  demo_itemlist.h
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__demo_itemlist__
#define __c36__demo_itemlist__


    #include <stdio.h>
    #include "../../framework/view/ui.h"
    #include "../../framework/view/modifier.h"
    #include "../../framework/view/modifier_imagearea.h"
	#include "../../framework/view/generator_view.h"

	typedef struct _demo_itemlist_t demo_itemlist_t;
	struct _demo_itemlist_t
	{
		void( *setup )( demo_itemlist_t* delegate , char* path_resources , char* path_library );
		void( *timer )( demo_itemlist_t* delegate );
		void( *event )( demo_itemlist_t* delegate , modifierevent_t* event );
		ui_t* ui;
		view_t* itemlist;
    };

    demo_itemlist_t* demo_itemlist_init( );
    void demo_itemlist_setup( demo_itemlist_t* demo , char* path_resources , char* path_library );
    void demo_itemlist_timer( demo_itemlist_t* demo );
    void demo_itemlist_event( demo_itemlist_t* demo , modifierevent_t* event );


#endif /* defined(__c36__demo_itemlist__) */
