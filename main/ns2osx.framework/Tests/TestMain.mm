//
//  TestMain.mm
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 7/27/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//



#import "SearchTest.h"
#import "TextLookup.h"

int main (int argc, char **argv) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    // declare test objects
    StringSearch *stringSearch = [[StringSearch alloc] init];
    TextLookup *textLookup = [[TextLookup alloc] init];
    TermSearch *termSearch = [[TermSearch alloc] init];
    WildcardSearch *wildcardSearch = [[WildcardSearch alloc] init];
    IntersectionSearch *intersectionSearch = [[IntersectionSearch alloc] init];
    UnionSearch *unionSearch = [[UnionSearch alloc] init];
    VerbSearch *verbSearch = [[VerbSearch alloc] init];
    SubstantiveSearch *substantiveSearch = [[SubstantiveSearch alloc] init];
    StressTest *stressTest = [[StressTest alloc] init];
    DominusBug *dominusBug = [[DominusBug alloc] init];
    
    // put them in a dictionary by test name
    NSMutableArray *names = [[NSMutableArray alloc] init];
	NSArray *testObjects = [NSArray arrayWithObjects:
		stringSearch,
		textLookup,
		termSearch,
		wildcardSearch,
		intersectionSearch,
		unionSearch,
		verbSearch,
        substantiveSearch,
        stressTest,
        dominusBug,
		nil];
    NSEnumerator *oEnum = [testObjects objectEnumerator];
    NSTest *t;
    
    while (t = [oEnum nextObject]) {
        NSAutoreleasePool *testPool = [[NSAutoreleasePool alloc] init];
		NSLog(@"starting test, \"%@\"", t->name);
        int result = [t run];
        if (result == 0) {
            NSLog(@"test \"%@\" passed", t->name);
        } else {
            NSLog(@"test \"%@\" failed, returning %d", t->name, result);
        }
        [testPool release];
    }
 
    // Cleanup
    [testObjects release];
    [names release];
	[stringSearch release];
	[textLookup release];
	[termSearch release];
	[wildcardSearch release];
	[intersectionSearch release];
	[unionSearch release];
	[verbSearch release];
	[substantiveSearch release];
	[stressTest release];

    // Still not sure why this drain is failing
    //[pool drain]; 
    return 0;
}
