//
//  Search.mm
//  ns2osx
//
//  Created by Joshua Schnarr on 2/26/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "Search.h"

@implementation Search

-(id)dealloc
{
    [searchTerms dealloc];
    [searchStrings dealloc];
    [super dealloc];
    return self;
}

-(id)init
{
    self = [super init];
    searchStrings = [[NSMutableArray alloc] init];
    searchTerms = [[NSMutableArray alloc] init]; 
    if(!self || !searchStrings || !searchTerms) {
        [searchStrings dealloc];
        [searchTerms dealloc];
        [self dealloc];
        return nil;
    } else {
        proximity = 25;
        ambiguityChecking = true;
        disambiguationChecking = false;
    }
    return self;
}

-(NSMutableArray*)searchStrings
{
	return searchStrings;
}

-(NSMutableArray*)setSearchStrings:(NSArray *)stringArray
{
    [searchStrings removeAllObjects];
    [searchStrings addObjectsFromArray:stringArray];
    return searchStrings;
}

-(NSMutableArray*)setSearchTerms:(NSArray *)termArray
{
    [searchTerms removeAllObjects];
    [searchTerms addObjectsFromArray:termArray];
    return searchStrings;
}

-(NSMutableArray*)searchTerms
{
	return searchTerms;
}

-(bool)_ambiguityChecking
{
	return ambiguityChecking;
}

-(bool)_disambiguationChecking
{
	return disambiguationChecking;
}

-(int)_proximity
{
	return proximity;
}
@end
