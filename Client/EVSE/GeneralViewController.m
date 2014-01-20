#import "GeneralViewController.h"

@interface GeneralViewController ()
- (IBAction)onNetwork:(UIBarButtonItem *)sender;
@property (weak, nonatomic) IBOutlet UIBarButtonItem *NetworkButton;
@end

@implementation GeneralViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
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

- (IBAction)onNetwork:(UIBarButtonItem *)sender
{
    
}

- (void)showNetworkForm
{
    if(self.view.window)
       [self.NetworkButton.target performSelector:self.NetworkButton.action];
}

@end
