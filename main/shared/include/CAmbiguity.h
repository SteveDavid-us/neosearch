// *******************************************************************************************
// CAmbiguity.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 1994 -- HBL
// -------------------------------------------------------------------------------------------
// Class definitions for the ambiguity tables:
//		External
//		Middle
//		Internal
//
// Used by the GTOE only.
// *******************************************************************************************

#ifndef _CAMBIGUITY_H_
#define _CAMBIGUITY_H_

// ###########################################################################################
#include "CRoseCode.h"

// ###########################################################################################
class CAmbRoot {							//	Abstract root class for 3 tables

protected:

	short			arraySize;
	short			maxReplacements;
	CRoseCode		*ambTblPtr;				// 2D array: ambTblPtr[arraySize][maxReplacements]

	CRoseCode		searchCode;				// code that iterator is looking for
	short			iLine;					// iterator current line
	short			iCol;					// iterator current column
	
	CRoseCode		LookUp(short row, short col)	// Access the "array"
						{return ambTblPtr[row*maxReplacements + col];};
	
public:

	CAmbRoot(char* fileName);		// constructor to be invoked by inherited classes
	~CAmbRoot();
	
	virtual int		FirstReplacement(CRoseCode& findMe) = 0;	// Pure virtual: Must be overriden
	int				NextReplacement(CRoseCode& fixMe);
};

// ###########################################################################################
class CExtAmbTable : public CAmbRoot {

public:

	CExtAmbTable(void);
	int		FirstReplacement(CRoseCode& findMe);
};

// ###########################################################################################
class CMidAmbTable : public CAmbRoot {

public:
	
	CMidAmbTable(void);
	int		FirstReplacement(CRoseCode& findMe);
};

// ###########################################################################################
class CIntAmbTable : public CAmbRoot {

public:
	
	CIntAmbTable(void);
	int		FirstReplacement(CRoseCode& findMe);
};
// ###########################################################################################

#endif // _CAMBIGUITY_H_

