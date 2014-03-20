//
//  StartLoadingTableViewController.m
//  EVSE
//
//  Created by Manuel on 16.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "StartLoadingTableViewController.h"


@interface StartLoadingTableViewController ()
@property (weak, nonatomic) IBOutlet UITableViewCell *requestLoadingCurrentCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *cancelLoadingCell;

@end

@implementation StartLoadingTableViewController

- (void)viewDidAppear:(BOOL)animated
{
    [[Client sharedClient] addDelegate:self];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [[Client sharedClient] rmDelegate:self];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    if(cell == self.cancelLoadingCell)
    {
        [[Client sharedClient] send:@"stoploading"];
        
        [self dismissViewControllerAnimated:YES completion:nil];
    }
    
}

- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    if([key compare:@"requestLoadingCurrent"] == 0)
    {
        self.requestLoadingCurrentCell.detailTextLabel.text = val;
    }
}

- (void)client:(Client *)p onDisconnect:(BOOL)disconnected
{
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
