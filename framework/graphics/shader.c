//
//  shader.c
//  KineticUI
//
//  Created by Milan Toth on 26/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

#include "shader.h"


    void shader_printerrors( const char* place )
    {
        #ifdef DEBUG
        GLenum error = 0;

        do
        {
            GLenum error = glGetError( );
            if ( error > GL_NO_ERROR ) printf( "\nGL Error at %s : %i" , place , error );
        }
        while ( error > GL_NO_ERROR );
        #endif
    }


    // creates shader info hashmap

    shader_t* shader_default( const char* vertex_source , const char* fragment_source , char** uniform_structure , char** attribute_structure )
    {

        shader_t* shader = malloc( sizeof( shader_t ) );

        int uniform_locations_length = atoi( uniform_structure[0] );
        int attribute_locations_length = atoi( attribute_structure[0] );
    
        GLuint program = 0;
        GLint* uniform_locations = malloc( sizeof( GLint ) * uniform_locations_length );
        
        // create program

        program = glCreateProgram( );
        shader_printerrors( "CreateProgram" );

        // compile shaders
        
        GLuint vertex_shader = shader_compile( GL_VERTEX_SHADER , vertex_source );
        if ( vertex_shader == 0 ) printf( "\nFailed to compile vertex shader" );
        shader_printerrors( "CompileVertex" );
        GLuint fragment_shader = shader_compile( GL_FRAGMENT_SHADER , fragment_source );
        if ( fragment_shader == 0 ) printf( "\nFailed to compile fragment shader" );
        shader_printerrors( "CompileFragment" );
        
        // attach shaders
        
        glAttachShader( program , vertex_shader );
        shader_printerrors( "AttachVertex" );

        glAttachShader( program , fragment_shader );
        shader_printerrors( "AttachFragment" );
        
        // attach attribute locations before link

        for ( int index = 0 ; index < attribute_locations_length ; index++ )
        {
        
            const GLchar* name = attribute_structure[ index + 1 ];
            glBindAttribLocation( 	program ,
                                    index ,
                                    name );

        }
        
        // link program
        
        int success = shader_link( program );
        if ( success == 1 )
        {
        
            for ( int index = 0 ; index < uniform_locations_length ; index++ )
            {
                const GLchar* name = uniform_structure[ index + 1 ];
                uniform_locations[ index ] = glGetUniformLocation( program , name );
            }
            
        }
        else printf("\nShader : Failed to link program" );
		
		// everything is ok, release shaders, program stays
		
		if ( vertex_shader > 0 )
		{
			glDetachShader( program , vertex_shader );
			glDeleteShader( vertex_shader );
		}
		
		if ( fragment_shader > 0 )
		{
			glDetachShader( program , fragment_shader );
			glDeleteShader( fragment_shader );
		}
		
		shader->program_name = program;
		shader->uniform_locations = uniform_locations;

		glUseProgram( program );

        return shader;

    }


    // deletes shader

    void shader_free( shader_t* shader )
    {

		if ( shader->program_name != 0 ) glDeleteProgram( shader->program_name );
        if ( shader->uniform_locations != NULL ) free( shader->uniform_locations );
		
		free( shader );
		
    }


    // compile shader
	
	GLuint shader_compile ( GLenum type , const GLchar* source )
    {
    
        GLint status, logLength, realLength;
        GLuint shader = 0;
		GLchar* log;
		
		// create shader
        
        status = 0;

        shader = glCreateShader( type );
        shader_printerrors( "CreateShader" );
		
		if ( shader > 0 )
		{

			// upload source
			
			glShaderSource( shader , 1 , &source , NULL );
            shader_printerrors( "ShaderSource" );

            // compile
            
            glCompileShader( shader );
            shader_printerrors( "compile_shader_type_source" );

            // get compile log
            
            glGetShaderiv( shader , GL_INFO_LOG_LENGTH , &logLength );

            if ( logLength > 0 )
            {
                log = ( GLchar* ) malloc( logLength );
                glGetShaderInfoLog( shader , logLength , &realLength , log );
                printf("\nShader compile log:\n%s" , log );
                free( log );
            }
            
            // get status
            
            glGetShaderiv( shader , GL_COMPILE_STATUS , &status );
            
            if ( status != GL_TRUE ) return 0;
			
		}
		else printf( "\nCannot create shader" );
		
		return shader;
        
    }


    // link shaders together in gpu

	char shader_link ( GLuint program )
    {

        GLint status, logLength;
		GLchar* log;
		
		// link program

        glLinkProgram( program );
        shader_printerrors( "LinkProgram" );
		
		// get linkage log
        
        glGetProgramiv( program , GL_INFO_LOG_LENGTH , &logLength );

        if ( logLength > 0 )
        {
            log = ( GLchar* ) malloc( logLength );
            glGetProgramInfoLog( program , logLength , &logLength , log );
            printf("\nShader : Program link log:\n%s" , log );
            free( log );
        }
		

		// get status
        
        glGetProgramiv( program , GL_LINK_STATUS , &status );
        
        if ( status == GL_TRUE ) return 1;
        
        return 0;
		
    }
