// NeoUtils.h


void	CToPascal(Str255 pString, const char* cString);
char*	PascalToC(Str255 pString);  // allocates memory
Boolean	GetNextLowerCaseWord(char* dest, const char* source, int& position);
void AddNumberSuffix(int inSuffix, Str255 ioNewTitle);
void SetWatchCursor(Boolean onFlag);
