//
//  NSTest.h
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 7/27/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "SearchEngine.h"


@interface NSTest : NSObject {
@public
    NSString *name;
    NSString *description;
}

- (int)run;

+ (int) stringCheckSum: (NSString*)sourceStr;
+ (int) resultCheckSum: (SearchResult*)result;
+ (int) resultCount: (SearchResult*)result;
+ (bool) verifyCheckSum: (int)sum ofResult:(SearchResult*)result;
+ (int) resultSummary: (SearchResult*)result;

@end
