//
//  PreferenceController.m
//  ns2osx
//
//  Created by Michael Pigg on 6/3/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "PreferenceController.h"

NSString *NS2ShowSearchWindowOnStartKey = @"ShowSearchWindowOnStart";

@implementation PreferenceController
- (IBAction)changeNewSearchWindowOnStartup:(id)sender
{
	[[NSUserDefaults standardUserDefaults] setBool:[sender state] forKey:NS2ShowSearchWindowOnStartKey];
}

- (void) windowDidLoad
{
	[searchWindowOnStartup setState:[self showSearchWindowOnStartup]];
}

- (BOOL)showSearchWindowOnStartup
{
	NSUserDefaults *defaults;
	defaults = [NSUserDefaults standardUserDefaults];
	return [defaults boolForKey:NS2ShowSearchWindowOnStartKey];
}

- (id) init {
	if (![super initWithWindowNibName:@"PreferencesPanel"]) {
		return nil;
	}
	return self;
}

- (void) dealloc {
	[super dealloc];
}

@end
