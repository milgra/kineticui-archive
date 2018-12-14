//
//  AppDelegate.m
//  c36
//
//  Created by Milan Toth on 22/01/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#import "AppDelegate.h"

	#define wth 1000
	#define hth 800

	@implementation AppDelegate

	static BOOL fullscreen = NO;
	static NSRect last;
	static GLView* glview;
	static NSWindow* window;

    - (void) applicationDidFinishLaunching : (NSNotification*) theNotification
    {
        NSOpenGLPixelFormatAttribute attributes[] = { NSOpenGLPFADoubleBuffer,NSOpenGLPFAOpenGLProfile,NSOpenGLProfileVersionLegacy,0};
        glview = [ [ GLView alloc	] initWithFrame		  : NSMakeRect( 0 , 0 , wth , hth )
									  pixelFormat		  : [ [ NSOpenGLPixelFormat alloc ] initWithAttributes : attributes ] ];
        window = [ [ GLWindow alloc ] initWithContentRect : NSMakeRect( ( [ [ NSScreen mainScreen ] frame ].size.width - wth ) / 2 , ( [ [ NSScreen mainScreen ] frame ].size.height  - hth ) / 2 , wth , hth )
									  styleMask           : NSBorderlessWindowMask | NSClosableWindowMask | NSResizableWindowMask
									  backing             : NSBackingStoreBuffered
									  defer               : YES ];
        [ glview setAutoresizingMask : NSViewWidthSizable | NSViewHeightSizable ];
		[ glview setAcceptsTouchEvents : YES ];
        [ window setLevel : NSNormalWindowLevel ];
        [ window setDelegate : self ];
        [ window setHasShadow : YES ];
        [ window setContentView : glview ];
        [ window setAcceptsMouseMovedEvents : YES ];
        [ window makeKeyAndOrderFront : self ];
        [ window makeFirstResponder : glview ];
        [ window makeMainWindow ];
		[ NSApp activateIgnoringOtherApps : YES ];
		[ glview load ];
	}

	-(BOOL)applicationShouldHandleReopen:(NSApplication *)sender hasVisibleWindows:(BOOL)flag
	{
		if ( window == nil )
		{
			window = [ [ GLWindow alloc ] initWithContentRect : NSMakeRect( ( [ [ NSScreen mainScreen ] frame ].size.width - wth ) / 2 , ( [ [ NSScreen mainScreen ] frame ].size.height  - hth ) / 2 , wth , hth )
										  styleMask           : NSBorderlessWindowMask | NSClosableWindowMask | NSResizableWindowMask
										  backing             : NSBackingStoreBuffered
										  defer               : YES ];
			[ window setLevel : NSNormalWindowLevel ];
			[ window setDelegate : self ];
			[ window setHasShadow : YES ];
			[ window setContentView : glview ];
			[ window setAcceptsMouseMovedEvents : YES ];
			[ window makeKeyAndOrderFront : self ];
			[ window makeFirstResponder : glview ];
			[ window makeMainWindow ];
			[ glview render ];
		}
		return NO;
	}

	-(void)windowWillClose:(NSNotification *)notification
	{
		window = nil;
	}

	@end

	void proxy_closewindow( )
	{
		[window close];
	}

	void proxy_maximizewindow( )
	{
		NSRect frame;
		if ( !fullscreen )
		{
			last = window.frame;
			frame = [[NSScreen mainScreen] visibleFrame];
		}
		else frame = last;
		fullscreen = !fullscreen;
		[window setFrame:frame display:YES];
	}

	@implementation GLWindow
	- ( BOOL ) canBecomeKeyWindow {	return YES; }
	- ( BOOL ) canBecomeMainWindow { return YES; }
	@end

