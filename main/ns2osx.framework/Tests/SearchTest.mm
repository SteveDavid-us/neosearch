//
//  SearchTest.mm
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 7/27/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//


#import "SearchTest.h"
#import "SearchEngine.h"



@implementation StringSearch

-(id)init
{
    description = @"Runs a few basic searches and verifies the results";
    name = @"stringsearch";
    return self;
}

-(int)run
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];
    
    NSArray *stringTerms = [NSArray arrayWithObjects: @"amo", nil];
    
    [s setSearchStrings:stringTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
    
    if (![NSTest verifyCheckSum:64853 ofResult:sr]) {
        return 4;
    }
    
    return 0;
}

@end

@implementation TermSearch
-(id)init
{
    description = @"Runs a SearchTerm search and verifies the result";
    name = @"termsearch";
    return self;
}

-(int)run
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

	// Basic string search	
	SearchTerm *st1 = [[SearchTerm alloc] init];
	[st1 setTermString:@"amo"];
    [st1 autorelease];

    NSArray *searchTerms = [NSArray arrayWithObjects: st1, nil];
    
    [s setSearchTerms:searchTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
    
    if (![NSTest verifyCheckSum:64853 ofResult:sr]) {
        return 4;
    }
    
    return 0;
}

@end

@implementation WildcardSearch
-(id)init
{
    description = @"Runs a search with a wildcard and verifies the result";
    name = @"wildcardsearch";
    return self;
}

-(int)run
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

	// Wildcard string search
	SearchTerm *st1 = [[SearchTerm alloc] init];
	[st1 setTermString:@"deu*"];
    [st1 autorelease];
    
    NSArray *searchTerms = [NSArray arrayWithObjects: st1, nil];
    
    [s setSearchTerms:searchTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
    
    if (![NSTest verifyCheckSum:42712446 ofResult:sr]) {
        return 4;
    }
    
    return 0;
}
@end

@implementation IntersectionSearch
-(id)init
{
    description = @"Runs a search for an intersection of two terms and verifies the result";
    name = @"intersectionsearch";
    return self;
}

-(int)run
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

	SearchTerm *st1 = [[SearchTerm alloc] init];
	[st1 setTermString:@"nempe"];
    [st1 autorelease];

	// Intersection is the default
	SearchTerm *st2 = [[SearchTerm alloc] init];
	[st2 setTermString:@"manifestum"];
    [st2 autorelease];
    
    NSArray *searchTerms = [NSArray arrayWithObjects: st1, st2, nil];
    
    [s setSearchTerms:searchTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
   
    if (![NSTest verifyCheckSum:22646 ofResult:sr]) {
        return 4;
    }
    
    return 0;
}

@end


@implementation UnionSearch
-(id)init
{
    description = @"Runs a search for an union of two terms and verifies the result";
    name = @"unionsearch";
    return self;
}

-(int)run
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

	SearchTerm *st1 = [[SearchTerm alloc] init];
	[st1 setTermString:@"transierant"];
    [st1 autorelease];

	// Intersection is the default
	SearchTerm *st2 = [[SearchTerm alloc] init];
	[st2 setTermString:@"quadrangularis"];
	[st2 setIntersect:false];
    [st2 autorelease];
    
    NSArray *searchTerms = [NSArray arrayWithObjects: st1, st2, nil];
    
    [s setSearchTerms:searchTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
   
    if (![NSTest verifyCheckSum:217238 ofResult:sr]) {
        return 4;
    }
    
    return 0;
}
@end


@implementation VerbSearch
-(id)init
{
    description = @"Tests the various verb search parameters";
    name = @"verbsearch";
    return self;
}

-(int) verifyResultCount: (int)count withSearchEngine:(SearchEngine *)se withMood:(NSString *)m andTense:(NSString *)t andVoice:(NSString *)v andPerson:(NSString *)p andNumber:(NSString *)n andString:(NSString *)str
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
	SearchTerm *st1 = [[SearchTerm alloc] init];
    [st1 autorelease];
	[st1 setVerbWithMood:m andTense:t andVoice:v andPerson:p andNumber:n];
	[st1 setTermString:str];
    NSArray *searchTerms = [NSArray arrayWithObjects: st1, nil];
    [s setSearchTerms:searchTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
  
  	int realCount = [NSTest resultCount:sr];
    if (realCount != count) {
		NSLog(@"Found %d results, expected %d (%@, %@, %@, %@, %@, %@)", realCount, count ,
			m, t, v, p, n, str);

        [NSTest resultSummary:sr];
		return 4;
    }
    
    return 0;
}

-(int)run
{
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

	NSArray *tests = [NSArray arrayWithObjects:
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 50], 
            @"", @"Pluperfect", @"Active", @"", @"", @"dixeramus", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 2974], 
            @"", @"Pluperfect", @"Active", @"", @"", @"", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 31702], 
            @"", @"Imperfect", @"", @"", @"", @"", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 7738], 
            @"", @"", @"Passive", @"", @"", @"dixeramus", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 7001], 
            @"Subjunctive", @"", @"", @"2nd", @"Singular", @"", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 2], 
            @"Indicative", @"Pluperfect", @"Active", @"1st", @"Plural", @"", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 23613], 
            @"", @"", @"", @"", @"", @"dixeramus", nil],
		nil];

	NSEnumerator *testEnum = [tests objectEnumerator];	
	NSArray *test;
	while (test = [testEnum nextObject]) {
		int retval = [self verifyResultCount: [[test objectAtIndex: 0] intValue] 
				withSearchEngine:se withMood:[test objectAtIndex: 1] 
				andTense:[test objectAtIndex: 2] andVoice:[test objectAtIndex: 3] 
				andPerson:[test objectAtIndex: 4] andNumber:[test objectAtIndex: 5]
                andString:[test objectAtIndex: 6]];
		if (retval) {
			return retval;
		}
	}
	return 0;
}
@end


@implementation SubstantiveSearch
-(id)init
{
    description = @"Tests the various substantive search parameters";
    name = @"substantivesearch";
    return self;
}

-(int) verifyResultCount: (int)count withSearchEngine:(SearchEngine *)se withCase:(NSString *)c andGender:(NSString *)g andParticiple:(NSString *)p andNumber:(NSString *)n andString:(NSString *)str
{
    Search *s = [[Search alloc] init];
    if (!s) {
        return 1;
    }
    [s autorelease];
    
	SearchTerm *st1 = [[SearchTerm alloc] init];
    [st1 autorelease];
	[st1 setSubstantiveWithCase:c andGender:g andParticiple:p andNumber:n];
	[st1 setTermString:str];
    NSArray *searchTerms = [NSArray arrayWithObjects: st1, nil];
    [s setSearchTerms:searchTerms];
    
    SearchResult *sr = [se runSearch:s];
    if (!sr) {
        return 3;
    }
  
  	int realCount = [NSTest resultCount:sr];
    if (realCount != count) {
		NSLog(@"Found %d results, expected %d (%@, %@, %@, %@, %@)", realCount, count ,
			c, g, p, n, str);

        [NSTest resultSummary:sr];
		return 4;
    }
    
    return 0;
}

-(int)run
{
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

	NSArray *tests = [NSArray arrayWithObjects:
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 1087], 
            @"", @"", @"", @"", @"manifestius", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 137], 
            @"Nominative", @"", @"", @"", @"manifestius", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 967], 
            @"", @"Masculine", @"", @"", @"manifestius", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 53], 
            @"", @"", @"Comparative of Adverb or Adjective", @"", @"manifestius", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 1080], 
            @"", @"", @"", @"Singular", @"manifestius", nil],
		[NSArray arrayWithObjects: [NSNumber numberWithInt: 9], 
            @"Nominative", @"Masculine", @"Gerundive", @"Singular", @"purificandus", nil],
		nil];

	NSEnumerator *testEnum = [tests objectEnumerator];	
	NSArray *test;
	while (test = [testEnum nextObject]) {
		int retval = [self verifyResultCount: [[test objectAtIndex: 0] intValue] 
				withSearchEngine:se withCase:[test objectAtIndex: 1] 
				andGender:[test objectAtIndex: 2] andParticiple:[test objectAtIndex: 3] 
				andNumber:[test objectAtIndex: 4] andString:[test objectAtIndex: 5]];
		if (retval) {
			return retval;
		}
	}
	return 0;
}
@end

@implementation StressTest
-(id)init
{
    description = @"Test for memory leaks, etc";
    name = @"stresstest";
    return self;
}

-(int)run
{
    char str[4];
    str[2] = '*'; str[3] = '\0';

    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

    Search *s = [[Search alloc] init];
    if (!s) {
        return 3;
    }
    [s autorelease];

	SearchTerm *st1 = [[SearchTerm alloc] init];
    if (!st1) {
        return 4;
    }
    [st1 autorelease];

    try {
    for (str[0] = 'j'; str[0] <= 'z'; ++str[0]) {
        for (str[1] = 'a'; str[1] <= 'z'; ++str[1]) {
            NSString *nsStr = [[NSString alloc] initWithCString:str
                    encoding:NSASCIIStringEncoding];
            [st1 setTermString: nsStr];
            [nsStr release];
            NSArray *searchTerms = [NSArray arrayWithObjects: st1, nil];
            [s setSearchTerms:searchTerms];
            SearchResult *sr = [se runSearch:s];
            if (!sr) {
                return 5;
            }
            NSLog(@"%s %d\n", str, [NSTest resultCount:sr]);
            [sr close];
        }
    }
    } catch (CFatalError error) {
        NSLog(@"Found a CFatalError: %s, %s\n", error.Diagnostic(), 
            error.WhatFailed());
    }
	return 0;
}
@end

@implementation DominusBug
-(id)init
{
    description = @"Test for search broken in the main UI";
    name = @"dominusbug";
    return self;
}

-(int)run
{
    SearchEngine *se = [[SearchEngine alloc] init];
    if (!se) {
        return 2;
    }
    [se autorelease];

    Search *s = [[Search alloc] init];
    if (!s) {
        return 3;
    }
    [s autorelease];

	SearchTerm *st1 = [[SearchTerm alloc] init];
    if (!st1) {
        return 4;
    }
    [st1 autorelease];

	SearchTerm *st2 = [[SearchTerm alloc] init];
    if (!st1) {
        return 5;
    }
    [st2 autorelease];

	SearchTerm *st3 = [[SearchTerm alloc] init];
    if (!st1) {
        return 6;
    }
    [st3 autorelease]; 

    try {
        [st1 setTermString: @"dominus"];
        [st2 setTermString: @"quod"];
        [st3 setTermString: @"prorsus"];
        NSArray *searchTerms = [NSArray arrayWithObjects: st1, st2, st3, nil];
        [s setSearchTerms:searchTerms];
        SearchResult *sr = [se runSearch:s];
        if (!sr) {
            return 5;
        }

        NSEnumerator *volEnum = [[sr hitVolumes] objectEnumerator]; 
        NSEnumerator *passEnum;
        NSNumber *v, *p;
        int i = 0;
        while (v = [volEnum nextObject]) {
            NSLog(@"%@ %d:", [[sr hitVolumeNames] objectAtIndex:i++], [v intValue]);
            passEnum = [[sr hitPassagesForVolume:[v intValue]] objectEnumerator];
            while (p = [passEnum nextObject]) {
                NSLog(@"%d ", [p intValue]);
            }
            NSLog(@"\n");
        }

        NSLog(@"Result count: %d\n", [NSTest resultCount:sr]);
        [sr close];

    } catch (CFatalError error) {
        NSLog(@"Found a CFatalError: %s, %s\n", error.Diagnostic(), 
            error.WhatFailed());
    }

    return 0;
}
@end
