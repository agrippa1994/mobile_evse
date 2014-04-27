#import "LoadingStateTableViewController.h"

@interface LoadingStateTableViewController ()
@property (weak, nonatomic) IBOutlet UITableViewCell *loadingCurrentCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *currentLoadingTimeCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *statusCell;
@property (weak, nonatomic) IBOutlet UISlider *changeCurrentSlider;
@property (weak, nonatomic) IBOutlet UITableViewCell *changeCurrentCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *temperatureCell;

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
        NSMutableArray *strings = [NSMutableArray arrayWithObjects:@"Kein Fahrzeug angeschlossen.", @"Fahrzeug angeschlossen.",
                            @"Fahrzeug angeschlossen.", @"Fahrzeug angeschlossen.", @"Kurzschluss oder Fehler.",
                            @"Tankstelle nicht verfügbar", nil];
        
        BOOL isLoading = [((NSString *)[[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"]) compare:@"1"] == 0;
        NSInteger state = [val integerValue];
        
        if(isLoading)
        {
            if(state == 3) // State C
                [strings replaceObjectAtIndex:state withObject:@"Ladung aktiv / ohne Lüfter."];
            if(state == 4) // State D
                [strings replaceObjectAtIndex:state withObject:@"Ladung aktiv / mit Lüfter."];
        }
        
        self.statusCell.detailTextLabel.text = [strings objectAtIndex:state];
    }
    
    if([key compare:@"isLoading" options:NSCaseInsensitiveSearch] == 0)
    {
        NSInteger state = [val integerValue];
        if(state)
        {
            self.changeCurrentCell.detailTextLabel.text = @"";
            self.changeCurrentCell.textLabel.textColor = [UIColor blackColor];
            self.changeCurrentCell.userInteractionEnabled = YES;
            self.changeCurrentSlider.userInteractionEnabled = YES;
            
        }
        else
        {
            self.changeCurrentCell.detailTextLabel.text = @"Keine Ladung aktiv!";
            self.changeCurrentCell.textLabel.textColor = [UIColor grayColor];
            self.changeCurrentCell.detailTextLabel.textColor = [UIColor redColor];
            self.changeCurrentCell.userInteractionEnabled = NO;
            self.changeCurrentSlider.userInteractionEnabled = NO;
            self.changeCurrentSlider.value = 0.0f;
        }
    }
    
    if([key compare:@"temperature" options:NSCaseInsensitiveSearch] == 0)
    {
        self.temperatureCell.detailTextLabel.text = [NSString stringWithFormat:@"%.0f°C", [val floatValue]];
    }
}

- (IBAction)currentSliderValueChanged:(id)sender
{
    UISlider *slider = (UISlider *)sender;
    
    self.changeCurrentCell.textLabel.adjustsFontSizeToFitWidth = YES;
    self.changeCurrentCell.textLabel.text = [NSString stringWithFormat:@"Ladestrom auf %ldA ändern", (long)slider.value];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if(cell == self.changeCurrentCell)
    {
        [[Client sharedClient] send:[NSString stringWithFormat:@"changecurrent --current %ld", (long)self.changeCurrentSlider.value]];
    }
}

@end
