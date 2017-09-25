//
//  GameViewController.h
//  Tutorial1App
//
//  Created by localadmin on 25/11/2015.
//  Copyright © 2015 localadmin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <GameKit/GameKit.h>
#import <AVFoundation/AVFoundation.h>
#import <ARKit/ARKit.h>

@interface GameViewController : GLKViewController
<
AVCaptureVideoDataOutputSampleBufferDelegate,
ARSessionDelegate
>
{
    AVCaptureSession                           *captureSession;
}

@property (nonatomic, strong) AVCaptureSession *captureSession;
@property (nonatomic) ARTrackingState currentTrackingState;

@property (nonatomic, retain) ARWorldTrackingConfiguration *arConfig;
@property (nonatomic, retain) ARSession *arSession;

@end
