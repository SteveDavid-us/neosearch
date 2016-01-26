// *******************************************************************************************
// CHitList.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// *******************************************************************************************
// This file contains code for:
//      CHitMemoryStream
//      CHitList


#include "CHitList.h"
#include "Engine.h"
#include "CDataFile.h"
#include "NeoException.h"
#include "QSortRand.h"
#include <stdlib.h>
#include <string.h>
//#include <iostream.h>


int compareHits(const void *h1, const void *h2);

// ###########################################################################################
// ###########################################################################################
// ###########################################################################################
// ########################     CHITMEMORYSTREAM METHODS      ################################
// ###########################################################################################
// ###########################################################################################
// ###########################################################################################

CHitMemoryStream::CHitMemoryStream() {
    theData = new short[HIT_CHAIN_START_SIZE];
    handleSize = HIT_CHAIN_START_SIZE;
    ResetPosition();
    totalHits = totalShorts = 0;
}

// ###########################################################################################
CHitMemoryStream::~CHitMemoryStream() {
    delete[] theData;
}

// ###########################################################################################
void    CHitMemoryStream::Reset(void) {
//  if (theData) {
        delete[] theData;
        theData = new short[HIT_CHAIN_START_SIZE];
//    }
    handleSize = HIT_CHAIN_START_SIZE;
    ResetPosition();
    totalHits = totalShorts = 0;
}

// ###########################################################################################
void    CHitMemoryStream::ResetPosition(void) {
    iPos = 0;
    iMovingNextWards = TRUE;
    lastPas = -30000;   // something iterator won't find therefore next passage is new passage
}

// ###########################################################################################
Boolean CHitMemoryStream::WriteHit(short pas, short wrd) {
    Boolean     memFullFlag = FALSE;
    short       p = 0 - pas;                // store passages in memory as negative

    if ((totalShorts + 2) * sizeof(short) >= handleSize)
        handleSize = AttemptGrow(memFullFlag);

    if (lastPas != p) {
        theData[totalShorts++] = p;
        lastPas = p;
    }
    theData[totalShorts++] = wrd;
    totalHits++;

    return memFullFlag;
}

// ###########################################################################################
Boolean CHitMemoryStream::ReadNextHit(short& pas, short& wrd) {
    short   info;

    if (!(iMovingNextWards)) {
        iMovingNextWards = TRUE;        // turn around!
        iPos += 2;
    }

    if (iPos >= totalShorts)            // end of list!
        return FALSE;
        
    info = theData[iPos++];
    if (info <= 0) {
        lastPas = info;
        info = theData[iPos++];
    }
    pas = 0 - lastPas;
    wrd = info;

    return TRUE;
}

// ###########################################################################################
Boolean CHitMemoryStream::ReadNextPassageHit(short& pas, short& wrd) {
    short info;

    if (!(iMovingNextWards)) {
        iMovingNextWards = TRUE;        // turn around!
        iPos += 2;
    }

    do {                                // go until new passage
        if (iPos >= totalShorts)
            return FALSE;
        info = theData[iPos++];
    } while (info > 0);

    lastPas = info;
    info = theData[iPos++];         // get the word that follows

    pas = 0 - lastPas;
    wrd = info;
    return TRUE;
}

// ###########################################################################################
Boolean CHitMemoryStream::ReadPreviousHit(short& pas, short& wrd) {
    short       info;
    long        rememberPos;
    
    if (iMovingNextWards) {
        iMovingNextWards = FALSE;       // turn around!
        iPos -= 2;
    }

    if (iPos < 1)                       // hit front of this list
        return FALSE;
    
    info = theData[iPos--];
    if (info <= 0) {                    // this is the current passage marker --
        rememberPos = iPos;             // we must retrace to find last one
                        
        do {                            // backtrack to find what passage we're in
            info = theData[iPos--];
        } while (info > 0);

        lastPas = info;
        iPos = rememberPos;             // restore position
        info = theData[iPos--];     // get the word that follows
    }

    pas = 0 - lastPas;
    wrd = info;
    return TRUE;
}

// ###########################################################################################
Boolean CHitMemoryStream::ReadPreviousPassageHit(short& pas, short& wrd) {
    short       info;
    long        rememberPos;
    
    if (iMovingNextWards) { 
        iMovingNextWards = FALSE;       // turn around!
        iPos -= 2;
    }
    if (iPos < 1)
        return FALSE;

    rememberPos = iPos;
        
    do {                                // move to start of this passage
        info = theData[iPos--];
    } while (info > 0);
    
    if (iPos < 1) {
        iPos = rememberPos;
        return FALSE;                   // moved to front this passage, discovered it's the 1st
    }
        
    rememberPos = iPos;
                                        // backtrack to find what passage we're in
    do {
        info = theData[iPos--];
    } while (info > 0);

    lastPas = info;
    iPos = rememberPos;                 // restore position
    info = theData[iPos--];         // get the word that follows

    pas = 0 - lastPas;
    wrd = info;
    return TRUE;
}

// ###########################################################################################
void    CHitMemoryStream::PushPosition(void) {
    iStoredPos = iPos - 1;              // of last thing read (only works moving forward?)
    storedLastPas = lastPas;
}

// ###########################################################################################
void    CHitMemoryStream::PopPosition(void) {
    iPos = iStoredPos;
    lastPas = storedLastPas;
}

// ###########################################################################################
long    CHitMemoryStream::GetTotalHits(void) {
    return totalHits;
}

// ###########################################################################################
long    CHitMemoryStream::AttemptGrow(Boolean& memFullFlag) {
    long    newSize;
    long    oldSize = handleSize;
    static Boolean returnedMemoryFullErrorLastTime = FALSE;
    short *newData;
        

    newSize = oldSize * 2 + 10;             // try to double size (add a bit in case it's 0)
    newData = new short[newSize];
    if (!newData) {
        throw CMemoryFullError("CHitMemoryStream::AttemptGrow: NO MEMORY LEFT IN MEMORY STREAM!");
    }
    int i;
    for (i = 0; i < oldSize; ++i) {
        newData[i] = theData[i];
    }
    for (; i < newSize; i++) {
        newData[i] = 0;
    }
    delete[] theData;
    theData = newData;


//  SetHandleSize((Handle)theData, newSize);
//  if (MemError() == memFullErr) {

//      newSize = (float)oldSize * 1.1 + 10;  // add a bit in case it's 0
//      SetHandleSize((Handle)theData, newSize);
//      if (MemError() == memFullErr) {
             
//          memFullFlag = TRUE;             // tells the caller to try to free up memory
//          newSize = oldSize + 10;         // couldn't double, just add a bit
//          SetHandleSize((Handle)theData, newSize);
            // quit if we're 10% away from being full on this stream, after shrinking streams once
//          if ((MemError() == memFullErr) || (returnedMemoryFullErrorLastTime))
//              throw CMemoryFullError("CHitMemoryStream::AttemptGrow: NO MEMORY LEFT IN MEMORY STREAM!");
//      }
//  }
    returnedMemoryFullErrorLastTime = memFullFlag;
    return newSize;
}

// ###########################################################################################
void    CHitMemoryStream::ShrinkToMinimum(void) {

    handleSize = totalShorts * sizeof(short);
    short *newData = new short[handleSize];
    if (!newData) {
        throw CMemoryFullError("CHitMemoryStream::AttemptGrow: NO MEMORY LEFT IN MEMORY STREAM!");
    }
    int i;
    for (i = 0; i < handleSize; i++) {
        newData[i] = theData[i];
    }
    delete[] theData;
    theData = newData;
}

// ###########################################################################################
void    CHitMemoryStream::SortHits(void) {
    long            j;
    expanded*       tempChain;
    expanded        tempItem;
    long            outpos;
    
    // need to expand line for sorting, then contract...
    tempChain = new expanded [totalHits * sizeof(expanded)];
    if (tempChain==NULL)
        throw CMemoryFullError("CHitMemoryStream::SortHits: SORT MEMORY FULL!");

    ResetPosition();
    for (j = 0; j < totalHits; j++) {
        ReadNextHit( tempItem.passage, tempItem.word );
        tempChain[j] = tempItem;
    }       
    outpos = j;
    
    qsortrand(tempChain, totalHits, sizeof(expanded), compareHits);
    
    // Time to shrink the line back into place
    Reset();            // getting rid of old list completely (consider not shrinking memory to save time)
    tempItem.passage = tempItem.word = -1;      // set weird values so there isn't leftover from reading
    for (j = 0; j < outpos; j++)
    {
        if (!( tempItem == tempChain[j] )) {    // skip repeats
            tempItem = tempChain[j];
            WriteHit( tempItem.passage, tempItem.word );    // make this faster (no reset)
        }
    }       

    ShrinkToMinimum();
    delete[] tempChain;
}

// ###########################################################################################
// ###########################################################################################
// ###########################################################################################
// ############################     CHITLIST METHODS      ####################################
// ###########################################################################################
// ###########################################################################################
// ###########################################################################################

CHitList::CHitList()
{
    hitFile.OpenPath(DATA_FILE_PATH, HIT_LIST_FILE);
    
    intersectModeOn = FALSE;
    intersectList = NULL;
}

// ###########################################################################################
CHitList::~CHitList()
{
    if (intersectList)
        delete intersectList;
    hitFile.Close();
}

// ###########################################################################################
void CHitList::ResetList(void)
{
    short   v;
    for (v = 0; v < MAX_VOLUMES; v++)
        volumeH[v].Reset();

    intersectModeOn = FALSE;
    intersectList = NULL;
}

// ###########################################################################################
void    CHitList::OpenIntersectMode(int proximity) {
    hProximity = proximity;
    intersectModeOn = TRUE;
    intersectList = new CHitList;
    if (!intersectList)
        throw CFatalError("CHitList::OpenIntersectMode 0");
}

// ###########################################################################################
CHitList*   CHitList::CloseIntersectMode(void) {
    CHitList        *theList;

    intersectModeOn = FALSE;
    theList = intersectList;
    intersectList = NULL;   // be sure to set this to NULL so the destructor doesn't kill it
    return theList;
}

// ###########################################################################################
//  The function addHits gets called by one of the table searching methods, which
//  sends the file offset pointer stored in the table.  This function then loads the
//  corresponding string of hits from disk into ram, allocating a handle if the
//  volume has had no previous hits.
//  The exceptions handle can point to a list of volumes/offsets to EXCLUDE because
//  the word has been disambiguated there (skip them when reading from disk).
void    CHitList::AddHits(long int diskOffset, std::vector<hit> *exceptions)
{
    short       vol, pas, s;
    CHitList    *destination;
    Boolean     memFull;
    Boolean     writeWord = TRUE;

    if (intersectModeOn)                // if we're intersecting, it has to go to the seconary list
        destination = intersectList;
    else                                // otherwise new hits just go to this object itself
        destination = this;
            
    vol = -1;
    hitFile.Seek(diskOffset);
    
    while (s = hitFile.GetShort()) {                // stop for a /0
        if (s>0) {                                  // ****** WORD ******* 
            if (exceptions)
                writeWord = CheckForException(*exceptions, vol, pas, s);

            if (writeWord) {
                if (intersectModeOn)
                    WriteThemIfTheyIntersect(destination, vol, pas, s);
                else {
                    memFull = destination->volumeH[vol].WriteHit(pas, s);
                    if (memFull)
                        destination->ShrinkAllStreams();
                }
            }

        } else
        if (s > PASSAGE_LIMIT) {                    // ****** VOLUME *******
            vol = 0-s -1;           // Range on disk: 1->MAX.  Range in mem: 0->(MAX-1)
            if (vol > MAX_VOLUMES) 
                throw CFatalError("CHitList::AddHits 0");
            if (intersectModeOn) {  // we're writing to a separate list so we can reset the read
                ResetVolume(vol);   // volume to save time in WriteThemIfTheyIntersect()
//              if (!(ReportNextHit(ihit)))
//                  ihit.BecomeLastHit();           // get the first hit ready to look at
            }
        }
        else {                                      // ****** PASSAGE ******* 
            pas = 0-(s - PASSAGE_LIMIT);            // now it's a straight positive
        }

    }

    delete exceptions;
}

// ###########################################################################################
Boolean CHitList::CheckForException(std::vector<hit> &exceptions, short vol, short pas, short wrd) {
    unsigned    excepPos;
    hit         tempHit;

    tempHit.Set(vol, pas, wrd);
    
    for (excepPos = 0; excepPos < exceptions.size(); excepPos++)
        if (tempHit == exceptions[excepPos]) 
            return FALSE;
            
    return TRUE;
}

// ###########################################################################################
// This function uses the hit iterator, so it remembers where it is within the
// hits for the current volume -- it doesn't have to retrace from the start every time it
// compares a new hit.
// (The hits already here should be in sorted order, as should the current group coming from
//  AddHits()).
void CHitList::WriteThemIfTheyIntersect(CHitList *destination, short vol, short pas, short wrd) {
    hit         diskHit;
    diskHit.Set(vol, pas, wrd);
    Boolean     anyWritten, memFull;
    int         distance;
    
//  static HitBitArray  bitArray;       // remembers words we've added so duplicates won't get added    
    
    do {
        if (!(ReportNextHit(ihit)))
            return;
    } while (ihit < diskHit);
    
    if (ihit > diskHit) {       // we've gone too far -- same passage not found
        ReportPreviousHit(ihit);    // back up one, so next time we get the same one we just read
        return;     
    }

//  while ( (ihit < diskHit) && !(ihit.LastHit()) )     // go until >= target hit
//      if (!(ReportNextHit(ihit))) return;
//  if ( ihit.LastHit() || (ihit > diskHit) )           // we've gone too far -- not found
//      return;

    // ---------------- both hits are in the same passage ----------------
    volumeH[vol].PushPosition();    // remembers last hit read (pop then read gets same one)
    
    // we're gonna add duplicate words if this same passage matches next time, but they'll be removed
    // during the sort

    anyWritten = FALSE;
    Boolean endReached = FALSE;
    do {
        distance = abs( diskHit.word - ihit.word );
        if ( (distance > 0) && ((hProximity == 0) || (distance <= hProximity)) ) {
            if (!(bitArray.LookUpWord(ihit))) {     // only write if we haven't already
                memFull = destination->volumeH[vol].WriteHit( ihit.passage, ihit.word );
                if (memFull)
                    destination->ShrinkAllStreams();

                if (hProximity > 0)         // add other hits to side in case of triple+ searches 
                    WriteLateralProximityHits(destination);
            }
            anyWritten = TRUE;      // written w/ intent even if not actually
        }

        if (!ReportNextHit(ihit))
            endReached = TRUE;
    } while ( (!endReached) && (ihit.SamePassageAs(diskHit)) );

    if (anyWritten) {                                   // write the hit we're intersecting
        memFull = destination->volumeH[vol].WriteHit( pas, wrd );
        if (memFull)
            destination->ShrinkAllStreams();
    }

//  if (ihit.LastHit())
//      ihit.volume = vol;              // restore volume number if end was reached
    volumeH[vol].PopPosition();         // restore iterator to start of this passage for next pass
//  if (!ReportNextHit(ihit)) ihit.BecomeLastHit();
}

// ###########################################################################################
// Useful for intersection searches of 3 or more words.
// If you have a group of words all in proximity, and you intersect a new word that lies just
// outside them all, you would just add the new word and whichever of the original group it's
// close enough to.  This function tries to keep the whole group in there, by scanning along
// from the new hit, keeping everything that's within proximity of something else that's close
// enough (proximity by association)

void CHitList::WriteLateralProximityHits(CHitList* destination)
{
    Boolean memFull;
    hit tempHit = ihit; 
    short   lastWordOffset = ihit.word;     
                                                    // search backward
    while ((ReportPreviousHit(ihit)) && (ihit.SamePassageAs(tempHit))) {
        if ((lastWordOffset - ihit.word <= hProximity) && (!(bitArray.LookUpWord(ihit)))) {
            memFull = destination->volumeH[ihit.volume].WriteHit(ihit.passage, ihit.word);
            if (memFull) destination->ShrinkAllStreams();
            lastWordOffset = ihit.word;
        } else
            break;                
    }
    while (!(tempHit == ihit))                      // get back to center
        ReportNextHit(ihit);

    lastWordOffset = ihit.word;
                                                    // search forward
    while ((ReportNextHit(ihit)) && (ihit.SamePassageAs(tempHit))) {
        if ((ihit.word - lastWordOffset <= hProximity) && (!(bitArray.LookUpWord(ihit)))) {
            memFull = destination->volumeH[ihit.volume].WriteHit(ihit.passage, ihit.word);
            if (memFull) destination->ShrinkAllStreams();
            lastWordOffset = ihit.word;
        } else
            break;                
    }
    while (!(tempHit == ihit))                      // get back to center
        ReportPreviousHit(ihit);

}               

// ###########################################################################################
//  Here is the compareHits function used by sortHits:
int compareHits(const void *h1, const void *h2)
{
    return(memcmp(h1, h2, sizeof(expanded)) );          // works on Mac but not PC
}

// ###########################################################################################
void    CHitList::SortHits(void)
{
    int     v;

//  cout << "Sorting Hits...\n";
    
    for (v = 0; v < MAX_VOLUMES; v++)
        if (volumeH[v].GetTotalHits())
            volumeH[v].SortHits();

//  cout << "Done Sorting...\n";
}

// ###########################################################################################
/*void  CHitList::ShowGuts(void) {
    int     j;
    
//  for (j = 0; j < MAX_VOLUMES; j++)
//      cout << "VOL: " << j << "  HITS: " << volumeH[j].GetTotalHits() << "\n";
}
*/
// ###########################################################################################
//                              HIT RETRIEVAL FUNCTIONS
// ###########################################################################################
long    CHitList::ReportTotalHits(void) {
    int     j;
    long    total = 0;
    
    for (j = 0; j < MAX_VOLUMES; j++)
        total += volumeH[j].GetTotalHits();
        
    return total;
}

// ###########################################################################################
short   CHitList::ReportNumberOfVolumes(void) {
    int     j;
    short   number = 0;
    
    for (j = 0; j < MAX_VOLUMES; j++)
        if (volumeH[j].GetTotalHits() > 0)
            number++;
    
    return number;
}

// ###########################################################################################
long*   CHitList::ReportHitsPerVol(void) {
    static long hitsPerVol[MAX_VOLUMES];
    short   j;
    
    for (j = 0; j < MAX_VOLUMES; j++)
        hitsPerVol[j] = volumeH[j].GetTotalHits();

    return hitsPerVol;
}

// ###########################################################################################
void    CHitList::ResetVolume(short volume) {

    ihit.volume = volume;
    volumeH[volume].ResetPosition();
}

// ###########################################################################################
Boolean CHitList::ReportNextHit(hit& h) {

    if ( volumeH[ihit.volume].ReadNextHit(ihit.passage, ihit.word) ) {
        h = ihit;
        return TRUE;
    }
    return FALSE;
}

// ###########################################################################################
Boolean CHitList::ReportNextPassageHit(hit& h) {

    if ( volumeH[ihit.volume].ReadNextPassageHit(ihit.passage, ihit.word) ) {
        h = ihit;
        return TRUE;
    }
    return FALSE;
}

// ###########################################################################################
Boolean CHitList::ReportPreviousHit(hit& h) {

    if ( volumeH[ihit.volume].ReadPreviousHit(ihit.passage, ihit.word) ) {
        h = ihit;
        return TRUE;
    }
    return FALSE;
}

// ###########################################################################################
Boolean CHitList::ReportPreviousPassageHit(hit& h) {

    if ( volumeH[ihit.volume].ReadPreviousPassageHit(ihit.passage, ihit.word) ) {
        h = ihit;
        return TRUE;
    }
    return FALSE;
}

// ###########################################################################################
void    CHitList::ShrinkAllStreams(void) {
    int     j;
    
    for (j = 0; j < MAX_VOLUMES; j++)
        volumeH[j].ShrinkToMinimum();
}

// ###########################################################################################

HitBitArray::HitBitArray() {
    for (int i = 0; i < MAX_WORDS_PER_PASSAGE; i++)
        flag[i] = FALSE;
    volume = passage = -1;
    highestFlagWritten = -1;
}

// ###########################################################################################

Boolean HitBitArray::LookUpWord(hit newWord) {
    if ((newWord.word < 0) || (newWord.word > MAX_WORDS_PER_PASSAGE))
        throw CFatalError("HitBitArray::LookUpWord OUT OF BOUNDS");
    if ((newWord.volume != volume) || (newWord.passage != passage)) {
        volume = newWord.volume;
        passage = newWord.passage;
        for (int i = 0; i <= highestFlagWritten; i++)
            flag[i] = FALSE;
        flag[newWord.word] = TRUE;
        highestFlagWritten = newWord.word;
        return FALSE;
    }
    Boolean foundThere = flag[newWord.word];
    flag[newWord.word] = TRUE;
    if (newWord.word > highestFlagWritten)
        highestFlagWritten = newWord.word;
    return foundThere;  
}

// ###########################################################################################
// ###########################################################################################

