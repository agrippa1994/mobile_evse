#import "EVSETableViewBase.h"

@interface EVSETableViewBase ()
    -(void)didBecomeActive;
@end

@implementation EVSETableViewBase

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

@end
