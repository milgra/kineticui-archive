//
//  strips.h
//  kineticui
//
//  Created by Milan Toth on 21/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __kineticui__strips__
#define __kineticui__strips__


    #include <stdio.h>
    #include "../../framework/view/ui.h"
    #include "../../framework/view/modifier.h"
	#include "../../framework/view/generator_view.h"
    #include "modifier_infinitescroller.h"
	#include "types.h"

	typedef struct _strips_t strips_t;
	struct _strips_t
	{
		void( *setup )( strips_t* delegate , char* path_resources , char* path_library );
		void( *timer )( strips_t* delegate );
		void( *event )( strips_t* delegate , modifierevent_t* event );
		ui_t* ui;
		view_t* base;
    };

    strips_t* strips_init( );
    void strips_setup( strips_t* demo , char* path_resources , char* path_library );
    void strips_timer( strips_t* demo );
    void strips_event( strips_t* demo , modifierevent_t* event );

	void strips_showcalendar( float left , float right , float scale );


#endif /* defined(__kineticui__strips__) */
