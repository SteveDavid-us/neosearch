//
//  UISearchTerm.h
//  ns2osx.framework
//
//  Created by Michael Pigg on 10/17/09.
//	A bindings-compatible class for SearchTerm.
//

#import <Cocoa/Cocoa.h>
#import "SearchTerm.h"

@interface UISearchTerm : NSObject {
	NSString *termString;
	NSString *termVerbType;
	NSString *termCase;
	NSString *termParticiple;
	NSString *termNumber;
	NSString *termGender;
	NSString *termMood;
	NSString *termVoice;
	NSString *termTense;
	NSString *termPerson;
	
}

#pragma mark SearchTerm conversion
- (SearchTerm *)toSearchTerm;

# pragma mark Getter/setters
- (NSString *)termString;
- (void)setTermString:(NSString *)value;

- (NSString *)termVerbType;
- (void)setTermVerbType:(NSString *)value;

- (NSString *)termCase;
- (void)setTermCase:(NSString *)value;

- (NSString *)termParticiple;
- (void)setTermParticiple:(NSString *)value;

- (NSString *)termNumber;
- (void)setTermNumber:(NSString *)value;

- (NSString *)termGender;
- (void)setTermGender:(NSString *)value;

- (NSString *)termMood;
- (void)setTermMood:(NSString *)value;

- (NSString *)termVoice;
- (void)setTermVoice:(NSString *)value;

- (NSString *)termTense;
- (void)setTermTense:(NSString *)value;

- (NSString *)termPerson;
- (void)setTermPerson:(NSString *)value;


@end
