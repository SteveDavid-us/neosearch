// *******************************************************************************************
// CDataFile.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// *******************************************************************************************

#include <Types.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "CDataFile.h"
#include "NeoException.h"
#include "EndianSwap.h"

using namespace std;

static char BASE_PATH_BUF[1000];
static char *BASE_PATH = "";
static size_t BASE_PATH_LEN = 0;

// ###########################################################################################
CDataFile::CDataFile() {
    theFile = NULL;
}

/******************************************************************************
 SetBasePath

        Store the base path used in all file operations
 ******************************************************************************/
void CDataFile::SetBasePath(const char *path) {
    size_t len = strlen(path);
    BASE_PATH = &BASE_PATH_BUF[0];
    if (len >= sizeof(BASE_PATH_BUF)) {
        throw CFatalError("CDataFile::SetBasePath (buffer overflow)", (char *)path);
    }
    strncpy(BASE_PATH, path, len);
    BASE_PATH[len] = '\0';
    BASE_PATH_LEN = len;
}

/******************************************************************************
 GetLength

        Retrieve the length in bytes of the data fork of a file. File
        must be already open.
 ******************************************************************************/
long    CDataFile::GetLength()  {
    long    theLength;
    fpos_t  store_pos;

    if (theFile==NULL) return 0;

    fgetpos(theFile, &store_pos);       // store position
    fseek(theFile, 0, SEEK_END);
    theLength = ftell(theFile);
    fsetpos(theFile, &store_pos);       // recall position

    return theLength;
}

static int filesOpen = 0;

/******************************************************************************
 Open

        Open the data fork of a file with the specified access permission
 ******************************************************************************/
void    CDataFile::Open(char* fname, char* permission)
{
    theFile = fopen(fname, permission);
#if 0
    cout << "CDataFile::Open(fname=" << fname << ", permission=" << permission << ", errno=" << errno << ", filesOpen=" << filesOpen << ")" << endl;
#endif
    if (!theFile) {
        throw CFatalError("CDataFile::Open (file missing?)", fname);
    }
    filesOpen++;
}

void    CDataFile::OpenPath(char* path, char* fname, char* permission)
{
    char fullName[1000];
    if (BASE_PATH_LEN + strlen(path) + strlen(fname) + 3 > sizeof(fullName)) {
        throw CFatalError("CDataFile::OpenPath (buffer overflow)", fname);
    }
    strcpy(fullName, BASE_PATH);
    strcat(fullName, "/");
    strcat(fullName, path);
    strcat(fullName, "/");
    strcat(fullName, fname);
    Open(fullName, permission);
}


/******************************************************************************
 Close

        Close a File and make sure contents are written to disk
 ******************************************************************************/
void    CDataFile::Close()
{
//    cout << "CDataFile::Close(fname=" << fullName << ", filesOpen=" << filesOpen << ")" << endl;
    fclose(theFile);
    theFile = NULL;
    filesOpen--;
}


/******************************************************************************
 IsOpen

        Return TRUE if file is open
 ******************************************************************************/
Boolean CDataFile::IsOpen()
{
    return (theFile != NULL);
}


/******************************************************************************
 ReadAll

        Read the entire contents of a file into memory.
 ******************************************************************************/
void*   CDataFile::ReadAll()
{
    long        length;
    size_t      numRead;
    void*       p;
    
    length = GetLength();
    p = new char[length];
    if (!p)
        throw CFatalError("CDataFile::ReadAll 1");

    rewind(theFile);
    numRead = fread(p, sizeof(char), length, theFile);
    if (length != numRead)
        throw CFatalError("CDataFile::ReadAll 2");
        
    return p;
}


/******************************************************************************
 ReadChunk

        Read part of a file into memory.
 ******************************************************************************/
void*   CDataFile::ReadChunk(long size) {

    size_t      numRead;
    void*       p;
    
    p = new char[size];
    if (!p)
        throw CFatalError("CDataFile::ReadChunk 1");

    numRead = fread(p, sizeof(char), size, theFile);
    if (size != numRead)
        throw CFatalError("CDataFile::ReadChunk 2");
        
    return p;
}


/******************************************************************************
 WriteAll

        Write all the data in a handle to a file. The existing contents
        of the file are replaced. File must already be open.
 ******************************************************************************/
void    CDataFile::WriteAll(void* contents, long num)
{
    size_t      numWritten;
    
    rewind(theFile);
    numWritten = fwrite(contents, 1, num, theFile);
    if (numWritten != num)
        throw CFatalError("CDataFile::WriteAll 1");
}

/******************************************************************************
 Seek
 
        Seek a file position from a certain spot.
 ******************************************************************************/
void    CDataFile::Seek(long seekSpot, int whence)
{
    int err;
    
    err = fseek(theFile, seekSpot, whence);
    if (err)
        throw CFatalError("CDataFile::Seek 0");
}

/******************************************************************************
 GetChar

        Read a character.
 ******************************************************************************/
char    CDataFile::GetChar(void) {

    return fgetc(theFile);
}

/******************************************************************************
 GetShort
 
        Read a short.
 ******************************************************************************/
short   CDataFile::GetShort(void) {
    int     numread;
    short   s;

    numread = fread(&s, sizeof(short), 1, theFile);
    if (numread==0)
        throw CFatalError("CDataFile::GetShort 0");
#ifndef __BIG_ENDIAN__
    endian_swap((unsigned short&)s);    
#endif
    return s;
}

/******************************************************************************
 GetLong
 
        Read a long.
 ******************************************************************************/
long    CDataFile::GetLong(void) {
    int     numread;
    long    s;

    numread = fread(&s, sizeof(long), 1, theFile);
    if (numread==0) {
        throw CFatalError("CDataFile::GetLong 0");
    }
#ifndef __BIG_ENDIAN__
    endian_swap((unsigned int&)s);    
#endif
    return s;
}


/******************************************************************************
 WhereAmI
 
        Return current file position.
 ******************************************************************************/
long    CDataFile::WhereAmI(void) {

    return ftell(theFile);
}


/******************************************************************************
 GetLine
 
        Read an entire line, up to maxChars.
 ******************************************************************************/
Boolean CDataFile::GetLine(char* string, int maxChars) {

    if (fgets(string, maxChars, theFile))
        return TRUE;
    return FALSE;
}

