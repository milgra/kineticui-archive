//
//  view.h
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__view__
    #define __KineticUI__view__


    #include <stdio.h>
	#include <assert.h>
	#include "input.h"
	#include "modifier.h"
    #include "view_types.h"

    view_t* view_default( float x , float y , float width , float height );
    void view_free( view_t* view );
    void view_addsubview( view_t* view , view_t* subview );
    void view_addsubviewatindex( view_t* view , view_t* subview , uint32_t index );
    char view_containssubview( view_t* view , view_t* subview );
	void view_updatepath( view_t* view , char* path );
    void view_removesubview( view_t* view , view_t* subview );
    void view_addmodifier( view_t* view , modifier_t* modifier , char geometry );
    void view_removemodifier( view_t* view , modifier_t* modifier );
	void* view_getmodifier( view_t* view , char* type );
	view_t* view_getsubview( view_t* view , char* name );
	void view_setname( view_t* view , char* name );
	vector2_t view_getposition( view_t* view );
	void view_setposition( view_t* view , vector2_t position );
	vector2_t view_getsize( view_t* view );
	void view_setsize( view_t* view , vector2_t size );
	square2_t view_getsquare( view_t* view );
	void view_setsquare( view_t* view , square2_t square );
	void view_collect_subviews( view_t* view , vector_t* vector );
	void view_new( modifier_args_t* args );

    void view_finalize_geometry( view_t* view , view_t* parent , matrix3_t matrix_parent , char parent_changed , toolset_t* toolset , uint32_t* counter , uint32_t* quadcounter );
    void view_collect_touched_views( view_t* view , vector_t* vector , float touch_x , float touch_y );
    void view_collect_vertexes( view_t* view , GLfloat* collector , uint32_t* quadcount );
	void view_describe( view_t* view , int level );


    #endif
