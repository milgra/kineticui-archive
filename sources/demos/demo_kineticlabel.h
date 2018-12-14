//
//  demo_kineticlabel.h
//  c36
//
//  Created by Milan Toth on 14/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__demo_kineticlabel__
#define __c36__demo_kineticlabel__


    #include <stdio.h>
    #include "../../framework/view/ui.h"
    #include "../../framework/view/modifier.h"
    #include "../../framework/view/modifier_imagearea.h"
	#include "../../framework/view/generator_view.h"

	typedef struct _demo_kineticlabel_t demo_kineticlabel_t;
	struct _demo_kineticlabel_t
	{
		void( *setup )( demo_kineticlabel_t* delegate , char* path_resources , char* path_library );
		void( *timer )( demo_kineticlabel_t* delegate );
		void( *event )( demo_kineticlabel_t* delegate , modifierevent_t* event );
		ui_t* ui;
		view_t* labelviewa;
		view_t* labelviewb;
		view_t* labelviewc;
		view_t* labelviewd;
    };

    demo_kineticlabel_t* demo_kineticlabel_init( );
    void demo_kineticlabel_setup( demo_kineticlabel_t* demo , char* path_resources , char* path_library );
    void demo_kineticlabel_timer( demo_kineticlabel_t* demo );
    void demo_kineticlabel_event( demo_kineticlabel_t* demo , modifierevent_t* event );


#endif /* defined(__c36__demo_kineticlabel__) */
