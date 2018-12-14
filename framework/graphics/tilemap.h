//
//  tilemap.h
//  KineticUI
//
//  Created by Milan Toth on 05/10/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

    #ifndef __KineticUI__tilemap__
    #define __KineticUI__tilemap__


    #include "../core/map.h"
    #include "../core/math2.h"
    #include "bitmap.h"


    typedef struct _tilemap_t tilemap_t;
    struct _tilemap_t
    {
        map_t* posmap;
        bitmap_t* bitmap;
        uint8_t updated;
        uint32_t north_height;
        uint32_t north_x;
        uint32_t north_y;
    };


    tilemap_t* tilemap_create( );
	void tilemap_free( tilemap_t* tilemap );
    void tilemap_reset( tilemap_t* tilemap );
    vector2_t* tilemap_get( tilemap_t* tilemap , char* name );
    vector2_t* tilemap_put( tilemap_t* tilemap , char* name , bitmap_t* tile , int inset );


    #endif /* defined(__KineticUI__tilemap__) */
