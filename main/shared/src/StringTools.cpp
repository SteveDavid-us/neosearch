// *******************************************************************************************
// StringTools.cpp
// -------------------------------------------------------------------------------------------
// NeoSearch 2 -- 2008 -- JPS
// *******************************************************************************************


#include "StringTools.h"

// glob-style sting match tool
// currently only accepts '*' character
// should only be used with small strings (function is recursive)
// returns true if the string is a match
bool strmatch (char *str, char *pattern) 
{
    if (!*str && !*pattern) {
        // empty string and pattern
        return true;
    }
    if (*pattern == '*' && !*(pattern+1)) {
        // star matches anything
        return true;
    }
    if (!*str || !*pattern) {
        // empty string xor empty pattern
        return false;
    }
    if (*str == *pattern) {
        if (strmatch(str+1, pattern+1)) {
            return true;
        }
    }
    if (*pattern == '*') {
        // try without the star         
        if (strmatch(str, pattern+1)) {
            return true;
        }
        // try without the first letter
        return strmatch(str+1, pattern);
    }
    return false;
}

