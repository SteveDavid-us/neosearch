//
//  SearchTerm.h
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 6/9/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Shared.h"

@interface SearchTerm : NSObject {
    CTarget *target;
	NSString *_string;
	bool intersect;		// Intersect or union with previous search terms
}

// Public methods

-(id)init;
-(void)dealloc;
-(void)setTermString:(NSString *)str;
-(NSString *)termString;
-(void)setSubstantiveWithCase:(NSString *)c andGender:(NSString *)g andParticiple:(NSString *)p andNumber:(NSString *)n;
-(void)setVerbWithMood:(NSString *)m andTense:(NSString *)t andVoice:(NSString *)v andPerson:(NSString *)p andNumber:(NSString *)n;
-(void)clearGrammar;
-(void)setIntersect: (bool)b;
-(bool)getIntersect;

// For wrapper only

-(void)initInterfaceDicts;
-(CTarget *)getTarget;

-(NSDictionary *)participleDict;
-(NSDictionary *)caseDict;
-(NSDictionary *)moodDict;
-(NSDictionary *)tenseDict;
-(NSDictionary *)voiceDict;
-(NSDictionary *)personDict;
-(NSDictionary *)genderDict;
-(NSDictionary *)numberDict;

@end
