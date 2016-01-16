/* SearchWindow */

#import <Cocoa/Cocoa.h>

#import "AppController.h"
#import "ResultBrowserDelegate.h"
#import "Search.h"
#import "SearchResult.h"
#import "SearchRowContainer.h"
#import "SearchTerm.h"

@class AppController;

static NSString *NS2SearchWindowToolbar = @"NS2SearchWindowToolbar";

@protocol SearchWindowPrivate
- (void)adjustVerticalAlignment;
- (void)clearSearch;

- (NSMutableArray*) createSearch;
@end

@interface SearchWindow : NSWindowController <SearchWindowPrivate>
{
    // application connection
    AppController *mainApp;
    
    // main window controls
    IBOutlet NSBrowser *resultBrowser;
    IBOutlet NSTextView *textView;
    IBOutlet NSSplitView *splitView;
	IBOutlet NSTextField *statusText;
    IBOutlet NSSearchField *searchField; // Template search field used for toolbar
	IBOutlet NSView *arrowsView;
	IBOutlet NSMenu *searchMenu;
	
    // search controls
	IBOutlet NSProgressIndicator *searchRunningIndicator;	
	IBOutlet SearchRowContainer *termsView;
	
	NSButton *runSearchButton;
	// curent search displayed in UI - nil if not search is currently displayed
	Search *currentSearch;
	
	
}

#pragma mark initializer
- (id)initWithApp:(AppController *)app;

#pragma mark api
- (IBAction) search:(id)sender;

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar;
- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar;
- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag;

- (IBAction)nextAction:(id)sender;
- (IBAction)previousAction:(id)sender;

// Called when toolbar search field changes value
- (IBAction) toolbarSearchChange:(id)sender;

// called to clear current search in current window
- (IBAction)newSearch:(id)sender;



@end
