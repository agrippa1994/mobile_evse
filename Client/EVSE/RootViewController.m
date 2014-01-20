#import "RootViewController.h"
#import "GeneralViewController.h"

@interface RootViewController ()
- (void)onTimer:(NSTimer *)timer;
@end

@implementation RootViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    for(UIViewController *ctrl in self.viewControllers)
    {
        if([ctrl isKindOfClass:[GeneralViewController class]])
        {
            [NSTimer scheduledTimerWithTimeInterval:0 target:self selector:@selector(onTimer:) userInfo:nil repeats:NO];
        }
    }
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)onTimer:(NSTimer *)timer
{
    for(UIViewController *ctrl in self.viewControllers)
    {
        if([ctrl isKindOfClass:[GeneralViewController class]])
        {
            [(GeneralViewController *)ctrl showNetworkForm];
        }
    }
}

@end
