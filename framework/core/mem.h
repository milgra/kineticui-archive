//
//  memguard.h
//  c36
//
//  Created by mtoth on 02/06/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

#ifndef __c36__mem__
#define __c36__mem__


    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>

    void* mem_calloc( void* oldpointer , size_t elements , size_t size , const char* place );
    void* mem_malloc( void* oldpointer , size_t size , const char* place );
    void* mem_realloc( void* pointer , size_t size , const char* place );
    void  mem_free( void* pointer , const char* place );


#endif /* defined(__c36__memguard__) */
