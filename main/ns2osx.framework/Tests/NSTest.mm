//
//  NSTest.m
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 7/27/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "NSTest.h"

@implementation NSTest

+ (int) stringCheckSum: (NSString*)sourceStr
{
    if (!sourceStr) {
        return -1;
    }
    const char *str = [sourceStr cString];
    int sum = 0;
    while (*str != 0) {
        sum += *str;
        str++;
    }
    return sum;
}

+ (int) resultCheckSum: (SearchResult*)result
{
    int sum = 0;
    if (!result) {
        return -1;
    }
    
    NSArray *volumes = [result hitVolumes];
    if (!volumes) {
        return -2;
    }
    NSArray *passages;
    NSEnumerator *passEnum, *volEnum = [volumes objectEnumerator];
    NSNumber *vol, *pass;
    while (vol = [volEnum nextObject]) {
        passages = [result hitPassagesForVolume:[vol intValue]];
        if (!passages) {
            return -3;
        }
        passEnum = [passages objectEnumerator];
        while (pass = [passEnum nextObject]) {
			sum += [vol intValue] * [pass intValue];
        }
    }
    return sum;
}

+ (bool) verifyCheckSum: (int)sum ofResult:(SearchResult*)result 
{
	int realSum = [self resultCheckSum:result];
	if (realSum != sum) {
		NSLog(@"Found checksum %d, expected %d", realSum, sum);
		return false;
	}
	return true;
}

+ (int) resultCount: (SearchResult*)result
{
    int count = 0;
    if (!result) {
        return -1;
    }
    
    NSArray *volumes = [result hitVolumes];
    if (!volumes) {
        return -2;
    }
    NSArray *passages, *hits;
    NSEnumerator *passEnum, *volEnum = [volumes objectEnumerator];
    NSNumber *vol, *pass;
    while (vol = [volEnum nextObject]) {
        passages = [result hitPassagesForVolume:[vol intValue]];
        if (!passages) {
            return -3;
        }
        passEnum = [passages objectEnumerator];
        while (pass = [passEnum nextObject]) {
            hits = [result hitsForVolume:[vol intValue] andPassage:[pass intValue]];
            count += [hits count];
        }
    }
    return count;
}

+ (int) resultSummary: (SearchResult*)result
{
    if (!result) {
        return -1;
    }
    
    NSArray *volumes = [result hitVolumes], *volNames = [result hitVolumeNames];
    if (!volumes) {
        return -2;
    }
    NSArray *passages;
    NSEnumerator *passEnum, *volEnum = [volumes objectEnumerator], 
        *volNameEnum = [volNames objectEnumerator];
    NSNumber *vol, *pass;
    NSMutableString *volName;
    while (vol = [volEnum nextObject]) {
        volName = [[volNameEnum nextObject] mutableCopy];
        [volName appendString: @":"];
        passages = [result hitPassagesForVolume:[vol intValue]];
        if (!passages) {
            return -3;
        }
        passEnum = [passages objectEnumerator];
        while (pass = [passEnum nextObject]) {
            [volName appendFormat:@" %@", pass];
        }
        NSLog(volName);
    }
    return 0;
}

- (int)run
{
    // must be implemented by derived classes
    return 1;
}

@end
