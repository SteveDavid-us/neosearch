//
//  SearchRowContainer.m
//  ns2osx
//
//  Created by Michael Pigg on 7/19/08.
//


#import "SearchRowContainer.h"


@implementation SearchRowContainer

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
       searchRows = [[NSMutableArray alloc] initWithCapacity:5];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(searchRowResized:) name:NS2SearchRowdidResizeEvent object:nil];
    }
    return self;
}

- (BOOL)isFlipped
{
	return YES;
}

- (void)setSearchTerms:(NSMutableArray *)searchTerms
{
	[self removeAllRows];
	NSEnumerator *termEnum = [searchTerms objectEnumerator];
	UISearchTerm *searchTerm = nil;
	while ((searchTerm = [termEnum nextObject]) != nil) {
		[self addNewRow];
		SearchRow *row = [searchRows lastObject];
		[row setSearchTerm:searchTerm];
	}
}

- (Search *) search
{
	NSMutableArray *terms = [NSMutableArray arrayWithCapacity:[searchRows count]];
	NSEnumerator *rowEnum = [searchRows objectEnumerator];
	SearchRow *row;
	while ((row = [rowEnum nextObject])!= nil)
	{
		[terms addObject:[[row searchTerm] toSearchTerm]];
	}
	Search *search = [[Search alloc] init];
	[search setSearchTerms:terms];
	return [search autorelease];
}

- (void)setDelegate:(id)adelegate
{
	if (adelegate) {
		[adelegate retain];
		[delegate release];
		delegate = adelegate;
	}
}

- (void)removeAllRows
{
	[searchRows removeAllObjects];
	NSEnumerator *viewEnum = [[self subviews] objectEnumerator];
	NSView *subview;
	while (subview = [viewEnum nextObject])
	{
		[subview removeFromSuperview];
	}
	[self searchRowResized:nil];
}

- (int)count
{
	return [searchRows count];
}

- (IBAction)removeRow:(id)sender
{
	NSView *target = [sender superview];
	NSEnumerator *viewEnum = [searchRows objectEnumerator];
	SearchRow *row;
	while (row = [viewEnum nextObject])
	{
		if (target == [row view]) {
			[target removeFromSuperview];
			[searchRows removeObject:row];
			[self searchRowResized:nil];
			break;
		}
	}

}

- (IBAction)addRow:(id)sender
{
	[self addNewRow];
}

- (SearchRow *)addNewRow
{
	bool isFirstRow = [searchRows count] == 0;
	SearchRow *newRow = [[SearchRow alloc] init];
	[[newRow addButton] setTarget:self];
	[[newRow addButton] setAction:@selector(addRow:)];
	if (!isFirstRow) {
		[[newRow removeButton] setEnabled:YES];
		[[newRow removeButton] setTarget:self];
		[[newRow removeButton] setAction:@selector(removeRow:)];
	} else {
		[[newRow removeButton] setEnabled:NO];
		[[newRow removeButton] setHidden:YES];
	}
	NSView *rowView = [newRow view];
	rowHeight = [rowView frame].size.height;
	[rowView setFrameOrigin:NSMakePoint(0, [self desiredHeight])];
	[rowView setFrameSize:NSMakeSize([self frame].size.width, rowHeight)];
	NSLog(@"Adding row at point %f, %f", [rowView frame].origin.x, [rowView frame].origin.y);
	[searchRows addObject:newRow];
	[self addSubview:rowView];
	[self resize];
	return newRow;	
}

- (void)searchRowResized:(NSNotification *)notification
{
	[self adjustRows];
	[self resize];

}

- (void)adjustRows
{
	float y = 0.0;
	NSEnumerator *viewEnum = [searchRows objectEnumerator];
	SearchRow *row;
	while (row = [viewEnum nextObject])
	{
		NSView *view = [row view];
		[view setFrameOrigin:NSMakePoint(0.0,y)];
		y += [view frame].size.height;
	}

}

- (void)resize
{
		NSRect oldFrame = [self frame];
		NSSize newSize = NSMakeSize(oldFrame.size.width,[self desiredHeight]);
		[self setFrameSize:newSize];
		if (delegate)
		{
			NSObject *obj = delegate;
			[obj performSelector:@selector(termsViewDidResize)];
		}
		[self setNeedsDisplay:YES];		

}

- (float)desiredHeight
{
	float rv = 0.0;
	NSEnumerator *viewEnum = [searchRows objectEnumerator];
	SearchRow *row;
	while (row = [viewEnum nextObject])
	{
		rv += [[row view] frame].size.height;
	}
	NSLog(@"Returning desired height of %f with %d rows.", rv, [searchRows count]);
	return rv;
}
@end
