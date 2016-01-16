//
//  PreferenceController.h
//  ns2osx
//
//  Created by Michael Pigg on 6/3/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

extern NSString *NS2ShowSearchWindowOnStartKey;

@interface PreferenceController : NSWindowController {
	IBOutlet NSButton *searchWindowOnStartup;
}

- (BOOL)showSearchWindowOnStartup;
- (IBAction)changeNewSearchWindowOnStartup:(id)sender;

@end
