#import "NetworkTableViewController.h"
#import <SystemConfiguration/CaptiveNetwork.h>

@interface NetworkTableViewController ()

@property (weak, nonatomic) IBOutlet UITableViewCell *Connect;
@property (weak, nonatomic) IBOutlet UITableViewCell *ConnectIP;
@property (weak, nonatomic) IBOutlet UITextField *ipTextField;

- (BOOL) isWlanConnectedToEVSE;

@end

@implementation NetworkTableViewController

- (void)viewDidLoad
{
    self.ipTextField.delegate = self;
}

- (void)viewDidAppear:(BOOL)animated
{
    [[Client sharedClient] addDelegate:self];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [[Client sharedClient] rmDelegate:self];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    [self.tableView deselectRowAtIndexPath:indexPath animated:YES];

    if(cell == self.Connect)
    {
        if(![self isWlanConnectedToEVSE])
        {
            NSString *title = @"Fehler";
            NSString *message = @"Sie sind nicht mit dem Tankstellennetzwerk verbunden!"
            "Bitte überprüfen Sie die Verbindung zum Netzwerk";
            
            UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            
            [view show];
        }
        else
        {
            // Verbinden mit dem Server
            [[Client sharedClient] connect:@"10.0.10.1" withPort:2425];
        }
    }
    if(cell == self.ConnectIP)
    {
        NSString *ip = [self.ipTextField text];
        if(![ip length])
        {
            NSString *title = @"Fehler";
            NSString *message = @"Bitte geben Sie eine gültige IP-Adresse an!";
            
            UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            
            [view show];
        }
        else
        {
            // Verbinden mit dem Server
            [[Client sharedClient] connect:ip withPort:2425];
        }
    }
    
}

- (BOOL) isWlanConnectedToEVSE
{
    NSArray *ifs = (__bridge id)CNCopySupportedInterfaces();
    if([ifs count] == 0)
        return NO;
    
    id info = nil;
    for(NSString *ifname in ifs)
    {
        info = (__bridge_transfer id)CNCopyCurrentNetworkInfo((__bridge CFStringRef)ifname);
        if(info && [info count])
            return [[info objectForKey:@"SSID"] rangeOfString:@"evse_" options:NSCaseInsensitiveSearch].length > 0;
        
    }
    
    return NO;
}

- (void)client:(Client *)p openRequest:(BOOL)isOpen
{
    if(isOpen)
    {
        [self dismissViewControllerAnimated:YES completion:nil];
        return;
    }
    
    NSString *title = @"Fehler";
    NSString *message = @"Die Verbindung zum Server kann nicht hergestellt werden!\n"
    "Möglicherweise ist der Server abgestürzt oder aus einem anderen Grund nicht erreichbar!";
    
    UIAlertView *view = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
    
    [view show];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}
@end
