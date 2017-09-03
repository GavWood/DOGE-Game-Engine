//
//  AppDelegate.m
//  Tutorial1App
//
//  Created by localadmin on 25/11/2015.
//  Copyright © 2015 localadmin. All rights reserved.
//

#import "AppDelegate.h"
#import "PeerToPeer.h"
#import "string.h"

@interface AppDelegate ()

@end

@implementation AppDelegate
@synthesize sessionController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    sessionController = [[SessionController alloc] init];
    self.sessionController.delegate = self;
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


-(void)sendData:(NSData*)data
{
    [self.sessionController sendData:data];
}

- (void)sessionDidChangeState
{
    // Add the peers to our vector
    unsigned long count = self.sessionController.connectedPeers.count;
    for( unsigned long i=0; i<count; i++ )
    {
        MCPeerID *peerID = self.sessionController.connectedPeers[i];
        
        // Cache the IOS network name
        BtU8 *networkName = (BtU8*)( [[peerID displayName] UTF8String] );
        
        // Add the peer
        MpPeer peer;
        peer.m_address = 0;
        peer.m_port = 0;
        peer.m_startTime = MpPeerToPeer::GetTimeStarted();
        strcpy( (char*)peer.m_networkName, (char*)networkName );
        MpPeerToPeer::AddPeer( peer );
    }
}

@end
