//
//  ResultBrowserDelegate.m
//  ns2osx
//
//  Created by Michael Pigg on 6/5/08.
//

#import "ResultBrowserDelegate.h"

#define PASSAGES_WIDTH 60.0
#define BROWSER_PADDING 2.0

enum {
    VOLUMES_COL = 0,
    PASSAGES_COL,
    COLUMN_COUNT
};

@implementation ResultBrowserDelegate

#pragma mark NSBrowser delegate methods
- (void)registerClick:(NSBrowser *)browser
{
	NSLog(@"registerClick");
	[browser setAction:@selector(browserSingleClick:)];
}

- (int)browser:(NSBrowser *)sender numberOfRowsInColumn:(int)column
{
	//NSLog([NSString stringWithFormat:@"numberOfRowsInColumn %d", column]);
	if (column == VOLUMES_COL) 
	{
		return [[searchResult hitVolumes] count];
	} else if (column == PASSAGES_COL) {
		int volRow = [sender selectedRowInColumn:VOLUMES_COL];
		int vol = [[[searchResult hitVolumes] objectAtIndex:volRow] intValue];
		return [[searchResult hitPassagesForVolume:vol] count];
	}
	else
	{
		return 0;
	}
}

- (NSString *)browser:(NSBrowser *)sender titleOfColumn:(int)column
{
	if (column == VOLUMES_COL)
	{
		return @"Volumes";
	}
	else
	{
		return @"Passages";
	}
	
}

- (void)browserColumnConfigurationDidChange:(NSNotification *)notification
{
#if 0
    NSBrowser *b = [notification object];

    NSLog(@"%f, %f\n", [b widthOfColumn:0], 
        [b widthOfColumn:1]);
#endif
}

- (CGFloat)browser:(NSBrowser *)browser shouldSizeColumn:(NSInteger)columnIndex
    forUserResize:(BOOL)forUserResize toWidth:(CGFloat)suggestedWidth
{
    if (columnIndex == PASSAGES_COL) {
        return PASSAGES_WIDTH;
    } else if (columnIndex == VOLUMES_COL) {
        float browserWidth = [resultBrowser frame].size.width - BROWSER_PADDING;
        return (browserWidth - PASSAGES_WIDTH);
    } else {
        return suggestedWidth;
    }
}

- (void)browser:(NSBrowser *)sender willDisplayCell:(id)cell atRow:(int)row column:(int)column
{
	//NSLog([NSString stringWithFormat:@"willDisplayCell %d:%d", column, row]);
	if (column == VOLUMES_COL) {
		NSString *volName = [[searchResult hitVolumeNames] objectAtIndex:row];
		[cell setTitle:volName];
		
	} else if (column == PASSAGES_COL) {
		int volRow = [sender selectedRowInColumn:VOLUMES_COL];
		int vol = [[[searchResult hitVolumes] objectAtIndex:volRow] intValue];
		NSMutableArray *hitPassages = [searchResult hitPassagesForVolume:vol];
		
		NSString *psg = [[hitPassages objectAtIndex:row] stringValue];
		[cell setTitle:psg];
		[cell setLeaf:YES];
	}
}

- (BOOL)browser:(NSBrowser *)sender selectRow:(int)row inColumn:(int)column
{
	NSLog([NSString stringWithFormat:@"selectRowinColumn %d:%d"]);
	return YES;
}

- (IBAction)browserSingleClick:(id)browser
{
	NSLog(@"browserSingleClick");
	NSBrowserCell *cell = [browser selectedCellInColumn:PASSAGES_COL];
	if (cell != nil) {
		int volRow = [browser selectedRowInColumn:VOLUMES_COL];
		int vol = [[[searchResult hitVolumes] objectAtIndex:volRow] intValue];
		int psg = [[cell title] intValue];
		NSMutableArray *words = [searchResult hitsForVolume:vol andPassage:psg];
		NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:[words count]];
		NSEnumerator *wordEnum = [words objectEnumerator];
		while (NSNumber *word = [wordEnum nextObject])
		{
			[dict setObject:[NSNull null] forKey:word];
		}
		NSString *txt = [engine textForVolume:vol andPassage:psg withHits:dict];
		//NSAttributedString *passageText = [[NSAttributedString alloc] initWithString:txt];
		NSRange textRange; 
		textRange.location = 0;
		textRange.length = [[textView textStorage] length];
		[textView replaceCharactersInRange:textRange withString:txt];
		[self markupString:[textView textStorage] withHits:dict];

	}		
}

- (void)markupString:(NSTextStorage *)textStorage withHits:(NSDictionary *)hitWords
{
	if ([hitWords count] < 1) {
		return;
	}
	NSEnumerator *hitWordEnum = [hitWords objectEnumerator];
	while (HitRange *hitRange = [hitWordEnum nextObject])
	{
		unsigned start = [[hitRange start] unsignedIntValue];
		unsigned end = [[hitRange end] unsignedIntValue];
		unsigned length = end - start;
		NSRange wordRange = NSMakeRange(
			start,
			length
		);
		
		NSDictionary *attrDict = [NSDictionary dictionaryWithObject:[NSColor yellowColor] forKey:NSBackgroundColorAttributeName];
		
		// add attribute to string
		[textStorage setAttributes:attrDict range:wordRange];

	}		
	
	
}

- (void)moveDown:(id)sender
{
	NSLog(@"Move down");
}


- (BOOL) _browser: (NSBrowser *) sender
	    keyEvent: (NSEvent *) event inColumn: (int) column
{
	NSLog(@"Browser delegate key event");
	NSLog( [event charactersIgnoringModifiers]);
	return NO;
}
		
#pragma mark NSSplitView delegate methods
- (void)splitViewDidResizeSubviews:(NSNotification *)notification
{
    [self resizeColumns];
}

#pragma mark instance methods
- (void)setTextView:(NSTextView *)view
{
	[textView release];
	[view retain];
	textView = view;
}

- (void)setBrowser:(NSBrowser *)browser
{
    [resultBrowser release];
    [browser retain];
    resultBrowser = browser;
}

- (void)setSearchResult:(SearchResult *)result
{
	[searchResult release];
	[result retain];
	searchResult = result;
}

- (void)setSearchEngine:(SearchEngine *)searchEngine
{
	[engine release];
	[searchEngine retain];
	engine = searchEngine;
}

- (id) initWithResult:(SearchResult *)result
{
	self = [super init];
	if (self != nil) {
		[self setSearchResult:result];
		[self setSearchEngine:[[SearchEngine alloc] init]];
	}
	NSLog(@"initWithResult");
	return self;
}

- (id) init {
	self = [super init];
	if (self != nil) {
	}
	return self;
}

- (void) dealloc 
{
	[self setTextView:nil];
	[self setSearchResult:nil];
	[self setSearchEngine:nil];
	[super dealloc];
}

- (void) resizeColumns;
{
    for (int i = 0; i < COLUMN_COUNT; ++i) {
        float width = [self browser:Nil shouldSizeColumn:i forUserResize:NO toWidth:0.0];
        [resultBrowser setWidth:width ofColumn:i];
    }
}

@end
