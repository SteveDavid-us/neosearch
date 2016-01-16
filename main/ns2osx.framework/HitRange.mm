//
//  HitRange.mm
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 2/4/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "HitRange.h"

@implementation HitRange
-(id)initWithStart:(NSNumber*)newStart andEnd:(NSNumber*)newEnd
{
	[super init];
    [self setStart:newStart];
    [self setEnd:newEnd];
    return self;
}
-(void)dealloc
{
	[start release];
	[end release];
	[super dealloc];
}
-(NSNumber*)start 
{
	return start;
}
-(NSNumber*)end
{
	return end;
}
-(void)setStart:(NSNumber*)newStart
{
	[newStart retain];
	[start release];
	start = newStart;
}
-(void)setEnd:(NSNumber*)newEnd;
{
	[newEnd retain];
	[end release];
	end = newEnd;
}
@end
