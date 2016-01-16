//
//  TextLookup.mm
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 7/27/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "TextLookup.h"
#import "SearchEngine.h"

// TODO - expand this so it covers more passages
int volumes[] = { 8, 8, 8, 8 };
int passages[] = { 1, 2, 500, 5000 };
int checksums[] = { 490454, 158506, 63864, -1 };

@implementation TextLookup

-(id)init
{
    name = @"textlookup";
    description = @"Looks up a sampling of passages and makes sure that the text matches expectations";
    return self;
}

-(int)run
{
    SearchEngine *s = [[SearchEngine alloc] init];
    if (!s) {
        return 1;
    }
 
    int retval = 0;
    for (int i = 0; i < sizeof(passages) / sizeof(int); i++) {
		NSMutableDictionary *emptyDict = [[NSMutableDictionary alloc] init];
        NSString *text = [s textForVolume:volumes[i] andPassage:passages[i] withHits:emptyDict];
        int sum = [NSTest stringCheckSum:text];
        if (sum != checksums[i]) {
            NSLog(@"checksum failure: vol %d pas %d sum %d expected %d", volumes[i], passages[i], sum, checksums[i]);
            retval = i + 10;
        }
    }

    return retval;
}

@end
