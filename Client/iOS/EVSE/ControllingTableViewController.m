#import "ControllingTableViewController.h"

@interface ControllingTableViewController ()
-(void)didBecomeActive;
@end

@implementation ControllingTableViewController

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

- (void)client:(Client *)p onKeyAndValue:(NSString *)key value:(NSString *)val
{
    [self.tableView reloadData];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:nil];
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    NSArray *allKeys = [[[[Client sharedClient] keyAndValues] keyEnumerator] allObjects];
    cell.textLabel.text = [allKeys objectAtIndex:indexPath.row];
    cell.detailTextLabel.text = [[[Client sharedClient] keyAndValues] objectForKey:[allKeys objectAtIndex:indexPath.row]];
    
    return cell;
}


- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [[[Client sharedClient] keyAndValues] count];
}


@end
