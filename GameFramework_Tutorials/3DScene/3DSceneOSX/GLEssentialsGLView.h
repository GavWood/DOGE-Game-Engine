#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface GLEssentialsGLView : NSOpenGLView
{
	CVDisplayLinkRef displayLink;
}

@end
