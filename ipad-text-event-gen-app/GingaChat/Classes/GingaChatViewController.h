//
//  GingaChatViewController.h
//  GingaChat
//
//  Created by Jose A C Martins JR on 24/08/10.
//  Copyright none 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ServicesListViewController.h"
#import "Connection.h"
@class ServicesListViewController;

@interface GingaChatViewController : UIViewController <UIPopoverControllerDelegate, UITextFieldDelegate> {

	UIPopoverController*		popoverController;
	UITextView*					customTextView;
	UITextField*				customTextField;
	UIToolbar*					customToolbar;
	Connection*					remoteControlConnection;
	ServicesListViewController*	servicesListViewController;
	UIButton*					myButton;
	UIBarButtonItem*			myBarButton;
	NSString*					initial;
	NSString*					final;
	id							detailItem;
}

@property(nonatomic, retain) IBOutlet UIPopoverController*	popoverController;	
@property(nonatomic, retain) IBOutlet UITextView*			customTextView;
@property(nonatomic, retain) IBOutlet UITextField*			customTextField;
@property(nonatomic, retain) IBOutlet UIToolbar*			customToolbar;
@property(nonatomic, retain) IBOutlet UIButton*				myButton;
@property(nonatomic, retain) IBOutlet UIBarButtonItem*		myBarButton;
@property(nonatomic, retain) ServicesListViewController*	servicesListViewController;
@property(nonatomic, retain) Connection*					remoteControlConnection;
@property(nonatomic, retain) id								detailItem;

- (void) adjustViewsForOrientation:(UIInterfaceOrientation)orientation;
- (IBAction)showServices:(id)sender;
- (IBAction)sendTextChat:(id)sender;

@end

