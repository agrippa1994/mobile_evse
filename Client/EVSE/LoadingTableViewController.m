#import "LoadingTableViewController.h"

@interface stHourInfo : NSObject
{
    enum eAmpere{ one_phase_16A, three_phase_63A };

@public
    NSString *label;
    enum eAmpere ampere;
}

+ (id) init:(NSString *)text current:(enum eAmpere)amps;
@end

@implementation stHourInfo

+ (id) init:(NSString *)text current:(enum eAmpere)amps
{
    stHourInfo *obj = [[stHourInfo alloc] init];
    obj->label = text;
    obj->ampere = amps;
    
    return obj;
}

@end


@interface LoadingTableViewController ()
{
@private
    NSArray *hours;
}

@property (weak, nonatomic) IBOutlet UIPickerView *hourPicker;
@property (weak, nonatomic) IBOutlet UITableViewCell *startLoadingCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *loadingStateCell;
@property (weak, nonatomic) IBOutlet UITableViewCell *stopLoadingCell;
@end


@implementation LoadingTableViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // 1 phase 16A 6h - 8h
    // 3 phase 63A < 1h
    hours = [NSArray arrayWithObjects:[stHourInfo init:@"weniger als eine Stunde" current:three_phase_63A], [stHourInfo init:@"6 - 8 Stunden" current:one_phase_16A], nil];
    
    self.clearsSelectionOnViewWillAppear = YES;
    
    self.hourPicker.delegate = self;
    self.hourPicker.dataSource = self;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return hours.count;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return ((stHourInfo *)[hours objectAtIndex:row])->label;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    
    if(cell == self.startLoadingCell)
    {
        NSInteger selected = [self.hourPicker selectedRowInComponent:0];
        stHourInfo *info = [hours objectAtIndex:selected];
        
        switch(info->ampere)
        {
            case one_phase_16A:
                break;
            case three_phase_63A:
                break;
        }
    }
    
    else if(cell == self.stopLoadingCell)
    {
        
    }
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
}

- (void)client:(Client *)p loadingReply:(BOOL)wasLoadingStarted
{
    
}

@end
