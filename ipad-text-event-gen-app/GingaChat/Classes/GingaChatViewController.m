//
//  GingaChatViewController.m
//  GingaChat
//
//  Created by Jose A C Martins JR on 24/08/10.
//  Copyright none 2010. All rights reserved.
//

#import "GingaChatViewController.h"

@implementation GingaChatViewController

@synthesize popoverController;
@synthesize customTextField, customTextView, customToolbar, myButton, myBarButton;
@synthesize remoteControlConnection, servicesListViewController;
@synthesize detailItem;


/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	customTextField.delegate = self;
	NSString* path = [[NSBundle mainBundle] pathForResource:@"comeco" ofType:@"xml"];
	initial = [[NSString alloc] initWithContentsOfFile:path];
	final = [[NSString alloc] initWithString:@"\t</body>\n</multimodal>"];
	remoteControlConnection = [[Connection alloc] init];
	NSLog(@"X=%f Y=%f", customTextField.center.x, customTextField.center.y);
	
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


-(IBAction) showServices: (id) sender{
	
	if (self.popoverController == nil) {
		ServicesListViewController *services = 
				[[ServicesListViewController alloc] 
					initWithNibName:@"ServicesListViewController" 
					bundle:[NSBundle mainBundle]];
		
		UIPopoverController *popover = 
						[[UIPopoverController alloc] initWithContentViewController:services]; 
		
        popover.delegate = self;
		servicesListViewController = services;
        [services release];
		
        self.popoverController = popover;
        [popover release];
	}
	
	[self.popoverController 
	 presentPopoverFromBarButtonItem:sender 
	 permittedArrowDirections:UIPopoverArrowDirectionAny 
	 animated:YES];
}

- (void)setDetailItem:(id)newDetailItem {
	
    if (detailItem != newDetailItem) {
        [detailItem release];
        detailItem = [newDetailItem retain];
		
        //---update the view---
        //ServicesListViewController.text = [detailItem description];
    }
	
    if (popoverController != nil) {
        [popoverController dismissPopoverAnimated:YES];
    }        
	
}

- (void) willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    [self adjustViewsForOrientation:toInterfaceOrientation];
}

- (void) adjustViewsForOrientation:(UIInterfaceOrientation)orientation {
    if (orientation == UIInterfaceOrientationLandscapeLeft || orientation == UIInterfaceOrientationLandscapeRight) {
		customTextField.autoresizingMask = (UIViewAutoresizingFlexibleWidth |UIViewAutoresizingFlexibleLeftMargin
											|UIViewAutoresizingFlexibleRightMargin
											|UIViewAutoresizingFlexibleBottomMargin);
		
		customTextField.center = CGPointMake(337.5f, 368.5f);
		
		myButton.center = CGPointMake(customTextField.bounds.size.width+(myButton.bounds.size.width), customTextField.center.y);
		myButton.autoresizingMask = (UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleLeftMargin
									 |UIViewAutoresizingFlexibleRightMargin
									 |UIViewAutoresizingFlexibleBottomMargin);
									 
		customTextView.autoresizingMask = (UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight
										   |UIViewAutoresizingFlexibleLeftMargin|UIViewAutoresizingFlexibleRightMargin
										   |UIViewAutoresizingFlexibleTopMargin|UIViewAutoresizingFlexibleBottomMargin);
		
        customToolbar.autoresizingMask = (UIViewAutoresizingFlexibleLeftMargin|UIViewAutoresizingFlexibleRightMargin
										  |UIViewAutoresizingFlexibleTopMargin|UIViewAutoresizingFlexibleBottomMargin
										  |UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight);

    }
    else if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) {
		
		customTextField.autoresizingMask = (UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleLeftMargin
											|UIViewAutoresizingFlexibleRightMargin
											|UIViewAutoresizingFlexibleBottomMargin);
		
		customTextField.center = CGPointMake(450.0f, 688.5f);
		
		myButton.center = CGPointMake(customTextField.bounds.size.width+(myButton.bounds.size.width), customTextField.center.y);
		myButton.autoresizingMask = (UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleLeftMargin
									 |UIViewAutoresizingFlexibleRightMargin
									 |UIViewAutoresizingFlexibleBottomMargin);
		
		customTextView.autoresizingMask = (UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight
										   |UIViewAutoresizingFlexibleLeftMargin|UIViewAutoresizingFlexibleRightMargin
										   |UIViewAutoresizingFlexibleTopMargin|UIViewAutoresizingFlexibleBottomMargin);
        customToolbar.autoresizingMask = (UIViewAutoresizingFlexibleLeftMargin|UIViewAutoresizingFlexibleRightMargin
										  |UIViewAutoresizingFlexibleTopMargin|UIViewAutoresizingFlexibleBottomMargin
										  |UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight);
		
    }
}

- (IBAction)sendTextChat:(id)sender{
	NSString* conteudoChat;
	conteudoChat = [[NSString alloc] initWithString:initial];
	conteudoChat = [conteudoChat stringByAppendingString:@"\t\t<text>"];
	conteudoChat = [conteudoChat stringByAppendingString:customTextField.text];
	conteudoChat = [conteudoChat stringByAppendingString:@"</text>\n"];
	conteudoChat = [conteudoChat stringByAppendingString:final];
	customTextView.text = @"";
	customTextView.text = customTextField.text;
	customTextField.text = @"";
	[remoteControlConnection sendSingleStringMessage:conteudoChat toNetService:[servicesListViewController myNetService]];

}

- (BOOL)textFieldShouldReturn:(UITextField *)theTextField {
	
	NSString* conteudoChat;
	conteudoChat = [[NSString alloc] initWithString:initial];
	conteudoChat = [conteudoChat stringByAppendingString:@"\t\t<text>"];
	conteudoChat = [conteudoChat stringByAppendingString:customTextField.text];
	conteudoChat = [conteudoChat stringByAppendingString:@"</text>\n"];
	conteudoChat = [conteudoChat stringByAppendingString:final];
	customTextView.text = @"I'm fine, thanks!!!";
	customTextView.text = customTextField.text;
	customTextField.text = @"";
	[remoteControlConnection sendSingleStringMessage:conteudoChat toNetService:[servicesListViewController myNetService]];
	
	return NO;
}
	
- (void)dealloc {
	[popoverController release];
	[Connection release];
	[customTextField release]; 
	[customTextView release];
	[customToolbar release];
    [detailItem release];
    [super dealloc];
}

@end
