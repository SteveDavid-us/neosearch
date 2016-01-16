// ===========================================================================
//	CBrowserWind.h					©1994 Metrowerks Inc. All rights reserved.
// ===========================================================================
//	CBrowserWind.cp		<- double-click + Command-D to see class implementation
//
//	This class implements a pane that displays a moving rectangle controlled
//	by four VCR-like icon-based controls.

#pragma once

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

struct HitOffsetData {
	short	wordNumber;
	long	startOffset;
	long	endOffset;
	short	ambiguityCode;
};

class CHitOffsetList {
public:
	CHitOffsetList();
	~CHitOffsetList() {delete[] array;}
	void Reset() {count = 0;}		// should shrink too?
	void AppendHit(short wordNumber);
	short GetListSize() {return count;}

	long GetStartOffset(short n) {return array[GetIndex(n)].startOffset;}
	long GetEndOffset(short n) {return array[GetIndex(n)].endOffset;}
	short GetAmbCode(short n) {return array[GetIndex(n)].ambiguityCode;}

	long GetStartOffsetIdx(short i) {return array[i].startOffset;}
	long GetEndOffsetIdx(short i) {return array[i].endOffset;}
	short GetAmbCodeIdx(short i) {return array[i].ambiguityCode;}

	void SetStartOffset(short n, long o) {array[GetIndex(n)].startOffset = o;}
	void SetEndOffset(short n, long o) {array[GetIndex(n)].endOffset = o;}
	void SetAmbCode(short n, short c) {array[GetIndex(n)].ambiguityCode = c;}

	Boolean CheckFor(short n) {return GetIndex(n) != -1;}
	
protected:
	void GrowList();
	short GetIndex(short wordNum);

	HitOffsetData*	array;
	short			size;
	short			count;
};

