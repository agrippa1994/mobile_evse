//
//  StopLoadingTableViewController.m
//  EVSE
//
//  Created by Manuel on 16.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "StopLoadingTableViewController.h"

@interface StopLoadingTableViewController ()
@property (weak, nonatomic) IBOutlet UITableViewCell *stopLoadingCell;

-(void)didBecomeActive;
@end

@implementation StopLoadingTableViewController

- (void)viewDidAppear:(BOOL)animated
{
    [[Client sharedClient] addDelegate:self];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(didBecomeActive) name:UIApplicationDidBecomeActiveNotification object:nil];
    
    if(![[Client sharedClient] isConnected])
    {
        [self performSegueWithIdentifier:@"showNetworkForm" sender:self];
    }
}

- (void)viewDidDisappear:(BOOL)animated
{
    [[Client sharedClient] rmDelegate:self];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIApplicationDidBecomeActiveNotification object:nil];
}

- (void)didBecomeActive
{
    if(![[Client sharedClient] isConnected])
    {
        [self performSegueWithIdentifier:@"showNetworkForm" sender:self];
    }
}

- (void)client:(Client *)p onDisconnect:(BOOL)disconnected
{
    [self performSegueWithIdentifier:@"showNetworkForm" sender:self];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if(cell == self.stopLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"];
        if(state != nil)
        {
            if([state compare:@"1"] == 0)
            {
                return;
            }
            
        }
        NSString *title = @"Fehler";
        NSString *message = @"Momentan lädt die Tankstelle nicht!";
        
        UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [view show];
    }
}

@end
