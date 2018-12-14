//
//  modifier_imagearea.h
//  KineticUI
//
//  Created by Milan Toth on 06/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__modifier_imagearea__
    #define __KineticUI__modifier_imagearea__


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
    #include "../graphics/bitmap.h"
    #include "../graphics/tilemap.h"
    #include "view.h"
	#include "modifier.h"


    typedef struct _modifier_imagearea_t modifier_imagearea_t;
    struct _modifier_imagearea_t
    {
		char* path;
		char* imagename;
        bitmap_t* image;
		vector2_t* coords;
    };


    modifier_t* modifier_imagearea_default( char* imagename );


    #endif
