// *******************************************************************************************
// Engine.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Main header file for the NeoSearch engine -- universal #defines.
//
// Needs to be included by just about everything.
// *******************************************************************************************

#ifndef _ENGINE_H_
#define _ENGINE_H_

#define MAX_VOLUMES                     26 // was 53        // The # of volumes that were indexed

#ifdef __APPLE__
#define DATA_FILE_PATH                  "data"
#define BOOK_FILE_PATH                  "texts"
#else // mingw
#define DATA_FILE_PATH                  "../data"
#define BOOK_FILE_PATH                  "../texts"
#endif

#define GTOE_FILE                       "Giant Table"
#define GTOE_STRING_FILE                "Giant Table Strings"
#define DICT_FILE                       "Dictionary Data"
#define DICT_STRING_FILE                "Dictionary Strings"
#define INT_AMB_FILE                    "Int Amb Table"
#define MID_AMB_FILE                    "Mid Amb Table"
#define EXT_AMB_FILE                    "Ext Amb Table"
#define DIS_AMB_FILE                    "Disambiguation Table"
#define HIT_LIST_FILE                   "Hit List"
#define BOOK_LIST_FILE                  "Book List"

#define MAX_WORDS_PER_PASSAGE           30000
#define MAX_STYLES_PER_PASSAGE          5000
#define MAX_UNAMBIGUOUS_REPLACEMENTS    50                  // used by GTOE search methods
#define PASSAGE_LIMIT                   -1000               // offset of passage #'s on disk
#define HIT_CHAIN_START_SIZE            (5 * sizeof(short)) // default start size for hitlist/vol
#define EXCEPTION_LIST_START_SIZE       (50 * sizeof(hit))  // default start size (disambiguations)
#define NOT_FOUND                       -1
#define LAST_HIT                        -1

#define CREATE_NEW                      1                   // search modes
#define UNION                           2
#define INTERSECT                       3

#define PASSAGE_OFFSET_BLANK            -1

#endif // _ENGINE_H_

