//
//  DebugTableViewController.m
//  EVSE
//
//  Created by Mani on 03.03.14.
//  Copyright (c) 2014 Mani. All rights reserved.
//

#import "DebugTableViewController.h"

@interface DebugTableViewController ()

@end

@implementation DebugTableViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) viewDidAppear:(BOOL)animated
{
    [[Client sharedClient] addDelegate:self];
}

- (void) viewDidDisappear:(BOOL)animated
{
    [[Client sharedClient] rmDelegate:self];
}

- (void)client:(Client *)p openRequest:(BOOL)isOpen
{
    
}

- (void)client:(Client *)p onDisconnect:(BOOL)disconnected
{
    
}

- (void)client:(Client *)p onData:(const uint8_t *)data length:(NSInteger)len
{
    
}

- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    NSArray *insertIndexPath =  [NSArray arrayWithObjects:
                                [NSIndexPath indexPathForRow:0 inSection:0],
                                 nil];
    
    UITableView *tv = (UITableView *)self.view;
    [tv beginUpdates];
    [tv insertRowsAtIndexPaths:insertIndexPath withRowAnimation:UITableViewRowAnimationRight];
    [tv endUpdates];
    
    UITableViewCell *cell = [tv cellForRowAtIndexPath:[insertIndexPath firstObject]];
    cell.textLabel.text = key;
    cell.detailTextLabel.text = val;
}

@end
