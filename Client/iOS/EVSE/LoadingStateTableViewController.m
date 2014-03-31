//
//  LoadingStateTableViewController.m
//  EVSE
//
//  Created by Mani on 31.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "LoadingStateTableViewController.h"

@interface LoadingStateTableViewController ()
@property (weak, nonatomic) IBOutlet UITableViewCell *loadingCurrentCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *currentLoadingTimeCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *statusCell;
@end

@implementation LoadingStateTableViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.loadingCurrentCell.detailTextLabel.text = @"";
    self.currentLoadingTimeCell.detailTextLabel.text = @"";
    self.statusCell.detailTextLabel.text = @"asdf";
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
