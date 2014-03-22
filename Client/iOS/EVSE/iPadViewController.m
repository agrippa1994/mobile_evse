//
//  iPadViewController.m
//  EVSE
//
//  Created by Manuel on 22.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "iPadViewController.h"

@interface iPadViewController ()

@end

@implementation iPadViewController


- (void)viewDidLoad
{
    self.delegate = self;
}

- (BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc inOrientation:(UIInterfaceOrientation)orientation
{
    return NO;
}

@end
