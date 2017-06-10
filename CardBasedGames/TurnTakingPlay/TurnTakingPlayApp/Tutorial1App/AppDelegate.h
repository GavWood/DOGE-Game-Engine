//
//  AppDelegate.h
//  TalkAboutSexApp
//
//  Created by localadmin on 04/06/2015.
//  Copyright (c) 2015 localadmin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MultipeerConnectivity/MultipeerConnectivity.h>
#import "SessionController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate, SessionControllerDelegate>

@property (strong, nonatomic) UIWindow *window;
@property SessionController *sessionController;

-(void)sendData:(NSData*)data;

@end
