//
//  AppDelegate.h
//  TalkAboutSexApp
//
//  Created by localadmin on 04/06/2015.
//  Copyright (c) 2015 localadmin. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MultipeerConnectivity/MultipeerConnectivity.h>
#import "McManager.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property MCManager *sessionManager;

-(void)sendData:(NSData*)data;

@end
