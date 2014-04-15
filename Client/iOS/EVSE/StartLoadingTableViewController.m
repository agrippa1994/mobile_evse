//
//  StartLoadingTableViewController.m
//  EVSE
//
//  Created by Manuel on 16.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "StartLoadingTableViewController.h"


@interface StartLoadingTableViewController ()
{
    UIAlertView *waitAlertView;
    BOOL waitAlertViewActive;
}
@property (weak, nonatomic) IBOutlet UIPickerView *hourPicker;
@property (weak, nonatomic) IBOutlet UITableViewCell *startLoadingCell;
@end

@implementation StartLoadingTableViewController

- (void)viewDidLoad
{
    waitAlertViewActive = NO;
    
    self.hourPicker.delegate = self;
    self.hourPicker.dataSource = self;
    
    self.startLoadingCell.detailTextLabel.text = @"";
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
                
                NSString *title = @"Info";
                NSString *message = @"Warte bis die Ladung gestartet wird...";
                
                waitAlertView = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:nil otherButtonTitles:nil, nil];
                [waitAlertView show];
                waitAlertViewActive = YES;
                return;
            }
        }
    }
}

- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    if([key compare:@"state"] == 0)
    {
        NSInteger state = [val integerValue];
        if(state == 2)
        {
            self.startLoadingCell.textLabel.textColor = [UIColor greenColor];
            self.startLoadingCell.userInteractionEnabled = YES;
        }
        else
        {
            self.startLoadingCell.textLabel.textColor = [UIColor redColor];
            self.startLoadingCell.userInteractionEnabled = NO;
        }
        
        switch (state)
        {
            case 0:
                self.startLoadingCell.detailTextLabel.text = @"Start nicht möglich (Fehler!)";
                break;
            case 1:
                self.startLoadingCell.detailTextLabel.text = @"Start nicht möglich (Kein Fahrzeug angeschlossen)";
                break;
            case 2:
                self.startLoadingCell.detailTextLabel.text = @"";
                break;
            case 3:
            case 4:
                self.startLoadingCell.detailTextLabel.text = @"Start nicht möglich (Ladung aktiv!)";
                break;
            case 5:
                self.startLoadingCell.detailTextLabel.text = @"Start nicht möglich (Kurzschluss / Spannungsausfall!)";
                break;
            default:
                self.startLoadingCell.detailTextLabel.text = @"Start nicht möglich (Tankstelle nicht vefügbar!)";
                break;
        }
        
        [self.tableView reloadData];
    }
    
    if([key compare:@"state"] == 0 && waitAlertViewActive)
    {
        if([val integerValue] == 2) // State B
            return;
        
        NSString *title = @"Info";
        NSString *message = @"";
        
        if([val integerValue] < 2)
            message = @"Fehler beim Starten der Ladung! Fahrzeug wurde abgeschlossen!";
        if([val integerValue] > 2)
            message = @"Ladung wurde erfolgreich gestartet!";
        if([val integerValue] > 4)
            message = @"Fehler beim Starten der Ladung! Kurzschluss oder nicht bereit!";
    
        [waitAlertView dismissWithClickedButtonIndex:0 animated:YES];
        
        waitAlertViewActive = NO;

        UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [view show];
        
    }
}

@end
