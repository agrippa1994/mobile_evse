#import "NetworkViewController.h"

@interface NetworkViewController ()
@property (weak, nonatomic) IBOutlet UITableViewCell *wlanConnect;
@property (weak, nonatomic) IBOutlet UITableViewCell *ipConnect;
@property (weak, nonatomic) IBOutlet UITableViewCell *ipSettings;
@property (weak, nonatomic) IBOutlet UITextField *ipText;

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
        
    }
    else if(cell == self.ipConnect)
    {
        NSString *ip_addr = self.ipText.text;
        if([ip_addr length] == 0)
        {
            UIAlertView *view = [[UIAlertView alloc] initWithTitle:@"Fehler" message:@"IP-Adresse ist ungültig!" delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
            
            [view show];
        }
    }
    
}

@end
