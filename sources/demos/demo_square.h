//
//  test_square.h
//  c36
//
//  Created by Milan Toth on 10/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__test_square__
#define __c36__test_square__

    #include <stdio.h>
    #include "../../framework/view/ui.h"
    #include "../../framework/view/modifier.h"
    #include "../../framework/view/modifier_imagearea.h"
	#include "../../framework/view/generator_view.h"

	typedef struct _demo_square_t demo_square_t;
	struct _demo_square_t
	{
		void( *setup )( demo_square_t* delegate , char* path_resources , char* path_library );
		void( *timer )( demo_square_t* delegate );
		void( *event )( demo_square_t* delegate , modifierevent_t* event );
		ui_t* ui;
		view_t* imageview;
    };

    demo_square_t* demo_square_init( );
    void demo_square_setup( demo_square_t* demo , char* path_resources , char* path_library );
    void demo_square_timer( demo_square_t* demo );
    void demo_square_event( demo_square_t* demo , modifierevent_t* event );

#endif /* defined(__c36__test_square__) */
