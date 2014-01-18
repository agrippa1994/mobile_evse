#import "NetworkViewController.h"
#import <SystemConfiguration/CaptiveNetwork.h>

@interface NetworkViewController ()
@property (weak, nonatomic) IBOutlet UITableViewCell *wlanConnect;
@property (weak, nonatomic) IBOutlet UITableViewCell *ipConnect;
@property (weak, nonatomic) IBOutlet UITableViewCell *ipSettings;
@property (weak, nonatomic) IBOutlet UITextField *ipText;

- (BOOL) isWlanConnectedToEVSE;
@end

@implementation NetworkViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.ipText.delegate = self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    [super prepareForSegue:segue sender:sender];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    if(cell == self.wlanConnect)
    {
        if(![self isWlanConnectedToEVSE])
        {
            UIAlertView *view = [[UIAlertView alloc] initWithTitle:@"Fehler" message:@"Sie sind nicht mit dem Tankstellennetzwerk verbunden!\nBitte überprüfen Sie die Verbindung zum Netzwerk!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            
            [view show];
        }
    }
    else if(cell == self.ipConnect)
    {
        NSString *ip_addr = self.ipText.text;
        if([ip_addr length] == 0)
        {
            UIAlertView *view = [[UIAlertView alloc] initWithTitle:@"Fehler" message:@"IP-Adresse ist ungültig!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            
            [view show];
        }
        else
        {
            [self performSegueWithIdentifier:@"showApp" sender:self];
        }
    }
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self.ipText resignFirstResponder];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
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
            return [[info objectForKey:@"SSID"] compare:@"evse" options:NSCaseInsensitiveSearch] == 0;
    }
    
    return NO;
}

@end
