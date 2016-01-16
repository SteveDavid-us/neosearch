// *******************************************************************************************
// CGiant.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// NeoSearch 2 -- 2006 -- JPS
// *******************************************************************************************

#include <string.h>
#include <vector>
#include <iostream>
//#include <stdlib.h>
#include "CDataFile.h"
#include "CGiant.h"
#include "CAmbiguity.h"
#include "CHitList.h"
#include "CTarget.h"
#include "CRoseCode.h"
#include "StringTools.h"
#include "NeoException.h"
#include "QSortRand.h"
#include "CGenericProgress.h"
#include "EndianSwap.h"

void liget::EndianSwap()
{
    endian_swap((unsigned int&)latStringOffSet);
    endian_swap((unsigned int&)hitListFileOffset);
    fourByte.EndianSwap(); 
}

// ###########################################################################################
CGiant::CGiant()
{
    numLines = 0;
    table = NULL;
    stringBlock = NULL;
    grammaticalDescriptionIterator = -1;

    //  -------------------------------------------------------------------------------
    //  Read Giant table from the disk

    CDataFile       mainFile;
    long            mainFileLength;

    mainFile.OpenPath(DATA_FILE_PATH, GTOE_FILE);
    mainFileLength = mainFile.GetLength();
    numLines = mainFileLength / sizeof(liget);
    table = (liget*) mainFile.ReadAll();
#ifndef __BIG_ENDIAN__
    for (int i=0; i < numLines; i++) {
        table[i].EndianSwap();
    }
#endif
    mainFile.Close();

    //  -------------------------------------------------------------------------------
    //  Read the block of strings that accompany the table
    CDataFile   stringFile;
    
    stringFile.OpenPath(DATA_FILE_PATH, GTOE_STRING_FILE);
    stringBlock = (char*) stringFile.ReadAll();
    stringFile.Close();

    // reserve memory to use for expanding ambiguities
    unList = new CRoseCode[MAX_UNAMBIGUOUS_REPLACEMENTS];
    if (!unList)
        throw CFatalError("CGiant::CGiant 0");
    unCount = 0;
    
    checkDisambiguations = FALSE;       // what should default be? maybe saved in disamb file?
    checkAmbiguity = TRUE;
    SetSearchMode( CREATE_NEW, NULL, 0 );
    SetError(NO_SEARCH_ERROR);\
}

// ###########################################################################################
CGiant::~CGiant()
{

    if (unList)
        delete[] unList;
    if (stringBlock)
        delete[] (char*)stringBlock;
    if (table)
        delete[] (char*)table;
}

// ###########################################################################################
//  GetCode returns the four byte code in the giant table
//  It and the following functions are inline for speed  -- they are small enough
CRoseCode    CGiant::GetRoseCode(long tIndex)
{
    return(table[tIndex].fourByte);
}

// ###########################################################################################
inline unsigned short CGiant::GetTBase(long tIndex)
{
    return(table[tIndex].fourByte.GetStem());
}


// ###########################################################################################
//  GetString returns a pointer to a string in the giant table
char*   CGiant::GetString(long tIndex)
{
    if ( (tIndex < 0) || (tIndex > numLines) ) {
        throw CFatalError("CGiant::GetString -- Invalid tIndex");
    }
    return(  stringBlock + table[tIndex].latStringOffSet   );
}

// ###########################################################################################
inline Boolean  CGiant::IsAmb(long tIndex)
{
    return(GetRoseCode(tIndex).IsAmb());
}

// ###########################################################################################
//  Returns the disk offset at which the hit list for the word begins
inline long CGiant::GetDiskHitOffset(long tIndex)
{
    return( table[tIndex].hitListFileOffset  );
}

// ###########################################################################################
// Performs a binary search to find the requested string.
long    CGiant::GetIndex(const char* tString)
{
    long        lower, upper, mid, result;

    lower = 0;
    upper = numLines-1;

    while (lower <= upper) {
        mid = (upper + lower)/2;
        result = strcmp( GetString(mid), tString );
        if (result < 0) 
            lower = mid + 1;
        else if (result > 0)
            upper = mid - 1;
        else return mid;
    }
    return NOT_FOUND;
}

// ###########################################################################################
// Performs a binary search to find the requested string.
long    CGiant::GetFamilyIndex(const char* tString)
{
    long result = GetIndex(tString);
    if (result == NOT_FOUND) {
        unsigned short base = dictionary[tString];
        long j;
        for (j = 0; j < numLines; j++)
            if (base == GetTBase(j))                // doesn't yet check for external ambigs!
                break;
        if (j < numLines)
            result = j;
    }
    return result;
}

// ###########################################################################################
void    CGiant::SetSearchMode(int mode, CHitList *oldList, int proxim) {

    searchMode = (oldList != NULL?  mode : CREATE_NEW); // in case NULL list sent for intersect
    oldHitList = oldList;
    proximity = proxim;
}

// ###########################################################################################
CHitList*   CGiant::StartHitsBySearchMode(void) {
    CHitList        *theList;
    
    if (searchMode == CREATE_NEW)       // start new list or add to an old one?
        theList = new CHitList;
    else
        theList = oldHitList;
    if (searchMode == INTERSECT)        // the hit list should know if it'll be creating a separate
        theList->OpenIntersectMode(proximity);  // on-the-side intersection list to give back later

    return theList;
}

// ###########################################################################################
CHitList*   CGiant::FinishHitsBySearchMode(CHitList *doneList, Boolean moreThanOneAdded) {
    CHitList        *finalList;

    if (searchMode == INTERSECT) {
        finalList = doneList->CloseIntersectMode();
        delete doneList;
    } else
        finalList = doneList;
//  try {
        if ( (searchMode != CREATE_NEW) || moreThanOneAdded )
            finalList->SortHits();
//  } catch (CMemoryFullError) {
//      SignalPStr_("\pRan out of memory while sorting! Skipping this target...");
//      if (finalList) {
//          delete finalList;
//          finalList = NULL;
//      }
//  }
    
    return finalList;       
}

// ###########################################################################################
CHitList*   CGiant::FindHitsWithStringOf(CTarget theTarget) {
    long            foundAt;
    CHitList        *hitList;
    char            *tString;
    
    hitList = StartHitsBySearchMode();
        
    tString = theTarget.GetString();    
    foundAt = GetIndex(tString);
    if (foundAt != NOT_FOUND) {
        try {
            hitList->AddHits( GetDiskHitOffset(foundAt), NULL );
        } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                      delete hitList; throw m;}
    }   

    hitList = FinishHitsBySearchMode(hitList, FALSE);
    return hitList;
}

// ###########################################################################################
CHitList*   CGiant::FindHitsWithSubstringOf(CTarget theTarget) {
//  long            foundAt;
    CHitList        *hitList;
    char            *tString;
    long            j;
    int             hitSetsAdded = 0;
    
    hitList = StartHitsBySearchMode();
    tString = theTarget.GetString();    

    CGenericProgress        progressIndicator;  
    progressIndicator.Initialize(numLines/400); 

    for (j = 0; j < numLines; j++) {
        if (strstr( GetString(j), tString )) {      // if a substring
            try {
                hitList->AddHits( GetDiskHitOffset(j), NULL );
            } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                          delete hitList; throw m;}
            hitSetsAdded++;
        }
    }

    hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1));
    return hitList;
}

// ###########################################################################################
CHitList*   CGiant::FindHitsWithMatchOf(CTarget theTarget) {
//  long            foundAt;
    CHitList        *hitList;
    char            *tString;
    long            j;
    int             hitSetsAdded = 0;
    
    hitList = StartHitsBySearchMode();
    tString = theTarget.GetString();    

    for (j = 0; j < numLines; j++) {
        if (strmatch( GetString(j), tString )) {      // if a substring
            try {
                hitList->AddHits( GetDiskHitOffset(j), NULL );
            } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                          delete hitList; throw m;}
            hitSetsAdded++;
        }
    }

    hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1));
    return hitList;
}

// ###########################################################################################
//  This function takes a string as an argument, searches for it in the table,
//  and adds to the hit list all the strings with the same lemma.
CHitList*   CGiant::FindHitsWithLemmaOf(CTarget theTarget)
{
    int         j, k;
    CHitList    *hitList;
    unsigned short  lemma;
    CRoseCode   thisRoseCode, disAmbCode;
    hit         disAmbHit;
    long        foundAt;
    Boolean     atLeastOneIsValid;
    int         hitSetsAdded = 0;
    std::vector<hit>    *exceptions;
    char        *tString;
    
    hitList = StartHitsBySearchMode();

    tString = theTarget.GetString();    
    foundAt = GetFamilyIndex(tString);
    if (foundAt == NOT_FOUND) {
        hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) ); 
        return hitList;     // should be empty
    }

    if (GetRoseCode(foundAt).IsExtAmb()) {
//      cout << "SORRY, YOU CAN'T SEARCH FOR SOMETHING THAT'S EXTERNALLY AMBIGUOUS YET!\n";
//      cout << "(I DON'T KNOW WHICH BASE YOU WANT)        (hit return)\n";
//      getchar();
        SetError(EXT_AMB_SEARCH_ERROR);
        hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) ); 
        delete hitList;
        return NULL;
    }

    lemma = GetTBase(foundAt);
    if (lemma == 0) {
//      cout << "Sorry, this string was from the NON-PLACEABLES list and currently has no\n";
//      cout << "grammatical meaning.       (hit return)\n";
//      getchar();
        SetError(NON_PLACE_SEARCH_ERROR);
        hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) ); 
        delete hitList;
        return NULL;
    }
    
    for (j = 0; j < numLines; j++) {

        if (GetRoseCode(j).IsExtAmb()) {                // externally ambigous
            if (!(checkAmbiguity))                      // skip this line if checking turned off
                continue;
            MakeUnAmbList( GetRoseCode(j) );                
            atLeastOneIsValid = FALSE;
            exceptions = NULL;
            
            for (k = 0; k < unCount; k++)               // SCAN EXPANDED "MEANING" LIST
                if (unList[k].GetStem() == lemma) {
                    atLeastOneIsValid = TRUE;
                    break;
                }

            if (atLeastOneIsValid) {
                // We know that the ambiguos word can mean what we're looking for, but we must check
                // each disambiguation to see if that meaning doesn't match, and if so add that
                // location to the exceptions list
        // ***************** SECTION TEMPORARILY REMOVED FOR VERSION 1.0 ************************
/*              if (checkDisambiguations) {
                    thisRoseCode = GetRoseCode(j);
                    if (disAmbTbl.LookForAmbCode(thisRoseCode)) {
                        exceptions = StartHitExceptionList(); // hit list doesn't really care if list is empty
                        while ( disAmbTbl.GetNextDisCode(disAmbCode, disAmbHit) ) {
                            if (disAmbCode.GetStem() != lemma)
                                AddHitException(exceptions, disAmbHit);
                        }
                    }
                }
*/
        // ***************** SECTION TEMPORARILY REMOVED FOR VERSION 1.0 ************************

                try {
                    hitList->AddHits( GetDiskHitOffset(j), exceptions );
                    hitSetsAdded++;
                } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                              delete hitList; throw m;}
                    
            } // end if at least one is valid
        
        } else {                                        // no external ambiguity
            if (GetTBase(j) == lemma) {
                try {
                    hitList->AddHits( GetDiskHitOffset(j), NULL );
                    hitSetsAdded++;
                } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                              delete hitList; throw m;}
            }
        }
    }

    hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) ); 
    return hitList;
}



// ###########################################################################################
CHitList*   CGiant::FindHits(CTarget theTarget)
{
	// Here we decide what to do for this target, depending on
	// 1. whether a string was supplied
	// 2. whether that string has wildcards
	// 3. whether grammar was supplied
    if (!theTarget.DoesGrammarMatter()) {
		return FindHitsWithMatchOf(theTarget);
	}
	if (theTarget.DoesStemMatter() && strstr(theTarget.GetString(), "*")) {
		return FindHitsWithMatchAndGrammarOf(theTarget);
	} else {
		return FindHitsWithLemmaAndGrammarOf(theTarget);
	}
}

CHitList*   CGiant::FindHitsWithMatchAndGrammarOf(CTarget theTarget)
{
    int         j, k;
    CHitList*   hitList;
    unsigned short  lemma;
    CRoseCode   thisRoseCode, disAmbCode;
    hit         disAmbHit;
    long        foundAt;
    Boolean     atLeastOneIsValid, stringMatch;
    int         hitSetsAdded = 0;
    std::vector<hit> *exceptions = NULL;
    char        *tString;

    hitList = StartHitsBySearchMode();
    
    if (!theTarget.DoesStemMatter()) {
		SetError(SEARCH_ARGS_ERROR);
		hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
		delete hitList;
		return NULL;
	}

	tString = theTarget.GetString();
	if (!strstr(tString, "*")) {
		SetError(SEARCH_ARGS_ERROR);
		hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
		delete hitList;
		return NULL;
	}

    CGenericProgress        progressIndicator;  
    progressIndicator.Initialize(numLines/400); 
    
    for (j = 0; j < numLines; j++) {

        if (j%400 == 0) {
            if (progressIndicator.UpdateProgress(j/400) == TRUE) {  // search cancelled
                hitList = FinishHitsBySearchMode(hitList, FALSE);
                delete hitList;
                SetError(CANCEL_SEARCH_ERROR);
                return NULL;
            }
        }

        // skip ambiguity check

		if (theTarget.DoesMyGrammarMatchThis( GetRoseCode(j) ) && 
				strmatch( GetString(j), tString )) {
			try {
				hitList->AddHits( GetDiskHitOffset(j), NULL );
				hitSetsAdded++;
			} catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
										  delete hitList; throw m;}
		}
    }

    delete exceptions;
    hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
    return hitList;
}

CHitList*   CGiant::FindHitsWithLemmaAndGrammarOf(CTarget theTarget)
{
    int         j, k;
    CHitList*   hitList;
    unsigned short  lemma;
    CRoseCode   thisRoseCode, disAmbCode;
    hit         disAmbHit;
    long        foundAt;
    Boolean     atLeastOneIsValid;
    int         hitSetsAdded = 0;
    std::vector<hit> *exceptions = NULL;
    char        *tString;

    hitList = StartHitsBySearchMode();
    
    if (theTarget.DoesStemMatter()) {                   // set up the lemma code to be found
        tString = theTarget.GetString();                // (strip the string)
        foundAt = GetFamilyIndex(tString);
        if (foundAt == NOT_FOUND) {
            hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
            return hitList;     // should be empty
        }

        lemma = GetTBase(foundAt);
        if (lemma == 0) {
//          cout << "Sorry, this string was from the NON-PLACEABLES list and currently has no\n";
//          cout << "grammatical meaning.       (hit return)\n";
//          getchar();
            SetError(NON_PLACE_SEARCH_ERROR);
            hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
            delete hitList;
            return NULL;
        }

        if (GetRoseCode(foundAt).IsExtAmb()) {
//          cout << "SORRY, YOU CAN'T SEARCH FOR SOMETHING THAT'S EXTERNALLY AMBIGUOUS YET!\n";
//          cout << "(I DON'T KNOW WHICH BASE YOU WANT)        (hit return)\n";
//          getchar();
            SetError(EXT_AMB_SEARCH_ERROR);
            hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
            delete hitList;
            return NULL;
        }
        lemma = GetTBase(foundAt);
        theTarget.SetStemValue(lemma);
    }


    CGenericProgress        progressIndicator;  
    progressIndicator.Initialize(numLines/400); 

    
    for (j = 0; j < numLines; j++) {

        if (j%400 == 0) {
            if (progressIndicator.UpdateProgress(j/400) == TRUE) {  // search cancelled
                hitList = FinishHitsBySearchMode(hitList, FALSE);
                delete hitList;
                SetError(CANCEL_SEARCH_ERROR);
                return NULL;
            }
        }

        if (GetRoseCode(j).IsAmb()) {                   // ambigous

            if (!(checkAmbiguity))                      // skip this line if checking turned off
                continue;

            MakeUnAmbList( GetRoseCode(j) );                
            atLeastOneIsValid = FALSE;
            if (exceptions) {
                delete exceptions;
                exceptions = NULL;
            }
            
            for (k = 0; k < unCount; k++)               // SCAN EXPANDED "MEANING" LIST
                if (theTarget.DoesMyGrammarMatchThis( unList[k] ) ) {
                    atLeastOneIsValid = TRUE;
                    break;
                }

            if (atLeastOneIsValid) {
                // We know that the ambiguos word can mean what we're looking for, but we must check
                // each disambiguation to see if that meaning doesn't match, and if so add that
                // location to the exceptions list
        // ***************** SECTION TEMPORARILY REMOVED FOR VERSION 1.0 ************************
/*              if (checkDisambiguations) {
                    thisRoseCode = GetRoseCode(j);
                    if (disAmbTbl.LookForAmbCode(thisRoseCode)) {
                        exceptions = StartHitExceptionList(); // hit list doesn't really care if list is empty
                        while ( disAmbTbl.GetNextDisCode(disAmbCode, disAmbHit) ) {
                            if (!(theTarget.DoesMyGrammarMatchThis( disAmbCode )))
                                AddHitException(exceptions, disAmbHit);
                        }
                    }
                }
*/
        // ***************** SECTION TEMPORARILY REMOVED FOR VERSION 1.0 ************************

                try {
                    hitList->AddHits( GetDiskHitOffset(j), exceptions );
                    hitSetsAdded++;
                } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                              delete hitList; throw m;}
            }
        
        } else {                                        // no ambiguity
            if (theTarget.DoesMyGrammarMatchThis( GetRoseCode(j) )) {
                try {
                    hitList->AddHits( GetDiskHitOffset(j), NULL );
                    hitSetsAdded++;
                } catch (CMemoryFullError m) {hitList = FinishHitsBySearchMode(hitList, FALSE);
                                              delete hitList; throw m;}
            }
        }
    }

    delete exceptions;
    hitList = FinishHitsBySearchMode(hitList, (hitSetsAdded > 1) );
    return hitList;
}


// ###########################################################################################
// This function looks at an ambigous code and expands it into all possible unambigous
// replacements by doing successive lookups in the 3 ambiguity tables.
// The final list is stored in the array unList[], a member of CGiant.
void    CGiant::MakeUnAmbList(CRoseCode ambCode)
{
    unList[0]       = ambCode;
    unCount         = 1;
    int j           = 0;    

    do {                        // work on each line in list until complete
                                // put first new code in place of old code, others at end of list
        if (unList[j].IsExtAmb())  {
            extAmbTbl.FirstReplacement(unList[j]);
            while (extAmbTbl.NextReplacement( unList[unCount]) )
                unCount++;
        }
        
        if (unList[j].IsMidAmb()) {
            midAmbTbl.FirstReplacement(unList[j]);      
            while (midAmbTbl.NextReplacement( unList[unCount]) )
                unCount++;
        }

        if (unList[j].IsIntAmb()) {
            intAmbTbl.FirstReplacement(unList[j]);
            while (intAmbTbl.NextReplacement( unList[unCount]) )
                unCount++;
        }
    
        j++;
    } while (j < unCount);
}

// ###########################################################################################
void    CGiant::SetHitListOffset(long tIndex, long offset) {
    if ((tIndex < 0) || (tIndex >= numLines)) {
        throw CFatalError("Bad Index: CGiant::SetHitListOffset");
//      cout << "Bad Index: CGiant::SetHitListOffset\n";
//      return;
    }
    table[tIndex].hitListFileOffset = offset;
}


// ###########################################################################################
// Write whole GTOE to disk (used after indexer sets hitlist offsets)
void    CGiant::WriteAllInfo(void) {
    CDataFile       mainFile;

    mainFile.OpenPath(DATA_FILE_PATH, GTOE_FILE, "wb");
    mainFile.WriteAll((char*)table, numLines * sizeof(liget));
    mainFile.Close();
}

// ###########################################################################################
void    CGiant::ZeroAllOffsets(void) {
    long    j;
    for (j = 0; j < numLines; j++)
        SetHitListOffset(j, 0);
}

// ###########################################################################################
void    CGiant::SetDisambiguationChecking(Boolean setting) {
    checkDisambiguations = setting;
}

// ###########################################################################################
void    CGiant::SetAmbiguityChecking(Boolean setting) {
    checkAmbiguity = setting;
}

// ###########################################################################################
std::vector<hit>*   CGiant::StartHitExceptionList(void) {
    return new std::vector<hit>;
}

// ###########################################################################################
void    CGiant::AddHitException(std::vector<hit> &theList, hit skipMe) {
    theList.push_back(skipMe);
}

// ###########################################################################################
char* CGiant::LookupDictionaryFormOfWord(char* word)
{
    long    index = GetIndex(word);
    if (index == NOT_FOUND) return NULL;

    unsigned short base = GetTBase( index );
    
    return dictionary[base];
}

// ###########################################################################################

int CGiant::InitializeGrammaticalDescriber(const char* word)
{
    long    index = GetIndex(word);
    if (index == NOT_FOUND)
        return 0;

    MakeUnAmbList( GetRoseCode(index) );
    grammaticalDescriptionIterator = 0;     // will iterate through unamblist through uncount
    // sort the list so it shows up in a nice order
    qsortrand(unList, unCount, sizeof(unList[0]), CRoseCode::CompareRoses);
    return unCount;
}

// ###########################################################################################
Boolean CGiant::GetNextGrammaticalDescription(char* description)
{
    if (grammaticalDescriptionIterator >= unCount)
        return FALSE;
        
    CRoseCode       rCode = unList[grammaticalDescriptionIterator++];
    
    description[0] = '\0';
    for (int j = 1; j <= 4; j++) {
        const char* text = rCode.DescribeGrammar(j);
        if (text[0] != '\0') {
            if (description[0] != '\0')     // if this isn't the first word
                strcat(description, ", ");
            strcat(description, text);
        }
    }
    unsigned short  base = rCode.GetStem();
    char*   dictionaryEntry = dictionary[base];
    strcat(description, " < ");
    strcat(description, (dictionaryEntry != NULL ? dictionaryEntry : "DICTIONARY_ENTRY_MISSING"));

    return TRUE;
}

// ###########################################################################################

