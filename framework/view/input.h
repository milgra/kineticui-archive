//
//  input.h
//  KineticUI
//
//  Created by Milan Toth on 12/11/14.
//  Copyright (c) 2014 Milan Toth. All rights reserved.
//

	#ifndef KineticUI_input_h
	#define KineticUI_input_h

    #include "string.h"

	#define kInputTypeInit			0
	#define kInputTypeUpdate		1
	#define kInputTypeTouchDrag 	2
	#define kInputTypeTouchDown 	3
	#define kInputTypeTouchUp		4
	#define kInputTypeScroll		5
	#define kInputTypeScrollEnd		6
	#define kInputTypeLayoutFix		7
	#define kInputTypeLayoutUpdate	8
	#define kInputTypeFocus			9
	#define kInputTypeUnFocus		10
	#define kInputTypeKeyPress		11
	#define kInputTypeTimer			12
	#define kInputTypeRefillTileMap	13

	#define kInputKeyTypeNormal		0
	#define kInputKeyTypeReturn		1
	#define kInputKeyTypeBackspace	2
	#define kInputKeyTypeShift		3

    typedef struct _input_t input_t;
    struct _input_t
    {
		// input type
		char type;
		
		// touch/scroll
		float value_x;
		float value_y;

		// key
		char key;
		string_t* characters;
		
		// layout
    };

	#endif
