//  GameViewController.mm

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
#import <CoreVideo/CoreVideo.h>

#import <stdio.h>
#import <sys/utsname.h>
#import "GaProject.h"
#import "ApConfig.h"
#import "RsImpl.h"
#import "ShTouch.h"
#import "BtTime.h"
#import "SdSoundImpl.h"
#import "ShIMU.h"
#import "BtString.h"
#import "MtVector3.h"
#import "BtTime.h"
#import "ShNetwork.h"
#import "ScMain.h"
#import "AppDelegate.h"
#import "PeerToPeer.h"
#import "ShCamera.h"

// My globals
ScMain myProject;
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

void interruptionListenerCallback (void *inUserData, UInt32 interruptionState )
{
    // For breakpointing
    int a=0;
    a++;
}

// Model                            Resolution
// iPhone 1,3,3GS Standard Display  (320x480px)
// iPhone 4,4S Retina Display 3.5"  (640x960px)
// iPhone 5 Retina Display 4"       (640x1136px)
// iPad 1,2 Standard Display        (1024x768px)
// iPhone 5
// iPad 3 Retina Display            (2048x1536px)
// iPhone 6                         ()
// iPhone 6 Plus                    (1280x1080px)
// iPhone X                         (2436x1125)

MtVector2 v2PixelResolution( 0, 0 );
MtVector2 g_v2TouchScale( 0, 0 );

- (void)resolution
{
    UIScreen *mainScreen = [UIScreen mainScreen];
    
    RenderScale = ([mainScreen respondsToSelector:@selector(scale)] ? mainScreen.scale : 1.0f);
    
    CGFloat pixelWidth  = CGRectGetWidth(  mainScreen.nativeBounds );
    CGFloat pointWidth  = CGRectGetWidth(  mainScreen.bounds );
    
    CGFloat pixelHeight = CGRectGetHeight( mainScreen.nativeBounds );
    CGFloat pointHeight = CGRectGetHeight( mainScreen.bounds );
    
    if( pixelWidth < pixelHeight )
    {
        BtSwap( pixelWidth, pixelHeight );
    }
    if( pointWidth < pointHeight )
    {
        BtSwap( pointWidth, pointHeight );
    }
    
    // Touch setup for landscape
    g_v2TouchScale.x = pixelHeight / pointHeight,
    g_v2TouchScale.y = pixelWidth  / pointWidth,
    
    v2PixelResolution = MtVector2( pixelWidth, pixelHeight );
    
    // Set the device pixel dimension in our renderer
    RsImpl::pInstance()->SetDimension( v2PixelResolution );
}

NSString* deviceFullName()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    
    return [NSString stringWithCString:systemInfo.machine
                              encoding:NSUTF8StringEncoding];
}

-(void)GetDeviceType
{
    NSString *iDeviceFullName = deviceFullName();
    (void)iDeviceFullName;
    
    NSString *iDeviceType = [[UIDevice currentDevice] model];
    if( [iDeviceType isEqual:@"iPhone"] )
    {
        ApConfig::SetDevice( ApDevice_iPhone );
    }
    else if( [iDeviceType isEqual:@"iPad"] )
    {
        ApConfig::SetDevice( ApDevice_iPad );
    }
    else{
        ApConfig::SetDevice( ApDevice_iPhone );
    }
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

- (void)gameSetup
{
    BtTime::SetTick( 1.0f / 30.0f );
    
    // Set the apps configuration
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
    
    // Set the device
    ApConfig::SetDevice( ApDevice_iPhone );
    
    // Set the platform
    ApConfig::SetPlatform( ApPlatform_GLES );
    
#if TARGET_IPHONE_SIMULATOR
    ApConfig::SetSimulator( BtTrue );
#endif
    
    MtVector2 v2Dimension( 0, 0 );
    RenderScale = 1;
    
    // Get the resolution
    [self resolution];
    
    // Setup device
    [self GetDeviceType];

    // Create the renderer implementation
    RsImpl::pInstance()->Create();
    
    // Destroy the sound manager
    SdSoundImpl::CreateManager();
    
    // Init the time
    BtTime::Init();
    
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
    
    // Allow for recording and playback of sound
    [self setupAudioAsMixed];
}


- (void)setupAR
{
    if (@available(iOS 11.0, *))
    {
        self.arSession = [ARSession new];
        self.arSession.delegate = self;
    }
}

- (void)runAR
{
    if (@available(iOS 11.0, *))
    {
        // Create a session configuration
        ARWorldTrackingConfiguration *configuration = [ARWorldTrackingConfiguration new];
        
        // Run the view's session
        [self.arSession runWithConfiguration:configuration];
    }
}

- (void)pauseAR {
    if (@available(iOS 11.0, *))
    {
        [self.arSession pause];
    }
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
    
    // Capture the phones main camera
    if( ApConfig::IsAR() )
    {
        // initialize video texture cache
        CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, self.context, NULL, &_videoTextureCache);
        if (err){
            NSLog(@"Error creating CVOpenGLESTextureCacheCreate %d", err);
        }
        
        [self setupAR];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self runAR];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self pauseAR];
}

////////////////////////////////////////////////////////////////////////////////
// Vibrate the phone

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
    
    MtVector2 v2PixelPosition;
    v2PixelPosition.x = currentPosition.x * g_v2TouchScale.x;
    v2PixelPosition.y = currentPosition.y * g_v2TouchScale.y;
    return v2PixelPosition;
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


- (void)session:(ARSession *)session didFailWithError:(NSError *)error {
    // Present an error message to the user
}

- (void)sessionWasInterrupted:(ARSession *)session {
    // Inform the user that the session has been interrupted, for example, by presenting an overlay
    
}

- (void)sessionInterruptionEnded:(ARSession *)session {
    // Reset tracking and/or remove existing anchors if consistent tracking is required
    
}

CVOpenGLESTextureRef createTexture( CVOpenGLESTextureCacheRef textureCache,
                                    CVPixelBufferRef pixelBuffer,
                                    int planeIndex,
                                    GLenum format )
{
    GLsizei imageWidth = (GLsizei)CVPixelBufferGetWidthOfPlane(pixelBuffer, planeIndex);
    GLsizei imageHeight = (GLsizei)CVPixelBufferGetHeightOfPlane(pixelBuffer, planeIndex);
    
    CVOpenGLESTextureRef texture = NULL;
    CVReturn err = noErr;
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                       textureCache,
                                                       pixelBuffer,
                                                       NULL,
                                                       GL_TEXTURE_2D,
                                                       format,
                                                       imageWidth,
                                                       imageHeight,
                                                       format,
                                                       GL_UNSIGNED_BYTE,
                                                       planeIndex,
                                                       &texture);
    
    if (err != kCVReturnSuccess) {
        CVBufferRelease(texture);
        texture = nil;
        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
    }
    return texture;
}

- (void)session:(ARSession *)session didUpdateFrame:(ARFrame *)frame
{
    // Release the last version of the textures
    CVBufferRelease(yTexture);
    CVBufferRelease(CbCrTexture);
    
    // Get handles to resources
    CVPixelBufferRef pixelBuffer = frame.capturedImage;
    CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
    
    // Check the number of planes
    int numberOfPlanes = (int)CVPixelBufferGetPlaneCount(pixelBuffer);
    assert( numberOfPlanes > 1 );
   
    // Create the Y texture. This determines the brightness of the colour (referred to as luminance or luma)
    yTexture = createTexture(_videoTextureCache, pixelBuffer, 0, GL_LUMINANCE );
    
    // cbcr components determine the color itself (the chroma).
    CbCrTexture = createTexture(_videoTextureCache, pixelBuffer, 1, GL_LUMINANCE_ALPHA );
    
    // Bind the Y texture and set up its texture stage states
    glBindTexture(CVOpenGLESTextureGetTarget(yTexture), CVOpenGLESTextureGetName(yTexture));
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(CVOpenGLESTextureGetTarget(yTexture), 0);
    
    // Bind the cbcr texture and set up its texture stage states
    glBindTexture(CVOpenGLESTextureGetTarget(CbCrTexture), CVOpenGLESTextureGetName(CbCrTexture));
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(CVOpenGLESTextureGetTarget(CbCrTexture), 0);
    
    int yTextureGL = CVOpenGLESTextureGetName(yTexture);
    int CbCrTextureGL = CVOpenGLESTextureGetName(CbCrTexture);
    
    ShCamera::Update( yTextureGL, CbCrTextureGL );
    
    // Unlock the base address
    CVPixelBufferUnlockBaseAddress( pixelBuffer, kCVPixelBufferLock_ReadOnly );
    
    // Flush every update
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
    
    BtAssert( CVOpenGLESTextureGetTarget(CbCrTexture) == GL_TEXTURE_2D );
    
    // Set the orientation
    UIInterfaceOrientation orient = [[UIApplication sharedApplication] statusBarOrientation];
    matrix_float4x4 rotatedMatrix = matrix_identity_float4x4;
    
    // rotation  matrix
    // [ cos    -sin]
    // [ sin     cos]
    switch (orient) {
        case UIInterfaceOrientationPortrait:
            rotatedMatrix.columns[0][0] = 0;
            rotatedMatrix.columns[0][1] = 1;
            rotatedMatrix.columns[1][0] = -1;
            rotatedMatrix.columns[1][1] = 0;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            rotatedMatrix.columns[0][0] = -1;
            rotatedMatrix.columns[0][1] = 0;
            rotatedMatrix.columns[1][0] = 0;
            rotatedMatrix.columns[1][1] = -1;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            rotatedMatrix.columns[0][0] = 0;
            rotatedMatrix.columns[0][1] = -1;
            rotatedMatrix.columns[1][0] = 1;
            rotatedMatrix.columns[1][1] = 0;
            break;
        default:
            break;
    }
    
    // Create a matrix in the game engine's representation from the frame's own world transform
    matrix_float4x4 matrix = matrix_multiply(frame.camera.transform, rotatedMatrix);
    MtMatrix4 m4Transform;
    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            m4Transform[col][row] = matrix.columns[col][row];
        }
    }
    
    // Extract the position from the world transform
    matrix_float4x4 original = frame.camera.transform;
    MtVector3 v3Position( original.columns[3].x,
                          original.columns[3].y,
                         -original.columns[3].z );
    
    // Set these on the IMU class so we can use them in the game logic
    ShIMU::SetPosition( 0, v3Position );
    MtQuaternion quaternion = m4Transform;
    ShIMU::SetQuaternion( 0, quaternion );
}

- (void)session:(ARSession *)session cameraDidChangeTrackingState:(ARCamera *)camera {
    ARTrackingState trackingState = camera.trackingState;
    if (self.currentTrackingState == trackingState) {
        return;
    }
    self.currentTrackingState = trackingState;
    
    switch(trackingState) {
        case ARTrackingStateNotAvailable:
            NSLog(@"Camera tracking is not available on this device");
            break;
            
        case ARTrackingStateLimited:
            switch(camera.trackingStateReason) {
                case ARTrackingStateReasonExcessiveMotion:
                    NSLog(@"Limited tracking: slow down the movement of the device");
                    break;
                    
                case ARTrackingStateReasonInsufficientFeatures:
                    NSLog(@"Limited tracking: too few feature points, view areas with more textures");
                    break;
                    
                case ARTrackingStateReasonNone:
                    NSLog(@"Tracking limited none");
                    break;
                case ARTrackingStateReasonInitializing:
                    break;
                
                default:
                    break;
            }
            break;
            
        case ARTrackingStateNormal:
            //[self showMessage:@"Tracking is back to normal"];
            break;
    }
}

@end
