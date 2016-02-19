// *******************************************************************************************
// CHitList.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Prototype for the class that holds the hit list.
//
// This class will be created by the GTOE and dealt with by the GUI.
// *******************************************************************************************

#ifndef _CHITLIST_H_
#define _CHITLIST_H_

// ###########################################################################################
#include "CDataFile.h"
#include "Engine.h"
#include <vector>
//#include <stdlib.h>


// ###########################################################################################
// The expanded class is used for sorting the hits for each volume
class expanded {
public:
    short   passage;
    short   word;
    
    bool operator== (expanded e)    {return (e.word == word) && (e.passage == passage); };
};

// ###########################################################################################
class hit {
public:
    short   volume;                 // the IMPORTANT STUFF that anyone can read
    short   passage;
    short   word;
                                    // all sorts of specialized methods
    void    Set(short v, short p, short w)  {volume = v; passage = p; word = w; };
    bool operator== (hit h)             {return (h.volume == volume) && (h.passage == passage)
                                                     && (h.word == word); };
    bool    SamePassageAs(hit h)            {return (h.volume == volume) && (h.passage == passage);};

    bool operator< (hit h)  {
        if (volume < h.volume) return true;
        else if (volume == h.volume) return (passage < h.passage);
        else return false; };

    bool operator> (hit h)  {
        if (volume > h.volume) return true;
        else if (volume == h.volume) return (passage > h.passage);
        else return false; };

    bool LastHit(void)      {return volume == LAST_HIT; };
    void    BecomeLastHit(void) {volume = LAST_HIT; };
};

// ###########################################################################################
// The HitBitArray was added this year (1995).  Previously duplicate hits were added to the hit
// stream and removed in the final sort.  That could take too much memory.  This class provides
// a way to check and only add a hit if it hasn't already been added.
// It isn't written optimally for speed -- it could use a redesign.

class HitBitArray {
private:
    bool    flag[MAX_WORDS_PER_PASSAGE];
    short   volume;
    short   passage;
    short   highestFlagWritten;

public:
    HitBitArray();
    bool    LookUpWord(hit newWord);    // mark it in array, return true if it was already there
};

// ###########################################################################################
// This class holds passages and word offsets in "compressed" form in memory (used by CHitList)
class   CHitMemoryStream {
    
    short*      theData;                    // pointer to the data
    long        handleSize;
    short       lastPas, storedLastPas;
    long        iPos, iStoredPos;           // iterator variables
    long        totalHits,                  // number of hits held by this class
                totalShorts,                // actual memory spaces occupied by these hits
                totalPassages;              // passages hit
    
    bool        iMovingNextWards;           // true means iterator last NEXTed, false = PREVIOUSed

    long        AttemptGrow(bool& memFullFlag); // returns new memory size and a (success?) flag

public:

    CHitMemoryStream();
    ~CHitMemoryStream();
    
    void        Reset(void);                            // CLEAR all data
    void        ResetPosition(void);                    // Resets the iterator
    bool        WriteHit(short pas, short wrd);         // Returns whether enough memory
    bool        ReadNextHit(short& pas, short& wrd);    // Returns whether there was another to read
    bool        ReadNextPassageHit(short& pas, short& wrd);
    bool        ReadPreviousHit(short& pas, short& wrd);
    bool        ReadPreviousPassageHit(short& pas, short& wrd);
    void        PushPosition(void);                     // Store the iterator's position
    void        PopPosition(void);                      // Recall the last stored position
    
    long        GetTotalHits(void);
    long        GetPassagesHit(void);

    void        SortHits(void);
    void        ShrinkToMinimum(void);                  // Shrinks the memory to lowest needed
};

// ###########################################################################################
class CHitList {

    CHitMemoryStream volumeH[MAX_VOLUMES];  // one stream object per volume
    CDataFile   hitFile;                    // hit list file class
    bool        intersectModeOn;            // should we be intersecting?
    CHitList    *intersectList;             // Temp list to contain intersected final product
    int         hProximity;                 // Hit proximity to use for intersections
    hit         ihit;                       // helps control stream iterators

    HitBitArray bitArray;       // remembers words we've added so duplicates won't get added    

    // Is this hit a member of this list of exceptions?
    bool        CheckForException(std::vector<hit> &exceptions, short vol, short pas, short wrd);
    // If this hit is within proximity of something in my own list, write them both to destination
    void        WriteThemIfTheyIntersect(CHitList *destination, short vol, short pas, short wrd);
    void        WriteLateralProximityHits(CHitList *destination);   
    void        ShrinkAllStreams(void);     // Frees up memory

public:
    
    CHitList();
    ~CHitList();
    void        ResetList(void);                    // CLEAR all hits

    void        OpenIntersectMode(int proximity);   // says to dump future AddHits into intersectList
    CHitList*   CloseIntersectMode(void);           // returns the intersected list
    void        AddHits(long diskOffset, std::vector<hit> *exceptions); // Add hits to memory from disk
    void        SortHits(void);                     // Sorts each book's hitlist (to be done at end)

    long        ReportTotalHits(void);              // Information methods
    long        ReportTotalPassagesHit(void);
    short       ReportNumberOfVolumes(void);
    long*       ReportHitsPerVol(void);
    void        ReportPassagesHitPerVol(long (&passagesPerVol)[MAX_VOLUMES]);
    
    void        ResetVolume(short volume);          // Iterator methods
    bool        ReportNextHit(hit&);
    bool        ReportNextPassageHit(hit&);
    bool        ReportPreviousHit(hit&);
    bool        ReportPreviousPassageHit(hit&); 

//  void        ShowGuts(void);                     // TEMPORARY PROCEDURE TO SPIT OUT ALL DATA
};
// ###########################################################################################

#endif // _CHITLIST_H_

