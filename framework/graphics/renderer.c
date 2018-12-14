//
//  Renderer.c
//  KineticUI
//
//  Created by Milan Toth on 11/09/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "Renderer.h"

	const char* vertex_source =
	#if defined(IOS) || defined(BRY)
		"attribute vec4 position;\n"
		"attribute vec2 texcoord;\n"
		"varying highp vec2 texcoordv;\n"
		"uniform mat4 projection;\n"
		"void main ( )\n"
		"{\n"
			"gl_Position = projection * position;\n"
			"texcoordv = texcoord;"
		"}\n";
	#else
		"attribute vec4 position;\n"
		"attribute vec2 texcoord;\n"
		"varying vec2 texcoordv;\n"
		"uniform mat4 projection;\n"
		"void main ( )\n"
		"{\n"
			"gl_Position = projection * position;\n"
			"texcoordv = texcoord;"
		"}\n";
	#endif

	const char* fragment_source =
	#if defined(IOS)
		"varying mediump vec2 texcoordv;\n"
		"uniform sampler2D maintexture;\n"
		"void main( )\n"
		"{\n"
		"	gl_FragColor = texture2D( maintexture , texcoordv , 0.0 );\n"
		"}\n";
	#elif defined(BRY)
		"varying mediump vec2 texcoordv;\n"
		"uniform sampler2D maintexture;\n"
		"void main( )\n"
		"{\n"
		"	gl_FragColor = texture2D( maintexture , texcoordv , 0.0 );\n"
		"}\n";
	#else
		"varying vec2 texcoordv;\n"
		"uniform sampler2D maintexture;\n"
		"void main( )\n"
		"{\n"
		"	gl_FragColor = texture2D( maintexture , texcoordv , 0.0 );\n"
		"}\n";
	#endif

    // creates shader

    renderer_t* renderer_default( float width , float height )
    {
		// setup context
        glEnable( GL_BLEND );
        glEnable( GL_CULL_FACE );
        glEnable( GL_TEXTURE_2D );
        glViewport( 0.0 , 0.0 , width , height );
        glClearColor( 1.0 , 1.0 , 1.0 , 1.0 );
        glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
        glClear( GL_COLOR_BUFFER_BIT );

		// create and use shader
        char* uniforms[ ]   = { "2" , "projection" , "maintexture" };
        char* attributes[ ] = { "2" , "position" , "texcoord" };
        shader_t* shader = shader_default( vertex_source , fragment_source , uniforms , attributes );

		// set uniform texture slots
		glUniform1i( glGetUniformLocation( shader->program_name , "maintexture" ) , 0 );

		// setup vertex buffer
        GLuint buffer_name;
        glGenBuffers ( 1 , &buffer_name );
        glBindBuffer(GL_ARRAY_BUFFER, buffer_name );
        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 0 , 2 , GL_FLOAT , GL_FALSE , 16 , 0 );
        glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 16 , (const GLvoid*) 8 );
		
		// setup element buffer
		GLuint elementbuffer;
		glGenBuffers(1, &elementbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		GLushort* indices = malloc( sizeof( GLushort ) * 6 * 1000 );
		int vertex = 0;
		for ( int index = 0 ; index < 6000 ; index+=6 )
		{
			indices[index+0] = vertex;
			indices[index+1] = vertex + 1;
			indices[index+2] = vertex + 2;
			indices[index+3] = vertex;
			indices[index+4] = vertex + 2;
			indices[index+5] = vertex + 3;
			vertex += 4;
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * 6 * 1000 , indices, GL_STATIC_DRAW);
		free( indices );

		// setup main texture
        GLuint texture_id;
        glGenTextures( 1, &texture_id );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, texture_id );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
		glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , 1024 , 1024 , 0 , GL_RGBA , GL_UNSIGNED_BYTE , NULL );

		renderer_t* renderer = malloc( sizeof( renderer_t ) );

		renderer->shader = shader;
		renderer->width = width;
		renderer->height = height;
        renderer->vertexdata = malloc( sizeof( GLfloat ) * 1024 * 1024 * 8 );
		renderer->buffer_name = buffer_name;
		renderer->maintexture = texture_id;
		renderer->matrix_projection = malloc( sizeof( GLfloat ) * 16 );
		renderer->resized = 1;
		renderer->indexcount = 1000;
		renderer->element_buffer = elementbuffer;

		matrix4_t proj_matrix = matrix4_defaultortho(0.0,width,-height,0.0,0.0,1.0);
		matrix4_toarray( &proj_matrix , renderer->matrix_projection );
		glUniformMatrix4fv( renderer->shader->uniform_locations[ 0 ] , 1 , 0 , renderer->matrix_projection );

		return renderer;

    }

	// deletes renderer

	void renderer_free( renderer_t* renderer )
	{
		shader_free( renderer->shader );
		glDeleteTextures( 1 , &(renderer->maintexture) );
		glDeleteTextures( 1 , &(renderer->blurtexture) );
        glDeleteBuffers( 1 , &(renderer->buffer_name) );

		free( renderer->matrix_projection );
		free( renderer );
	}

	// resize

	void renderer_resize( renderer_t* renderer , float width , float height )
	{
        glViewport( 0.0 , 0.0 , width , height );
		renderer->width = width;
		renderer->height = height;
		matrix4_t proj_matrix = matrix4_defaultortho(0.0,width,-height,0.0,0.0,1.0);
		matrix4_toarray( &proj_matrix , renderer->matrix_projection );
		glUniformMatrix4fv( renderer->shader->uniform_locations[ 0 ] , 1 , 0 , renderer->matrix_projection );
	}

    // draw to context

    void renderer_draw( renderer_t* renderer , tilemap_t* tilemap , GLfloat* vertexes , int quadcount )
    {
        glClear( GL_COLOR_BUFFER_BIT );

        // upload tilemap as texture if needed
        if ( tilemap->updated == 1 )
        {
			glActiveTexture( GL_TEXTURE0 );
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1024,1024,0,GL_RGBA,GL_UNSIGNED_BYTE,tilemap->bitmap->bytes);
            tilemap->updated = 0;
        }
		
		if ( quadcount > renderer->indexcount )
		{
			renderer->indexcount += 1000;
			// increase indices array
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->element_buffer);
			GLushort* indices = malloc( sizeof( GLushort ) * 6 * renderer->indexcount );
			int vertex = 0;
			for ( int index = 0 ; index < renderer->indexcount * 6 ; index += 6 )
			{
				indices[index+0] = vertex;
				indices[index+1] = vertex + 1;
				indices[index+2] = vertex + 2;
				indices[index+3] = vertex;
				indices[index+4] = vertex + 2;
				indices[index+5] = vertex + 3;
				vertex += 4;
			}
			glBufferData( GL_ELEMENT_ARRAY_BUFFER , sizeof( GLushort ) * 6 * renderer->indexcount , indices , GL_STATIC_DRAW );
			free( indices );
		}

        glBufferData( GL_ARRAY_BUFFER , 64 * quadcount , vertexes , GL_DYNAMIC_DRAW );
		glDrawElements( GL_TRIANGLES , quadcount * 6 , GL_UNSIGNED_SHORT , (void*)0	);
    }

//	opengl es3/4.0 versions of shaders

//        const char* vertex_source =
//        #ifdef IOS
//        "#version 300 es\n"
//        "precision highp float;\n"
//
//        "in vec4   position;\n"
//        "in vec2   texcoord;\n"
//		  "in float  alpha;\n"
//
//        "out vec2  texcoordv;\n"
//		  "out float alphav;\n"
//
//        "uniform mat4 projection;\n"
//        "void main ( )\n"
//        "{\n"
//			"gl_Position = projection * position;\n"
//          "texcoordv = texcoord;\n"
//          "alphav = alpha;\n"
//        "}\n";
//        #else
//        "#version 410\n"
//
//        "in vec4   position;\n"
//        "in vec2   texcoord;\n"
//		  "in float  alpha;\n"
//
//        "out vec2  texcoordv;\n"
//        "out vec3  positionv;\n"
//		  "out float alphav;\n"
//
//        "uniform mat4 projection;\n"
//
//        "void main ( )\n"
//        "{\n"
//			"gl_Position = projection * position;\n"
//			"positionv = vec3(position);\n"
//          "texcoordv  = texcoord;\n"
//          "alphav = alpha;\n"
//        "}\n";
//        #endif


//        const char* fragment_source =
//        #ifdef IOS
//            "#version 300 es\n"
//            "precision highp float;\n"
//
//            "in vec2  texcoordv;\n"
//			"in float alphav;\n"
//
//            "out vec4 fragColor;\n"
//
//            "uniform sampler2D maintexture;\n"
//            "uniform sampler2D frametexture;\n"
//
//            "void main ( )\n"
//            "{\n"
//            "    fragColor = texture(maintexture, texcoordv, 0.0);\n"
//            "}\n";
//        #else
//            "#version 410\n"
//
//            "in vec2  texcoordv;\n"
//			"in vec3  positionv;\n"
//			"in float alphav;\n"
//
//            "out vec4 fragColor;\n"
//
//            "uniform sampler2D maintexture;\n"
//            "uniform sampler2D frametexture;\n"
//            "void main ( )\n"
//            "{\n"
//            "    fragColor = texture(maintexture, texcoordv, 0.0);\n"
//			"	 fragColor *= ( 100.0 + positionv.z ) / 100.0;\n"
//			"	 fragColor.w *= alphav;\n"
//            "}\n";
//        #endif
//

//		mapbuffer texture upload
//
//		GLubyte *ptr = glMapBufferRange( GL_PIXEL_UNPACK_BUFFER, 0, 1024*1024*4, GL_MAP_WRITE_BIT|GL_MAP_UNSYNCHRONIZED_BIT );
//		memcpy( ptr , bitmap->bytes , bitmap->bytes_length );
//		glUnmapBuffer( GL_PIXEL_UNPACK_BUFFER );
//
//		glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, 1024, 1024 );
//		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
//		glGenerateMipmap( GL_TEXTURE_2D );

//		mapbuffer vertexbuffer upload
//
//        if( buffer->stream_offset + data_size > 1024*1024*8 )
//        {
//            glBufferData( GL_ARRAY_BUFFER , 1024*1024*8 , NULL , GL_STREAM_DRAW );
//            buffer->stream_offset = 0;
//            buffer->draw_offset = 0;
//        }
//
//        // get buffer map
//        GLfloat* vertices = glMapBufferRange(   GL_ARRAY_BUFFER,
//                                                buffer->stream_offset,
//                                                data_size,
//                                                GL_MAP_WRITE_BIT|GL_MAP_UNSYNCHRONIZED_BIT);
//
//        // place new data in map
//        link_t* one_link = elements->head;
//        while ( one_link != NULL )
//        {
//            GLfloat* data = one_link->data;
//            GLuint size = sizeof( GLfloat ) * 6 * 6;
//            memcpy( vertices , data , size );
//            vertices += size / 4;
//            one_link = one_link->next;
//        }
//
//        // unmap and draw
//        glUnmapBuffer( GL_ARRAY_BUFFER );


//  3d projection
//
//	float camera_fov_y = M_PI / 3.0;
//	float camera_fov_x = camera_fov_y * ( *screen_width / *screen_height );
//	float camera_eye_z = ( height / 2.0 ) / ( tanf( camera_fov_y / 2.0 ) );
//	matrix4_t view_matrix = matrix4_defaulttranslation( - width / 2.0 , height / 2.0 , -camera_eye_z );
//	matrix4_t pers_matrix = matrix4_defaultperspective( camera_fov_y , width / height , 50.0 , camera_eye_z * 2 );
//	matrix4_t proj_matrix = matrix4_multiply( pers_matrix , view_matrix );
