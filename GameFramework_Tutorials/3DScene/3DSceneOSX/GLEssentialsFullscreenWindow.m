#import "GLEssentialsFullscreenWindow.h"

@implementation GLEssentialsFullscreenWindow

-(id) init
{
	// Create a screen-sized window on the display you want to take over
	NSRect screenRect = [[NSScreen mainScreen] frame];

	// Initialize the window making it size of the screen and borderless
	self = [super initWithContentRect:screenRect styleMask:0 backing:NSBackingStoreBuffered defer:YES];

	// Set the window level to be above the menu bar to cover everything else
	[self setLevel:NSMainMenuWindowLevel+1];

	// Set opaque
	[self setOpaque:YES];

	// Hide this when user switches to another window (or app)
	[self setHidesOnDeactivate:YES];

	return self;
}

-(BOOL) canBecomeKeyWindow
{
	// Return yes so that this borderless window can receive input
	return YES;
}

- (void) keyUp:(NSEvent *)event
{
    // Implement keyDown since controller will not get [ESC] key event which
    // the controller uses to kill fullscreen
    [[self windowController] keyUp:event];
}

- (void) keyDown:(NSEvent *)event
{
	// Implement keyDown since controller will not get [ESC] key event which
	// the controller uses to kill fullscreen
	[[self windowController] keyDown:event];
}

@end
