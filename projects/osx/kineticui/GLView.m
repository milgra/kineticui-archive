//
//  GLView.m
//  DynamicsX
//
//  Created by Milan Toth on 9/4/13.
//  Copyright (c) 2013 Milan Toth. All rights reserved.
//

	#import "GLView.h"
	#import "AppDelegate.h"

	@implementation GLView
	{
        float scale;
		NSTimer* timer;
		NSPoint last;
		ui_t* controller;
		NSMutableArray* deltaArray;
	}

	static GLView* staticView;

	- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format
	{
		self = [super initWithFrame:frameRect pixelFormat:format];
		if ( self )
		{
			[self setWantsBestResolutionOpenGLSurface:YES];
			[ self.openGLContext makeCurrentContext ];
			staticView = self;
			deltaArray = [ [ NSMutableArray alloc ] init ];
		}
		return self;
	}

	- (void)load
	{
		NSString* resPath = [ [ NSBundle mainBundle ] resourcePath ];
		NSString* libPath = [ NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex : 0 ];
		NSRect backingBounds = [self convertRectToBacking:[self bounds]];
		scale = backingBounds.size.width / self.bounds.size.width;
        printf( "scale %f" , scale );
        c36_t* c36 = c36_init();
        demo_square_t* demo_a = demo_square_init();
        demo_textarea_t* demo_b = demo_textarea_init();
        demo_scroller_t* demo_c = demo_scroller_init();
        demo_itemlist_t* demo_d = demo_itemlist_init();
        demo_kineticui_t* demo_e = demo_kineticui_init();
        demo_kineticlabel_t* demo_f = demo_kineticlabel_init();
		strips_t* strips = strips_init();
		controller = ui_init(
			backingBounds.size.width ,
			backingBounds.size.height ,
			scale ,
			(char*) [ resPath cStringUsingEncoding : NSUTF8StringEncoding ] ,
			(char*) [ libPath cStringUsingEncoding : NSUTF8StringEncoding ] ,
            ( uidelegate_t* ) c36 );
            
		timer = [ NSTimer scheduledTimerWithTimeInterval : 1.0 / 60.0
						  target : self
						  selector : @selector(timerEvent)
						  userInfo : nil
						  repeats : YES ];
	}

	- (void)mouseDown:(NSEvent*)theEvent
	{
		NSRect backingBounds = [self convertRectToBacking:[self bounds]];
		NSPoint location = [ theEvent locationInWindow ];
		last = [ NSEvent mouseLocation ];
		self.position = self.window.frame.origin;
		ui_touch_down( controller , location.x * scale , location.y * scale - backingBounds.size.height );
	}

	- (void)mouseUp:(NSEvent*)theEvent
	{
		NSRect backingBounds = [self convertRectToBacking:[self bounds]];
		NSPoint location = [ theEvent locationInWindow ];
		ui_touch_up( controller , location.x * scale , location.y * scale - backingBounds.size.height );
	}

	-(void)mouseDragged:(NSEvent*)theEvent
	{
		NSRect backingBounds = [self convertRectToBacking:[self bounds]];
		NSPoint location = [ theEvent locationInWindow ];
		NSPoint mouse = [ NSEvent mouseLocation ];
		NSPoint delta = NSMakePoint( mouse.x - last.x , mouse.y - last.y );
		ui_touch_move( controller , location.x * scale , location.y * scale - backingBounds.size.height );
		
		if ( self.dragged )
		{
			NSRect windowFrame = [self.window frame];
			NSPoint newOrigin = windowFrame.origin;

			newOrigin.x = self.position.x + delta.x;
			newOrigin.y = self.position.y + delta.y;

			[self.window setFrameOrigin:newOrigin];
		}
	}

	-(BOOL)recognizeTwoFingerGestures
	{
		return YES;
	}

    - (void)keyDown:(NSEvent*)theEvent
    {
		char* chars = ( char* ) [ theEvent.characters cStringUsingEncoding : NSUTF8StringEncoding ];
		if ( chars[0] == 127 )ui_key_down( controller , NULL , kInputKeyTypeBackspace );
		else if ( chars[0] == 13 || chars[0] == 10 ) ui_key_down( controller , NULL , kInputKeyTypeReturn );
		else ui_key_down( controller , chars , kInputKeyTypeNormal );
    }
    
    - (void)keyUp:(NSEvent*)theEvent
    {

    }

	- (void)scrollWheel:(NSEvent*)event
	{
		NSRect backingBounds = [self convertRectToBacking:[self bounds]];
		// NSLog( @"scroll %@" , event );
		[ deltaArray addObject : [ NSValue valueWithPoint : NSMakePoint( event.deltaX , event.deltaY ) ] ];
		if ( [deltaArray count] > 4 ) [deltaArray removeObjectAtIndex:0];

		// normal scroll events
		if ( event.phase == NSEventPhaseBegan )
		{
			ui_scroll_start( controller , event.locationInWindow.x * scale , ( event.locationInWindow.y - backingBounds.size.height ) * scale );
		}
		else if ( event.phase == NSEventPhaseChanged )
		{
			ui_scroll_move( controller , event.deltaX * 4.0 , event.deltaY * 4.0);
		}
		else if ( event.phase == NSEventPhaseEnded || event.phase == NSEventPhaseCancelled )
		{
			ui_scroll_end( controller , 0.0 , 0.0 );
		}
		
		// scroll momentum events
		if ( event.momentumPhase == NSEventPhaseChanged )
		{
			ui_scroll_move( controller , event.deltaX * 4.0 , event.deltaY * 4.0);
		}
		else if ( event.momentumPhase == NSEventPhaseEnded ||
				  event.momentumPhase == NSEventPhaseCancelled )
		{
			float sumx = 0;
			float sumy = 0;
			int count = 0;
			for ( int index = 0 ; index < [deltaArray count] ; index++ )
			{
				NSValue* value = [deltaArray objectAtIndex:index];
				if ( value != nil )
				{
					CGPoint point = [value pointValue];
					sumx += point.x;
					sumy += point.y;
					count++;
				}
			}
			if ( fabs( sumx / count ) > 0.01 || fabs( sumy / count ) > 0.01 )
			{
				ui_scroll_end( controller , event.deltaX * 2.0 , event.deltaY * 2.0 );
			}
			else
			{
				ui_scroll_end( controller , 0.0 , 0.0 );
			}
			[deltaArray removeAllObjects];
		}

	}

	- (void)timerEvent
	{
		ui_timer( controller );
	}

	- (void)viewDidChangeBackingProperties
	{
		// self.layer.contentsScale = [[self window] backingScaleFactor];
	}

	- (void)update
	{
		[self.openGLContext update];
		[ staticView.openGLContext flushBuffer ];
		ui_resize( controller , self.frame.size.width * scale , self.frame.size.height * scale , 1 );
	}

	- (void)render
	{
		ui_render( controller , 1 );
	}

	- (BOOL)canBecomeFirstResponder
	{
		return YES;
	}

	@end

	void proxy_flushbuffer( )
	{
		[ staticView.openGLContext flushBuffer ];
	}
	
	void proxy_dragwindow( )
	{
		staticView.dragged = YES;
	}

	void proxy_dropwindow( )
	{
		staticView.dragged = NO;
	}

	void proxy_showkeyboard( )
	{

	}

	void proxy_hidekeyboard( )
	{

	}