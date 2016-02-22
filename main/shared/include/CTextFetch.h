// *******************************************************************************************
// CTextFetch.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Class to retrieve encoded text from disk and to return book titles.
//
// Needed by GUI text display procedure.
// Also used by indexer and wordcounter to get the names of the files on disk.
// *******************************************************************************************

#ifndef _CTEXTFETCH_H_
#define _CTEXTFETCH_H_


#include "Engine.h"
#include "CDataFile.h"

// ###########################################################################################
struct bookInfo {
    char    volFile[30];                // filename on disk
    char    volName[70];                // full book name
};

// ###########################################################################################
class CTextFetch {

    bookInfo    book[MAX_VOLUMES];
    short       previousVol; 
    CDataFile   theBook;            // file of most recent volume opened

public:

    static const unsigned char BookOrder[MAX_VOLUMES];
    CTextFetch();
    ~CTextFetch();
    int*        ReadPassage(short vol, short pas, long& length);
    char*       GetBookName(short vol);
    char*       GetBookFile(short vol);
    int         GetHighestPassageNumber(short vol);
    int         GetLowestPassageNumber(short vol);
};

// ###########################################################################################
#endif // _CTEXTFETCH_H_

