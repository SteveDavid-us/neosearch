//
//  UISearchTerm.m
//  ns2osx.framework
//
//  Created by Michael Pigg on 10/17/09.
//

#import "UISearchTerm.h"


@implementation UISearchTerm
- (id) init
{
	self = [super init];
	if (self != nil) {
		[self setTermString:@""];
		[self setTermVerbType:@""];
		[self setTermCase:@""];
		[self setTermParticiple:@""];
		[self setTermNumber:@""];
		[self setTermGender:@""];
		[self setTermMood:@""];
		[self setTermVoice:@""];
		[self setTermTense:@""];
		[self setTermPerson:@""];		
	}
	return self;
}

- (SearchTerm *)toSearchTerm {
	SearchTerm *term = [[SearchTerm alloc] init];
	[term setTermString:[termString copy]];
	if ([[self termVerbType] isEqualToString:@"Substantive"]) 
	{
		[term setSubstantiveWithCase:[termCase copy] andGender:[termGender copy] andParticiple:[termParticiple copy] andNumber:[termNumber copy]];
	} else if ([[self termVerbType] isEqualToString:@"Verb"]) {
		[term setVerbWithMood:[termMood copy] andTense:[termTense copy] andVoice:[termVoice copy] andPerson:[termPerson copy] andNumber:[termNumber copy]];
	}
	return [term autorelease];
}

- (NSString *)termString {
    return [[termString retain] autorelease];
}

- (void)setTermString:(NSString *)value {
    if (termString != value) {
        [termString release];
        termString = [value copy];
    }
}

- (NSString *)termVerbType {
    return [[termVerbType retain] autorelease];
}

- (void)setTermVerbType:(NSString *)value {
    if (termVerbType != value) {
        [termVerbType release];
        termVerbType = [value copy];
    }
}

- (NSString *)termCase {
    return [[termCase retain] autorelease];
}

- (void)setTermCase:(NSString *)value {
    if (termCase != value) {
        [termCase release];
        termCase = [value copy];
    }
}

- (NSString *)termParticiple {
    return [[termParticiple retain] autorelease];
}

- (void)setTermParticiple:(NSString *)value {
    if (termParticiple != value) {
        [termParticiple release];
        termParticiple = [value copy];
    }
}

- (NSString *)termNumber {
    return [[termNumber retain] autorelease];
}

- (void)setTermNumber:(NSString *)value {
    if (termNumber != value) {
        [termNumber release];
        termNumber = [value copy];
    }
}

- (NSString *)termGender {
    return [[termGender retain] autorelease];
}

- (void)setTermGender:(NSString *)value {
    if (termGender != value) {
        [termGender release];
        termGender = [value copy];
    }
}

- (NSString *)termMood {
    return [[termMood retain] autorelease];
}

- (void)setTermMood:(NSString *)value {
    if (termMood != value) {
        [termMood release];
        termMood = [value copy];
    }
}

- (NSString *)termVoice {
    return [[termVoice retain] autorelease];
}

- (void)setTermVoice:(NSString *)value {
    if (termVoice != value) {
        [termVoice release];
        termVoice = [value copy];
    }
}

- (NSString *)termTense {
    return [[termTense retain] autorelease];
}

- (void)setTermTense:(NSString *)value {
    if (termTense != value) {
        [termTense release];
        termTense = [value copy];
    }
}

- (NSString *)termPerson {
    return [[termPerson retain] autorelease];
}

- (void)setTermPerson:(NSString *)value {
    if (termPerson != value) {
        [termPerson release];
        termPerson = [value copy];
    }
}


@end
