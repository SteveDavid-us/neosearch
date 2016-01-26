// *******************************************************************************************
// CTextFetch.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// *******************************************************************************************

#include "CTextFetch.h"
//#include "CDataFile.h"
#include "NeoException.h"
#include <string.h>
#include <iostream>

using namespace std;

// ###########################################################################################
CTextFetch::CTextFetch() {

    CDataFile       bookList;
    char            line[150];
    int             j, pos;
    long int        len;
    
    bookList.OpenPath(DATA_FILE_PATH, BOOK_LIST_FILE, "r");

    j = 0;
    while ((j < MAX_VOLUMES) && (bookList.GetLine(line, 150))) {

        for (pos=0; (line[pos] != ' ') && (line[pos] != '\t'); pos++); // to end 1st word
        memcpy(book[j].volFile, line, pos);
        book[j].volFile[pos] = '\0';

        while ( (line[pos] == ' ') || (line[pos] == '\t') )         // to start 2nd word
            pos++;
        strcpy(book[j].volName, line + pos);
        len = strlen(line + pos);
        (book[j].volName)[len - 1] = '\0';                          // eliminate newline 

        j++;
    }

    bookList.Close();
    previousVol = -1;
}

// ###########################################################################################
CTextFetch::~CTextFetch()
{
    if (theBook.IsOpen())
        theBook.Close();
}

// ###########################################################################################
// The last long in the data file is the number of passages.  Preceeding that is a backwards
// array of passage location offsets.
int*   CTextFetch::ReadPassage(short vol, short pas, long& length) {
#if 0
    cerr << "CTextFetch::ReadPassage(vol=" << vol << ", pas=" << pas <<", length="<< length << 
        ")" << endl; 
#endif

    long            maxPassage, startAt, endAt;
    int             *p;
    short           tempPas;
    
    length = 0;                                     // in case we fail & exit
    
    if (vol != previousVol) {
        if (theBook.IsOpen())
            theBook.Close();
        previousVol = vol;
        theBook.OpenPath(BOOK_FILE_PATH, book[vol].volFile);
    }
    theBook.Seek(-sizeof(int), SEEK_END);          // jump to last long in file
    maxPassage = theBook.GetInt();
    if ((pas > maxPassage) || (pas < 0))            // error checking: passage too big?
        {/*theBook.Close();*/ return NULL;}
        
    theBook.Seek(-(pas+2)*sizeof(int), SEEK_END);  // jump to passage array index
    startAt = theBook.GetInt();
    
    if (startAt == PASSAGE_OFFSET_BLANK)
      {/*theBook.Close();*/ return NULL;}

    if (startAt < 0) 
        throw CFatalError("CTextFetch::ReadPassage 0");
    
    if (pas == maxPassage)
        endAt = theBook.WhereAmI() - sizeof(int);  // this is the last passage index -- the text lies before it
    else {
        theBook.Seek(-(pas+3)*sizeof(int), SEEK_END);  // find next passage start for end of this
        endAt = theBook.GetInt();

        tempPas = (pas+3);
        while (endAt==PASSAGE_OFFSET_BLANK) {   // the next number was MISSING when indexed, so we need to look farther
            theBook.Seek(-(++tempPas)*sizeof(int), SEEK_END);
            endAt = theBook.GetInt();
        }
        if (tempPas-3 >= maxPassage)    // the LAST FEW #'S IN THE BOOK were missing, so we hit
                                        // then end without finding a valid offset
            endAt = theBook.WhereAmI() - sizeof(int);
    }   

    length = endAt - startAt;
    if (length < 0) 
        throw CFatalError("CTextFetch::ReadPassage 1");
    theBook.Seek(startAt);
    p = (int*)theBook.ReadChunk(length);
//  theBook.Close();
    return p;
}


// ###########################################################################################
char*   CTextFetch::GetBookName(short vol) {
    return book[vol].volName;
}

// ###########################################################################################
char*   CTextFetch::GetBookFile(short vol) {
    return book[vol].volFile;
}

// ###########################################################################################
int CTextFetch::GetHighestPassageNumber(short vol)
{
    if (vol != previousVol) {
        if (theBook.IsOpen())
            theBook.Close();
        previousVol = vol;
        theBook.OpenPath(BOOK_FILE_PATH, book[vol].volFile);
    }
    theBook.Seek(-sizeof(int), SEEK_END);          // jump to last long in file
    return theBook.GetInt();
}
        
// ###########################################################################################
int CTextFetch::GetLowestPassageNumber(short vol)
{
    return 0;       // change this later to store lowest passage
}

// ###########################################################################################
