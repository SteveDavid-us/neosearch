// *******************************************************************************************
// CDictionary.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// *******************************************************************************************

#ifndef _CDICTIONARY_H_
#define _CDICTIONARY_H_

// ###########################################################################################
//#include <Types.h>
//#include "CRoseCode.h"
//#include "CAmbiguity.h"
//#include "CDisAmbTable.h"
//#include "CHitList.h"
//#include "CTarget.h"

//enum {NO_SEARCH_ERROR, EXT_AMB_SEARCH_ERROR, NON_PLACE_SEARCH_ERROR, CANCEL_SEARCH_ERROR};

// ###########################################################################################
struct DictionaryLine {
    unsigned short  baseNum;
    long            stringOffset;        // offset in block of text in memory
};

// ###########################################################################################
class CDictionary {
protected:
    DictionaryLine      *dictionaryData;
    char                *dictionaryStringBlock;
    long                numLines;
    
    char*               GetString(long index) {return dictionaryStringBlock +
                                                 dictionaryData[index].stringOffset;}
    unsigned short      GetBase(long index) {return dictionaryData[index].baseNum;}
    
public:
    CDictionary();
    ~CDictionary();
    
    char*   operator[](unsigned short whichBase);       // look up a word from a base
    unsigned short operator[](const char* whichString); // look up a base from a word   

};

#endif // _CDICTIONARY_H_

