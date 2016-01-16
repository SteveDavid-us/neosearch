//
//  HitRange.h
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 2/4/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

@interface HitRange : NSObject {
    NSNumber *start;
    NSNumber *end;
}
-(id)initWithStart:(NSNumber*)start andEnd:(NSNumber*)end;
-(void)dealloc;
-(NSNumber*)start;
-(NSNumber*)end;
-(void)setStart:(NSNumber*)start;
-(void)setEnd:(NSNumber*)end;

@end
