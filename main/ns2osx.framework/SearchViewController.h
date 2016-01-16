//
//  SearchViewController.h
//  ns2osx
//
//  Created by Michael Pigg on 7/17/08.
//
// Provides UI selection values for search terms

#import <Cocoa/Cocoa.h>
#import "SearchTerm.h"

@interface SearchViewController : NSObject {
	IBOutlet NSArrayController *searchTermsController;

	@private
	SearchTerm *_searchTerm;

}
- (NSArray *)substantiveGrammarCriteriaValues;
- (NSArray *)verbGrammarCriteriaValues;
- (NSArray *)substantiveGrammarCriteria;
- (NSArray *)verbGrammarCriteria;
- (NSArray *)grammarCaseValues;
- (NSArray *)grammarGenderValues;
- (NSArray *)grammarParticipleValues;
- (NSArray *)grammarMoodValues;
- (NSArray *)grammarTenseValues;
- (NSArray *)grammarVoiceValues;
- (NSArray *)grammarPersonValues;
- (NSArray *)grammarNumberValues;
- (NSArray *)grammarSearchTypes;
@end
