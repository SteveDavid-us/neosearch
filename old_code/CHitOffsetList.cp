// ===========================================================================
//	CHitOffsetList.cp					©1994 Metrowerks Inc. All rights reserved.
// ===========================================================================


#include "CHitOffsetList.h"


//==================================================================================

CHitOffsetList::CHitOffsetList() {
	size = 4;
	array = NEW HitOffsetData[size]; ThrowIfNULL_(array);
	count = 0;
}

//==================================================================================

void CHitOffsetList::GrowList() {
	HitOffsetData *oldArray = array;
	int oldSize = size;
	
	size = oldSize * 2 - 1;
	array = NEW HitOffsetData[size];	ThrowIfNULL_(array);
	for (int j = 0; j < oldSize; j++)
		array[j] = oldArray[j];
	delete[] oldArray;
}

//==================================================================================

void CHitOffsetList::AppendHit(short wordNumber) {
	if (count == size)
		GrowList();
	array[count++].wordNumber = wordNumber;
}

//==================================================================================
short CHitOffsetList::GetIndex(short wordNum) {
	for (int j = 0; j < count; j++)
		if (array[j].wordNumber == wordNum)
			return j;
	return -1;	// change this!!!
}
//==================================================================================
//==================================================================================
//==================================================================================
//==================================================================================
//==================================================================================



