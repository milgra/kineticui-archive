//
//  modifier_animation.c
//  KineticUI
//
//  Created by Milan Toth on 28/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#include "modifier_animator.h"

	void modifier_animator_new( modifier_t* modifier , modifier_args_t* args );
	void modifier_animator_free( modifier_t* modifier );

	// animates transformation matrixes

	// creates animator for view with starting transformation

	modifier_t* modifier_animator_default( transformation_t transformation )
	{
		modifier_t* modifier		= malloc( sizeof( modifier_t ));
		modifier_animator_t* data	= malloc( sizeof( modifier_animator_t ) );

		modifier->type = "animator";
		modifier->data = data;
		modifier->_new = modifier_animator_new;
		modifier->_free = modifier_animator_free;

		data->queue		= vector_create();	// transformation queue
		data->actual    = transformation;	// actual transformation
		data->playing	= 0;				// are we playing something
		data->action	= NULL;

		return modifier;
	}

	// deletes animator

	void modifier_animator_free( modifier_t* modifier )
	{
		modifier_animator_t* data = modifier->data;
	
		if ( data->action != NULL ) free( data->action );
		vector_free( data->queue );
		free( data );
		free( modifier );
	}

	// applies actual transformation to view

	void modifier_animator_apply_actual( modifier_t* modifier , view_t* view )
	{
		modifier_animator_t* data = modifier->data;

		matrix3_t matrix = matrix3_defaultidentity( );
		// matrix = matrix3_multiply( matrix , matrix3_defaulttranslation( view->width / 2.0 , view->height / 2.0 ) );
		matrix = matrix3_multiply( matrix , matrix3_defaulttranslation( data->actual.translation.x , data->actual.translation.y ) );
		matrix = matrix3_multiply( matrix , matrix3_defaultscale( data->actual.scale.x , data->actual.scale.y ) );
		matrix = matrix3_multiply( matrix , matrix3_defaultrotationz( data->actual.rotation.z ) );
		// matrix = matrix3_multiply( matrix , matrix3_defaulttranslation( -view->width / 2.0 , -view->height / 2.0 ) );
		view->matrixes.modifier = matrix;
		view->redraw = 1;
	}

	// plays next phase

	void modifier_animator_playnext( modifier_t* modifier , char force )
	{
		modifier_animator_t* data = modifier->data;

		if ( data->playing == 0 || force )
		{
		
			if ( data->playing == 1 )
			{
				if ( data->action != NULL ) free( data->action );
			}
		
			if ( data->queue->length > 0 )
			{
				animation_phase_t* phase = data->queue->data[ 0 ];
				vector_removedataatindex( data->queue , 0 );
				
				vector3_t translation_diff = vector3_sub( phase->transformation.translation , data->actual.translation );
				vector3_t rotation_diff = vector3_sub( phase->transformation.rotation , data->actual.rotation );
				vector3_t scale_diff = vector3_sub( phase->transformation.scale , data->actual.scale );
				
				if ( phase->easing == 0 )
				{
					data->delta.translation = vector3_scale( translation_diff , 1.0 / (float) phase->steps );
					data->delta.rotation = vector3_scale( rotation_diff , 1.0 / (float) phase->steps );
					data->delta.scale = vector3_scale( scale_diff , 1.0 / (float) phase->steps );
				}
				else
				{
					int sum = 0;
					for ( int index = 0 ; index < phase->steps + 1 ; index++ ) sum += index;
					data->delta.translation = vector3_scale( translation_diff , 1.0 / sum );
					data->delta.rotation = vector3_scale( rotation_diff , 1.0 / sum );
					data->delta.scale = vector3_scale( scale_diff , 1.0 / sum );
				}
				data->steps_left = phase->steps;
				data->playing = 1;
				data->easing = phase->easing;
				data->action = string_copycstring( phase->action );
				
				if ( phase->action != NULL ) free( phase->action );
				free( phase );
			}
			else data->playing = 0;
		}
	}

	// animator stepping from timer

	void modifier_animator_timer( modifier_t* modifier , view_t* view , vector_t* events , char* changed )
	{
		modifier_animator_t* data = modifier->data;

		if ( data->playing == 1 )
		{
			if ( data->steps_left > 0 )
			{
				if ( data->easing == 0 )
				{
					data->actual.translation = vector3_add( data->actual.translation , data->delta.translation );
					data->actual.rotation = vector3_add( data->actual.rotation , data->delta.rotation );
					data->actual.scale = vector3_add( data->actual.scale , data->delta.scale );
				}
				else
				{
					vector3_t actual_translation = vector3_scale( data->delta.translation , data->steps_left );
					vector3_t actual_rotation = vector3_scale( data->delta.rotation , data->steps_left );
					vector3_t actual_scale = vector3_scale( data->delta.scale , data->steps_left );
					data->actual.translation = vector3_add( data->actual.translation , actual_translation );
					data->actual.rotation = vector3_add( data->actual.rotation , actual_rotation );
					data->actual.scale = vector3_add( data->actual.scale , actual_scale );
				}

				*changed = 1;
				data->steps_left -= 1;
			}
			else
			{
				if ( data->action != NULL )
				{
					object_t* eventobject = modifier_defaultevent( "animationend", data->action , modifier , NULL );
					vector_adddata( events , eventobject );
					free( data->action );
					data->action = NULL;
				}
				data->playing = 0;
				modifier_animator_playnext( modifier , 0 );
				// don't force remove from timeds if there is a next phase
				if ( data->playing == 1 ) *changed = 1;
			}
		}
	}

	void modifier_animator_new( modifier_t* modifier , modifier_args_t* args )
	{
        assert( args != NULL );
        assert( modifier != NULL );

		modifier_animator_t* data = modifier->data;

		if ( strcmp( args->command.name , "timer" ) == 0 )
		{
			if ( data->playing == 1 )
			{
				modifier_animator_timer( modifier , args->views.view , args->toolset->events , args->generator.changed_flag );
				modifier_animator_apply_actual( modifier , args->views.view );
			}
		}
		else if ( strcmp( args->command.name , "setphase" ) == 0 )
		{
            animation_phase_t* phase = args->command.arguments;

            data->actual = phase->transformation;
            modifier_animator_apply_actual( modifier , args->views.view );
            
		}
		else if ( strcmp( args->command.name , "addphase" ) == 0 )
		{
            animation_phase_t* phase = args->command.arguments;
            animation_phase_t* copy = malloc( sizeof( animation_phase_t ) );

            memcpy(copy, phase, sizeof(animation_phase_t));
			
			copy->action = string_copycstring( phase->action );
                    
            if ( phase->force == 1 ) vector_reset( data->queue );

            vector_adddata( data->queue , copy );
            modifier_animator_playnext( modifier , phase->force );
		}
	}
	