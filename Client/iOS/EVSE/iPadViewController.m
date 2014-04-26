#import "iPadViewController.h"

@interface iPadViewController ()

@end

@implementation iPadViewController


- (void)viewDidLoad
{
    self.delegate = self;
}

- (BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc inOrientation:(UIInterfaceOrientation)orientation
{
    return NO;
}

@end
