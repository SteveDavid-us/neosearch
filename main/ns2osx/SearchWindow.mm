#import "SearchWindow.h"

static NSString *SearchFieldToolbarItemIdentifier = @"Search Field Toobar Item Identifier";
static NSString *ArrowControlToolbarItemIdentifier = @"Arrow Control Toobar Item Identifier";
static NSString *DrawerControlToolbarItemIdentifier = @"Drawer Control Toobar Item Identifier";
static NSString *NewWindowToolbarItemIdentifier = @"New Window Toolbar Item Identifier";
static NSString *NewSearchToolbarItemIdentiifer = @"New Search Toobar Item Identifier";
static NSString *RunSearchToolBarItemIdentifier = @"Run Search Toolbar Item Identifier";

@implementation SearchWindow

- (id)initWithApp:(AppController *)app
{
    self = [super initWithWindowNibName:@"SearchWindow"];
    if(self) {
        mainApp = app;
    }
    return self;
}

- (id) init {
	self = [super init];
	if (self != nil) {
		
	}
	return self;
}

- (void) dealloc {
	NSLog(@"SearchWindow dealloc");
	[[[self window] toolbar] dealloc];
	[super dealloc];
}

- (void)windowDidLoad
{
	NSToolbar *toolbar = [[NSToolbar alloc] initWithIdentifier:NS2SearchWindowToolbar];
	[toolbar setDelegate:self];
	[toolbar setAllowsUserCustomization:YES];
	[toolbar setDisplayMode:NSToolbarDisplayModeIconOnly];
	[[self window] setToolbar:toolbar];
	[[self window] makeFirstResponder:searchField];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(searchTermsUpdated) name:NS2SearchTermsDidChangeEvent object:nil];
	[termsView setDelegate:self];
	[termsView setFrameSize:NSMakeSize([termsView frame].size.width,0)];
	[self newSearch:nil];
	[self adjustVerticalAlignment];
}
    
- (void) awakeFromNib
{
	[statusText setStringValue:@""];
}

- (void) initialize
{

}

- (void)addSearchTerm
{
	[termsView addRow:self];
}

- (void)termsViewDidResize
{
	[self adjustVerticalAlignment];
}

- (void)adjustVerticalAlignment
{
	float termsHeight;
	termsHeight = [termsView frame].size.height;
	NSRect splitRect = [splitView frame];
	NSLog(@"split rect height %f", splitRect.size.height);
	NSRect contentRect = [[[self window] contentView] frame];
	NSLog(@"content rect height %f", contentRect.size.height);
	splitRect.size.height = contentRect.size.height - termsHeight - splitRect.origin.y;
	NSLog(@"setting split rect height %f", splitRect.size.height);
	[splitView setFrameSize:splitRect.size];
	//[splitView setBoundsSize:splitRect.size];
	NSPoint termsOrigin = [splitView frame].origin;
	termsOrigin.y += splitRect.size.height;
	NSLog(@"Terms view size %f, %f", [termsView frame].size.height, [termsView frame].size.width);
	[termsView setFrameOrigin:termsOrigin];
	[termsView setNeedsDisplay:YES];
	//[termsView setBoundsOrigin:NSMakePoint(0,0)];
}

- (NSMutableArray *) createSearch
{
	NSArray *strings = [[searchField stringValue] componentsSeparatedByString:@" "];
	NSEnumerator *strenum = [strings objectEnumerator];
	NSString *string = nil;
	NSMutableArray *terms = [[NSMutableArray alloc] init];
	while ( (string = [strenum nextObject]) != nil) {
		UISearchTerm *st = [[UISearchTerm alloc] init];
		[st setTermString:string];
		[terms addObject:st];
	}
   
    return [terms autorelease];
}

- (IBAction) toolbarSearchChange:(id)sender
{
	if ([[searchField stringValue] length] > 0) {
		[termsView setSearchTerms:[self createSearch]];
		[self search:nil];
	} else {
	}
}

- (IBAction) search:(id)sender
{
    NSLog(@"search");
	[searchRunningIndicator startAnimation:nil];
	[runSearchButton setEnabled:false];
	SearchEngine *engine = [[SearchEngine alloc] init];
    SearchResult *r = [engine runSearch:[termsView search]];
    [r retain];
	ResultBrowserDelegate *rd = [[ResultBrowserDelegate alloc] initWithResult:r];
    [resultBrowser setDelegate:rd];
	[resultBrowser setAction:@selector(browserSingleClick:)];
	[resultBrowser setTarget:rd];
    [resultBrowser setHasHorizontalScroller:false]; 
	[resultBrowser setColumnResizingType:NSBrowserNoColumnResizing];
    // The ResultBrowserDelegate also handles resizes of the split view.
    [splitView setDelegate:rd];
	[rd setTextView:textView];
	[rd setBrowser:resultBrowser];
	[statusText setStringValue:[NSString stringWithFormat:@"%d hits found in %d volumes", [r totalHits], [[r hitVolumes] count]]];
    [resultBrowser loadColumnZero];
	[searchRunningIndicator stopAnimation:nil];
	[engine release];
}

- (void)searchTermsUpdated {
	[searchField setStringValue:@""];
	[self search:nil];
}

- (IBAction)newSearch:(id)sender
{
	[self clearSearch];
	[self addSearchTerm];	
}

- (void)clearSearch
{
	NSString *emptyString = @"";
	[statusText setStringValue:emptyString];
	[resultBrowser setDelegate:nil];
	[resultBrowser reloadColumn:0];
	NSRange allTextRange = NSMakeRange(0,[[textView textStorage] length]);
	[textView replaceCharactersInRange:allTextRange withString:emptyString]; 
	if ([termsView count] > 0)
	{
		[termsView removeAllRows];
	}
}

#pragma mark boring accessor methods
- (Search *)currentSearch {
    return [[currentSearch retain] autorelease];
}

- (void)setCurrentSearch:(Search *)value {
    if (currentSearch != value) {
        [value retain];
        [currentSearch release];
		currentSearch = value;
    }
}


#pragma mark toolbar delegate methods
- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar
{
	return [NSArray arrayWithObjects:NSToolbarShowFontsItemIdentifier,
	NSToolbarSeparatorItemIdentifier,
	NSToolbarFlexibleSpaceItemIdentifier,
	NSToolbarCustomizeToolbarItemIdentifier,
	NSToolbarSpaceItemIdentifier,
	NewWindowToolbarItemIdentifier,
	SearchFieldToolbarItemIdentifier,
	ArrowControlToolbarItemIdentifier,
	DrawerControlToolbarItemIdentifier,
	NewSearchToolbarItemIdentiifer,
    RunSearchToolBarItemIdentifier,
	nil];
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar
{
    return nil;
	//return [NSArray arrayWithObjects:RunSearchToolBarItemIdentifier, NewSearchToolbarItemIdentiifer, ArrowControlToolbarItemIdentifier, NSToolbarSpaceItemIdentifier, SearchFieldToolbarItemIdentifier, nil];
}

- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag
{
	NSToolbarItem *toolbarItem = nil;
	if ([itemIdentifier isEqual:NSToolbarShowFontsItemIdentifier]) {
		toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier:NSToolbarShowFontsItemIdentifier];		
	} else if ([itemIdentifier isEqual:NSToolbarSpaceItemIdentifier]) {
		toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
	
	} else if ([itemIdentifier isEqual:SearchFieldToolbarItemIdentifier]) {
		toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
		[toolbarItem setLabel:@"Search text"];
		[toolbarItem setPaletteLabel:@"Search text"];
		[toolbarItem setToolTip:@"Search text"];
		searchField = [[NSSearchField alloc] initWithFrame:[searchField frame]];
		[searchField setSearchMenuTemplate:searchMenu];
		[[searchField cell] setSendsWholeSearchString:YES];
		[searchField setAction:@selector(toolbarSearchChange:)];
		[toolbarItem setView:searchField];
	} else if ([itemIdentifier isEqual:ArrowControlToolbarItemIdentifier]) {
		toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
		[toolbarItem setLabel:@"Next/Previous"];
		[toolbarItem setPaletteLabel:@"Next/Previous"];
		[toolbarItem setToolTip:@"Next/Previous"];
		[toolbarItem setView:arrowsView];	
	} else if([itemIdentifier isEqual:NewSearchToolbarItemIdentiifer]) {
		toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
		[toolbarItem setLabel:@"New Query"];
		[toolbarItem setPaletteLabel:@"New Query"];
		[toolbarItem setToolTip:@"Delete existing query and start new query in current window"];
		NSButton *newQueryButton = [[NSButton alloc] initWithFrame:NSMakeRect(0,0,32,32)];
		[newQueryButton setTitle:@"New"];
		[newQueryButton setButtonType:NSMomentaryLightButton];
		[newQueryButton setAction:@selector(newSearch:)];
		[newQueryButton setTarget:self];
		[toolbarItem setView:newQueryButton];
		
	} else if([itemIdentifier isEqual:NewWindowToolbarItemIdentifier]) {
		toolbarItem = [[NSToolbarItem alloc] initWithItemIdentifier:itemIdentifier];
		[toolbarItem setLabel:@"New Window"];
		[toolbarItem setPaletteLabel:@"New Window"];
		[toolbarItem setToolTip:@"Open new search window"];
		NSButton *newQueryButton = [[NSButton alloc] initWithFrame:NSMakeRect(0,0,32,32)];
		[newQueryButton setTitle:@"New Window"];
		[newQueryButton setButtonType:NSMomentaryLightButton];
		[newQueryButton setAction:@selector(showSearchWindow:)];
		[newQueryButton setTarget:mainApp];
		[toolbarItem setView:newQueryButton];
	}
	return toolbarItem;
}

- (IBAction)nextAction:(id)sender
{
	NSLog(@"nextAction");
//	int col = [resultBrowser selectedColumn];
//	int row = [resultBrowser selectedRowInColumn:col];
//	row++;
	
	//[resultBrowser tryToPerform:@selector(moveDown:) with:self];
	//[resultBrowser moveRight:self];
	[resultBrowser tryToPerform:@selector(moveRight:) with:self];
}

- (IBAction)previousAction:(id)sender;
{
	NSLog(@"previousAction");
	[resultBrowser moveLeft:self];
}

@end
