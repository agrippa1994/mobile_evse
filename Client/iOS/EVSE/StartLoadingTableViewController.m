//
//  StartLoadingTableViewController.m
//  EVSE
//
//  Created by Manuel on 16.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "StartLoadingTableViewController.h"


@interface StartLoadingTableViewController ()
@property (weak, nonatomic) IBOutlet UIPickerView *hourPicker;
@property (weak, nonatomic) IBOutlet UITableViewCell *startLoadingCell;

-(void)didBecomeActive;
@end

@implementation StartLoadingTableViewController

- (void)viewDidLoad
{
    self.hourPicker.delegate = self;
    self.hourPicker.dataSource = self;
}

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


- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return 9;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    NSMutableArray *hours = [NSMutableArray array];
    for(int i=4; i<=12; i++)
        [hours addObject:[NSString stringWithFormat:@"%d Stunden", i]];
    
    return [hours objectAtIndex:row];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if(cell == self.startLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"state"];
        if(state != nil)
        {
            if([state compare:@"2"] == 0) // State B
            {
                int currents[] = {18, 16, 14, 13, 12, 11, 10, 9, 8};
                NSUInteger idx = [self.hourPicker selectedRowInComponent:0];
                [[Client sharedClient] send:[NSString stringWithFormat:@"startloading --current %d", currents[idx]]];
                return;
            }
            else
            {
                NSString *title = @"Fehler";
                NSString *message = @"Es muss ein Fahrzeug angeschlossen sein und es darf keine Ladung im Gange sein!";
                
                UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
                [view show];
                return;
            }
        }
    }
}

@end
