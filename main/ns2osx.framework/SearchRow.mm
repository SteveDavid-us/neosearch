//
//  SearchRow.m
//  ns2osx
//
//  Created by Michael Pigg on 7/18/08.
//

#import "SearchRow.h"

@implementation SearchRow

- (id) init {
	self = [super init];
	if (self != nil) {
		BOOL success = [NSBundle loadNibNamed:@"SearchControls" owner:self];
		if (!success) {
			NSLog(@"Could not find resource SearchControls.nib");
			[self release];
			return nil;
		}
		_view = rowPrototype;
		[[self grammarPopup] setBezelStyle:NSRoundRectBezelStyle];
		[[self textView] setBezelStyle:NSTextFieldRoundedBezel];
		[[self addButton] setBezelStyle:NSRoundRectBezelStyle];
		[[self removeButton] setBezelStyle:NSRoundRectBezelStyle];
		[self setRowContainerVisible:NO];
		[self setSupressChangeMessages:NO];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termCase" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termString" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termParticiple" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termNumber" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termGender" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termMood" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termVoice" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termTense" options:nil context:nil];
		[uisearchTermController addObserver:self forKeyPath:@"selection.termPerson" options:nil context:nil];
	}
	return self;
}

- (void) dealloc
{
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termCase"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termString"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termParticiple"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termNumber"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termGender"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termMood"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termVoice"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termTense"];
	[uisearchTermController removeObserver:self forKeyPath:@"selection.termPerson"];
	[super dealloc];
}

- (NSView *)view
{
	return _view;
}

- (void)setSupressChangeMessages:(BOOL)supress
{
	// This may need to be made thread-safe
	supressChangeMessages = supress;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
	NSLog(@"Observed change on key path %@", keyPath);
	if (supressChangeMessages != YES)
	{
		[[NSNotificationCenter defaultCenter] postNotificationName:NS2SearchTermsDidChangeEvent object:self];
	}
}

- (IBAction)onTypeChange:(id)sender
{
	// Remove existing grammar criterion, if any
	if (_curView)
	{
		[_curView removeFromSuperview];
	}
	
	currentSearchSelection = [[uisearchTermController selection] valueForKey:@"termVerbType"];
	NSView *newView = nil;
	if ([currentSearchSelection isEqualToString:@"Substantive"]) {
		newView = substantiveGrammarPrototype;
	} else if ([currentSearchSelection isEqualToString:@"Verb"]) {
		newView = verbGrammarPrototype;
	}
	
	if (newView) {
		[rowContainerView addSubview:newView];
		NSRect subViewRect = [newView frame];
		NSRect containerRect = [rowContainerView frame];
		containerRect.size = subViewRect.size;
		[rowContainerView setFrame:containerRect];
		
		// make popups use round rect style
		NSEnumerator *views = [[newView subviews] objectEnumerator];
		NSView *subview;
		while ((subview = [views nextObject])) {
			if ([subview isKindOfClass:[NSPopUpButton class]])
			{
				[subview setBezelStyle:NSRoundRectBezelStyle];
			}
		}
	}

	if ([[rowContainerView subviews] count] > 0 ) {
		[self setRowContainerVisible:YES];
	}
	else
	{
		[self setRowContainerVisible:NO];
		
	}
	_curView = newView;
	
	NSLog(@"selected item %@", currentSearchSelection);
}

- (void)setRowContainerVisible:(BOOL)isVisible
{
	NSRect rowViewFrame = [_view frame];
	NSSize initSize = rowViewFrame.size;
	// Set height to accomodate fixed portion of search row UI
	NSSize adjustedSize = NSMakeSize(initSize.width,28);

	// If the type-specific container is changing hidden state, then adjustements will be needed in the
	// height of the row frame.
	if (isVisible && [rowContainerView isHidden])
	{
		// show the rowContainerView
		[rowContainerView setHidden:NO];
	}
	else if (!isVisible && ![rowContainerView isHidden])
	{
		// hide the rowContainerView
		[rowContainerView setHidden:YES];
	}

	if (![rowContainerView isHidden]) {
		adjustedSize.height += [rowContainerView frame].size.height;
	}
	
	if (initSize.height != adjustedSize.height || initSize.width != adjustedSize.width)
	{
		[self resetViewSize:adjustedSize];
	}
}

- (void) resetViewSize:(NSSize)newsize
{
	NSRect viewFrame = [_view frame];
	viewFrame.size = newsize;
	[_view setFrame:viewFrame];
	[_view setNeedsDisplay:YES];
	[[NSNotificationCenter defaultCenter] postNotificationName:NS2SearchRowdidResizeEvent
	 object:self
	  userInfo:[NSDictionary dictionaryWithObject:_view forKey:@"view"]];
}

- (void)setSearchTerm:(UISearchTerm *)searchTerm
{
	[self setSupressChangeMessages:YES];
	[uisearchTermController setContent:searchTerm];
	[self setSupressChangeMessages:NO];
}

- (UISearchTerm *)searchTerm
{
	return [uisearchTermController content];
}

- (NSButton *)addButton
{
	return addButton;
}

- (NSButton *)removeButton
{
	return removeButton;
}

- (NSTextField *)textView
{
	return searchText;
}

- (NSPopUpButton *)grammarPopup
{
	return grammarTypePopup;
}
@end
