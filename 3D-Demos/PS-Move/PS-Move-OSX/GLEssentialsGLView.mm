#include "RsPlatform.h"
#import "GLEssentialsGLView.h"
#import "ScMain.h"
#import "BtTime.h"
#import "RsImpl.h"
#import "ApConfig.h"
#import "SdSoundImpl.h"
#import "UiKeyboard.h"
#import "UiInputImpl.h"
#import "ShIMU.h"
#import "MtVector3.h"
#import "HlKeyboard.h"
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/AudioServices.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/AudioServices.h>
#import <AVFoundation/AVFoundation.h>
#include <iostream>
#include <sys/time.h>                // for gettimeofday()
#include "stdio.h"                   // for gettimeofday()
#include "UiKeyboard.h"

#import <queue>

// Thomas Perl's PS-Move-API
#import "psmove.h"

//#define MADGWICK
#ifdef MADGWICK
#include "madgwickAHRS.h"
#endif

#define SUPPORT_RETINA_RESOLUTION 1

AVAudioPlayer* audioPlayer;

@interface GLEssentialsGLView (PrivateMethods)
- (void) initGL;

@end

@implementation GLEssentialsGLView

//--------------------------------------------------------------------------------------------
BtU32 numControllers;
PSMove *moveArr[16];
static int lastTrigger[16];

#ifdef MADGWICK
Madgwick madgwick[16];
#endif

-(void)setupIdentity
{
    numControllers = psmove_count_connected();
    
    ShIMU::SetNumSensors( numControllers );
    
    printf("Connected PS Move controllers: %d\n", numControllers );
    
    // Connect all the controllers
    for( int i=0; i<numControllers; i++)
    {
        lastTrigger[i] = 0;
        
        moveArr[i] = psmove_connect_by_id(i);
        
        // Set the rumble to 0
        psmove_set_rumble( moveArr[i], 0 );
        
        // Set the lights to 0
        psmove_set_leds( moveArr[i], 0, 0, 0 );
    }
    [self resetIdentity];
}

-(void)resetIdentity
{
#ifdef MADGWICK
    for( BtU32 i=0; i<numControllers; i++ )
    {
        Madgwick &madge = madgwick[i];
        madge.q0 = 1.0f; madge.q1 = madge.q2 = madge.q3 = 0;

        MtQuaternion quaternion = MtQuaternion( -madge.q1, -madge.q3, -madge.q2, madge.q0 );
        ShIMU::SetQuaternion( i, quaternion );
    }
#endif
}

-(void)updateIdentity
{
    for( BtU32 i=0; i<numControllers; i++ )
    {
        PSMove *move = moveArr[i];
        
        int res = psmove_poll( move );
        if (res)
        {
#ifdef MADGWICK
            MtQuaternion quaternion;
            BtFloat fax, fay, faz;
            BtFloat fgx, fgy, fgz;
            
            for( BtU32 j=0; j<2; j++ )
            {
                PSMove_Frame frame = (PSMove_Frame)j;
                psmove_get_accelerometer_frame( move, frame, &fax, &fay, &faz );
                psmove_get_gyroscope_frame( move, frame, &fgx, &fgy, &fgz );
                
                Madgwick &madge = madgwick[i];
                madge.MadgwickAHRSupdateIMU( fgx, fgy, fgz, fax, fay, faz );
            }
            
            // Works vertically with x, z, y
            Madgwick &madge = madgwick[i];
            quaternion = MtQuaternion( -madge.q1, -madge.q3, -madge.q2, madge.q0 );
            ShIMU::SetQuaternion( 0, quaternion );
#endif
           
            // Respond to the trigger
            int trigger = psmove_get_trigger( move );
            
            if( trigger != lastTrigger[i] )//(128 + 64 + 32 + 16) )
            {
                psmove_set_leds( move, trigger, trigger, trigger );
                lastTrigger[i] = trigger;
            }
                
            // Respond to the buttons
            unsigned int pressed, released;
            psmove_get_button_events( move, &pressed, &released);
            if( pressed == Btn_TRIANGLE )
            {
                psmove_set_leds( move, 0, 255, 0 );
            }
            if( pressed == Btn_CROSS )
            {
                psmove_set_leds( move, 0, 0, 255 );
            }
            if( pressed == Btn_CIRCLE )
            {
                psmove_set_leds( move, 255, 0, 0 );
            }
            if( pressed == Btn_TRIANGLE )
            {
                psmove_set_leds( move, 0, 255, 0 );
            }
            if( pressed == Btn_SQUARE )
            {
                psmove_set_leds( move, 0xFF, 0x69, 0xB4 );
            }
            if( pressed == Btn_MOVE )
            {
                // Integrate the battery level and set the colour from RED to WHITE
                PSMove_Battery_Level batt = psmove_get_battery( move );
                
                switch( batt )
                {
                    case Batt_MIN :
                        psmove_set_leds( move, 255, 0, 0 );
                        break;
                    case Batt_20Percent:
                        psmove_set_leds( move, 128, 128, 0 );
                        break;
                    case Batt_40Percent:
                        psmove_set_leds( move, 128, 200, 0 );
                        break;
                    case Batt_60Percent:
                        psmove_set_leds( move, 0, 200, 0 );
                        break;
                    case Batt_80Percent :
                        psmove_set_leds( move, 0, 220, 0 );
                        break;
                    case Batt_MAX:
                        psmove_set_leds( move, 0, 255, 0 );
                        break;
                    case Batt_CHARGING:
                        psmove_set_leds( move, 0, 0, 255 );
                        break;
                    case Batt_CHARGING_DONE:
                        psmove_set_leds( move, 255, 255, 255 );
                        break;
                }
            }
            if( released )
            {
                psmove_set_leds( move, 0, 0, 0 );
            }

            // Update any changes to the lights
            psmove_update_leds( move );
        }
    }
}
//--------------------------------------------------------------------------------------------



ScMain myProject;

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
    // There is no autorelease pool when this method is called
    // because it will be called from a background thread.
    // It's important to create one or app can leak objects.
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    [self drawView];
    
    [pool release];
    return kCVReturnSuccess;
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,
                                      const CVTimeStamp* now,
                                      const CVTimeStamp* outputTime,
                                      CVOptionFlags flagsIn,
                                      CVOptionFlags* flagsOut,
                                      void* displayLinkContext)
{
    CVReturn result = [(GLEssentialsGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void) awakeFromNib
{
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        // Must specify the 3.2 Core Profile to use OpenGL 3.2
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
#endif
        0
    };
    
    NSOpenGLPixelFormat *pf = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attrs] autorelease];
    
    if (!pf)
    {
        NSLog(@"No OpenGL pixel format");
    }
	   
    NSOpenGLContext* context = [[[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil] autorelease];
    
#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 && defined(DEBUG)
    // When we're using a CoreProfile context, crash if we call a legacy OpenGL function
    // This will make it much more obvious where and when such a function call is made so
    // that we can remove such calls.
    // Without this we'd simply get GL_INVALID_OPERATION error for calling legacy functions
    // but it would be more difficult to see where that function was called.
    CGLEnable([context CGLContextObj], kCGLCECrashOnRemovedFunctions);
#endif
    
    [self setPixelFormat:pf];
    
    [self setOpenGLContext:context];
    
#if SUPPORT_RETINA_RESOLUTION
    // Opt-In to Retina resolution
    [self setWantsBestResolutionOpenGLSurface:YES];
#endif // SUPPORT_RETINA_RESOLUTION
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    // Make all the OpenGL calls to setup rendering
    //  and build the necessary rendering objects
    [self initGL];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(displayLink);
    
    // Register to be notified when the window closes so we can stop the displaylink
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowWillClose:)
                                                 name:NSWindowWillCloseNotification
                                               object:[self window]];
}

- (void) windowWillClose:(NSNotification*)notification
{
    // Stop the display link when the window is closing because default
    // OpenGL render buffers will be destroyed.  If display link continues to
    // fire without renderbuffers, OpenGL draw calls will set errors.
    
    CVDisplayLinkStop(displayLink);
}

- (void) initGL
{
    // The reshape function may have changed the thread to which our OpenGL
    // context is attached before prepareOpenGL and initGL are called.  So call
    // makeCurrentContext to ensure that our OpenGL context current to this
    // thread (i.e. makeCurrentContext directs all OpenGL calls on this thread
    // to [self openGLContext])
    [[self openGLContext] makeCurrentContext];
    
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a vertex array object (VAO) to cache model parameters
    GLuint vaoName;
    glGenVertexArrays(1, &vaoName);
    glBindVertexArray(vaoName);
    
    BtTime::SetTick( 1.0f / 60.0f );
    
    NSString *resourceDirectory = [[NSBundle mainBundle] resourcePath];
    resourceDirectory = [resourceDirectory stringByAppendingString:@"/"];
    const BtChar *resources = [resourceDirectory cStringUsingEncoding:NSASCIIStringEncoding];
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    documentsDirectory = [documentsDirectory stringByAppendingString:@"/"];
    const BtChar *documents = [documentsDirectory cStringUsingEncoding:NSASCIIStringEncoding];
    
    // Set the game config
    ApConfig::SetResourcePath(resources);
    ApConfig::SetDocuments(documents);
    ApConfig::SetPlatform( ApPlatform_WinGL );
    ApConfig::SetDevice( ApDevice_OSX );
    
    // Set the extension
    ApConfig::Init();
   
    // Create the renderer implementation
    RsImpl::pInstance()->Create();
    
    // Init the time
    BtTime::Init();
    
    // Destroy the sound manager
    SdSoundImpl::CreateManager();

    // Get the view size in Points
    NSRect viewRectPoints = [self bounds];
    
#if SUPPORT_RETINA_RESOLUTION
    
    // Rendering at retina resolutions will reduce aliasing, but at the potential
    // cost of framerate and battery life due to the GPU needing to render more
    // pixels.
    
    // Any calculations the renderer does which use pixel dimentions, must be
    // in "retina" space.  [NSView convertRectToBacking] converts point sizes
    // to pixel sizes.  Thus the renderer gets the size in pixels, not points,
    // so that it can set it's viewport and perform and other pixel based
    // calculations appropriately.
    // viewRectPixels will be larger (2x) than viewRectPoints for retina displays.
    // viewRectPixels will be the same as viewRectPoints for non-retina displays
    NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
    
#else //if !SUPPORT_RETINA_RESOLUTION
    
    // App will typically render faster and use less power rendering at
    // non-retina resolutions since the GPU needs to render less pixels.  There
    // is the cost of more aliasing, but it will be no-worse than on a Mac
    // without a retina display.
    
    // Points:Pixels is always 1:1 when not supporting retina resolutions
    NSRect viewRectPixels = viewRectPoints;
    
#endif // !SUPPORT_RETINA_RESOLUTION
    
    // Set the device pixel dimension in our renderer
    MtVector2 v2Dimension( viewRectPixels.size.width, viewRectPixels.size.height );
    RsImpl::pInstance()->SetDimension( v2Dimension );
    
    // Create the project
    myProject.Create();
    
    // Initialise the project
    myProject.Init();
    
    // Initialise the PS move controllers
    [self setupIdentity];
}

- (void) reshape
{
    [super reshape];
    
    // We draw on a secondary thread through the display link. However, when
    // resizing the view, -drawRect is called on the main thread.
    // Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing.
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    // Get the view size in Points
    NSRect viewRectPoints = [self bounds];
    
#if SUPPORT_RETINA_RESOLUTION
    
    // Rendering at retina resolutions will reduce aliasing, but at the potential
    // cost of framerate and battery life due to the GPU needing to render more
    // pixels.
    
    // Any calculations the renderer does which use pixel dimentions, must be
    // in "retina" space.  [NSView convertRectToBacking] converts point sizes
    // to pixel sizes.  Thus the renderer gets the size in pixels, not points,
    // so that it can set it's viewport and perform and other pixel based
    // calculations appropriately.
    // viewRectPixels will be larger (2x) than viewRectPoints for retina displays.
    // viewRectPixels will be the same as viewRectPoints for non-retina displays
    NSRect viewRectPixels = [self convertRectToBacking:viewRectPoints];
    
#else //if !SUPPORT_RETINA_RESOLUTION
    
    // App will typically render faster and use less power rendering at
    // non-retina resolutions since the GPU needs to render less pixels.  There
    // is the cost of more aliasing, but it will be no-worse than on a Mac
    // without a retina display.
    
    // Points:Pixels is always 1:1 when not supporting retina resolutions
    NSRect viewRectPixels = viewRectPoints;
    
#endif // !SUPPORT_RETINA_RESOLUTION
    
    // Set the device pixel dimension in our renderer
    MtVector2 v2Dimension( viewRectPixels.size.width, viewRectPixels.size.height );
    RsImpl::pInstance()->SetDimension( v2Dimension );
    
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
    
    if( v2Dimension.x )
    {
        // GAV TO DO
        // Resize the window
        //myProject.Resize();
    }
}

- (void)renewGState
{
    // Called whenever graphics state updated (such as window resize)
    
    // OpenGL rendering is not synchronous with other rendering on the OSX.
    // Therefore, call disableScreenUpdatesUntilFlush so the window server
    // doesn't render non-OpenGL content in the window asynchronously from
    // OpenGL content, which could cause flickering.  (non-OpenGL content
    // includes the title bar and drawing done by the app with other APIs)
    [[self window] disableScreenUpdatesUntilFlush];
    
    [super renewGState];
}

- (void) drawRect: (NSRect) theRect
{
    // Called during resize operations
    
    // Avoid flickering during resize by drawiing
    [self drawView];
}

- (void) drawView
{
    [[self openGLContext] makeCurrentContext];
    
    // We draw on a secondary thread through the display link
    // When resizing the view, -reshape is called automatically on the main
    // thread. Add a mutex around to avoid the threads accessing the context
    // simultaneously when resizing
    CGLLockContext([[self openGLContext] CGLContextObj]);
    
    // Update the input
    UiInputWinGL::Update();
    
    // Update the project
    myProject.Update();
    
    if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_R ) )
    {
        [self resetIdentity];
    }
    
    // Update identity
    [self updateIdentity];
    
    // Empty render targets
    RsImpl::pInstance()->EmptyRenderTargets();
    
    // Render the project
    myProject.Render();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0 );

    // Render
    RsImpl::pInstance()->Render();
    
    CGLFlushDrawable([[self openGLContext] CGLContextObj]);
    CGLUnlockContext([[self openGLContext] CGLContextObj]);
}

- (void) dealloc
{
    // Stop the display link BEFORE releasing anything in the view
    // otherwise the display link thread may call into the view and crash
    // when it encounters something that has been release
    CVDisplayLinkStop(displayLink);
    
    CVDisplayLinkRelease(displayLink);
    
    [super dealloc];
}

@end
