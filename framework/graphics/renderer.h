//
//  Renderer.h
//  KineticUI
//
//  Created by Milan Toth on 11/09/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef KineticUI_Renderer_h
	#define KineticUI_Renderer_h


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

    #include "../core/map.h"
    #include "../core/math4.h"
    #include "tilemap.h"
	#include "shader.h"

	typedef struct _renderer_t renderer_t;
	struct _renderer_t
	{
		shader_t* shader;
		GLfloat* matrix_projection;
		
		float width;
		float height;
		char resized;
		int indexcount;

		// vertex buffer
		GLuint buffer_name;
        GLfloat* vertexdata;

		// elements texture
		GLuint maintexture;
		
		// frambeuffer clone texture for post processing
		GLuint offscr_framebuffer;
		GLuint offscr_renderbuffer;
        GLuint blurtexture;
		GLuint element_buffer;
	};


    renderer_t* renderer_default( float width , float height );
	void renderer_free( renderer_t* renderer );
	void renderer_resize( renderer_t* renderer , float width , float height );
    void renderer_draw( renderer_t* renderer , tilemap_t* tilemap , GLfloat* vertexes , int quadcount );


	#endif
