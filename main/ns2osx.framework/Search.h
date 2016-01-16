//
//  Search.h
//  ns2osx
//
//  Created by Joshua Schnarr on 2/26/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Shared.h"
#import "SearchTerm.h"

@interface Search : NSObject {
    bool ambiguityChecking;
    bool disambiguationChecking;
    int proximity;
    NSMutableArray *searchStrings;
	// If searchTerms is set, searchStrings are ignored
    NSMutableArray *searchTerms;
}

-(id)init;
-(id)dealloc;
-(NSMutableArray*)searchStrings;
-(NSMutableArray*)searchTerms;
-(NSMutableArray*)setSearchStrings:(NSArray *)stringArray;
-(NSMutableArray*)setSearchTerms:(NSArray *)termArray;

@end

@interface Search(FrameworkPrivate)
-(bool)_ambiguityChecking;
-(bool)_disambiguationChecking;
-(int)_proximity;

@end
