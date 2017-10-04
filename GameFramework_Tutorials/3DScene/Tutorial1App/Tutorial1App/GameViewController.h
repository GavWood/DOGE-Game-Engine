//  GameViewController.h

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <GameKit/GameKit.h>
#import <AVFoundation/AVFoundation.h>
#import <ARKit/ARKit.h>
#import <CoreVideo/CoreVideo.h>

@interface GameViewController : GLKViewController
<
AVCaptureVideoDataOutputSampleBufferDelegate,
ARSessionDelegate
>
{
    CVOpenGLESTextureRef                       yTexture;
    CVOpenGLESTextureRef                       CbCrTexture;
    CVOpenGLESTextureCacheRef                  _videoTextureCache;
    AVCaptureSession                           *captureSession;
}

@property (nonatomic, strong) AVCaptureSession *captureSession;
@property (nonatomic) ARTrackingState currentTrackingState;

@property (nonatomic, retain) ARWorldTrackingConfiguration *arConfig;
@property (nonatomic, retain) ARSession *arSession;

@end
