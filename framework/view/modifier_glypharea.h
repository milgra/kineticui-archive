//
//  modifier_glypharea.h
//  c36
//
//  Created by Milan Toth on 03/04/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__modifier_glypharea__
#define __c36__modifier_glypharea__

    #ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
    #elif defined OSX
    #include <OpenGL/gl3.h>
    #elif defined BRY
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #endif
    #include "../core/map.h"
    #include "../core/vector.h"
    #include "../core/string.h"
    #include "../graphics/font.h"
    #include "../graphics/bitmap.h"
    #include "../graphics/tilemap.h"
    #include "view.h"
	#include "modifier.h"


    typedef struct _modifier_glypharea_t modifier_glypharea_t;
    struct _modifier_glypharea_t
    {
		float size;
		char* name;
		uint32_t color;
		uint32_t codepoint;
		vector2_t* coords;
    };


    modifier_t* modifier_glypharea_default( uint32_t color , uint32_t codepoint , float size );

#endif /* defined(__c36__modifier_glypharea__) */
