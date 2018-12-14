//
//  generator_view.h
//  KineticUI
//
//  Created by Milan Toth on 02/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__generator_view__
	#define __KineticUI__generator_view__

	#include <stdio.h>
	#include "../core/map.h"
    #include "../core/string.h"
    #include "../graphics/font.h"
    #include "../graphics/tilemap.h"
	#include "view.h"
	#include "modifier.h"
	#include "modifier_hover.h"
	#include "modifier_button.h"
	#include "modifier_layout.h"
	#include "modifier_dragger.h"
	#include "modifier_scroller.h"
	#include "modifier_cropper.h"
	#include "modifier_animator.h"
	#include "modifier_blurcolor.h"
	#include "modifier_textarea.h"
	#include "modifier_glypharea.h"
	#include "modifier_itemlist.h"
	#include "modifier_imagearea.h"
	#include "modifier_solidcolor.h"

	map_t* generator_view_generate( char* text , float scale );
	view_t* generator_view_structure( map_t* viewmap , char* text );
	view_t* generator_view_default( char* viewdesc , char* structuredesc , float scale );

	#endif /* defined(__KineticUI__generator_view__) */
