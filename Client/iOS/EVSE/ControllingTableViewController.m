#import "ControllingTableViewController.h"

@implementation ControllingTableViewController

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
