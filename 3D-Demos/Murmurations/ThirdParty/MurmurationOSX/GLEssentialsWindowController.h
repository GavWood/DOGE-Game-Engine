#import <Cocoa/Cocoa.h>
#import "GLEssentialsGLView.h"

@interface GLEssentialsWindowController : NSWindowController
{
	// IBOutlet must be used so that, in Inteface Builder,
	// we can connect the view in the NIB to windowedView
	IBOutlet GLEssentialsGLView *view;
}

@end
