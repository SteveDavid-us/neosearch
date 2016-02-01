//
//  SearchEngine.mm
//  ns2osx.framework
//
//  Created by Joshua Schnarr on 6/8/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "SearchEngine.h"

@implementation SearchEngine

-(id)init
{
    self = [super init];
    if(!self) {
        return self;
    }
    
    try {
        CDataFile::SetBasePath(
            [[[NSBundle mainBundle] resourcePath] cString]);
        giantTable = new CGiant();
        textFetcher = new CTextFetch();
    } catch (CException f) {
        NSLog([NSString stringWithCString:f.Diagnostic()]);
        NSLog([NSString stringWithCString:f.WhatFailed()]);
        [self dealloc];
        return nil;
    }
    return self;
}

-(void)dealloc
{
    delete giantTable;
    delete textFetcher;
    [super dealloc];
}

// runSearchStrings is deprecated
-(SearchResult*)runSearchStrings:(Search*)search onResult:(SearchResult*)result
{
    NSEnumerator *enumerator = [[search searchStrings] objectEnumerator];
    NSString *term;
    int searchMode;
   
    CHitList *theHits = NULL;

    giantTable->SetAmbiguityChecking([search _ambiguityChecking]);
    giantTable->SetDisambiguationChecking([search _disambiguationChecking]);

    searchMode = CREATE_NEW;
    while(term = [enumerator nextObject]) {
        CTarget theTarget;
        theTarget.SetString([term cString]);
        giantTable->SetSearchMode(searchMode, theHits, [search _proximity]);
        searchMode = INTERSECT;
        theHits = giantTable->FindHitsWithMatchOf(theTarget);
        if (giantTable->CheckForError() != NO_SEARCH_ERROR) {
            NSLog(@"Error %d running search", giantTable->CheckForError());
            break;
        }
    }

    // the result will take care of deallocating theHits
    if (![result _setHits: theHits withTextFetch: textFetcher]) {
        NSLog(@"Error setting hits");
        return nil;
    }

    return result;
}

-(SearchResult*)runSearch:(Search*)search
{
    SearchResult *result = [[SearchResult alloc] init];
    if (!result) {
        return result;
    }
    [result autorelease];

    if ([[search searchTerms] count] == 0) {
        // Fallback to using searchStrings for backwards compatibility
        return [self runSearchStrings:search onResult:result];
    }
    
    NSEnumerator *enumerator = [[search searchTerms] objectEnumerator];
    SearchTerm *term;
    int searchMode;

    CHitList *theHits = NULL;

    giantTable->SetAmbiguityChecking([search _ambiguityChecking]);
    giantTable->SetDisambiguationChecking([search _disambiguationChecking]);

    searchMode = CREATE_NEW;
    while(term = [enumerator nextObject]) {

        if (searchMode != CREATE_NEW) {
            searchMode = [term getIntersect] ? INTERSECT : UNION;
        }

        giantTable->SetSearchMode(searchMode, theHits, [search _proximity]);
        theHits = giantTable->FindHits(*[term getTarget]);

        // Set the searchMode to something other than CREATE_NEW
        searchMode = INTERSECT;
    }

    // the result will take care of deallocating theHits
    if (![result _setHits: theHits withTextFetch: textFetcher]) {
        NSLog(@"Error setting hits");
        return nil;
    }

    return result;
}

- (NSString *)textForVolume:(int)volume andPassage:(int)passage withHits:(NSMutableDictionary *)hitList
{
    NSString *passageText = nil;
    
    // TODO - make these engine globals?
    CTextExploder textExploder;
    CHitOffsetList *hitOffsetList = new CHitOffsetList();
    hit ahit;
    ahit.volume = (short)volume;
    ahit.passage = (short)passage;

    // set up the hit list - this gets filled during the passage retrieval
    NSEnumerator *hitEnum = [hitList keyEnumerator];
    while (NSNumber *hitWord = [hitEnum nextObject])
    {
        hitOffsetList->AppendHit([hitWord shortValue]);
    }

    textExploder.Setup(textFetcher, giantTable, hitOffsetList);
    try {
        // XXX - what happens when ahit is not found?
        cstr passageStr = textExploder.RetrievePassageNumber(ahit);
        if (passageStr.text) {
            passageText = [NSString stringWithCString:passageStr.text length:passageStr.length];
        }
    }
    catch (CException f) {
        if (f.Diagnostic()) {
            NSLog([NSString stringWithCString:f.Diagnostic()]);
        }
        if (f.WhatFailed()) {
            NSLog([NSString stringWithCString:f.WhatFailed()]);
        }
    }
    
    // check for offset values
    for (int i = 0; i < hitOffsetList->GetListSize(); ++i) {
        NSNumber *hitWord = [NSNumber numberWithShort:hitOffsetList->GetWordNumberIdx(i)];
        HitRange *range = [[HitRange alloc] initWithStart:[NSNumber numberWithShort:hitOffsetList->GetStartOffsetIdx(i)]
                                                   andEnd:[NSNumber numberWithShort:hitOffsetList->GetEndOffsetIdx(i)]];
        [hitList setObject:range forKey:hitWord];
        [range release];
    }
    
    delete hitOffsetList;

    return passageText;
}



@end
