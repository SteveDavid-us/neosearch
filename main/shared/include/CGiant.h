// *******************************************************************************************
// CGiant.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Prototype for the Giant Table of Equivalents Class
// This is the biggie.
//
// CGiant is instantiated once by the application and used for all searching, etc...
// Each line of this table is of type "liget" (see the struct definition).
// *******************************************************************************************

#ifndef _CGIANT_H_
#define _CGIANT_H_

// ###########################################################################################
#include <Types.h>
#include "CRoseCode.h"
#include "CAmbiguity.h"
#include "CDisAmbTable.h"
#include "CHitList.h"
#include "CTarget.h"
#include "CDictionary.h"


enum {NO_SEARCH_ERROR, EXT_AMB_SEARCH_ERROR, NON_PLACE_SEARCH_ERROR,
	CANCEL_SEARCH_ERROR, SEARCH_ARGS_ERROR};


// ###########################################################################################
// LIGET = Line-In-Giant-Equivalence-Table
class liget {

public:
    CRoseCode       fourByte;
    long            latStringOffSet;
    long            hitListFileOffset;
    void            EndianSwap();
};
// fourByte         =   The four byte grammatical code.
// latStringOffSet  =   A location offset within the huge block of strings in memory.
//                      Add this to the block's base address to find yourself a string.
// hitListFileOffset=   A location offset within the hit list file on disk.  The
//                      searching methods will send this value to the AddHits() method of 
//                      the hit list class.

// ###########################################################################################
class CGiant {

protected:
    // INSTANCE VARIABLES
    // ==================
    liget               *table;             // the giant table itself
    char                *stringBlock;       // huge block containing strings
    long                numLines;

    CRoseCode           *unList;            // temporary list of unambiguous expanded ambiguities
    int                 unCount;            // # of above unambiguos codes
    Boolean             checkDisambiguations;
    Boolean             checkAmbiguity;

    long                exceptionListSize;  // used to keep track of Handle size for StartHitException and AddHitException
    long                exceptionListPos;   // where in the list are we now?
    
    int                 searchMode;         // CREATE_NEW / UNION / INTERSECT
    int                 proximity;
    CHitList            *oldHitList;        // stores hit list to intersect or union with
        
    CExtAmbTable        extAmbTbl;          // The ambiguity tables are managed solely by the GTOE,
    CMidAmbTable        midAmbTbl;          // which provides all access to them.
    CIntAmbTable        intAmbTbl;
//  CDisAmbTable        disAmbTbl;          // <-- remove in this version
    
    int                 errorCode;          // marks an error state
    
    CDictionary         dictionary;
    int                 grammaticalDescriptionIterator;
        
    // CLASS METHODS
    // =============
    long        GetDiskHitOffset(long tIndex);
    void        MakeUnAmbList(CRoseCode ambCode);
    std::vector<hit>* StartHitExceptionList(void);      // for disambiguated words to exclude
    void        AddHitException(std::vector<hit> &theList, hit skipMe);
    CHitList*   StartHitsBySearchMode(void);        // called at start of each search
    CHitList*   FinishHitsBySearchMode(CHitList *doneList, Boolean moreThanOneAdded);
    void        SetError(int err) {errorCode = err;}

//  char*           LookupDictionaryEntryForBase(unsigned short theBase) {return dictionary[theBase];}
    char*           LookupDictionaryFormOfWord(char* word);

public:

    CGiant();
    ~CGiant();
    void            SetDisambiguationChecking(Boolean setting);
    void            SetAmbiguityChecking(Boolean setting);

    // METHODS TO RETURN SIMPLE VALUES IN TABLE
    CRoseCode       GetRoseCode(long tIndex);
    unsigned short  GetTBase(long tIndex);          // return base code from the RoseCode
    char*           GetString(long tIndex);         // return pointer to a string at that index
    Boolean         IsAmb(long tIndex);             // Is it ambiguous?
    long            GetIndex(const char* tString);  // What's the index of this string?
    long            GetFamilyIndex(const char* tString); // more powerful form of GetIndex that works
                                                    // if you send it a dictionary word ! in table

    // PUBLIC SEARCH METHODS
    void            SetSearchMode(int mode, CHitList *oldList, int proxim); 
    CHitList*       FindHits(CTarget theTarget);
    CHitList*       FindHitsWithStringOf(CTarget theTarget);
    CHitList*       FindHitsWithSubstringOf(CTarget theTarget);
    CHitList*       FindHitsWithMatchOf(CTarget theTarget);
    CHitList*       FindHitsWithLemmaOf(CTarget theTarget);
    CHitList*       FindHitsWithLemmaAndGrammarOf(CTarget theTarget);
    CHitList*       FindHitsWithMatchAndGrammarOf(CTarget theTarget);
    int             CheckForError() {int e = errorCode; errorCode = NO_SEARCH_ERROR; return e;}
    
    // DICTIONARY
    int             InitializeGrammaticalDescriber(const char* word);
    Boolean         GetNextGrammaticalDescription(char* description);
    
    // SPECIAL METHODS FOR INDEXING ONLY
    void            ZeroAllOffsets(void);
    void            SetHitListOffset(long tIndex, long offset);
    void            WriteAllInfo(void);             // save entire table to file
};
// ###########################################################################################

#endif // _CGIANT_H_

