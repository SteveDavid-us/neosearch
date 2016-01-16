//
//  SearchRowContainer.h
//  ns2osx
//
//  Created by Michael Pigg on 7/19/08.
//
// Manages a view that contains one or more SearchRow views

#import <Cocoa/Cocoa.h>
#import "SearchTerm.h"
#import "SearchRow.h"
#import "Search.h"
#import "UISearchTerm.h"
#import "NS2Constants.h"


@protocol SearchRowContainterPrivate
// Internal API
- (void)adjustRows;
- (void)resize;
@end

@interface SearchRowContainer : NSView <SearchRowContainterPrivate> {
	NSMutableArray* searchRows;
	float rowHeight;
	id delegate;
}

// Populate from an existing Search object
- (void)setSearchTerms:(NSMutableArray *)searchTerms;

// Return Search object populated from current rows
- (Search *)search;

// return number of search term rows in this view
- (int)count;

- (void)setDelegate:(id)adelegate;

- (IBAction)addRow:(id)sender;
- (IBAction)removeRow:(id)sender;

- (void)removeAllRows;
- (SearchRow *)addNewRow;

// method to be called when a child row is resized
- (void)searchRowResized:(NSNotification *)notification;
- (float)desiredHeight;
@end
