//
//  view_types.h
//  kineticui
//
//  Created by mtoth on 24/07/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef kineticui_view_types_h
#define kineticui_view_types_h

    #ifdef IOS
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/ES2/glext.h>
    #elif defined OSX
    #include <OpenGL/gl3.h>
    #elif defined BRY
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #endif

    #include "../core/math2.h"
    #include "../core/math3.h"
    #include "../core/math4.h"
    #include "../core/vector.h"
    #include "../core/string.h"

	typedef struct _toolset_t toolset_t;
	struct _toolset_t
	{
		font_t* font;
		string_t* path;
		vector_t* logs;
		vector_t* events;
		vector_t* timeds;
		tilemap_t* tilemap;
		uisettings_t* settings;
	};

	typedef struct _corners_t corners_t;
	struct _corners_t
	{
		vector3_t ul;
		vector3_t ur;
		vector3_t ll;
		vector3_t lr;
	};

	typedef struct _texcoords_t texcoords_t;
	struct _texcoords_t
	{
		vector2_t ul;
		vector2_t ur;
		vector2_t ll;
		vector2_t lr;
	};

	typedef struct _matrixes_t matrixes_t;
	struct _matrixes_t
	{
		matrix3_t modifier;
		matrix3_t position;
		matrix3_t combined;
	};

    typedef struct _view_t view_t;
    struct _view_t
    {
        char*   	name;
		char*		path;
		float		width;
		float		height;
        char        invisible;
		char		focuslevel;
		
		char		nodraw;
		char		redraw;

        GLfloat*    vertexes;
        vector_t*   subviews;
		vector_t*	basicmodifiers;
		vector_t*	finalmodifiers;
		
		matrixes_t	matrixes;
		
		corners_t	corners;
		corners_t	realcorners;
		
		texcoords_t	coords;
		texcoords_t	realcoords;
    };

#endif
