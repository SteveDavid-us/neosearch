//
//  SearchResult.h
//  ns2osx
//
//  Created by Joshua Schnarr on 2/26/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Shared.h"
#import "SearchEngine.h"
#import "Search.h"

@interface SearchResult : NSObject {
    CHitOffsetList *hitOffsetList;
    NSMutableArray *hitVolumes;
    NSMutableArray *hitVolumeNames;
    CHitList *theHits;
  	long *hitsPerVol;
}

- (id)init;
- (id)dealloc;
// close - call this when you are done with a search result to ensure
//      that file handles get freed promptly
- (void)close;
- (NSMutableArray *)hitVolumes;
- (NSMutableArray *)hitVolumeNames;
// Note that this takes a volume id, not a volume index
- (NSArray *)hitPassagesForVolume:(int)volume;
- (long)totalHits;
// Note that this takes a volume id, not a volume index
- (NSMutableArray *)hitsForVolume:(int)volume andPassage:(int)passage;
@end

@interface SearchResult(FrameworkPrivate)
- (bool)_setHits:(CHitList *)hitList withTextFetch:(CTextFetch*)textFetch;
@end

