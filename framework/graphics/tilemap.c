//
//  tilemap.c
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #include "tilemap.h"


    // creates tile map hashmap

    tilemap_t* tilemap_create( )
    {
    
        tilemap_t* tilemap = malloc( sizeof( tilemap_t ) );
        tilemap->bitmap = bitmap_create( 1024 , 1024 );
        tilemap->posmap = map_create( 1000 );
        tilemap->updated = 0;
        tilemap->north_height = 0;
        tilemap->north_x = 0;
        tilemap->north_y = 0;
        
        return tilemap;
    }

	// deletes tile map

	void tilemap_free( tilemap_t* tilemap )
	{
		// delete coordinates from posmap
		vector_t* coords = map_values( tilemap->posmap );
		for ( int index = 0 ; index < coords->length ; index++ ) free( coords->data[ index ] );
		vector_free( coords );
		map_free( tilemap->posmap );
		bitmap_free( tilemap->bitmap );
		free( tilemap );
	}

	// resets tilemap

    void tilemap_reset( tilemap_t* tilemap )
	{
		vector_t* coords = map_values( tilemap->posmap );
		for ( int index = 0 ; index < coords->length ; index++ ) free( coords->data[ index ] );
		vector_free( coords );
		map_free( tilemap->posmap );
		bitmap_free( tilemap->bitmap );
        tilemap->bitmap = bitmap_create( 1024 , 1024 );
        tilemap->posmap = map_create( 1000 );
        tilemap->north_height = 0;
        tilemap->north_x = 0;
        tilemap->north_y = 0;
        tilemap->updated = 1;
	}

    // returns tile info in tilemap

    vector2_t* tilemap_get( tilemap_t* tilemap , char* name )
    {
        vector2_t* coordinates = map_get( tilemap->posmap , name );
		// if ( coordinates != NULL ) printf( "\ngetting from tilemap %s coords %f %f" , name , coordinates->x , coordinates->y );
        return coordinates;
    }


    // puts tile info and tile into tilemap

    vector2_t* tilemap_put( tilemap_t* tilemap , char* name , bitmap_t* tile , int inset )
    {
        // check dimensions
    
        if ( tile->width > tilemap->bitmap->width || tile->height > tilemap->bitmap->height ) { printf( "\nToo big tile" ); return NULL; }
    
        if ( tilemap->north_x + tile->width >= tilemap->bitmap->width )
        {
            tilemap->north_y += tilemap->north_height + 1;
            tilemap->north_x = 0;
            tilemap->north_height = 0;
        }

        if ( tilemap->north_y + tile->height >= tilemap->bitmap->height )
		{
			printf( "\nToo tall tile" );
			return NULL;
		}
        
        // add tile to bitmap
        
        bitmap_blend( tilemap->bitmap , tile , tilemap->north_x , tilemap->north_y );
        
        // calculate relative corner positions for textures

        float ulx = ( float ) ( tilemap->north_x + inset ) / ( float ) tilemap->bitmap->width;
        float uly = ( float ) ( tilemap->north_y + inset ) / ( float ) tilemap->bitmap->height;
        float urx = ( float ) ( tilemap->north_x + tile->width - inset ) / ( float ) tilemap->bitmap->width;
        float ury = uly;
        float llx = ulx;
        float lly = ( float ) ( tilemap->north_y + tile->height - inset ) / ( float ) tilemap->bitmap->height;
        float lrx = urx;
        float lry = lly;

        vector2_t* coordinates = malloc( sizeof( vector2_t ) * 4 );
        coordinates[0] = vector2_default( ulx , uly );
        coordinates[1] = vector2_default( urx , ury );
        coordinates[2] = vector2_default( llx , lly );
        coordinates[3] = vector2_default( lrx , lry );

        // increase cursor

        tilemap->north_x += tile->width + 1;
        if ( tilemap->north_height < tile->height ) tilemap->north_height = tile->height;
        
        map_put( tilemap->posmap , name , coordinates );
        tilemap->updated = 1;

        // printf( "\nadding to tilemap %s %i %i coords %f %f" , name , tile->width , tile->height , ulx , uly );
		
        return coordinates;
        
    }

