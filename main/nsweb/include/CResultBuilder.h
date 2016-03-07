// *******************************************************************************************
// CResultBuilder.h
// -------------------------------------------------------------------------------------------
// NeoSearch -- 2016 -- JPS
// -------------------------------------------------------------------------------------------
// Class that processes generates json output
//
// *******************************************************************************************

#ifndef _CRESULTBUILDER_H_
#define _CRESULTBUILDER_H_

#include <jsoncpp/json/json.h>
#include <set>

#include "CGiant.h"
#include "CTextFetch.h"
#include "CHitList.h"
#include "CHitOffsetList.h"

using namespace std;

// ###########################################################################################
class CResultBuilder {
public:
    unsigned int firstPassage;
    unsigned int passageCount;
    unsigned int preContext;
    unsigned int postContext;
    set<unsigned int> volumeFilter;
    int volume;
    int passage;

    CResultBuilder() :
        firstPassage(0),
        passageCount(10),
        preContext(40),
        postContext(40),
        volumeFilter(),
        volume(-1),
        passage(-1)
    {
    }

    void LoadResults(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList, Json::Value &passages);
    void LoadPassage(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList, Json::Value &passages);

    std::string Write(CGiant *giantTable, CTextFetch *textFetch, CHitList *hitList);

private:
    std::string WriteHit(char *passage, int length, CHitOffsetList &hitOffsetList);
    std::string WritePassage(char *passage, int length, CHitOffsetList &hitOffsetList);

};
// ###########################################################################################

#endif // _CRESULTBUILDER_H_
