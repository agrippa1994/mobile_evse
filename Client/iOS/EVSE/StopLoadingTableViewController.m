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
@end

@implementation StopLoadingTableViewController

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
