/*
     File: EAGLView.m
 Abstract: n/a
  Version: 1.3
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple
 Inc. ("Apple") in consideration of your agreement to the following
 terms, and your use, installation, modification or redistribution of
 this Apple software constitutes acceptance of these terms.  If you do
 not agree with these terms, please do not use, install, modify or
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and
 subject to these terms, Apple grants you a personal, non-exclusive
 license, under Apple's copyrights in this original Apple software (the
 "Apple Software"), to use, reproduce, modify and redistribute the Apple
 Software, with or without modifications, in source and/or binary forms;
 provided that if you redistribute the Apple Software in its entirety and
 without modifications, you must retain this notice and the following
 text and disclaimers in all such redistributions of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may
 be used to endorse or promote products derived from the Apple Software
 without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or
 implied, are granted by Apple herein, including but not limited to any
 patent rights that may be infringed by your derivative works or by other
 works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2014 Apple Inc. All Rights Reserved.
 
 */

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "EAGLView.h"
#import "AppDelegate.h"

@implementation EAGLView
{
    // The pixel dimensions of the backbuffer
    GLint backingWidth;
    GLint backingHeight;
    
    EAGLContext *context;
	BOOL dolayout;
	
    // OpenGL names for the renderbuffer and framebuffer used to render to this view
    GLuint viewRenderbuffer, viewFramebuffer;	

	NSTimer* timer;
	ui_t* controller;
	NSMutableArray* deltaArray;
}

@synthesize context;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

static EAGLView* staticView;

// The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithFrame:(CGRect)frame
{    
    if ((self = [super initWithFrame:frame]))
	{
		[UIView setAnimationsEnabled:NO];
		staticView = self;
		controller = nil;
		dolayout = NO;
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
			[NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking,
			kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
			nil];
		
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        if (!context || ![EAGLContext setCurrentContext:context])
		{
            return nil;
        }

		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(keyboardWillShow:)
													 name:UIKeyboardWillShowNotification
												   object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self
												 selector:@selector(keyboardWillHide:)
													 name:UIKeyboardWillHideNotification
												   object:nil];

        self.contentScaleFactor = 2.0;
		//self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		
		// Create system framebuffer object. The backing will be allocated in -reshapeFramebuffer
		glGenFramebuffersOES(1, &viewFramebuffer);
		glGenRenderbuffersOES(1, &viewRenderbuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
		
    }
    return self;
}

- (void)dealloc
{        
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
	self.context = nil;
	[super dealloc];
}

- (void)load
{
	NSString* resPath = [ [ NSBundle mainBundle ] resourcePath ];
	NSString* libPath = [ NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex : 0 ];
	c36_t* c36 = c36_init();
	demo_square_t* demo_a = demo_square_init();
	demo_textarea_t* demo_b = demo_textarea_init();
	demo_scroller_t* demo_c = demo_scroller_init();
	demo_itemlist_t* demo_d = demo_itemlist_init();
	demo_kineticui_t* demo_e = demo_kineticui_init();
	demo_kineticlabel_t* demo_f = demo_kineticlabel_init();
	controller = ui_init(
		self.frame.size.width * [[UIScreen mainScreen] scale] ,
		self.frame.size.height * [[UIScreen mainScreen] scale] ,
		self.contentScaleFactor ,
		(char*) [ resPath cStringUsingEncoding : NSUTF8StringEncoding ] ,
		(char*) [ libPath cStringUsingEncoding : NSUTF8StringEncoding ] ,
		( uidelegate_t* ) c36 );
	timer = [ NSTimer scheduledTimerWithTimeInterval : 1.0 / 60.0
					  target : self
					  selector : @selector(timerEvent)
					  userInfo : nil
					  repeats : YES ];
}

- (void)drawView
{
	// This application only creates a single (color) renderbuffer, so it is already bound here.
	// If there are multiple renderbuffers (for example color and depth), ensure the correct one is bound here.
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void)reshapeFramebuffer
{
	// Allocate GL color buffer backing, matching the current layer size
    [context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
}

- (void)layoutSubviews
{
    [self reshapeFramebuffer];
	ui_resize( controller , self.frame.size.width * [[UIScreen mainScreen] scale] , self.frame.size.height * [[UIScreen mainScreen] scale] , 1 );
	[self flushBuffer];
}


- (void)timerEvent
{
	ui_timer( controller );
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [ touches anyObject ];
    CGPoint location = [touch locationInView: self];

    float x = location.x * [[UIScreen mainScreen] scale];
    float y = location.y * [[UIScreen mainScreen] scale];

	ui_touch_down( controller , x , -y );
	
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [ touches anyObject ];
    CGPoint location = [touch locationInView: self];

    float x = location.x * [[UIScreen mainScreen] scale];
    float y = location.y * [[UIScreen mainScreen] scale];

	ui_touch_move( controller , x , -y );
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch* touch = [ touches anyObject ];
    CGPoint location = [touch locationInView: self];

    float x = location.x * [[UIScreen mainScreen] scale];
    float y = location.y * [[UIScreen mainScreen] scale];

	ui_touch_up( controller , x , -y );
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (BOOL)hasText
{
    return YES;
}
- (void)insertText:(NSString *)text
{
	char* chars = ( char* ) [ text cStringUsingEncoding : NSUTF8StringEncoding ];
	if ( chars[0] == 127 ) ui_key_down( controller , NULL , kInputKeyTypeBackspace );
	else if ( chars[0] == 13 || chars[0] == 10 ) ui_key_down( controller , NULL , kInputKeyTypeReturn );
	else ui_key_down( controller , chars , kInputKeyTypeNormal );
}

-(void)deleteBackward
{
	ui_key_down( controller , NULL , kInputKeyTypeBackspace );
}

- ( void ) showKeyboard
{
	if ( [ NSThread currentThread ] != [ NSThread mainThread ] )
	{
		[self performSelectorOnMainThread:@selector(showKeyboard) withObject:nil waitUntilDone:NO];
	}
	else
	{
		if( !self.isFirstResponder ) [self becomeFirstResponder];
	}
}

- ( void ) hideKeyboard
{
	if ( [ NSThread currentThread ] != [ NSThread mainThread ] )
	{
		[self performSelectorOnMainThread:@selector(hideKeyboard) withObject:nil waitUntilDone:NO];
	}
	else
	{
		if( self.isFirstResponder ) [self resignFirstResponder];
	}
}

- ( void ) keyboardWillShow : ( NSNotification* ) notification
{
	NSDictionary* userInfo = [notification userInfo];
	CGSize keyboardSize = [[userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
	CGRect viewFrame = self.frame;
	viewFrame.size.height -= keyboardSize.height;
	dolayout = YES;
	[UIView animateWithDuration:.3 delay:0 options:UIViewAnimationOptionLayoutSubviews animations:
	^{
		self.frame = viewFrame;
	} completion:
	^(BOOL finished){
		dolayout = NO;
	 } ];
}

- ( void ) keyboardWillHide : ( NSNotification* ) notification
{
	NSDictionary* userInfo = [notification userInfo];
	CGSize keyboardSize = [[userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
	CGRect viewFrame = self.frame;
	viewFrame.size.height += keyboardSize.height;
	dolayout = YES;
	[UIView animateWithDuration:.3 delay:0 options:UIViewAnimationOptionLayoutSubviews animations:
	^{
		self.frame = viewFrame;
	} completion:
	^(BOOL finished){
		dolayout = NO;
	 } ];
}


- ( void ) flushBuffer
{
	[ context presentRenderbuffer : GL_RENDERBUFFER_OES ];
}

@end

void proxy_flushbuffer( )
{
	[ staticView flushBuffer ];
}


void proxy_dragwindow( ) { }
void proxy_dropwindow( ) { }
void proxy_closewindow( ) { }
void proxy_maximizewindow( ) { }
void proxy_showkeyboard( )
{
	[ staticView showKeyboard ];
}
void proxy_hidekeyboard( )
{
	[ staticView hideKeyboard ];
}
