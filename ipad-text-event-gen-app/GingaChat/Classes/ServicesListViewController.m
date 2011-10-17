//
//  ServicesListViewController.m
//  ControleRemoto
//
//  Created by José Augusto Costa Martins Jr. on 03/03/10.
//  Copyright 2010 Universidade de São Paulo. All rights reserved.
//

#import "ServicesListViewController.h"

@implementation ServicesListViewController

@synthesize servicesBrowser, myNetService, remoteControlConnection;

/*
 - (id)initWithStyle:(UITableViewStyle)style {
 // Override initWithStyle: if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
 if (self = [super initWithStyle:style]) {
 }
 return self;
 }
 */


- (void)viewDidLoad {
	servicesBrowser = [[ServicesBrowser alloc] init];
	servicesBrowser.delegate = self;
	[servicesBrowser start];
}

- (void)viewDidUnload{
	[servicesBrowser stop];
}

- (void)activate{
	[servicesBrowser start];
}

/*
 // Override to allow orientations other than the default portrait orientation.
 - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
 // Return YES for supported orientations
 return (interfaceOrientation == UIInterfaceOrientationPortrait);
 }
 */

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

/*- (void)viewDidUnload {
 // Release any retained subviews of the main view.
 // e.g. self.myOutlet = nil;
 }*/


#pragma mark Table view methods

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}


// Customize the number of rows in the table view.
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	NSLog(@"Numero de Servidores %i", [servicesBrowser.services count] );
    return [servicesBrowser.services count];
}


// Customize the appearance of table view cells.
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
    // Set up the cell...
	NSNetService *service = [servicesBrowser.services objectAtIndex:indexPath.row];
	cell.textLabel.text = [service name];
	//cell.imageView.image = [UIImage imageNamed:@"ok.png"];
    return cell;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
	[self setMyNetService:[servicesBrowser.services objectAtIndex:indexPath.row]];
	NSLog(@"setMyNetService: %@", [self myNetService]);
	
	//remoteControlConnection = [[Connection alloc] init];

	//[remoteControlConnection sendSingleStringMessage:@"availability" toNetService:[self myNetService]];
	
	
	
	//if (result == 0) {
	//	cell.imageView.image = [UIImage imageNamed:@"ok.png"];
	//}
	//else if (result == 1){
	//	cell.imageView.image = [UIImage imageNamed:@"error.png"];
	//}
	
	
	//GingaChatAppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
	
	//appDelegate.viewController.detailItem = [servicesBrowser.services objectAtIndex:indexPath.row];
	
	// Navigation logic may go here. Create and push another view controller.
	// AnotherViewController *anotherViewController = [[AnotherViewController alloc] initWithNibName:@"AnotherView" bundle:nil];
	// [self.navigationController pushViewController:anotherViewController];
	// [anotherViewController release];
	// [self.navigationController dismissModalViewControllerAnimated:YES];
	
}


/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */


/*
 // Override to support editing the table view.
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
 
 if (editingStyle == UITableViewCellEditingStyleDelete) {
 // Delete the row from the data source
 [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:YES];
 }   
 else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
 }   
 }
 */


/*
 // Override to support rearranging the table view.
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
 }
 */


/*
 // Override to support conditional rearranging of the table view.
 - (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
 // Return NO if you do not want the item to be re-orderable.
 return YES;
 }
 */


- (void)updateServicesList{
	[servicesList reloadData];
}

- (void)dealloc {
	[servicesBrowser release];
	//[connection release];
	self.servicesBrowser = nil;
    [super dealloc];
}


@end

