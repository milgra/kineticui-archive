//
//  key_modifier_paragraph.h
//  KineticUI
//
//  Created by Milan Toth on 06/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__modifier_textarea__
    #define __KineticUI__modifier_textarea__


    #ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
    #elif defined OSX
    #include <OpenGL/gl3.h>
    #elif defined BRY
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #endif
    #include "../core/string.h"
    #include "../core/vector.h"
    #include "../graphics/font.h"
    #include "../graphics/tilemap.h"
    #include "../graphics/paragraph.h"
    #include "view.h"
    #include "modifier.h"
	#include "modifier_fader.h"
	#include "modifier_hover.h"
	#include "modifier_layout.h"
	#include "modifier_clipper.h"
	#include "modifier_animator.h"
	#include "modifier_solidcolor.h"
	#include "modifier_glypharea.h"

	typedef enum _textarea_linetype textarea_linetype;
	enum _textarea_linetype
	{
		textarea_linetype_singleline ,
		textarea_linetype_multiline
	};

	typedef enum _textarea_inputtype textarea_inputtype;
	enum _textarea_inputtype
	{
		textarea_inputtype_normal ,
		textarea_inputtype_input
	};

	typedef enum _textarea_aligntype textarea_aligntype;
	enum _textarea_aligntype
	{
		textarea_aligntype_justify ,
		textarea_aligntype_left ,
		textarea_aligntype_center ,
		textarea_aligntype_right
	};

	typedef struct _textarea_initializer textarea_initializer;
	struct _textarea_initializer
	{
		float margin;
		float linesize;
		uint32_t color;
		string_t* string;

		textarea_linetype linetype;
		textarea_inputtype inputtype;
		textarea_aligntype aligntype;
	};

    typedef struct _modifier_textarea_t modifier_textarea_t;
    struct _modifier_textarea_t
    {
		// text properties

		char openletters;
		float margin;
        float textsize;
		uint32_t textcolor;
		textarea_linetype linetype;
		textarea_inputtype inputtype;
		textarea_aligntype aligntype;

		// inner views

		view_t* cursor;
		view_t* contentview;
		
		// other stuff

		char focused;
		string_t* string;
		uint32_t cursorindex;
        paragraph_t* paragraph;
		
		// containers

		vector_t* glyphviews;
		vector_t* dyingviews;
		vector_t* stoppedviews;
		vector_t* removedglyphs;
		vector_t* animatingviews;
		
		// actions

		char* touchaction;
		char* enteraction;
		char* resizeaction;
		char* animendaction;
    };

    modifier_t* modifier_textarea_default( textarea_initializer* initializer );

	string_t* modifier_textarea_gettext( modifier_t* modifier );
	void modifier_textarea_animator_timer( modifier_t* modifier , modifier_args_t* args );
	void modifier_textarea_animator_alignglyphs_open( modifier_t* modifier , modifier_args_t* args );
	void modifier_textarea_animator_alignglyphs_slide( modifier_t* modifier , modifier_args_t* args );
	void modifier_textarea_animator_view_open( modifier_t* modifier , view_t* view , glyph_t* glyph , int delay , modifier_args_t* args );
	void modifier_textarea_animator_cursor_open( modifier_t* modifier , glyph_t* glyph , int delay , modifier_args_t* args );
	void modifier_textarea_animator_cursor_close( modifier_t* modifier , view_t* view , modifier_args_t* args );
	void modifier_textarea_animator_view_slidedown( modifier_t* modifier , view_t* view , glyph_t* glyph , float dx , float dy , modifier_args_t* args );
	void modifier_textarea_animator_view_slideup( modifier_t* modifier , view_t* view , glyph_t* glyph , float dx , float dy , modifier_args_t* args );
	void modifier_textarea_animator_view_slide( modifier_t* modifier , view_t* view , glyph_t* glyph , float dx , float dy , modifier_args_t* args );
	void modifier_textarea_animator_cursor_slidedown( modifier_t* modifier , view_t* view , glyph_t* glyph , vector2_t position , int delay , modifier_args_t* args );
	void modifier_textarea_animator_cursor_slideup( modifier_t* modifier , view_t* view , glyph_t* glyph , vector2_t position , int delay );

    #endif
