//
//  SearchViewController.m
//  ns2osx
//
//  Created by Michael Pigg on 7/17/08.
//

#import "SearchViewController.h"


@implementation SearchViewController

- (id) init
{
	self = [super init];
	if (self != nil) {
		_searchTerm = [[SearchTerm alloc] init];
	}
	return self;
}

- (void) dealloc
{
	[_searchTerm release];
	[super dealloc];
}


- (NSMutableDictionary *)dictionaryWithNameValue:(NSString *)nameValue
{
	return [NSMutableDictionary dictionaryWithObject:nameValue forKey:@"name"];
}

- (NSArray *)substantiveGrammarCriteriaValues
{
	NSArray *criteriaArray = [self substantiveGrammarCriteria];
	NSMutableArray *dictionaryArray = [NSMutableArray arrayWithCapacity:[criteriaArray count]];
	NSEnumerator *cenum = [criteriaArray objectEnumerator];
	NSString *criterion;
	while (criterion = [cenum nextObject])
	{
		SEL sel = NSSelectorFromString([NSString stringWithFormat:@"grammar%@Values", criterion]);
		NSArray *values = [self performSelector:sel];
		[dictionaryArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:criterion, @"name", values, @"values",nil]];
	}
	return dictionaryArray;
	
}

- (NSArray *)verbGrammarCriteriaValues
{
	NSArray *criteriaArray = [self verbGrammarCriteria];
	NSMutableArray *dictionaryArray = [NSMutableArray arrayWithCapacity:[criteriaArray count]];
	NSEnumerator *cenum = [criteriaArray objectEnumerator];
	NSString *criterion;
	while (criterion = [cenum nextObject])
	{
		SEL sel = NSSelectorFromString([NSString stringWithFormat:@"grammar%@Values", criterion]);
		NSArray *values = [self performSelector:sel];
		[dictionaryArray addObject:[NSDictionary dictionaryWithObjectsAndKeys:criterion, @"name", values, @"values",nil]];
	}
	return dictionaryArray;
}

- (NSArray *)substantiveGrammarCriteria
{
	return [NSArray arrayWithObjects:@"Case", @"Gender", @"Participle", @"Number", nil];
}

- (NSArray *)verbGrammarCriteria
{
	return [NSArray arrayWithObjects:@"Mood", @"Tense", @"Voice", @"Person", @"Number", nil];
}

- (NSArray *)grammarSearchTypes
{
	return [NSArray arrayWithObjects:@"Substantive", @"Verb", nil];
}

- (NSArray *)grammarCaseValues
{
	return [[_searchTerm caseDict] allKeys];
}

- (NSArray *)grammarGenderValues
{
	return [[_searchTerm genderDict] allKeys];
}

- (NSArray *)grammarParticipleValues
{
	return [[_searchTerm participleDict] allKeys];
									
}

- (NSArray *)grammarMoodValues
{
	return [[_searchTerm moodDict] allKeys];
}

- (NSArray *)grammarTenseValues
{
	return [[_searchTerm tenseDict] allKeys];
}

- (NSArray *)grammarVoiceValues
{
	return [[_searchTerm voiceDict] allKeys];
}

- (NSArray *)grammarPersonValues
{
	return [[_searchTerm personDict] allKeys];
}

- (NSArray *)grammarNumberValues
{
	return [[_searchTerm numberDict] allKeys];
}

@end
