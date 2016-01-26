// *******************************************************************************************
// CDataFile.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// A file-managing class.
//
// Used by many classes.  Packaged as such so that the file-handling scheme may be changed
// later.
// *******************************************************************************************

#ifndef _CDATAFILE_H_
#define _CDATAFILE_H_

// ###########################################################################################
#include <Types.h>
#include <stdio.h>

// ###########################################################################################
class CDataFile {

    FILE        *theFile;

public:

    CDataFile();
    static void SetBasePath(const char *path);

    long        GetLength();
    void        Open(char* fname, char* permission = "rb");
    void        OpenPath(char* path, char* fname, char* permission = "rb");
    void        Close();
    Boolean     IsOpen();

    void*       ReadAll();
    void*       ReadChunk(long size);
    void        WriteAll(void* contents, long Num);
    
    void        Seek(long int seekSpot, int whence = SEEK_SET);
    long        WhereAmI(void);

    char        GetChar(void);
    short       GetShort(void);
    int         GetInt(void);
    Boolean     GetLine(char* string, int maxChars);
};

// ###########################################################################################

#endif // _CDATAFILE_H_

