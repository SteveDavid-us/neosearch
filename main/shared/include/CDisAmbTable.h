// *******************************************************************************************
// CDisAmbTable.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Header for the table of user disambiguations.
//
// Used by the GTOE only.
// *******************************************************************************************

#ifndef _CDISAMBTABLE_H_
#define _CDISAMBTABLE_H_

// ###########################################################################################
#include <Types.h>
#include "CDataFile.h"
#include "CRoseCode.h"
#include "CHitList.h"

// ###########################################################################################
//  Structure for each line in table -- Represents one disambiguation
struct disAmbLine {
    CRoseCode       ambCode;        // ambiguous form
    CRoseCode       disCode;        // user-chosen non-ambigous replacement form
    hit             spot;           // specific location of disambiguation
};

// ###########################################################################################
class CDisAmbTable {

    long            disCount;       // # of lines
    disAmbLine      *disTable;      // pointer to table array
    CDataFile       disFile;
    
    CRoseCode       currentCode;    // ambiguous code being used by iterator
    long            iLine;          // iterator current line #
    
public:

    CDisAmbTable();
    ~CDisAmbTable();
    
    void        AddNewLine(CRoseCode ambCode, CRoseCode disCode, hit newSpot);  
    Boolean     LookForAmbCode(CRoseCode findMe);
    Boolean     GetNextDisCode(CRoseCode& fixMe, hit& mySpot);  
};

// ###########################################################################################

#endif // _CDISAMBTABLE_H_

