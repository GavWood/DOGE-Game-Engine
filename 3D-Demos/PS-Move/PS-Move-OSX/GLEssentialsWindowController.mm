#import "GLEssentialsWindowController.h"
#import "GLEssentialsFullscreenWindow.h"
#import "UiKeyboardImpl.h"

@interface GLEssentialsWindowController ()
@end

@implementation GLEssentialsWindowController

// Were we set in the background?
BOOL wasBackgroundedOutOfFullScreen;

// Fullscreen window 
GLEssentialsFullscreenWindow *fullscreenWindow;

// Non-Fullscreen window (also the initial window)
NSWindow* standardWindow;

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];

	if (self)
	{
		// Initialize to nil since it indicates app is not fullscreen
		fullscreenWindow = nil;
    }
	return self;
}

- (void) goFullscreen
{
	// If app is already fullscreen...
	if(fullscreenWindow)
	{
		//...don't do anything
		return;
	}

	// Allocate a new fullscreen window
	fullscreenWindow = [[GLEssentialsFullscreenWindow alloc] init];

	// Resize the view to screensize
	NSRect viewRect = [fullscreenWindow frame];

	// Set the view to the size of the fullscreen window
	[view setFrameSize: viewRect.size];

	// Set the view in the fullscreen window
	[fullscreenWindow setContentView:view];

	standardWindow = [self window];

	// Hide non-fullscreen window so it doesn't show up when switching out
	// of this app (i.e. with CMD-TAB)
	[standardWindow orderOut:self];

	// Set controller to the fullscreen window so that all input will go to
	// this controller (self)
	[self setWindow:fullscreenWindow];

	// Show the window and make it the key window for input
	[fullscreenWindow makeKeyAndOrderFront:self];
}

- (void) goWindow
{
	// If controller doesn't have a full screen window...
	if(fullscreenWindow == nil)
	{
		//...app is already windowed so don't do anything
		return;
	}

	// Get the rectangle of the original window
	NSRect viewRect = [standardWindow frame];
	
	// Set the view rect to the new size
	[view setFrame:viewRect];

	// Set controller to the standard window so that all input will go to
	// this controller (self)
	[self setWindow:standardWindow];

	// Set the content of the orginal window to the view
	[[self window] setContentView:view];

	// Show the window and make it the key window for input
	[[self window] makeKeyAndOrderFront:self];

	// Release the fullscreen window
	[fullscreenWindow release];

	// Ensure we set fullscreen Window to nil so our checks for 
	// windowed vs. fullscreen mode elsewhere are correct
	fullscreenWindow = nil;
}

extern UiKeyboardWin sKeyboardGL;

- (void) flagsChanged:(NSEvent *)event
{
    if( [event modifierFlags] & NSEventModifierFlagShift )
    {
        sKeyboardGL.SetKeyDown( UiKeyCode_LSHIFT );
        sKeyboardGL.SetKeyDown( UiKeyCode_RSHIFT );
    }
    else
    {
        sKeyboardGL.SetKeyUp( UiKeyCode_LSHIFT );
        sKeyboardGL.SetKeyUp( UiKeyCode_RSHIFT );
    }
    
    // Allow other character to be handled (or not and beep)
    [super flagsChanged:event];
}

enum {
    kOSX_A                    = 0x00,
    kOSX_S                    = 0x01,
    kOSX_D                    = 0x02,
    kOSX_F                    = 0x03,
    kOSX_H                    = 0x04,
    kOSX_G                    = 0x05,
    kOSX_Z                    = 0x06,
    kOSX_X                    = 0x07,
    kOSX_C                    = 0x08,
    kOSX_V                    = 0x09,
    kOSX_B                    = 0x0B,
    kOSX_Q                    = 0x0C,
    kOSX_W                    = 0x0D,
    kOSX_E                    = 0x0E,
    kOSX_R                    = 0x0F,
    kOSX_Y                    = 0x10,
    kOSX_T                    = 0x11,
    kOSX_1                    = 0x12,
    kOSX_2                    = 0x13,
    kOSX_3                    = 0x14,
    kOSX_4                    = 0x15,
    kOSX_6                    = 0x16,
    kOSX_5                    = 0x17,
    kOSX_Equal                = 0x18,
    kOSX_9                    = 0x19,
    kOSX_7                    = 0x1A,
    kOSX_Minus                = 0x1B,
    kOSX_8                    = 0x1C,
    kOSX_0                    = 0x1D,
    kOSX_RightBracket         = 0x1E,
    kOSX_O                    = 0x1F,
    kOSX_U                    = 0x20,
    kOSX_LeftBracket          = 0x21,
    kOSX_I                    = 0x22,
    kOSX_P                    = 0x23,
    kOSX_L                    = 0x25,
    kOSX_J                    = 0x26,
    kOSX_Quote                = 0x27,
    kOSX_K                    = 0x28,
    kOSX_Semicolon            = 0x29,
    kOSX_Backslash            = 0x2A,
    kOSX_Comma                = 0x2B,
    kOSX_Slash                = 0x2C,
    kOSX_N                    = 0x2D,
    kOSX_M                    = 0x2E,
    kOSX_Period               = 0x2F,
    kOSX_Grave                = 0x32,
    kOSX_KeypadDecimal        = 0x41,
    kOSX_KeypadMultiply       = 0x43,
    kOSX_KeypadPlus           = 0x45,
    kOSX_KeypadClear          = 0x47,
    kOSX_KeypadDivide         = 0x4B,
    kOSX_KeypadEnter          = 0x4C,
    kOSX_KeypadMinus          = 0x4E,
    kOSX_KeypadEquals         = 0x51,
    kOSX_Keypad0              = 0x52,
    kOSX_Keypad1              = 0x53,
    kOSX_Keypad2              = 0x54,
    kOSX_Keypad3              = 0x55,
    kOSX_Keypad4              = 0x56,
    kOSX_Keypad5              = 0x57,
    kOSX_Keypad6              = 0x58,
    kOSX_Keypad7              = 0x59,
    kOSX_Keypad8              = 0x5B,
    kOSX_Keypad9              = 0x5C,
    kOSX_Return                    = 0x24,
    kOSX_Tab                       = 0x30,
    kOSX_Space                     = 0x31,
    kOSX_Delete                    = 0x33,
    kOSX_Escape                    = 0x35,
    kOSX_Command                   = 0x37,
    kOSX_Shift                     = 0x38,
    kOSX_CapsLock                  = 0x39,
    kOSX_Option                    = 0x3A,
    kOSX_Control                   = 0x3B,
    kOSX_RightShift                = 0x3C,
    kOSX_RightOption               = 0x3D,
    kOSX_RightControl              = 0x3E,
    kOSX_Function                  = 0x3F,
    kOSX_F17                       = 0x40,
    kOSX_VolumeUp                  = 0x48,
    kOSX_VolumeDown                = 0x49,
    kOSX_Mute                      = 0x4A,
    kOSX_F18                       = 0x4F,
    kOSX_F19                       = 0x50,
    kOSX_F20                       = 0x5A,
    kOSX_F5                        = 0x60,
    kOSX_F6                        = 0x61,
    kOSX_F7                        = 0x62,
    kOSX_F3                        = 0x63,
    kOSX_F8                        = 0x64,
    kOSX_F9                        = 0x65,
    kOSX_F11                       = 0x67,
    kOSX_F13                       = 0x69,
    kOSX_F16                       = 0x6A,
    kOSX_F14                       = 0x6B,
    kOSX_F10                       = 0x6D,
    kOSX_F12                       = 0x6F,
    kOSX_F15                       = 0x71,
    kOSX_Help                      = 0x72,
    kOSX_Home                      = 0x73,
    kOSX_PageUp                    = 0x74,
    kOSX_ForwardDelete             = 0x75,
    kOSX_F4                        = 0x76,
    kOSX_End                       = 0x77,
    kOSX_F2                        = 0x78,
    kOSX_PageDown                  = 0x79,
    kOSX_F1                        = 0x7A,
    kOSX_LeftArrow                 = 0x7B,
    kOSX_RightArrow                = 0x7C,
    kOSX_DownArrow                 = 0x7D,
    kOSX_UpArrow                   = 0x7E
};

const BtU32 glfwKeys[] = {
    (BtU32)kOSX_Escape,
    UiKeyCode_ESCAPE,
    (BtU32)kOSX_Return,
    UiKeyCode_RETURN,
    (BtU32)kOSX_A,
    UiKeyCode_A,
    (BtU32)kOSX_B,
    UiKeyCode_B,
    (BtU32)kOSX_C,
    UiKeyCode_C,
    (BtU32)kOSX_D,
    UiKeyCode_D,
    (BtU32)kOSX_E,
    UiKeyCode_E,
    (BtU32)kOSX_F,
    UiKeyCode_F,
    (BtU32)kOSX_G,
    UiKeyCode_G,
    (BtU32)kOSX_H,
    UiKeyCode_H,
    (BtU32)kOSX_I,
    UiKeyCode_I,
    (BtU32)kOSX_J,
    UiKeyCode_J,
    (BtU32)kOSX_K,
    UiKeyCode_K,
    (BtU32)kOSX_L,
    UiKeyCode_L,
    (BtU32)kOSX_M,
    UiKeyCode_M,
    (BtU32)kOSX_N,
    UiKeyCode_N,
    (BtU32)kOSX_O,
    UiKeyCode_O,
    (BtU32)kOSX_P,
    UiKeyCode_P,
    (BtU32)kOSX_Q,
    UiKeyCode_Q,
    (BtU32)kOSX_R,
    UiKeyCode_R,
    (BtU32)kOSX_S,
    UiKeyCode_S,
    (BtU32)kOSX_T,
    UiKeyCode_T,
    (BtU32)kOSX_T,
    UiKeyCode_U,
    (BtU32)kOSX_V,
    UiKeyCode_V,
    (BtU32)kOSX_W,
    UiKeyCode_W,
    (BtU32)kOSX_X,
    UiKeyCode_X,
    (BtU32)kOSX_Y,
    UiKeyCode_Y,
    (BtU32)kOSX_Z,
    UiKeyCode_Z,
    (BtU32)kOSX_F1,
    UiKeyCode_F1,
    (BtU32)kOSX_F2,
    UiKeyCode_F2,
    (BtU32)kOSX_F3,
    UiKeyCode_F3,
    (BtU32)kOSX_F4,
    UiKeyCode_F4,
    (BtU32)kOSX_F5,
    UiKeyCode_F5,
    (BtU32)kOSX_F6,
    UiKeyCode_F6,
    (BtU32)kOSX_F7,
    UiKeyCode_F7,
    (BtU32)kOSX_F8,
    UiKeyCode_F8,
    (BtU32)kOSX_F9,
    UiKeyCode_F9,
    (BtU32)kOSX_F10,
    UiKeyCode_F10,
    (BtU32)kOSX_F11,
    UiKeyCode_F11,
    (BtU32)kOSX_F12,
    UiKeyCode_F12,
    kOSX_LeftArrow,
    UiKeyCode_LEFT,
    kOSX_RightArrow,
    UiKeyCode_RIGHT,
    kOSX_UpArrow,
    UiKeyCode_UP,
    kOSX_DownArrow,
    UiKeyCode_DOWN
};

- (void) keyUp:(NSEvent *)event
{
    int keyCode = [event keyCode];
    BtU32 key = 0;
    for (int i = 0; i < sizeof(glfwKeys); i = i + 2 )
    {
        if (glfwKeys[i] == keyCode)
        {
            key = glfwKeys[i + 1];
            break;
        }
    }
    sKeyboardGL.SetKeyUp( key );
    
    // Allow other character to be handled (or not and beep)
    //[super keyUp:event];
}

- (void) keyDown:(NSEvent *)event
{
    int keyCode = [event keyCode];
    BtU32 key = 0;
    for (int i = 0; i < sizeof(glfwKeys); i = i + 2 )
    {
        if (glfwKeys[i] == keyCode)
        {
            key = glfwKeys[i + 1];
            break;
        }
    }
    sKeyboardGL.SetKeyDown( key );
    
	// Allow other character to be handled (or not and beep)
	//[super keyDown:event];
}

@end
