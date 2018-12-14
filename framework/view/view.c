//
//  view.c
//  KineticUI
//
//  Created by Milan Toth on 06/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "view.h"

	// creates new view

    view_t* view_default( float x , float y , float width , float height )
	{
        view_t* view = malloc( sizeof( view_t ) );

		// public properties
        view->name			  = string_copycstring( "default" );
        view->path			  = string_copycstring( "default" );
		view->width			  = width;
		view->height		  = height;
        view->invisible		  = 0;		// view is excluded from quad generation, childs not
		view->focuslevel	  = 0;		// 0 - cannot get focus , 1 - gets focus , 2 - gets focus, ignores underlyings

		// internal
        view->nodraw		  = 0;		// there's no need to draw view or subviews because view is cropped out
        view->redraw		  = 1;		// position, dimension or texture change, forcing redraw
        view->vertexes		  = malloc( sizeof( GLfloat ) * 4 * 4 );	// vertex data cache
        view->subviews		  = vector_create( );	// subview list
		view->basicmodifiers  = vector_create( );	// basic modifiers
		view->finalmodifiers  = vector_create( );	// final geometry modifiers

		// stored corners, for cropping and faster quad generation
		view->corners.ul = vector3_default( 0.0   , 0.0    , 1.0 );
		view->corners.ur = vector3_default( width , 0.0    , 1.0 );
		view->corners.ll = vector3_default( 0.0   , height , 1.0 );
		view->corners.lr = vector3_default( width , height , 1.0 );
		view->realcorners = view->corners;

		// actual texture coords
		view->coords.ul = vector2_default( 0.0  , 1.0 );
		view->coords.ur = vector2_default( 1.0  , 1.0 );
		view->coords.ll = vector2_default( 0.0  , 0.0 );
		view->coords.lr = vector2_default( 1.0  , 0.0 );
		view->realcoords = view->coords;

		// trafo, position and combined(parent*position*trao) matrixes
		view->matrixes.position = matrix3_defaulttranslation( x , y );
		view->matrixes.modifier = matrix3_defaultidentity( );
		view->matrixes.combined = matrix3_defaultidentity( );

        return view;
	}

    // deletes view

    void view_free( view_t* view )
    {
		// delete basicmodifiers, they will delete their subviews !!! TODO
		for ( int index = 0 ; index < view->basicmodifiers->length ; index++ )
		{
			modifier_t* one_modifier = view->basicmodifiers->data[ index ];
			one_modifier->_free( one_modifier );
		}

		// delete finalmodifiers, they will delete their subviews !!! TODO
		for ( int index = 0 ; index < view->finalmodifiers->length ; index++ )
		{
			modifier_t* one_modifier = view->finalmodifiers->data[ index ];
			one_modifier->_free( one_modifier );
		}
		
		// delete subviews
		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* one_view = view->subviews->data[ index ];
			view_free( one_view );
		}
		
		// delete remaining stuff
		vector_free( view->basicmodifiers );
		vector_free( view->finalmodifiers );
		vector_free( view->subviews );
		free( view->vertexes );
		free( view->name );
		free( view );
    }

	// generate new modifier states and new view state with them

	void view_new( modifier_args_t* args )
	{
		// loop through all modifiers, let them generate new states
		for ( int index = 0 ; index < args->views.view->basicmodifiers->length ; index++ )
		{
			modifier_t* modifier = args->views.view->basicmodifiers->data[ index ];
			if ( modifier->_new != NULL )
			{
				modifier->_new( modifier , args );
			}
		}
		
		// go through subviews
		if ( args->generator.subviews_flag == 1 )
		{
			for ( int index = 0 ; index < args->views.view->subviews->length ; index++ )
			{
				view_t* subview =  args->views.view->subviews->data[ index ];
				view_new( &( ( modifier_args_t ) {
					.views      = { subview	, args->views.view } ,
					.generator  = args->generator ,
					.command    = args->command ,
					.toolset    = args->toolset	} ) );
			}
		}
	}

    // updates matrix chain

    void view_finalize_geometry( view_t* view , view_t* parent , matrix3_t matrix_parent , char parent_changed , toolset_t* toolset , uint32_t* counter , uint32_t* quadcounter )
    {
		*quadcounter += 1;
		if ( view->redraw == 1 || parent_changed )
		{
			view->redraw = 1;
			view->matrixes.combined = matrix3_multiply( view->matrixes.position , view->matrixes.modifier );
			view->matrixes.combined = matrix3_multiply( matrix_parent , view->matrixes.combined );
			
			// calculate new realcorners
			view->realcorners.ul = matrix3_multiply_vector3( view->matrixes.combined , view->corners.ul );
			view->realcorners.ur = matrix3_multiply_vector3( view->matrixes.combined , view->corners.ur );
			view->realcorners.ll = matrix3_multiply_vector3( view->matrixes.combined , view->corners.ll );
			view->realcorners.lr = matrix3_multiply_vector3( view->matrixes.combined , view->corners.lr );
			
			// update geometry modifiers
			if ( view->finalmodifiers->length > 0 )
			{
				for ( int index = 0 ; index < view->finalmodifiers->length ; index++ )
				{
					modifier_t* modifier = view->finalmodifiers->data[index];
					modifier->_new( modifier , &( ( modifier_args_t ) {
						.views      = { view		, parent } ,
						.generator  = { &freechar	, 1		 } ,
						.command    = { "update"	, NULL	 } ,
						.toolset    = toolset				 } ) );
				}
			}

			parent_changed = 1;
			*counter += 1;
		}
		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* subview = view->subviews->data[index];
			view_finalize_geometry( subview , view , view->matrixes.combined , parent_changed , toolset , counter , quadcounter );
		}
	}

    // generates gl quads for render

    void view_collect_vertexes( view_t* view , GLfloat* collector , uint32_t* quadcount )
	{
        if ( view->invisible == 0 && view->nodraw == 0 )
        {
			if ( view->redraw == 1 )
			{
				GLfloat local[ ] =
				{
					view->realcorners.ul.x , view->realcorners.ul.y , view->realcoords.ul.x , view->realcoords.ul.y ,
					view->realcorners.ll.x , view->realcorners.ll.y , view->realcoords.ll.x , view->realcoords.ll.y ,
					view->realcorners.lr.x , view->realcorners.lr.y , view->realcoords.lr.x , view->realcoords.lr.y ,
					view->realcorners.ur.x , view->realcorners.ur.y , view->realcoords.ur.x , view->realcoords.ur.y
				};
				memcpy( view->vertexes , local , sizeof( GLfloat ) * 4 * 4 );
				view->redraw = 0;
			}
			memcpy( collector + *quadcount * 16 , view->vertexes , sizeof( GLfloat ) * 4 * 4 );
			*quadcount += 1;
        }
		if ( view->nodraw == 0 )
		{
			for ( int index = 0 ; index < view->subviews->length ; index++ )
			{
				view_t* subview = view->subviews->data[index];
				view_collect_vertexes( subview , collector , quadcount );
			}
		}
	}

    // collects views that intersects with touch direction

    void view_collect_touched_views( view_t* view , vector_t* vector , float touch_x , float touch_y )
    {
        if ( view->focuslevel > 0 )
        {
			if ( touch_x >= view->realcorners.ul.x && touch_x <= view->realcorners.ur.x &&
				 touch_y <= view->realcorners.ul.y && touch_y >= view->realcorners.ll.y ) vector_adddata( vector , view );
			else return;
        }
		// check subviews
		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* subview = view->subviews->data[index];
			view_collect_touched_views( subview , vector , touch_x , touch_y );
		}
    }

	// sets name

	void view_setname( view_t* view , char* name )
	{
		string_t* oldname = string_createfrombytes( view->name );
		string_t* oldpath = string_createfrombytes( view->path );
		// remove old name from path
		string_removecodepointsinrange( oldpath , oldpath->length - oldname->length , oldpath->length - 1 );
		string_addbytearray( oldpath , name );
		// free old names
		free( view->name );
		free( view->path );
		// assign new names
		view->name = string_copycstring( name );
		view->path = string_bytes( oldpath );
		// cleanup
		string_free( oldname );
		string_free( oldpath );
	}

	// returns position

	vector2_t view_getposition( view_t* view )
	{
		return vector2_default( view->matrixes.position.m02 , view->matrixes.position.m12 );
	}

	// sets position

	void view_setposition( view_t* view , vector2_t position )
	{
		view->matrixes.position.m02 = position.x;
		view->matrixes.position.m12 = position.y;
		view->redraw = 1;
	}

	// returns position

	vector2_t view_getsize( view_t* view )
	{
		return vector2_default( view->width , view->height );
	}

	// sets position

	void view_setsize( view_t* view , vector2_t size )
	{
		view->width = size.x;
		view->height = size.y;
		view->corners.ul = vector3_default( 0.0    , 0.0    , 1.0 );
		view->corners.ur = vector3_default( size.x , 0.0    , 1.0 );
		view->corners.ll = vector3_default( 0.0    , size.y , 1.0 );
		view->corners.lr = vector3_default( size.x , size.y , 1.0 );
		view->redraw = 1;
	}

	// returns square

	square2_t view_getsquare( view_t* view )
	{
		return square_default( vector2_default( view->matrixes.position.m02 , view->matrixes.position.m12 ) , vector2_default( view->width , view->height ) );
	}

	// sets square

	void view_setsquare( view_t* view , square2_t square )
	{
		view_setposition( view , square.origo );
		view_setsize( view , square.extent );
	}

    // adds subview

    void view_addsubview( view_t* view , view_t* subview )
    {
		assert( view != NULL );	// if view is null, then descriptor text is probably invalid
		if ( vector_indexofdata( view->subviews , subview ) == UINT32_MAX )
		{
			vector_adddata( view->subviews , subview );
			view_updatepath( subview , view->path );
		}
    }

	// adds subview at given index

    void view_addsubviewatindex( view_t* view , view_t* subview , uint32_t index )
    {
		assert( view != NULL );	// if view is null, then descriptor text is probably invalid
		if ( vector_indexofdata( view->subviews , subview ) == UINT32_MAX )
		{
			vector_adddataatindex( view->subviews , subview , index );
			view_updatepath( subview , view->path );
		}
    }

    // removes subview

    void view_removesubview( view_t* view , view_t* subview )
    {
		assert( view != NULL );
		vector_removedata( view->subviews , subview );
    }

	// checks for subview

    char view_containssubview( view_t* view , view_t* subview )
    {
		return ( vector_indexofdata( view->subviews , subview ) != UINT32_MAX );
    }

	// update view path

	void view_updatepath( view_t* view , char* parentpathc )
	{
		string_t* parentpath = string_createfrombytes( parentpathc );
		string_addbytearray( parentpath , "/" );
		string_addbytearray( parentpath , view->name );
		free( view->path );
		view->path = string_bytes( parentpath );
		string_free( parentpath );
		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* subview = view->subviews->data[ index ];
			view_updatepath( subview , view->path );
		}
	}

    // adds modifier

    void view_addmodifier( view_t* view , modifier_t* modifier , char geometry )
    {
		assert( modifier != NULL );	// if modifier is null, then descriptor text is probably invalid
		if ( geometry == 0 ) vector_adddata( view->basicmodifiers , modifier );
		else vector_adddata( view->finalmodifiers , modifier );
    }

    // removes modifier

    void view_removemodifier( view_t* view , modifier_t* modifier )
    {
		assert( modifier != NULL );
        vector_removedata( view->basicmodifiers , modifier );
    }

	// returns modifier with given type

	void* view_getmodifier( view_t* view , char* type )
	{
		for ( int index = 0 ; index < view->basicmodifiers->length ; index++ )
		{
			modifier_t* one_modifier = view->basicmodifiers->data[ index ];
			if ( strcmp( one_modifier->type , type ) == 0 ) return one_modifier;
		}
		return NULL;
	}

	// returns subview with given name

	view_t* view_getsubview( view_t* view , char* name )
	{
		if ( strcmp( view->name , name ) == 0 ) return view;
		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* subview = view->subviews->data[ index ];
			view_t* result = view_getsubview( subview , name );
			if ( result != NULL ) return result;
		}
		return NULL;
	}

	// collects subviews in one flat vector

	void view_collect_subviews( view_t* view , vector_t* vector )
	{
		vector_adddatainvector( vector , view->subviews );
		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* subview = view->subviews->data[index];
			view_collect_subviews( subview, vector );
		}
	}

	// describes view to standard output

	void view_describe( view_t* view , int level )
	{
		printf( "\n" );
		for ( int index = 0 ; index < level ; index++ ) printf( " " );
		printf( "%s %.2f %.2f " , view->name , view->width , view->height );

		for ( int index = 0 ; index < view->basicmodifiers->length ; index++ )
		{
			modifier_t* one_modifier = view->basicmodifiers->data[ index ];
			printf( "%s," , one_modifier->type );
		}

		for ( int index = 0 ; index < view->subviews->length ; index++ )
		{
			view_t* subview = view->subviews->data[index];
            view_describe( subview , level + 1 );
        }
	}
