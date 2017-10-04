//  AppDelegate.h

#import <UIKit/UIKit.h>
#import <MultipeerConnectivity/MultipeerConnectivity.h>
#import "SessionController.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate, SessionControllerDelegate>

@property (strong, nonatomic) UIWindow *window;
@property SessionController *sessionController;

-(void)sendData:(NSData*)data;

@end
