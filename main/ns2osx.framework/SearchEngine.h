//
//  SearchEngine.h
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 6/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Shared.h"
#import "Search.h"
#import "SearchResult.h"
#import "SearchEngine.h"
#import "HitRange.h"

@class Search;
@class SearchResult;

// TODO - consider multithreading issues
@interface SearchEngine : NSObject {
    CGiant *giantTable;
    CTextFetch *textFetcher;
}

-(id)init;
-(void)dealloc;
-(SearchResult*)runSearch:(Search*)search;
-(NSString *)textForVolume:(int)volume andPassage:(int)passage withHits:(NSMutableDictionary*)hitList;


@end

@interface SearchEngine(FrameworkPrivate)
@end
