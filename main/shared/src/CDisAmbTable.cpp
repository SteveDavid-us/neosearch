// *******************************************************************************************
// CDisAmbTable.cp
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// *******************************************************************************************

#include "CDisAmbTable.h"
#include "Engine.h"
#include "CDataFile.h"
#include "NeoException.h"
#include "QSortRand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <iostream.h>
#include <Memory.h>


int		compareDis(const void *x1, const void *x2);


// ###########################################################################################
//	Try to read in the table of disambiguations from the disk -- create it if not there
CDisAmbTable::CDisAmbTable()
{
	long		fileLength;
	disAmbLine	*tempTable;

	disCount = 0;

//	if (!disFile.ExistsOnDisk())
//		disFile.CreateNew((OSType)"XXXX",(OSType)"NeoD");

	disFile.OpenPath(DATA_FILE_PATH, DIS_AMB_FILE, "rb+");
	fileLength	 = disFile.GetLength();
	disCount	 = fileLength / sizeof(disAmbLine);

//	disTable = (disAmbLine**) NewHandle(fileLength);
    disTable = (disAmbLine*) new char[fileLength];
	if (!disTable)
		throw CFatalError("CDisAmbTable::CDisAmbTable 0");
		
	tempTable = (disAmbLine*) disFile.ReadAll();

//	BlockMove( tempTable, *disTable, fileLength );
    memcpy(disTable, tempTable, fileLength );

	delete[] (char*)tempTable;
}

// ###########################################################################################
//	Update table on disk w/ new disambiguations, then clean up memory
CDisAmbTable::~CDisAmbTable()
{
	//*** Save the updated information!!!!!
	if (disFile.IsOpen() && disTable) {
		disFile.WriteAll(disTable, disCount*sizeof(disAmbLine));

		disFile.Close();
//		::DisposeHandle((Handle) disTable);
        delete[] (char*)disTable;
	}
}

// ###########################################################################################
int	compareDis(const void *x1, const void *x2)
{
    // probably broken
	return(memcmp( &( ((disAmbLine*)x1)->ambCode ),
				   &( ((disAmbLine*)x2)->ambCode ), sizeof(CRoseCode)) );
};

// ###########################################################################################
//	Add a new line to the disambiguation table, sorting afterwards so that
//	binary searching can still be done...
void	CDisAmbTable::AddNewLine(CRoseCode ambCode, CRoseCode disCode, hit newSpot)
{
	disAmbLine		tempLine;

	tempLine.spot = newSpot;
	tempLine.ambCode = ambCode;
	tempLine.disCode = disCode;
//	SetHandleSize((Handle)disTable, (++disCount)*sizeof(disAmbLine));	// maybe buffer this ahead of time
    disAmbLine *tmpTable = new disAmbLine[disCount+1];
	if (!tmpTable)
		throw CFatalError("CDisAmbTable::AddNewLine: MEMORY ERROR!");
    memcpy(tmpTable, disTable, sizeof(disAmbLine)*disCount);
    delete[] disTable;
    disTable = tmpTable;
    disCount++;

	memcpy(disTable+disCount, &tempLine, sizeof(tempLine));
	qsortrand(disTable, disCount, sizeof(disAmbLine), compareDis);
}

// ###########################################################################################
Boolean	CDisAmbTable::LookForAmbCode(CRoseCode findMe)
{
	for (iLine=0; iLine<disCount; iLine++) 				// replace with BINARY SEARCH later !!!!!!
		if (findMe==disTable[iLine].ambCode) return TRUE;	

	return FALSE;
}


// ###########################################################################################
Boolean	CDisAmbTable::GetNextDisCode(CRoseCode& fixMe, hit& mySpot)
{
	if (!(disTable[iLine].ambCode == currentCode) || (iLine >= disCount))
		return FALSE;
	fixMe = disTable[iLine].disCode;
	mySpot = disTable[iLine].spot;
	iLine++;
	return TRUE;
}

// ###########################################################################################

