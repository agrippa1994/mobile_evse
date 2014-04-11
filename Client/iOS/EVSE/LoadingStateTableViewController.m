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
@property (weak, nonatomic) IBOutlet UISlider *changeCurrentSlider;
@property (weak, nonatomic) IBOutlet UITableViewCell *changeCurrentCell;

- (IBAction)currentSliderValueChanged:(id)sender;
@end

@implementation LoadingStateTableViewController


- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    if([key compare:@"currentLoadingCurrent" options:NSCaseInsensitiveSearch] == 0)
    {
        self.loadingCurrentCell.detailTextLabel.text = [NSString stringWithFormat:@"%ld Ampere", (long)[val integerValue]];
    }
    
    if([key compare:@"chargingTime" options:NSCaseInsensitiveSearch] == 0)
    {
        NSUInteger numberOfSeconds = [val integerValue] / 1000;
        NSUInteger seconds = numberOfSeconds % 60;
        NSUInteger minutes = (numberOfSeconds / 60) % 60;
        NSUInteger hours = numberOfSeconds / 3600;
        
        if(hours)
            self.currentLoadingTimeCell.detailTextLabel.text = [NSString stringWithFormat:@"%luh:%02lum", (unsigned long)hours, (unsigned long)minutes];
        else if(minutes)
            self.currentLoadingTimeCell.detailTextLabel.text  = [NSString stringWithFormat:@"%lum:%02lus", (unsigned long)minutes, (unsigned long)seconds];
        else
            self.currentLoadingTimeCell.detailTextLabel.text = [NSString stringWithFormat:@"%lus", (unsigned long)seconds];
    }
    
    if([key compare:@"state" options:NSCaseInsensitiveSearch] == 0)
    {
        NSArray *strings = [NSArray arrayWithObjects:@"Keiner",@"Kein Fahrzeug angeschlossen", @"Fahrzeug angeschlossen",
                            @"Ladung aktiv / ohne Lüfter", @"Ladung aktiv / mit Lüfter", @"Kurzschluss oder Fehler",
                            @"Tankstelle nicht verfügbar", nil];
        
        NSInteger state = [val integerValue];
        self.statusCell.detailTextLabel.text = [strings objectAtIndex:state];
    }
    
    if([key compare:@"isLoading" options:NSCaseInsensitiveSearch] == 0)
    {
        NSInteger state = [val integerValue];
        if(state)
        {
            self.changeCurrentCell.detailTextLabel.text = @"";
            self.changeCurrentCell.textLabel.textColor = [UIColor greenColor];
            self.changeCurrentCell.selectionStyle = UITableViewCellSelectionStyleDefault;
            
        }
        else
        {
            self.changeCurrentCell.detailTextLabel.text = @"Kein Ladung aktiv!";
            self.changeCurrentCell.textLabel.textColor = [UIColor redColor];
            self.changeCurrentCell.selectionStyle = UITableViewCellSelectionStyleNone;
        }

    }
}

- (IBAction)currentSliderValueChanged:(id)sender
{
    UISlider *slider = (UISlider *)sender;
    self.changeCurrentCell.textLabel.text = [NSString stringWithFormat:@"Ladestrom auf %ldA ändern", ((long)(NSInteger)[slider value])];
}

@end
