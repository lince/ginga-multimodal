//
//  ServicesListViewController.h
//  ControleRemoto
//
//  Created by José Augusto Costa Martins Jr. on 03/03/10.
//  Copyright 2010 Universidade de São Paulo. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ServicesBrowser.h"
#import "Connection.h"
#import "GingaChatAppDelegate.h"
#import "GingaChatViewController.h"

@interface ServicesListViewController : UIViewController <UITableViewDelegate , ServicesBrowserDelegate> {
	
	ServicesBrowser *servicesBrowser;
	IBOutlet UITableView* servicesList;
	Connection *remoteControlConnection;
	NSNetService *myNetService;
}
@property(nonatomic, retain) Connection* remoteControlConnection;
@property(nonatomic, retain) ServicesBrowser* servicesBrowser;
@property(nonatomic, retain) NSNetService* myNetService;


//-(IBAction)selectService;

- (void)activate;

@end
