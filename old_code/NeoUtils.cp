// NeoUtils.cpp

#include "NeoUtils.h"
#include <string.h>
#include <ctype.h>
#include <String_Utils.h>
#include <UDrawingState.h>



void CToPascal(Str255 pString, const char* cString) {
	unsigned char j = 0;
	while ((j < 255) && (cString[j] != '\0')) {
		pString[j+1] = cString[j];
		j++;
	}
	pString[0] = j;
}

char* PascalToC(Str255 pString) {
	unsigned char	len = pString[0];
	char*			cString = NEW char[len+1];
	int				j;
	for (j = 0; j < len; j++)
		cString[j] = pString[j+1];
	cString[j] = '\0';
	return cString;
}


Boolean	GetNextLowerCaseWord(char* dest, const char* source, int& position) {
	int start;
	int i;
	
	while ((source[position] == ' ') || (source[position] == '\t'))
		position++;
	start = position;
	while ((source[position] != ' ') && (source[position] != '\t')
			&& (source[position] != '\0')) {
		position++;
	}
	for (i = start; i < position; i++)
		dest[i - start] = tolower(source[i]);
	dest[i - start] = '\0';

	while ((source[position] == ' ') || (source[position] == '\t'))	// need second scan in case
		position++;												// of blanks at end of input
	return (source[position] == '\0');
}

// ---------------------------------------------------------------------------
//		¥ AddNumberSuffix [static]
// ---------------------------------------------------------------------------
//	Given an integer  and a base string, change the string so that an the integer is
//	appended to the string.

void AddNumberSuffix(
	int		inSuffix,
	Str255	ioNewTitle)
{
	Str255	tmpString;

	NumToString(inSuffix, tmpString);
	ConcatPStr(ioNewTitle, "\p ");
	ConcatPStr(ioNewTitle, tmpString);
}




void SetWatchCursor(Boolean onFlag)
{
	static Boolean	firstTime = TRUE;
	static Cursor	watchC;
	CursHandle		cursH;
	
	if (firstTime == TRUE) {
		firstTime = FALSE;
		cursH = GetCursor(watchCursor);
		HLock((Handle)cursH);
		watchC = **cursH;
		DisposeHandle((Handle)cursH);		
	}
	if (onFlag)
		SetCursor(&watchC);
	else
		SetCursor(&UQDGlobals::GetQDGlobals()->arrow);		// standard arrow
}

