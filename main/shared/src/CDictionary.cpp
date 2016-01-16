// *******************************************************************************************
// CDictionary.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// *******************************************************************************************

#include "CDictionary.h"
#include "Engine.h"
#include "CDataFile.h"
#include <string.h>

// ###########################################################################################
CDictionary::CDictionary()
{
	numLines = 0;
	dictionaryData = NULL;
	dictionaryStringBlock = NULL;
	
	//	-------------------------------------------------------------------------------
	//	Read Dictionary table from the disk

	CDataFile		mainFile;
	long			mainFileLength;

	mainFile.OpenPath(DATA_FILE_PATH, DICT_FILE);
	mainFileLength = mainFile.GetLength();
	numLines = mainFileLength / sizeof(DictionaryLine);
	dictionaryData = (DictionaryLine*) mainFile.ReadAll();
	mainFile.Close();

	//	-------------------------------------------------------------------------------
	//	Read the block of strings that accompany the table

	CDataFile	stringFile;
	
	stringFile.OpenPath(DATA_FILE_PATH, DICT_STRING_FILE);
	dictionaryStringBlock = (char*) stringFile.ReadAll();
	stringFile.Close();

}

// ###########################################################################################
CDictionary::~CDictionary()
{
	if (dictionaryData)
		delete[] (char*)dictionaryData;
	if (dictionaryStringBlock)
		delete[] dictionaryStringBlock;
}

// ###########################################################################################
char* CDictionary::operator[](unsigned short whichBase)
{
	long		lower, upper, mid;
	unsigned short	b;

	lower = 0;
	upper = numLines-1;

	while (lower <= upper) {
		mid = (upper + lower)/2;
		b = dictionaryData[mid].baseNum;
		if (b < whichBase)
			lower = mid + 1;
		else if (b > whichBase)
			upper = mid - 1;
		else return GetString(mid);
	}
	return NULL;
}

// ###########################################################################################
unsigned short CDictionary::operator[](const char* whichString)
{
	long i;
	
	for (i = 0; i < numLines; i++)
		if (strcmp(whichString, GetString(i)) == 0)
			return GetBase(i);
			
	return 1;		// some things have been saved with a base of 0 so this incorrectly matches
					// if we return a 0 here...  Hopefully nothing's saved with '1'.
}

// ###########################################################################################

