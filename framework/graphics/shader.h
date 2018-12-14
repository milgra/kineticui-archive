//
//  shader.h
//  KineticUI
//
//  Created by Milan Toth on 26/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef __KineticUI__shader__
	#define __KineticUI__shader__

	#include <stdio.h>
	#include "../core/map.h"
    #ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
    #elif defined OSX
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
    #elif defined BRY
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #endif

	typedef struct _shader_t shader_t;
	struct _shader_t
	{
		GLuint program_name;
		GLint* uniform_locations;
	};

    shader_t* shader_default( const char* vertex_source , const char* fragment_source , char** uniform_structure , char** attribute_structure );
    void shader_free( shader_t* map );
	GLuint shader_compile ( GLenum the_type , const GLchar* the_source );
	char shader_link ( GLuint program );

	#endif /* defined(__KineticUI__shader__) */
