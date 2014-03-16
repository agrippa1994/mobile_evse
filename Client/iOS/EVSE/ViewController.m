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
    return 3;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    NSString *strs[3] = {@"6 - 7 Stunden", @"8 - 9 Stunden", @"10 - 11 Stunden"};
    return strs[row];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if(cell == self.startLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"];
        if(state != nil)
        {
            if([state compare:@"0"] == 0)
            {
                int currents[] = {16, 13, 10};
                NSUInteger idx = [self.hourPicker selectedRowInComponent:0];
                [[Client sharedClient] send:[NSString stringWithFormat:@"startloading --current %d", currents[idx]]];
                [self performSegueWithIdentifier:@"startLoadingForm" sender:self];
                return;
            }
        }
        
        NSString *title = @"Fehler";
        NSString *message = @"Sie müssen die Ladung stoppen, um eine neue starten zu können";
        
        UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [view show];
        
    }
    
    else if(cell == self.stopLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"];
        if(state != nil)
        {
            if([state compare:@"1"] == 0)
            {
                [self performSegueWithIdentifier:@"stopLoadingForm" sender:self];
                return;
            }

        }
        NSString *title = @"Fehler";
        NSString *message = @"Momentan lädt die Tankstelle nicht!";
        
        UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [view show];
    }
}

//  onKeyAndValue wird vom Clienten über eine Delegation aufgerufen
//  Hier wird es verwendet, um die Temperaturen, Status, etc. einzulesen und zu visualisieren
- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    if([key compare:@"state"] == 0)
    {
        [self.loadingStateCell.detailTextLabel setText:val];
    }
    
    if([key compare:@"temperature"] == 0)
    {
        [self.temperatureCell.detailTextLabel setText:val];
    }
}

@end
