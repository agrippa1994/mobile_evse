//
//  ViewController.h
//  EVSE
//
//  Created by Manuel on 05.03.14.
//  Copyright (c) 2014 Manuel. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "Client.h"

@interface ViewController : UITableViewController<UIPickerViewDelegate, UIPickerViewDataSource, ClientDelegate>

@end
