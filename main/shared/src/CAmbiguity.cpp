// *******************************************************************************************
// CAmbiguity.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// *******************************************************************************************

#include <iostream>
#include <String.h>
#include "CDataFile.h"
#include "CAmbiguity.h"
#include "CRoseCode.h"
#include "Engine.h"
#include "NeoException.h"


// ###########################################################################################
// The root constructor is called by the respective inherited constructor.
CAmbRoot::CAmbRoot(char* fileName)
{
	int			fileLength;
    CRoseCode	infoBlock;
	CDataFile	ambFile;
	
	ambFile.OpenPath(DATA_FILE_PATH, fileName);
	fileLength = ambFile.GetLength();
	ambTblPtr = (CRoseCode*) ambFile.ReadAll();
	ambFile.Close();
#ifndef __BIG_ENDIAN__
    int n = fileLength/sizeof(CRoseCode);
    for (int i=0; i < n; i++) {
        ambTblPtr[i].EndianSwap();
    }
#endif

	// the information block at the end of the file says the dimensions of the 2D table
	memcpy(&infoBlock, ambTblPtr + fileLength/sizeof(CRoseCode) - 1, sizeof(CRoseCode));
	arraySize 		= infoBlock.GetArraySize();
	maxReplacements = infoBlock.GetMaxReplacements();

	if (arraySize*maxReplacements*sizeof(CRoseCode) != fileLength-sizeof(CRoseCode))
		throw CFatalError("CAmbRoot::CAmbRoot", "Ambiguity File Data Invalid"); 
}

// ###########################################################################################
CAmbRoot::~CAmbRoot()
{
	if (ambTblPtr != NULL)
		delete[] (char*)ambTblPtr;
}

// ###########################################################################################

CExtAmbTable::CExtAmbTable() : CAmbRoot(EXT_AMB_FILE)
{
}

// ###########################################################################################

CMidAmbTable::CMidAmbTable() : CAmbRoot(MID_AMB_FILE)
{
}

// ###########################################################################################

CIntAmbTable::CIntAmbTable() : CAmbRoot(INT_AMB_FILE)
{
}

// ###########################################################################################
int	CExtAmbTable::FirstReplacement(CRoseCode& findMe)
{
	searchCode = findMe;
	iLine = searchCode.StripToExtBase();
	iCol = 0;
	findMe = searchCode | LookUp(iLine, iCol);
	return 1;
}

int	CMidAmbTable::FirstReplacement(CRoseCode& findMe)
{
	searchCode = findMe;
	iLine = searchCode.StripToMidBase();
	iCol = 0;
	findMe = searchCode | LookUp(iLine, iCol);
	return 1;
}

int	CIntAmbTable::FirstReplacement(CRoseCode& findMe)
{
	searchCode = findMe;
	iLine = searchCode.StripToIntBase();
	iCol = 0;
	findMe = searchCode | LookUp(iLine, iCol);
	return 1;
}

// ###########################################################################################
int	CAmbRoot::NextReplacement(CRoseCode& fixMe)
{
	if ((LookUp(iLine, ++iCol).BLANK_ROSE_CODE()) || (iCol >= maxReplacements))
		return 0;
		
	fixMe = searchCode | LookUp(iLine, iCol);
	return 1;
}

// ###########################################################################################

