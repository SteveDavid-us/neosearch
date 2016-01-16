//
//  SearchResult.m
//  ns2osx
//
//  Created by Joshua Schnarr on 2/26/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SearchResult.h"

@implementation SearchResult

-(id)init
{
    self = [super init];
    if(!self) {
        return self;
    }
	hitVolumes = nil;
	hitVolumeNames = nil;
    return self;
}

-(id)dealloc
{
    [super dealloc];
	delete theHits;
    theHits = nil;
    return self;
}

-(void)close
{
	delete theHits;
    theHits = nil;
}

- (bool)_setHits:(CHitList *)hitList withTextFetch:(CTextFetch*)textFetch
{
    theHits = hitList;
	if (!hitList) {
		return false;
	}
	hitVolumes = [[NSMutableArray alloc] init];
	if (!hitVolumes) {
		return false;
	}
    hitsPerVol = hitList->ReportHitsPerVol();
	for (int i = 0; i < MAX_VOLUMES; i++) {
		if (hitsPerVol[i] > 0) {
			[hitVolumes addObject:[NSNumber numberWithInt:i]];
		}
	}

	// build hit volume name list - Maybe use dictionary so that passages can be added
	hitVolumeNames = [[NSMutableArray alloc] initWithCapacity:[hitVolumes count]];
	NSEnumerator *objenum = [hitVolumes objectEnumerator];
	NSNumber *vol;
	while ((vol = [objenum nextObject]))
	{
		NSString *name = [NSString stringWithCString:textFetch->GetBookName([vol shortValue])];
		[hitVolumeNames addObject:name];
	}
	//[hitVolumeNames sortUsingSelector:@selector(caseInsensitiveCompare:)];
	return true;
}

- (NSMutableArray *)hitVolumes
{
	return hitVolumes;
}

- (NSMutableArray *)hitVolumeNames
{
	return hitVolumeNames;
}

- (NSArray *)hitPassagesForVolume:(int)volume
{
	NSMutableDictionary *passageDict = [[NSMutableDictionary alloc] init];
    
	theHits->ResetVolume((short)volume);
	hit ahit;
	while (theHits->ReportNextHit(ahit) && ahit.volume == (short)volume) {
		NSNumber *passage = [NSNumber numberWithShort:ahit.passage];
        // Use NSMutableDictionary as a set so we only store unique passages
        [passageDict setObject:passage forKey:passage];
    }
    [passageDict autorelease];
    return [[passageDict allKeys] sortedArrayUsingSelector:@selector(compare:)];
}

- (long)totalHits
{
	return theHits->ReportTotalHits();
}

- (NSMutableArray *)hitsForVolume:(int)volume andPassage:(int)passage
{
	NSMutableArray *hits = [[NSMutableArray alloc] init];
	theHits->ResetVolume((short)volume);
	hit ahit;
	while (theHits->ReportNextHit(ahit) && ahit.volume == (short)volume) {
			if (ahit.passage == (short)passage) {
				[hits addObject:[NSNumber numberWithShort:ahit.word]];
			}
	}
	[hits sortUsingSelector:@selector(compare:)];
	return hits;
	
}

@end
