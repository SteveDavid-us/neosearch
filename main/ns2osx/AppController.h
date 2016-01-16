//
//  AppController.h
//  ns2osx
//
//  Created by Joshua Schnarr on 10/30/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "PreferenceController.h"

@class SearchWindow;

@interface AppController : NSObject {
	PreferenceController *preferenceController;
@public
}
- (IBAction)showSearchWindow:(id)sender;
- (SearchWindow *)createSearchWindow;
- (IBAction)showPreferencesPanel:(id)sender;

@end
