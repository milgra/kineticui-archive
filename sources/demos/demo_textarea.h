//
//  demo_textarea.h
//  c36
//
//  Created by Milan Toth on 13/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__demo_textarea__
#define __c36__demo_textarea__

    #include <stdio.h>
    #include "../../framework/view/ui.h"
    #include "../../framework/view/modifier.h"
    #include "../../framework/view/modifier_imagearea.h"
	#include "../../framework/view/generator_view.h"

	typedef struct _demo_textarea_t demo_textarea_t;
	struct _demo_textarea_t
	{
		void( *setup )( demo_textarea_t* delegate , char* path_resources , char* path_library );
		void( *timer )( demo_textarea_t* delegate );
		void( *event )( demo_textarea_t* delegate , modifierevent_t* event );
		ui_t* ui;
		view_t* labelviewa;
		view_t* labelviewb;
		view_t* labelviewc;
		view_t* labelviewd;
    };

    demo_textarea_t* demo_textarea_init( );
    void demo_textarea_setup( demo_textarea_t* demo , char* path_resources , char* path_library );
    void demo_textarea_timer( demo_textarea_t* demo );
    void demo_textarea_event( demo_textarea_t* demo , modifierevent_t* event );

#endif /* defined(__c36__demo_textarea__) */
