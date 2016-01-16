//
//  SearchTerm.mm
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 6/9/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SearchTerm.h"

@implementation SearchTerm
// Mapping of parameter strings to their values
static NSDictionary *_participleDict, *_caseDict, *_moodDict, *_tenseDict,
	*_voiceDict, *_personDict, *_genderDict, *_numberDict;
static bool interfaceDictsInitialized = false;

-(void)initInterfaceDicts
{
    // We allocate and initialize these dictionaries once and never free them.
    // They should probably be moved to a global class so they (and their 
    // contents) can be released at app exit.
	if (interfaceDictsInitialized) {
		return;
	}
	_participleDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:SUBSTANTIVE_STEM],                @"Substantive",                         
		[NSNumber numberWithShort:PRESENT_STEM],                    @"Present",                             
		[NSNumber numberWithShort:PERFECT_ACTIVE_STEM],             @"Perfect Active",                      
		[NSNumber numberWithShort:PERFECT_PASSIVE_PPLE_STEM],       @"Perfect Passive",                   	
		[NSNumber numberWithShort:GERUNDIVE_STEM],                  @"Gerundive",                           
		[NSNumber numberWithShort:PRESENT_ACTIVE_PPLE_STEM],        @"Present Active",                    	
		[NSNumber numberWithShort:FUTURE_ACTIVE_PPLE_STEM],         @"Future Active",                     	
		[NSNumber numberWithShort:COMPAR_ADV_ADJ_STEM],             @"Comparative of Adverb or Adjective",  
		[NSNumber numberWithShort:SUPERL_ADV_ADJ_STEM],             @"Superlative of Adverb or Adjective",  
		[NSNumber numberWithShort:COMPAR_PERFECT_PASSIVE_PPLE_STEM],@"Comparative of Perfect Passive",    	
		[NSNumber numberWithShort:SUPERL_PERFECT_PASSIVE_PPLE_STEM],@"Superlative of Perfect Passive",    	
		[NSNumber numberWithShort:COMPAR_PRESENT_ACTIVE_PPLE_STEM], @"Comparative of Present Active",     	
		[NSNumber numberWithShort:SUPERL_PRESENT_ACTIVE_PPLE_STEM], @"Superlative of Present Active",     	
        nil];

	_caseDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_CASE],        @"None",								
		[NSNumber numberWithShort:NOMINATIVE_CASE],@"Nominative",                          
		[NSNumber numberWithShort:GENITIVE_CASE],  @"Genitive",                            
		[NSNumber numberWithShort:DATIVE_CASE],    @"Dative",                              
		[NSNumber numberWithShort:ACCUSATIVE_CASE],@"Accusative",                          
		[NSNumber numberWithShort:ABLATIVE_CASE],  @"Ablative",                            
		[NSNumber numberWithShort:VOCATIVE_CASE],  @"Vocative",                            
		[NSNumber numberWithShort:LOCATIVE_CASE],  @"Locative",                            
		nil];

	_moodDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_MOOD],         @"None",								
		[NSNumber numberWithShort:INDICATIVE_MOOD], @"Indicative",                         	
		[NSNumber numberWithShort:SUBJUNCTIVE_MOOD],@"Subjunctive",                         
		[NSNumber numberWithShort:INFINITIVE_MOOD], @"Infinitive",                          
		[NSNumber numberWithShort:IMPERATIVE_MOOD], @"Imperative",                          
        nil];

	_tenseDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_TENSE],            @"None",								
		[NSNumber numberWithShort:PRESENT_TENSE],       @"Present",                             
		[NSNumber numberWithShort:IMPERFECT_TENSE],     @"Imperfect",                           
		[NSNumber numberWithShort:FUTURE_TENSE],        @"Future",                              
		[NSNumber numberWithShort:PERFECT_TENSE],       @"Perfect",                             
		[NSNumber numberWithShort:PLUPERFECT_TENSE],    @"Pluperfect",                          
		[NSNumber numberWithShort:FUTURE_PERFECT_TENSE],@"Future Perfect",                      
        nil];

	_voiceDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_VOICE],     @"None",								
		[NSNumber numberWithShort:ACTIVE_VOICE], @"Active",								
		[NSNumber numberWithShort:PASSIVE_VOICE],@"Passive",								
        nil];

	_personDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_PERSON],    @"None",								
		[NSNumber numberWithShort:FIRST_PERSON], @"1st",									
		[NSNumber numberWithShort:SECOND_PERSON],@"2nd",									
		[NSNumber numberWithShort:THIRD_PERSON], @"3rd",									
        nil];

	_numberDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_NUMBER],      @"None",								
		[NSNumber numberWithShort:SINGULAR_NUMBER],@"Singular",							
		[NSNumber numberWithShort:PLURAL_NUMBER],  @"Plural",								
        nil];

	_genderDict = [[NSDictionary alloc] initWithObjectsAndKeys:
		[NSNumber numberWithShort:NO_GENDER],       @"None",								
		[NSNumber numberWithShort:MASCULINE_GENDER],@"Masculine",							
		[NSNumber numberWithShort:FEMININE_GENDER], @"Feminine",							
		[NSNumber numberWithShort:NEUTER_GENDER],   @"Neuter",								
        nil];
	interfaceDictsInitialized = true;
}

-(NSDictionary *)participleDict
{
	return _participleDict;
}

-(NSDictionary *)caseDict
{
	return _caseDict;
}

-(NSDictionary *)moodDict
{
	return _moodDict;
}

-(NSDictionary *)tenseDict
{
	return _tenseDict;
}

-(NSDictionary *)voiceDict
{
	return _voiceDict;
}

-(NSDictionary *)personDict
{
	return _personDict;
}

-(NSDictionary *)genderDict
{
	return _genderDict;
}

-(NSDictionary *)numberDict
{
	return _numberDict;
}

-(id)init
{
	if (self = [super init]) {
		target = new CTarget();
		[self initInterfaceDicts];
		intersect = true;
	}
    return self;
}

-(void)dealloc
{
	delete target;
	[super dealloc];
}

-(CTarget *)getTarget
{
	return target;	
}

-(void)setTermString:(NSString *)str
{
    [str retain];
    [_string release];
	_string = str;
	if ([_string isEqualToString:@""] || [_string isEqualToString:@"*"] || _string == Nil) {
		target->SetStemDontCare();
	} else {
		target->SetString([_string cString]);
	}
}

-(NSString *)termString
{
	return _string;
}

-(void)setSubstantiveWithCase:(NSString *)c andGender:(NSString *)g andParticiple:(NSString *)p andNumber:(NSString *)n
{
    NSNumber *val;
	[self clearGrammar];
    target->SetGrammarDoCare();
	if (![c isEqualToString:@""]) {
        val = [[self caseDict] objectForKey:c];
        assert(val);
		target->SetCase([val shortValue]);
	}
	if (![g isEqualToString:@""]) {
        val = [[self genderDict] objectForKey:g];
        assert(val);
		target->SetGender([val shortValue]);
	}
	if (![p isEqualToString:@""]) {
        val = [[self participleDict] objectForKey:p];
        assert(val);
		target->SetParticiple([val shortValue]);
	}
	if (![n isEqualToString:@""]) {
        val = [[self numberDict] objectForKey:n];
        assert(val);
		target->SetNumber([val shortValue]);
	}
}

-(void)setVerbWithMood:(NSString *)m andTense:(NSString *)t andVoice:(NSString *)v andPerson:(NSString *)p andNumber:(NSString *)n
{
    NSNumber *val;
	[self clearGrammar];
    target->SetGrammarDoCare();
	if (![m isEqualToString:@""]) {
        val = [[self moodDict] objectForKey:m];
        assert(val);
		target->SetMood([val shortValue]);
	}
	if (![t isEqualToString:@""]) {
        val = [[self tenseDict] objectForKey:t];
        assert(val);
		target->SetTense([val shortValue]);
	}
	if (![v isEqualToString:@""]) {
        val = [[self voiceDict] objectForKey:v];
        assert(val);
		target->SetVoice([val shortValue]);
	}
	if (![p isEqualToString:@""]) {
        val = [[self personDict] objectForKey:p];
        assert(val);
		target->SetPerson([val shortValue]);
	}
	if (![n isEqualToString:@""]) {
        val = [[self numberDict] objectForKey:n];
        assert(val);
		target->SetNumber([val shortValue]);
	}
}

-(void)clearGrammar
{
	target->ClearGrammar();
}

-(void)setIntersect: (bool)b
{
	intersect = b;
}

-(bool)getIntersect
{
	return intersect;
}

@end
