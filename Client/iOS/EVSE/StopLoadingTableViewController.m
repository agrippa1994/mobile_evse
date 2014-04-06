//
//  StopLoadingTableViewController.m
//  EVSE
//
//  Created by Manuel on 16.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import "StopLoadingTableViewController.h"

@interface StopLoadingTableViewController ()
{
    UIAlertView *waitAlertView;
    BOOL waitAlertViewActive;
}

@property (weak, nonatomic) IBOutlet UITableViewCell *stopLoadingCell;
@end

@implementation StopLoadingTableViewController

- (void)viewDidLoad
{
    waitAlertViewActive = NO;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    if(cell == self.stopLoadingCell)
    {
        NSString *state = [[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"];
        if(state != nil)
        {
            if([state compare:@"1"] == 0) // Ladevorgang
            {
                [[Client sharedClient] send:@"stoploading"];
                
                NSString *title = @"Info";
                NSString *message = @"Die Ladung wird gestoppt!\n"
                "Das kann mehrere Sekunden in Anspruch nehmen!\n"
                "Bitte trennen Sie nicht die Verbindung zu Ihrem Fahrzeug!";
                
                waitAlertView = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:nil otherButtonTitles:nil, nil];
                [waitAlertView show];
                waitAlertViewActive = YES;
                return;
            }
            else
            {
                NSString *title = @"Fehler";
                NSString *message = @"Es muss ein Ladevorgang vorhanden sein, um sie beenden zu können";
                
                UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
                [view show];
                return;
            }
        }
    }
}

- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    if([key compare:@"state"] == 0 && waitAlertViewActive)
    {
        NSInteger state = [val integerValue];
        
        if(state == 3 || state == 4) // State C / D
            return;
        
        BOOL isLoading = [((NSString *)[[[Client sharedClient] keyAndValues] objectForKey:@"isLoading"]) compare:@"1"] == 0;
        
        if(isLoading)
            return;
        
        NSString *title = @"Info";
        NSString *message = @"";
        
        switch(state)
        {
            case 1:
                message = @"Die Ladung wurde beendet (Fahrzeug direkt abgeschlossen!)";
                break;
            case 2:
                message = @"Die Ladung wurde erfolgreich beendet\n"
                "Das Fahrzeug kann jetzt abgeschlossen werden!";
                break;
            default:
                message = @"Schwerer Fehler!";
                break;
        }
        
        [waitAlertView dismissWithClickedButtonIndex:0 animated:YES];
        
        waitAlertViewActive = NO;
        
        UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
        [view show];
        
    }
}
@end
