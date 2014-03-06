//
//  ViewController.m
//  EVSE
//
//  Created by Manuel on 05.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UIPickerView *hourPicker;
@property (weak, nonatomic) IBOutlet UITableViewCell *startLoadingCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *stopLoadingCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *loadingStateCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *temperatureCell;

- (void) showNetworkForm;

@end

@implementation ViewController

- (void)viewDidLoad
{
    self.hourPicker.delegate = self;
    self.hourPicker.dataSource = self;
}

- (void)viewDidAppear:(BOOL)animated
{
    [[Client sharedClient] addDelegate:self];
    if(![[Client sharedClient] isConnected])
        [self showNetworkForm];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [[Client sharedClient] rmDelegate:self];
}

- (void)showNetworkForm
{
    [self performSegueWithIdentifier:@"showNetworkForm" sender:self];
}


- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return 2;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    switch(row)
    {
        case 0:
            return @"1-2 Stunden";
        case 1:
            return @"6-8 Stunden";
        default:
            return @"";
    }
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    
    if(cell == self.startLoadingCell)
    {
        
    }
    
    else if(cell == self.stopLoadingCell)
    {
        
    }
    
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    if([key compare:@"state"] == 0)
    {
        [self.loadingStateCell.detailTextLabel setText:val];
    }
    
    if([key compare:@"PWM"] == 0)
    {
        [self.temperatureCell.detailTextLabel setText:val];
    }
}

@end
