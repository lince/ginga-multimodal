//
//  GingaChatAppDelegate.h
//  GingaChat
//
//  Created by Jose A C Martins JR on 24/08/10.
//  Copyright none 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GingaChatViewController;

@interface GingaChatAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    GingaChatViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet GingaChatViewController *viewController;

@end

