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
    return 7;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [NSString stringWithFormat:@"%d Stunde%c", row + 1, (row == 0) ? 'n' : '\0'];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    
    if(cell == self.startLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"];
        if(state != nil)
        {
            if([state compare:@"0"] == 0)
            {
                // TODO: Ladung starten
                
                return;
            }
            
        }
        
        // TODO: Fehler ausgeben
    }
    
    else if(cell == self.stopLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"];
        if(state != nil)
        {
            if([state compare:@"1"] == 0)
            {
                // TODO: Ladung stoppen
                
                return;
            }
            
        }

        // TODO: Fehler ausgeben
        
    }
    
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
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
