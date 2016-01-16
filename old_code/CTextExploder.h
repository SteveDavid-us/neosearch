// ===========================================================================
//	CTextExploder.h					©1994 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "CTextFetch.h"
#include "CWASTEEdit.h"
#include "CHitList.h"
#include "CGiant.h"

#include "CHitOffsetList.h"	// for the hitoffsetlist


const	short	MAX_FOOTNOTES_PER_PASSAGE = 150;		// max seen=74
const	short	MAX_PASSAGENUMS_PER_PASSAGE = 50;		// includes sub passage #s

/*
class StyleInfo {
public:
	TextStyle		*style;
	short			mask;
	long			start;
	long			end;

	void Set(TextStyle *s, short m, long s1, long s2) {
			style = s; mask = m; start = s1; end = s2; } 
};
*/


class CTextExploder {
public:
	CTextExploder();
	~CTextExploder();
	void			LoadPassageNumber(hit theHit);
	void			FeedPassageToWASTE(long selStart, long selEnd);
	void			Setup(CTextFetch *t, CGiant *g, CWASTEEdit *e, CHitOffsetList *l);
//	virtual Boolean	ObeyCommand(CommandT inCommand, void* ioParam);	
//	void			HighlightHit(short wordNum);

protected:

	void			ExplodeWord(long wordNum);

	long			*codeText;
	long			codeTextLen;
	Handle			realTextHandle;
	char			*realTextPtr;		// quick access when handle is locked
	long			realTextLen;

	CTextFetch		*textFetcher;
	CGiant			*giantTable;
	CWASTEEdit		*textPane;
	CHitOffsetList	*ptrToList;
//	CHitList		*theHits;

	short			footnotePositions[MAX_FOOTNOTES_PER_PASSAGE*2];
	short			footnoteCount;
	short			numberPositions[MAX_PASSAGENUMS_PER_PASSAGE*2];
	short			numberCount;
};

