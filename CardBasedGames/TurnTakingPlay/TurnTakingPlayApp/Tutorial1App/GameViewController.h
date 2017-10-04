//  GameViewController.h

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <GameKit/GameKit.h>
#import <AVFoundation/AVFoundation.h>

@interface GameViewController : GLKViewController
<
AVCaptureVideoDataOutputSampleBufferDelegate
>
{
    AVCaptureSession                    *captureSession;
}

@property (nonatomic, strong) AVCaptureSession *captureSession;

@end
