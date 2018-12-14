//
//  GLView.h
//  DynamicsX
//
//  Created by Milan Toth on 9/4/13.
//  Copyright (c) 2013 Milan Toth. All rights reserved.
//

	#import <Cocoa/Cocoa.h>
	#import "ui.h"
    #import "c36.h"
	#import "strips.h"
    #import "demo_square.h"
    #import "demo_textarea.h"
    #import "demo_scroller.h"
    #import "demo_itemlist.h"
    #import "demo_kineticui.h"
    #import "demo_kineticlabel.h"

	@interface GLView : NSOpenGLView

	-(void)load;
	- (void)render;

	@property (readwrite) NSPoint position;
	@property (readwrite) BOOL dragged;

	@end