//
//  AppController.mm
//  ns2osx
//
//  Created by Joshua Schnarr on 10/30/07.
//  Copyright 2007 __MyCompanyName__. All rights reserved.
//

#import "AppController.h"
#import "SearchWindow.h"

@implementation AppController

- (SearchWindow*) createSearchWindow
{
    SearchWindow *sw = [[SearchWindow alloc] initWithApp:self];
    return sw;
}

- (IBAction)showPreferencesPanel:(id)sender
{
	//preferenceController = [[PreferenceController alloc] init];
	if (!preferenceController)
	{
		preferenceController = [[PreferenceController alloc] init];
	}
	[preferenceController showWindow:self];
}

- (IBAction)showSearchWindow:(id)sender
{
	SearchWindow *searchWindow = [self createSearchWindow];
	if (!searchWindow) {
		NSLog(@"Failed to create search window");
	} else {
		[searchWindow showWindow:self];
	}
}

- (void)awakeFromNib
{
	if ([[NSUserDefaults standardUserDefaults] boolForKey:NS2ShowSearchWindowOnStartKey ])
	{
		[self showSearchWindow:nil];
	}
	
}

- (void)initialize
{
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSMutableDictionary *defDictionary = [[NSMutableDictionary alloc] init];
	[defDictionary setValue:[NSNumber numberWithBool:NO] forKey:NS2ShowSearchWindowOnStartKey];
	[defaults registerDefaults:defDictionary];
}

//- (void)applicationDidBecomeActive:(NSNotification *)aNotification
//{
//	NSLog(@"App did become active");
//	NSArray *windows = [[NSApplication sharedApplication] windows];
//	NSLog(@"window = %d", [windows count]);
//	if ([[NSApplication sharedApplication] mainWindow] == nil) 
//	{
//		[self showSearchWindow:nil];
//	}
//}

- (void)dealloc
{
    [super dealloc];
}

- (id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

@end
