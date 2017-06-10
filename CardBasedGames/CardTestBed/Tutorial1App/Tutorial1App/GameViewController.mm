
//
//  MuViewController.m
//  GameViewController.mm
//
//  Created by Gavin Wood on 07/06/2012.
//  Copyright (c) 2012 Computer Science. All rights reserved.

#import "GameViewController.h"
#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/AudioServices.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#import <TargetConditionals.h>
#import <CoreMotion/CoreMotion.h>

// Struffy entry point
#import <stdio.h>
#import "GaProject.h"
#import "ApConfig.h"
#import "RsImpl.h"
#import "ShTouch.h"
#import "BtTimeGLES.h"
#import "SdSoundWinGL.h"
#import "ShIMU.h"
#import "BtString.h"
#import "MtVector3.h"
#import "BtTime.h"
#import "ShNetwork.h"
#import "SbMain.h"
#import "AppDelegate.h"
#import "PeerToPeer.h"
//#import "VideoCapture.h"

// My globals
SbMain myProject;
CFMutableDictionaryRef touchToLabelMapping;
NSMutableArray *availableLabels;
float RenderScale = 1;                          // Render scale allows switch between retina / normal devices
#define MotionInterval 0.01f                    // This is in seconds

@interface GameViewController () {
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController
@synthesize context = _context;
@synthesize captureSession;
CMMotionManager *motionManager;

//---------------------------------------------------------------------------------
// helpers

void interruptionListenerCallback (void   *inUserData, UInt32    interruptionState )
{
    /*
     // you could do this with a cast below, but I will keep it here to make it clearer
     YourSoundControlObject *controller = (YourSoundControlObject *) inUserData;
     
     if (interruptionState == kAudioSessionBeginInterruption) {
     [controller _haltOpenALSession];
     } else if (interruptionState == kAudioSessionEndInterruption) {
     [controller _resumeOpenALSession];
     }
     */
}

enum {
    UIDeviceResolution_Unknown          = 0,
    UIDeviceResolution_iPhoneStandard   = 1,    // iPhone 1,3,3GS Standard Display  (320x480px)
    UIDeviceResolution_iPhoneRetina35   = 2,    // iPhone 4,4S Retina Display 3.5"  (640x960px)
    UIDeviceResolution_iPhoneRetina4    = 3,    // iPhone 5 Retina Display 4"       (640x1136px)
    UIDeviceResolution_iPadStandard     = 4,    // iPad 1,2 Standard Display        (1024x768px)
    UIDeviceResolution_iPhoneRetina5    = 5,    // iPhone 5
    UIDeviceResolution_iPadRetina       = 6,    // iPad 3 Retina Display            (2048x1536px)
    UIDeviceResolution_iPhone6          = 7,    // iPhone 6                         ()
    UIDeviceResolution_iPhone65         = 8,    // iPhone 6 Plus                    (1280x1080px)
}; typedef NSUInteger UIDeviceResolution;

UIDeviceResolution deviceType;

- (void)resolution
{
    deviceType = UIDeviceResolution_Unknown;
    UIScreen *mainScreen = [UIScreen mainScreen];
    
    RenderScale = ([mainScreen respondsToSelector:@selector(scale)] ? mainScreen.scale : 1.0f);;
    
    CGFloat height = CGRectGetHeight(mainScreen.bounds);
    CGFloat pixelHeight = height * RenderScale;
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone)
    {
        if (RenderScale == 1.0f )
        {
            deviceType = UIDeviceResolution_iPhoneStandard;
        }
        else if( RenderScale == 2.0f)
        {
            if (pixelHeight == 960.0f)
            {
                deviceType = UIDeviceResolution_iPhoneRetina35;
            }
            else if (pixelHeight == 1136.0f)
            {
                deviceType = UIDeviceResolution_iPhoneRetina4;
            }
            else if (pixelHeight == 1136.0f)
            {
                deviceType = UIDeviceResolution_iPhoneRetina5;
            }
            else if (pixelHeight == 1334.0f )
            {
                deviceType = UIDeviceResolution_iPhone6;
            }
        }
        else if( RenderScale == 3.0f )
        {
            deviceType = UIDeviceResolution_iPhone65;
            
            if( pixelHeight == 1242.0f )
            {
                deviceType = UIDeviceResolution_iPhone65;
            }
        }
    }
    else
    {
        if( RenderScale == 2.0f && pixelHeight == 2048.0f)
        {
            deviceType = UIDeviceResolution_iPadRetina;
        }
        else if( RenderScale == 1.0f && pixelHeight == 1024.0f)
        {
            deviceType = UIDeviceResolution_iPadStandard;
        }
    }
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    
    MtVector2 v2Dimension( 0, 0 );
    
    switch( deviceType )
    {
        case UIDeviceResolution_Unknown:
        {
            // This is the fall back position
            CGFloat width  = screenBounds.size.width;
            CGFloat height = screenBounds.size.height;
            v2Dimension = MtVector2( width * RenderScale, height * RenderScale );
        }
            break;
        case UIDeviceResolution_iPhoneStandard:
            v2Dimension = MtVector2( 480.0f, 320.0f );
            ApConfig::SetDevice( ApDevice_iPhone );
            break;
        case UIDeviceResolution_iPhoneRetina35:
            v2Dimension = MtVector2( 960.0f, 640.0f );
            ApConfig::SetDevice( ApDevice_iPhone );
            break;
        case UIDeviceResolution_iPhoneRetina4:
            v2Dimension = MtVector2( 1136.0f, 640.0f );
            ApConfig::SetDevice( ApDevice_iPhone );
            break;
        case UIDeviceResolution_iPadStandard:
            v2Dimension = MtVector2( 1024.0f, 768.0f );
            ApConfig::SetDevice( ApDevice_iPad );
            break;
        case UIDeviceResolution_iPadRetina:
            v2Dimension = MtVector2( 2048.0f, 1536.0f );
            ApConfig::SetDevice( ApDevice_iPad );
            break;
        case UIDeviceResolution_iPhone6:
            v2Dimension = MtVector2( 1334.0f, 750.0f );
            ApConfig::SetDevice( ApDevice_iPhone );
            break;
        case UIDeviceResolution_iPhone65:
            v2Dimension = MtVector2( 1920.0f, 1080.0f );          // Real phone
            //v2Dimension = MtVector2( 2208.0f, 1242.0f );        // Simulator
            ApConfig::SetDevice( ApDevice_iPhone );
            break;
    }
    
    // Set the device pixel dimension in our renderer
    RsImpl::pInstance()->SetDimension( v2Dimension );
}

- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskLandscape;
}

////////////////////////////////////////////////////////////////////////////////
// setupAudioAsMixed

-(void)setupAudioAsMixed
{
    //get your app's audioSession singleton object
    AVAudioSession* session = [AVAudioSession sharedInstance];
    
    //error handling
    BOOL success;
    NSError* error;
    
    //set the audioSession category.
    //Needs to be Record or PlayAndRecord to use audioRouteOverride:
    
    success = [session setCategory:AVAudioSessionCategoryPlayAndRecord
                             error:&error];
    
    if (!success)  NSLog(@"AVAudioSession error setting category:%@",error);
    
    //set the audioSession override
    success = [session overrideOutputAudioPort:AVAudioSessionPortOverrideSpeaker
                                         error:&error];
    if (!success)  NSLog(@"AVAudioSession error overrideOutputAudioPort:%@",error);
    
    //activate the audio session
    success = [session setActive:YES error:&error];
    if (!success) NSLog(@"AVAudioSession error activating: %@",error);
    else NSLog(@"audioSession active");
}

-(void)setupTouch
{
    // Create the mapping for touch
    touchToLabelMapping = CFDictionaryCreateMutable (kCFAllocatorDefault, 8, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    availableLabels = [[NSMutableArray alloc] initWithObjects:
                       [NSNumber numberWithInt:1],
                       [NSNumber numberWithInt:2],
                       [NSNumber numberWithInt:3],
                       [NSNumber numberWithInt:4],
                       [NSNumber numberWithInt:5],
                       [NSNumber numberWithInt:6],
                       [NSNumber numberWithInt:7],
                       [NSNumber numberWithInt:8],
                       nil];
}


-(void)captureBackCamera : (GLKView *)view
{
    NSError *error = nil;
    
    // create a low-quality capture session
    captureSession = [[AVCaptureSession alloc] init];
    
    // Setup the configuration
    [captureSession beginConfiguration];
    
    // We might need to consider this for older iPhones - AVCaptureSessionPreset352x288
    // Add inputs and outputs.
    if ([captureSession canSetSessionPreset:AVCaptureSessionPreset1920x1080])
    {
        captureSession.sessionPreset = AVCaptureSessionPreset1920x1080;
    }
    else {
        NSLog(@"Cannot set session preset to 640x480");
    }
    
    // Get the back facing camera
    AVCaptureDevice *captureDevice = nil;
    NSArray *videoDevices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for( AVCaptureDevice *device in videoDevices )
    {
        if( device.position == AVCaptureDevicePositionBack )
        {
            captureDevice = device;
            break;
        }
    }
    //  couldn't find one on the front, so just get the default video device.
    if( captureDevice == nil )
    {
        captureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    }
    
    // ...and a device input
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:captureDevice error:&error];
    [captureSession addInput:input];
    
    // create a VideDataOutput to route output to us
    AVCaptureVideoDataOutput *output = [[AVCaptureVideoDataOutput alloc] init];
    [output setAlwaysDiscardsLateVideoFrames:YES];
    
    // set 32bpp BGRA pixel format, since I'll want to make sense of the frame
    [output setVideoSettings:
     [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA]
                                 forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    
    //[output setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    dispatch_queue_t queue = dispatch_queue_create("myQueue", NULL);
    [output setSampleBufferDelegate:self queue:queue];
    [captureSession addOutput:output];
    
    AVCaptureVideoPreviewLayer *previewLayer = [AVCaptureVideoPreviewLayer layerWithSession:captureSession];
    previewLayer.frame = view.bounds;
    previewLayer.hidden = true;
    [view.layer addSublayer:previewLayer];
    
    // Save this configuration
    [captureSession commitConfiguration];
    
    // Begin capturing the video
    [captureSession startRunning];
}

-(void)captureStop
{
    [captureSession stopRunning];
    self.captureSession = nil;
}

-(void) captureOutput:(AVCaptureOutput*)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection*)connection

{
    // Get the image buffer
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer( sampleBuffer );
    
    CVPixelBufferLockBaseAddress(imageBuffer,0);
    
    // Get the base address of the captured movie
    uint8_t *baseAddress = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
    
    if( baseAddress != NULL )
    {
        size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
        (void)bytesPerRow;
        //size_t width = CVPixelBufferGetWidth(imageBuffer);
        //size_t height = CVPixelBufferGetHeight(imageBuffer);
        
        static int displayed = false;
        if( !displayed )
        {
            // NSLog( @"Bytes per row %d", (int)bytesPerRow );
            // NSLog( @"Width %d",  (int)width );
            // NSLog( @"Height %d", (int)height );
            displayed = true;
        }
        
        // Set the camera in our software. Eventually we will have a separate shared camera class to handle this
        //CVVideoCapture::SetMemory( baseAddress, (BtU32)( bytesPerRow * height ) );
        
        int a=0;
        a++;
    }
    
    // Release the image buffer
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
}

- (void)gameSetup
{
    BtTime::SetTick( 1.0f / 30.0f );
    
    NSString *resourceDirectory = [[NSBundle mainBundle] resourcePath];
    resourceDirectory = [resourceDirectory stringByAppendingString:@"/"];
    const BtChar *resources = [resourceDirectory cStringUsingEncoding:NSASCIIStringEncoding];
    ApConfig::SetResourcePath(resources);
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const BtChar *documents = [documentsDirectory cStringUsingEncoding:NSASCIIStringEncoding];
    ApConfig::SetDocuments(documents);
    
    // Set the extension
    ApConfig::SetExtension( ".iPhonez" );
    
    // Set the platform
    ApConfig::SetPlatform( ApPlatform_GLES );
    
#if TARGET_IPHONE_SIMULATOR
    ApConfig::SetSimulator( BtTrue );
#endif
    
    MtVector2 v2Dimension( 0, 0 );
    RenderScale = 1;
    
    // Get the resolution
    [self resolution];
    
    // Create the renderer implementation
    RsImpl::pInstance()->Create();
    
    // Destroy the sound manager
    SdSoundWinGL::CreateManager();
    
    // Init the time
    BtTimeGLES::Init();
    
    // Initialise the project
    myProject.Init();
    
    // Create the project
    myProject.Create();
    
    // Become the first responder for all the delegates in this view controller
    [self becomeFirstResponder];
    
    // Setup touch
    [self setupTouch];
    
    // Setup the taps
    //[self setupTap];
    
    // Setup the sensor fusion
    [self setupSensorFusion];
    
    // Allow for recording and playback of sound
    [self setupAudioAsMixed];
    
    // Capture the phones main camera
    // GLKView *view = (GLKView *)self.view;
    // [self captureBackCamera : view ];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self setupAudioAsMixed];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.drawableMultisample = GLKViewDrawableMultisampleNone;
    //view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
    view.multipleTouchEnabled = true;
    
    [self setupGL];
    
    self.preferredFramesPerSecond = 60;
    
    [self gameSetup];
}

static void completionCallback (SystemSoundID  mySSID, void* myself)
{
}

-(void)vibrate
{
    AudioServicesAddSystemSoundCompletion (kSystemSoundID_Vibrate,NULL,NULL,
                                           completionCallback,(__bridge void*) self);
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}

////////////////////////////////////////////////////////////////////////////////
// viewDidUnload

- (void)viewDidUnload
{
    // Release the renderer
    RsImpl::pInstance()->Destroy();
    
    // Unload the view
    [super viewDidUnload];
    
    // End OpenGL and its related contexts
    [self tearDownGL];
    
    // Complete tearing down the view conxt
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}

-(MtVector2)getPosition:(UITouch*)touch
{
    // Cache the current position
    CGPoint currentPosition = [touch locationInView:self.view];
    
    MtVector2 v2Position;
    
    if( deviceType == UIDeviceResolution_iPhone65 )
    {
        v2Position.x = currentPosition.x / 2208.0f * 1920.0f * RenderScale;
        v2Position.y = currentPosition.y / 1242.0f * 1080.0f * RenderScale;
    }
    else
    {
        v2Position.x = currentPosition.x * RenderScale;
        v2Position.y = currentPosition.y * RenderScale;
    }
    return v2Position;
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch *touch in touches)
    {
        //NSLog( @"Touches began %d %d\r\n", (int)touch.self, [availableLabels count] );
        
        // Get the index using our dictionary
        NSNumber *label = (NSNumber *)[availableLabels objectAtIndex:0];
        
        CFDictionaryAddValue (touchToLabelMapping, (__bridge const void*)touch, (__bridge_retained const void*)label );
        
        [availableLabels removeObjectAtIndex:0];
        
        int touchIndex = (int)[label integerValue];
        
        MtVector2 v2Position = [self getPosition:touch];
        ShTouch::BeginTouch( touchIndex, v2Position );
    }
}

-(void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch *touch in touches)
    {
        // Get the label for this touch
        NSNumber *label = (__bridge_transfer NSNumber *)CFDictionaryGetValue(touchToLabelMapping, (__bridge const void*)touch);
        
        //NSLog( @"Touches ended %d %d\r\n", (int)touch.self, [label integerValue] );
        
        int touchIndex = (int)[label integerValue];
        
        MtVector2 v2Position = [self getPosition:touch];
        
        ShTouch::EndTouch( touchIndex, v2Position );
        
        // Remove this from the dictionary
        [availableLabels insertObject:label atIndex:0];
        
        CFDictionaryRemoveValue (touchToLabelMapping, (__bridge const void*)touch);
    }
}

-(void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch *touch in touches)
    {
        //NSLog( @"Touches moved %d\r\n", (int)touch.self );
        
        // Get the label for this touch
        NSNumber *label = (__bridge NSNumber *)CFDictionaryGetValue(touchToLabelMapping, (__bridge const void*)touch);
        
        int touchIndex = (int)[label integerValue];
        
        MtVector2 v2Position = [self getPosition:touch];
        
        ShTouch::MoveTouch( touchIndex, v2Position );
    }
}

-(void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch *touch in touches)
    {
        CGPoint currentPosition = [touch locationInView:self.view];
        
        // Get the label for this touch
        NSNumber *label = (__bridge_transfer NSNumber *)CFDictionaryGetValue(touchToLabelMapping, (__bridge const void*)touch);
        
        //NSLog( @"Touches ended %d %d\r\n", (int)touch.self, [label integerValue] );
        
        int touchIndex = (int)[label integerValue];
        
        ShTouch::EndTouch( touchIndex, MtVector2( currentPosition.x * RenderScale, currentPosition.y * RenderScale ) );
        
        // Remove this from the dictionary
        [availableLabels insertObject:label atIndex:0];
        
        CFDictionaryRemoveValue (touchToLabelMapping, (__bridge const void*)touch);
    }
}

static bool isShaken = false;

- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event {
    
    if ( event.subtype == UIEventSubtypeMotionShake ) {
        NSLog( @"Device shaken" );
        isShaken = true;
    }
    
    if ([super respondsToSelector:@selector(motionEnded:withEvent:)]) {
        [super motionEnded:motion withEvent:event];
    }
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        printf( "setupGL. glError: 0x%04X", err);
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

// processEvents

-(void)processEvents
{
    BtU32 count = MpPeerToPeer::GetNumSendQueue();
    if( count > 0 )
    {
        // Cache the delegate
        AppDelegate *delegate = (AppDelegate*)[[UIApplication sharedApplication] delegate];
        
        for( BtU32 i=0; i<count; i++ )
        {
            // Cache the send event
            MpEvent event = MpPeerToPeer::PopSendQueue();
            
            // Construct the data
            NSMutableData *data1 = [NSMutableData dataWithBytes:event.m_pData length:event.m_length];
            NSMutableData *data2 = [NSMutableData dataWithBytes:event.m_pData length:event.m_length];
            
            // Concatendate
            [data1 appendData:data2];
            
            // Send the data
            [delegate sendData:data1];
            
            // Remove the data
            MpPeerToPeer::DeleteSendPacket(event);
        }
    }
    
    /*
     if( ShVibration::GetNumItems() )
     {
     // Peek at the vibration action
     ShVibrationAction action = ShVibration::PeekAction();
     
     // Shall we start vibrating
     if( action.m_type == ShVibrationActionType_Start )
     {
     // Pop the action
     ShVibration::PopAction();
     
     // Vibrate the phone
     [self vibrate];
     }
     }
     */
}

static bool isUpdated = false;

- (void)update
{
    // Update the touch
    ShTouch::Update();
    
    if( isShaken )
    {
        // Set the device to indicate it's been shaken
        ShTouch::SetShaken();
        
        // Don't do this again
        isShaken = false;
    }
    
    // Update the project
    myProject.Update();
    
    // Look for programmed events
    [self processEvents];
    
    isUpdated = true;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if( isUpdated == true )
    {
        // Empty render targets
        RsImpl::pInstance()->EmptyRenderTargets();
        
        // Render the project
        myProject.Render();
        
        // Render
        RsImpl::pInstance()->Render();
    }
}

-(void)setupSensorFusion
{
    // Setup the motion manager
    motionManager = [[CMMotionManager alloc] init];
    
    // Capture device motion updates
    BtFloat SampleFrequency = 100.0f;
    [motionManager setDeviceMotionUpdateInterval: 1.0f/SampleFrequency ];   // SampleFrequency given in
    
    if( motionManager.isDeviceMotionAvailable )
    {
        // This device supports DeviceMotion events. Configure the sampling rate and start the IMU with an available reference frame
        [motionManager startDeviceMotionUpdatesUsingReferenceFrame: CMAttitudeReferenceFrameXArbitraryCorrectedZVertical
                                                           toQueue: [[NSOperationQueue alloc] init]
                                                       withHandler: ^(CMDeviceMotion *dmReceived, NSError *error)
         {
             MtVector3 v3Acceleration;
             v3Acceleration.x = motionManager.deviceMotion.userAcceleration.x;
             v3Acceleration.y = motionManager.deviceMotion.userAcceleration.y;
             v3Acceleration.z = motionManager.deviceMotion.userAcceleration.z;
             ShIMU::SetAccelerometer( 0, v3Acceleration );
             
             CMQuaternion currentAtt = motionManager.deviceMotion.attitude.quaternion;
             MtQuaternion quaternion( currentAtt.x, currentAtt.y, currentAtt.z, currentAtt.w );
             ShIMU::SetQuaternion( 0, quaternion );
         }
         ];
    }
}

@end
