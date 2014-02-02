#import "DebugViewController.h"

@interface DebugViewController ()
@property (weak, nonatomic) IBOutlet UIWebView *webView;
@property (weak, nonatomic) IBOutlet UIButton *sendButton;
@property (weak, nonatomic) IBOutlet UITextField *textField;

- (IBAction)onSendButton:(id)sender;
@end

@implementation DebugViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.textField.delegate = self;
}

- (IBAction)onSendButton:(id)sender
{
    
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    CGPoint scrollPoint = CGPointMake(0, textField.frame.origin.y);
    
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    
}

@end
