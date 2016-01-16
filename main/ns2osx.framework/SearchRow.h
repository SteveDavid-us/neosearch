//
//  SearchRow.h
//  ns2osx
//
//  Created by Michael Pigg on 7/18/08.
//
// Manages a view that allows a search term to be entered and modified

#import <Cocoa/Cocoa.h>
#import "SearchViewController.h"
#import "SearchTerm.h"
#import "UISearchTerm.h"
#import "NS2Constants.h"

#pragma mark Internal API
@protocol SearchRowPrivate
	// Hides or unhides the type-specific container
	- (void) setRowContainerVisible:(BOOL) isVisible;
		
	// Resize view and send notification of change
	- (void) resetViewSize:(NSSize)newsize;
	
	// The search row that this class is managing
	- (NSView *) view;

	// setter for supressing change notifications
	- (void)setSupressChangeMessages:(BOOL)supress;

	// accessors for search controls
	- (NSButton *) addButton;
	- (NSButton *) removeButton;
	- (NSTextField *) textView;
	- (NSPopUpButton *)grammarPopup;
@end

@interface SearchRow : NSObject <SearchRowPrivate> {
	IBOutlet NSView *rowPrototype;
	IBOutlet NSView *rowContainerView;
	IBOutlet NSView *substantiveGrammarPrototype;
	IBOutlet NSView *verbGrammarPrototype;
	IBOutlet NSObjectController *uisearchTermController;
	IBOutlet NSButton *addButton;
	IBOutlet NSButton *removeButton;
	IBOutlet NSTextField *searchText;
	IBOutlet NSPopUpButton *grammarTypePopup;
	
	NSView *_view;
	NSView *_curView;
	
	NSString *currentSearchSelection;
	
	BOOL supressChangeMessages;
}

// Handles change in search type
- (IBAction) onTypeChange:(id)sender;

- (UISearchTerm *) searchTerm;
- (void)setSearchTerm:(UISearchTerm *)searchTerm;

@end
