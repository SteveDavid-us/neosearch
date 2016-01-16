//
//  ResultBrowserDelegate.h
//  ns2osx
//
//  Created by Michael Pigg on 6/5/08.
//

#import <Cocoa/Cocoa.h>
#import "SearchEngine.h"

@class SearchResult;

@protocol ResultBrowserDelegatePrivate

- (void)markupString:(NSTextStorage *)textStorage withHits:(NSDictionary *)hitWords;

@end

@interface ResultBrowserDelegate : NSObject <ResultBrowserDelegatePrivate> {
	SearchResult *searchResult;
	NSTextView *textView;
	SearchEngine *engine;
    NSBrowser *resultBrowser;
}

#pragma mark Init methods
- (id) initWithResult:(SearchResult *)result;

- (void)setSearchResult:(SearchResult *)result;
- (void)setTextView:(NSTextView *)textView;
- (void)setBrowser:(NSBrowser *)browser;

- (void)resizeColumns;
#pragma mark NSBrowser delegate methods
- (IBAction)browserSingleClick:(id)browser;
- (int)browser:(NSBrowser *)sender numberOfRowsInColumn:(int)column;
- (void)browser:(NSBrowser *)sender willDisplayCell:(id)cell atRow:(int)row column:(int)column;
- (NSString *)browser:(NSBrowser *)sender titleOfColumn:(int)column;
- (BOOL)browser:(NSBrowser *)sender selectRow:(int)row inColumn:(int)column;
- (CGFloat)browser:(NSBrowser *)browser shouldSizeColumn:(NSInteger)columnIndex
    forUserResize:(BOOL)forUserResize toWidth:(CGFloat)suggestedWidth;
- (void)browserColumnConfigurationDidChange:(NSNotification *)notification;
#pragma mark NSSplitView delegate methods
- (void)splitViewDidResizeSubviews:(NSNotification *)notification;
@end
